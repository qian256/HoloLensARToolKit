/*
*  ARMarkerMulti.cpp
*  ARToolKitUWP
*
*  This work is a modified version of the original "ARMarkerMulti.cpp" of
*  ARToolKit. The copyright and license information of ARToolKit is included
*  in this document as required by its GNU Lesser General Public License
*  version 3.
*
*  This file is a part of ARToolKitUWP.
*
*  ARToolKitUWP is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  ARToolKitUWP is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public License
*  along with ARToolKitUWP.  If not, see <http://www.gnu.org/licenses/>.
*
*  Copyright 2017 Long Qian
*
*  Author: Long Qian
*  Contact: lqian8@jhu.edu
*
*/


/* The original copyright information: */
/*
*  ARMarkerMulti.cpp
*  ARToolKit5
*
*  This file is part of ARToolKit.
*
*  ARToolKit is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  ARToolKit is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public License
*  along with ARToolKit.  If not, see <http://www.gnu.org/licenses/>.
*
*  As a special exception, the copyright holders of this library give you
*  permission to link this library with independent modules to produce an
*  executable, regardless of the license terms of these independent modules, and to
*  copy and distribute the resulting executable under terms of your choice,
*  provided that you also meet, for each linked independent module, the terms and
*  conditions of the license of that module. An independent module is a module
*  which is neither derived from nor based on this library. If you modify this
*  library, you may extend this exception to your version of the library, but you
*  are not obligated to do so. If you do not wish to do so, delete this exception
*  statement from your version.
*
*  Copyright 2015 Daqri, LLC.
*  Copyright 2010-2015 ARToolworks, Inc.
*
*  Author(s): Philip Lamb.
*
*/

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
