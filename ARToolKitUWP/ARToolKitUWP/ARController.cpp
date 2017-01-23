#include "pch.h"
#include <ARController.h>
#include <algorithm>
#include <string>
#include <stdarg.h>

// DEBUG
#define ARLOGi printf
#define ARLOGe printf
#define ARLOGd printf


ARController::ARController() :
	state(NOTHING_INITIALISED),
	versionString(NULL),
	threshold(AR_DEFAULT_LABELING_THRESH),
	thresholdMode(AR_LABELING_THRESH_MODE_DEFAULT),
	imageProcMode(AR_DEFAULT_IMAGE_PROC_MODE),
	labelingMode(AR_DEFAULT_LABELING_MODE),
	pattRatio(AR_PATT_RATIO),
	patternDetectionMode(AR_DEFAULT_PATTERN_DETECTION_MODE),
	matrixCodeType(AR_MATRIX_CODE_TYPE_DEFAULT),
	markers(),
	doMarkerDetection(false),
	m_arHandle(NULL),
	m_arPattHandle(NULL),
	m_ar3DHandle(NULL),
	frameWidth(896),
	frameHeight(504),
	pixelFormat(AR_PIXEL_FORMAT_RGBA)
{
	;
}

ARController::ARController(int width, int height, int format) :
	state(NOTHING_INITIALISED),
	versionString(NULL),
	threshold(AR_DEFAULT_LABELING_THRESH),
	thresholdMode(AR_LABELING_THRESH_MODE_DEFAULT),
	imageProcMode(AR_DEFAULT_IMAGE_PROC_MODE),
	labelingMode(AR_DEFAULT_LABELING_MODE),
	pattRatio(AR_PATT_RATIO),
	patternDetectionMode(AR_DEFAULT_PATTERN_DETECTION_MODE),
	matrixCodeType(AR_MATRIX_CODE_TYPE_DEFAULT),
	markers(),
	doMarkerDetection(false),
	m_arHandle(NULL),
	m_arPattHandle(NULL),
	m_ar3DHandle(NULL),
	frameWidth(width),
	frameHeight(height),
	pixelFormat(AR_PIXEL_FORMAT(format))
{
	;
}


ARController::~ARController()
{
	shutdown();
	if (versionString) free(versionString);
}


const char* ARController::getARToolKitVersion()
{
	if (!versionString) arGetVersion(&versionString);
	return versionString;
}



bool ARController::initialiseBase(const int patternSize, const int patternCountMax)
{
	ARLOGd("ARWrapper::ARController::initialiseBase(): called\n");
	if (state != NOTHING_INITIALISED) {
		ARLOGe("Initialise called while already initialised. Will shutdown first, exiting, returning false\n");
		if (!shutdown())
			return false;
	}
	ARLOGi("ARWrapper::ARController::initialiseBase(): Initialising...\n");
	
	// Create pattern handle so markers can begin to be added.
	if ((m_arPattHandle = arPattCreateHandle2(patternSize, patternCountMax)) == NULL) {
		ARLOGe("Error: arPattCreateHandle2, exiting, returning false\n");
		return false;
	}

	state = BASE_INITIALISED;

	ARLOGd("ARWrapper::ARController::initialiseBase(): Initialised, exiting, returning true\n");
	return true;
}


void ARController::setFrameWidthHeight(int width, int height) {
	frameWidth = width;
	frameHeight = height;
}

void ARController::setPixelFormat(int format) {
	pixelFormat = (AR_PIXEL_FORMAT)format;
}


bool ARController::startRunning(const char* cparaName, const char* cparaBuff, const long cparaBuffLen)
{
	ARLOGi("ARController::startRunning(): called, start running\n");

	// Check for initialization before starting video
	if (state != BASE_INITIALISED) {
		ARLOGe("ARController::startRunning(): Error: not initialized, exiting, returning false\n");
		return false;
	}

	frameSource = FrameSource::newFrameSource(frameWidth, frameHeight, pixelFormat);
	if (!frameSource) {
		ARLOGe("ARController::startRunning(): Error: no frame source, exiting, returning false\n");
		return false;
	}

	frameSource->configure(cparaName, cparaBuff, cparaBuffLen);

	if (!frameSource->open()) {
		delete frameSource;
		frameSource = NULL;
		return false;
	}

	state = WAITING_FOR_FRAME;

	ARLOGd("ARController::startRunning(): exiting, returning true\n");
	return true;
}


bool ARController::initARMore(void)
{
	ARLOGi("ARController::initARMore() called\n");

	// Create AR handle
	if ((m_arHandle = arCreateHandle(frameSource->getCameraParameters())) == NULL) {
		ARLOGe("ARController::initARMore(): Error: arCreateHandle()\n");
		goto bail;
	}

	// Set the pixel format
	if (arSetPixelFormat(m_arHandle, frameSource->getPixelFormat()) < 0) {
		ARLOGe("ARController::initARMore(): Error: arSetPixelFormat\n");
		goto bail1;
	}

	arPattAttach(m_arHandle, m_arPattHandle);

	// Set initial configuration. One call for each configuration option.
	arSetLabelingThresh(m_arHandle, threshold);
	arSetLabelingThreshMode(m_arHandle, thresholdMode);
	arSetImageProcMode(m_arHandle, imageProcMode);
	arSetLabelingMode(m_arHandle, labelingMode);
	arSetPattRatio(m_arHandle, pattRatio);
	arSetPatternDetectionMode(m_arHandle, patternDetectionMode);
	arSetMatrixCodeType(m_arHandle, matrixCodeType);

	// Create 3D handle
	if ((m_ar3DHandle = ar3DCreateHandle(&frameSource->getCameraParameters()->param)) == NULL) {
		ARLOGe("ARController::initARMore(): Error: ar3DCreateHandle\n");
		goto bail1;
	}

	ARLOGd("ARController::initARMore() exiting, returning true\n");
	return true;

bail1:
	arDeleteHandle(m_arHandle);
	m_arHandle = NULL;
bail:
	ARLOGe("ARController::initARMore() exiting, returning false\n");
	return false;
}


bool ARController::stopRunning()
{
	ARLOGd("ARController::stopRunning(): called\n");
	if (state != DETECTION_RUNNING && state != WAITING_FOR_FRAME) {
		ARLOGe("ARController::stopRunning(): Error: Not running.\n");
		return false;
	}

	if (frameSource) {
		ARLOGd("ARController::stopRunning(): if (frameSource) true\n");
		ARLOGd("ARController::stopRunning(): calling frameSource->close()\n");
		frameSource->close();
		delete frameSource;
		frameSource = NULL;
	}
	
	if (m_ar3DHandle) {
		ARLOGd("ARController::stopRunning(): calling ar3DDeleteHandle(&m_ar3DHandle)\n");
		ar3DDeleteHandle(&m_ar3DHandle); // Sets ar3DHandle0 to NULL.
	}

	if (m_arHandle) {
		ARLOGd("ARController::stopRunning(): if (m_arHandle0) true\n");
		arPattDetach(m_arHandle);
		arDeleteHandle(m_arHandle);
		m_arHandle = NULL;
	}

	state = BASE_INITIALISED;

	ARLOGd("ARController::stopRunning(): exiting, returning true\n");
	return true;
}


bool ARController::update(ARUint8* frame)
{
	//
	// check ARController state
	//
	if (state != DETECTION_RUNNING) {
		if (state != WAITING_FOR_FRAME) {
			// State is NOTHING_INITIALISED or BASE_INITIALISED.
			ARLOGe("ARController::update(): Error-if (state != WAITING_FOR_VIDEO) true, exiting returning false\n");
			return false;

		}
		else {

			// First check there is a video source and it's open.
			if (!frameSource) {
				ARLOGe("ARController::update(): Error-no video source or video source is closed, exiting returning false\n");
				return false;
			}
			state = DETECTION_RUNNING;
		}
	}
	
	//
	// check frame and frameSource
	//
	if (!frame) {
		ARLOGe("ARController::update(): no frame parsed, exiting returning true\n");
		return false;
	}
	if (!frameSource) {
		ARLOGe("ARController::update(): Error-no video source or video source is closed, exiting returning false\n");
		return false;
	}
	else {
		frameSource->setFrame(frame);
	}

	//
	// Detect markers.
	//
	if (doMarkerDetection) {
		ARLOGd("ARController::update(): if (doMarkerDetection) true\n");

		ARMarkerInfo *markerInfo = NULL;
		int markerNum = 0;

		if (!m_arHandle) {
			if (!initARMore()) {
				ARLOGe("ARController::update(): Error initialising AR, exiting returning false\n");
				return false;
			}
		}

		if (m_arHandle) {
			if (arDetectMarker(m_arHandle, frame) < 0) {
				ARLOGe("ARController::update(): Error: arDetectMarker(), exiting returning false\n");
				return false;
			}
			markerInfo = arGetMarker(m_arHandle);
			markerNum = arGetMarkerNum(m_arHandle);
			ARLOGd("ARController::update(): number of detected markers: %d\n", markerNum);
			for (int i = 0; i < markerNum; i++) {
				ARLOGd("ARController::update(): marker %d with id: %d\n", i, markerInfo[i].idMatrix);
			}
		}

		// Update square markers.
		bool success = true;
			for (std::vector<ARMarker *>::iterator it = markers.begin(); it != markers.end(); ++it) {
				if ((*it)->type == ARMarker::SINGLE) {
					success &= ((ARMarkerSquare *)(*it))->updateWithDetectedMarkers(markerInfo, markerNum, m_ar3DHandle);
				}
				else if ((*it)->type == ARMarker::MULTI) {
					success &= ((ARMarkerMulti *)(*it))->updateWithDetectedMarkers(markerInfo, markerNum, m_ar3DHandle);
				}
			}
	} // doMarkerDetection

	ARLOGd("ARController::update(): exiting, returning true\n");

	return true;
}


bool ARController::shutdown()
{
	ARLOGd("ARController::shutdown(): called\n");
	do {
		switch (state) {
		case DETECTION_RUNNING:
		case WAITING_FOR_FRAME:
			ARLOGd("ARController::shutdown(): DETECTION_RUNNING or WAITING_FOR_FRAME, forcing stop.\n");
			stopRunning();
			break;

		case BASE_INITIALISED:
			if (countMarkers() > 0) {
				ARLOGd("ARController::shutdown(): BASE_INITIALISED, cleaning up markers.\n");
				removeAllMarkers();
			}

			if (m_arPattHandle) {
				arPattDeleteHandle(m_arPattHandle);
				m_arPattHandle = NULL;
			}

			state = NOTHING_INITIALISED;
			// Fall though.
		case NOTHING_INITIALISED:
			ARLOGd("ARController::shutdown(): NOTHING_INITIALISED, complete\n");
			break;
		}
	} while (state != NOTHING_INITIALISED);

	ARLOGd("ARController::shutdown(): exiting, returning true\n");
	return true;
}


bool ARController::canAddMarker()
{
	// Check we are in a valid state to add a marker (i.e. base initialisation has occurred)
	return (state != NOTHING_INITIALISED);
}

bool ARController::isRunning()
{
	return state == DETECTION_RUNNING;
}

bool ARController::frameParameters(int *width, int *height, AR_PIXEL_FORMAT *pixelFormat)
{
	FrameSource *fs = frameSource;
	if (!fs) return false;

	if (width) *width = fs->getFrameWidth();
	if (height) *height = fs->getFrameHeight();
	if (pixelFormat) *pixelFormat = fs->getPixelFormat();

	return true;
}


void ARController::setImageProcMode(int mode) {
	imageProcMode = mode;

	if (m_arHandle) {
		if (arSetImageProcMode(m_arHandle, mode) == 0) {
			ARLOGi("Image proc. mode set to %d.", imageProcMode);
		}
	}
}

int ARController::getImageProcMode() const
{
	return imageProcMode;
}

void ARController::setThreshold(int thresh)
{
	if (thresh < 0 || thresh > 255) return;
	threshold = thresh;
	if (m_arHandle) {
		if (arSetLabelingThresh(m_arHandle, threshold) == 0) {
			ARLOGi("Threshold set to %d", threshold);
		}
	}
}

int ARController::getThreshold() const
{
	return threshold;
}

void ARController::setThresholdMode(int mode)
{
	thresholdMode = (AR_LABELING_THRESH_MODE)mode;
	if (m_arHandle) {
		if (arSetLabelingThreshMode(m_arHandle, thresholdMode) == 0) {
			ARLOGi("Threshold mode set to %d", (int)thresholdMode);
		}
	}
}

int ARController::getThresholdMode() const
{
	return (int)thresholdMode;
}

void ARController::setLabelingMode(int mode)
{
	labelingMode = mode;
	if (m_arHandle) {
		if (arSetLabelingMode(m_arHandle, labelingMode) == 0) {
			ARLOGi("Labeling mode set to %d", labelingMode);
		}
	}
}

int ARController::getLabelingMode() const
{
	return labelingMode;
}

void ARController::setPatternDetectionMode(int mode)
{
	patternDetectionMode = mode;
	if (m_arHandle) {
		if (arSetPatternDetectionMode(m_arHandle, patternDetectionMode) == 0) {
			ARLOGi("Pattern detection mode set to %d.", patternDetectionMode);
		}
	}
}

int ARController::getPatternDetectionMode() const
{
	return patternDetectionMode;
}

void ARController::setPattRatio(ARdouble ratio)
{
	if (ratio <= 0.0f || ratio >= 1.0f) return;
	pattRatio = ratio;
	if (m_arHandle) {
		if (arSetPattRatio(m_arHandle, pattRatio) == 0) {
			ARLOGi("Pattern ratio size set to %f.", pattRatio);
		}
	}
}

ARdouble ARController::getPattRatio() const
{
	return pattRatio;
}

void ARController::setMatrixCodeType(int type)
{
	matrixCodeType = (AR_MATRIX_CODE_TYPE)type;
	if (m_arHandle) {
		if (arSetMatrixCodeType(m_arHandle, matrixCodeType) == 0) {
			ARLOGi("Matrix code type set to %d.", matrixCodeType);
		}
	}
}

int ARController::getMatrixCodeType() const
{
	return (int)matrixCodeType;
}


int ARController::addMarker(const char* cfg)
{
	if (!canAddMarker()) {
		ARLOGe("Error: Cannot add marker. ARToolKit not initialised\n");
		return -1;
	}

	ARMarker *marker = ARMarker::newWithConfig(cfg, m_arPattHandle);
	if (!marker) {
		ARLOGe("Error: Failed to load marker.\n\n");
		return -1;
	}
	if (!addMarker(marker)) {
		return -1;
	}
	return marker->UID;
}


bool ARController::addMarker(ARMarker* marker)
{
	ARLOGd("ARController::addMarker(): called\n");
	if (!canAddMarker()) {
		ARLOGe("Error: Cannot add marker. ARToolKit not initialised, exiting, returning false\n");
		return false;
	}

	if (!marker) {
		ARLOGe("Error: Cannot add a NULL marker, exiting, returning false\n");
		return false;
	}

	markers.push_back(marker);

	doMarkerDetection = true;

	ARLOGi("Added marker (UID=%d), total markers loaded: %d.", marker->UID, countMarkers());
	return true;
}

bool ARController::removeMarker(int UID)
{
	ARMarker *marker = findMarker(UID);
	if (!marker) {
		ARLOGe("ARController::removeMarker(): could not find marker (UID=%d).\n");
		return (false);
	}
	return removeMarker(marker);
}

bool ARController::removeMarker(ARMarker* marker)
{
	ARLOGd("ARController::removeMarker(): called\n");
	if (!marker) {
		ARLOGe("ARController::removeMarker(): no marker specified, exiting, returning false\n");
		return false;
	}

	int UID = marker->UID;
	std::vector<ARMarker *>::iterator position = std::find(markers.begin(), markers.end(), marker);
	bool found = (position != markers.end());
	if (!found) {
		ARLOGe("ARController::removeMarker(): Could not find marker (UID=%d), exiting, returning false", UID);
		return false;
	}

	delete marker; // std::vector does not call destructor if it's a raw pointer being stored, so explicitly delete it.
	markers.erase(position);

	int markerCount = countMarkers();
	if (markerCount == 0) {
		ARLOGi("Last square marker removed; disabling square marker detection.\n");
		doMarkerDetection = false;
	}

	ARLOGi("Removed marker (UID=%d), now %d markers loaded", UID, markerCount);
	ARLOGd("ARController::removeMarker(): exiting, returning %s", ((found) ? "true" : "false"));
	return (found);
}

int ARController::removeAllMarkers()
{
	int count = countMarkers();
	markers.clear();
	doMarkerDetection = false;
	ARLOGi("Removed all %d markers.", count);

	return count;
}

int ARController::countMarkers()
{
	return (int)(markers.size());
}


ARMarker* ARController::findMarker(int UID)
{
	std::vector<ARMarker *>::const_iterator it = markers.begin();
	while (it != markers.end()) {
		if ((*it)->UID == UID) return (*it);
		++it;
	}
	return NULL;
}


