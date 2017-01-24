#include "pch.h"
#include <ARMarkerMulti.h>
#include <ARController.h>

#ifdef ARDOUBLE_IS_FLOAT
#  define _0_0 0.0f
#  define _1_0 1.0f
#else
#  define _0_0 0.0
#  define _1_0 1.0
#endif

ARMarkerMulti::ARMarkerMulti() : ARMarker(MULTI),
								m_loaded(false),
								config(NULL),
								robustFlag(true)
{
}

ARMarkerMulti::~ARMarkerMulti()
{
	if (m_loaded) unload();
}

bool ARMarkerMulti::load(const char *multiConfig, ARPattHandle *arPattHandle)
{
	if (m_loaded) unload();

	config = arMultiReadConfigFile(multiConfig, arPattHandle);

	if (!config) {
		ARController::logv(AR_LOG_LEVEL_ERROR, "Error loading multimarker config %s", multiConfig);
		return false;
	}

	visible = visiblePrev = false;

	// ARPatterns to hold images and positions of the patterns for display to the user.
	allocatePatterns(config->marker_num);
	for (int i = 0; i < patternCount; i++) {
		if (config->marker[i].patt_type == AR_MULTI_PATTERN_TYPE_TEMPLATE) {
			patterns[i]->loadTemplate(config->marker[i].patt_id, arPattHandle, (float)config->marker[i].width);
		}
		else {
			patterns[i]->loadMatrix(config->marker[i].patt_id, AR_MATRIX_CODE_3x3, (float)config->marker[i].width); // TODO: Determine actual matrix code type in use.
		}
		patterns[i]->m_matrix[0] = config->marker[i].trans[0][0];
		patterns[i]->m_matrix[1] = config->marker[i].trans[1][0];
		patterns[i]->m_matrix[2] = config->marker[i].trans[2][0];
		patterns[i]->m_matrix[3] = _0_0;
		patterns[i]->m_matrix[4] = config->marker[i].trans[0][1];
		patterns[i]->m_matrix[5] = config->marker[i].trans[1][1];
		patterns[i]->m_matrix[6] = config->marker[i].trans[2][1];
		patterns[i]->m_matrix[7] = _0_0;
		patterns[i]->m_matrix[8] = config->marker[i].trans[0][2];
		patterns[i]->m_matrix[9] = config->marker[i].trans[1][2];
		patterns[i]->m_matrix[10] = config->marker[i].trans[2][2];
		patterns[i]->m_matrix[11] = _0_0;
		patterns[i]->m_matrix[12] = config->marker[i].trans[0][3];
		patterns[i]->m_matrix[13] = config->marker[i].trans[1][3];
		patterns[i]->m_matrix[14] = config->marker[i].trans[2][3];
		patterns[i]->m_matrix[15] = _1_0;
	}
	config->min_submarker = 0;
	m_loaded = true;
	return true;
}

bool ARMarkerMulti::unload()
{
	if (m_loaded) {
		freePatterns();
		if (config) {
			arMultiFreeConfig(config);
			config = NULL;
		}
		m_loaded = false;
	}

	return true;
}

bool ARMarkerMulti::updateWithDetectedMarkers(ARMarkerInfo* markerInfo, int markerNum, AR3DHandle *ar3DHandle)
{
	if (!m_loaded || !config) return false;			// Can't update without multimarker config

	visiblePrev = visible;

	if (markerInfo) {

		ARdouble err;

		if (robustFlag) {
			err = arGetTransMatMultiSquareRobust(ar3DHandle, markerInfo, markerNum, config);
		}
		else {
			err = arGetTransMatMultiSquare(ar3DHandle, markerInfo, markerNum, config);
		}

		// Marker is visible if a match was found.
		if (err >= 0) {
			visible = true;
			for (int j = 0; j < 3; j++) for (int k = 0; k < 4; k++) trans[j][k] = config->trans[j][k];
		}
		else visible = false;

	}
	else visible = false;

	return (ARMarker::update()); // Parent class will finish update.
}
