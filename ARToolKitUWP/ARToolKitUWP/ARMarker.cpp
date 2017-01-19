#include "pch.h"
#include <ARMarker.h>
#include <ARMarkerSquare.h>
#include <ARMarkerMulti.h>


#ifdef _WIN32
#  define MAXPATHLEN MAX_PATH
#else
#  include <sys/param.h>
#endif

static char *get_buff(char *buf, int n, FILE *fp, int skipblanks)
{
	char *ret;
	size_t l;

	do {
		ret = fgets(buf, n, fp);
		if (ret == NULL) return (NULL); // EOF or error.

										// Remove NLs and CRs from end of string.
		l = strlen(buf);
		while (l > 0) {
			if (buf[l - 1] != '\n' && buf[l - 1] != '\r') break;
			l--;
			buf[l] = '\0';
		}
	} while (buf[0] == '#' || (skipblanks && buf[0] == '\0')); // Reject comments and blank lines.

	return (ret);
}



// single;data/hiro.patt;80
// single_buffer;80;buffer=234 221 237...
// single_barcode;0;80
// multi;data/multi/marker.dat
ARMarker* ARMarker::newWithConfig(const char* cfg, ARPattHandle *arPattHandle)
{
	ARMarker *markerRet = NULL;

	// Ensure the configuration string exists
	if (!cfg) return NULL;

	// strtok modifies the string, so need to copy it.
	const char *bufferStart = strstr(cfg, ";buffer=");
	char *cfgCopy;
	if (bufferStart) {
		arMalloc(cfgCopy, char, bufferStart - cfg + 1); // +1 leaves space for nul terminator.
		strncpy(cfgCopy, cfg, (size_t)(bufferStart - cfg));
		cfgCopy[bufferStart - cfg] = '\0';
	}
	else {
		arMalloc(cfgCopy, char, strlen(cfg) + 1);
		strcpy(cfgCopy, cfg);
	}

	// First token is the marker type
	if (char* markerTypePtr = strtok(cfgCopy, ";")) {

		if (strcmp(markerTypePtr, "single") == 0) {

			// Single ARMarker, second token is the pattern file.
			if (char *path = strtok(NULL, ";")) {
				// Third token is the marker width.
				if (char *widthPtr = strtok(NULL, ";")) {
					ARdouble width = 0.0f;
					if (sscanf(widthPtr, "%f", &width) == 1)
					{
						markerRet = new ARMarkerSquare();
						if (!((ARMarkerSquare *)markerRet)->initWithPatternFile(path, width, arPattHandle)) {
							// Marker failed to load, or was not added
							delete markerRet;
							markerRet = NULL;
						}
					}
				}
			}

		}
		else if (strcmp(markerTypePtr, "single_buffer") == 0) {

			// ARMarkerSquare, second token is the marker width.
			if (bufferStart) {
				// Single ARMarker, second token is the marker width.
				if (char *widthPtr = strtok(NULL, ";")) {
					ARdouble width = 0.0f;
					if (sscanf(widthPtr, "%f", &width) == 1)
					{
						markerRet = new ARMarkerSquare();
						if (!((ARMarkerSquare *)markerRet)->initWithPatternFromBuffer(bufferStart + 8, width, arPattHandle)) {
							// Marker failed to load, or was not added
							delete markerRet;
							markerRet = NULL;
						}
					}
				}
			}

		}
		else if (strcmp(markerTypePtr, "single_barcode") == 0) {

			// ARMarkerSquare, second token is the barcode ID.
			if (char *barcodeIDPtr = strtok(NULL, ";")) {
				int barcodeID = -1;
				if (sscanf(barcodeIDPtr, "%d", &barcodeID) == 1) {
					// Third token is the marker width.
					if (char *widthPtr = strtok(NULL, ";")) {
						ARdouble width = 0.0f;
						if (sscanf(widthPtr, "%f", &width) == 1) {
							markerRet = new ARMarkerSquare();
							if (!((ARMarkerSquare *)markerRet)->initWithBarcode(barcodeID, width)) {
								// Marker failed to load, or was not added
								delete markerRet;
								markerRet = NULL;
							}
						}
					}
				}
			}


		}
		else if (strcmp(markerTypePtr, "multi") == 0) {

			// ARMarkerMultiSquare, second token is the multimarker config file.
			if (char *config = strtok(NULL, ";")) {
				markerRet = new ARMarkerMulti();
				if (!((ARMarkerMulti *)markerRet)->load(config, arPattHandle)) {
					// Marker failed to load, or was not added
					delete markerRet;
					markerRet = NULL;
				}
			}

		}
		else {

			// Unknown marker type
			ARLOGe("Error: Unknown marker type.");
		}

	}

	free(cfgCopy);
	return (markerRet);
}



ARMarker::ARMarker(MarkerType type) :
	m_ftmi(NULL),
	m_filterCutoffFrequency(AR_FILTER_TRANS_MAT_CUTOFF_FREQ_DEFAULT),
	m_filterSampleRate(AR_FILTER_TRANS_MAT_SAMPLE_RATE_DEFAULT),
	m_positionScaleFactor(1.0f),
	type(type),
	visiblePrev(false),
	visible(false),
	patternCount(0),
	patterns(NULL)
{
	static int nextUID = 0;
	UID = nextUID++;
}


ARMarker::~ARMarker()
{
	freePatterns();

	if (m_ftmi) arFilterTransMatFinal(m_ftmi);
}


void ARMarker::allocatePatterns(int count)
{
	freePatterns();

	if (count) {
		//ARController::logv("Allocating %d patterns on marker %d", patternCount, UID);
		patternCount = count;
		patterns = new ARPattern*[patternCount];
		for (int i = 0; i < patternCount; i++) {
			patterns[i] = new ARPattern();
		}
	}
}

void ARMarker::freePatterns()
{
	if (patternCount) ARLOGi("Freeing %d patterns on marker %d", patternCount, UID);

	for (int i = 0; i < patternCount; i++) {
		if (patterns[i]) {
			delete patterns[i];
			patterns[i] = NULL;
		}
	}
	if (patterns) {
		delete[] patterns;
		patterns = NULL;
	}

	patternCount = 0;
}


ARPattern* ARMarker::getPattern(int n)
{
	// Check n is in acceptable range
	if (!patterns || n < 0 || n >= patternCount) return NULL;

	return patterns[n];
}

void ARMarker::setPositionScalefactor(ARdouble scale)
{
	m_positionScaleFactor = scale;
}

ARdouble ARMarker::positionScalefactor()
{
	return m_positionScaleFactor;
}


void ARMarker::setFiltered(bool flag)
{
	if (flag && !m_ftmi) {
		m_ftmi = arFilterTransMatInit(m_filterSampleRate, m_filterCutoffFrequency);
	}
	else if (!flag && m_ftmi) {
		arFilterTransMatFinal(m_ftmi);
		m_ftmi = NULL;
	}
}

bool ARMarker::isFiltered()
{
	return (m_ftmi != NULL);
}

ARdouble ARMarker::filterSampleRate()
{
	return m_filterSampleRate;
}

void ARMarker::setFilterSampleRate(ARdouble rate)
{
	m_filterSampleRate = rate;
	if (m_ftmi) arFilterTransMatSetParams(m_ftmi, m_filterSampleRate, m_filterCutoffFrequency);
}

ARdouble ARMarker::filterCutoffFrequency()
{
	return m_filterCutoffFrequency;
}

void ARMarker::setFilterCutoffFrequency(ARdouble freq)
{
	m_filterCutoffFrequency = freq;
	if (m_ftmi) arFilterTransMatSetParams(m_ftmi, m_filterSampleRate, m_filterCutoffFrequency);
}



bool ARMarker::update()
{
	// Subclasses will have already determined visibility and set/cleared 'visible' and 'visiblePrev'
	if (visible) {
		// Filter the pose estimate.
		if (m_ftmi) {
			if (arFilterTransMat(m_ftmi, trans, !visiblePrev) < 0) {
				ARLOGe("arFilterTransMat error with marker %d.\n", UID);
			}
		}

		if (!visiblePrev) {
			ARLOGi("Marker %d now visible", UID);
		}

	}
	else {
		if (visiblePrev) {
			ARLOGi("Marker %d no longer visible", UID);
		}
	}
	return true;
}


