/*
*  ARMarkerSquare.cpp
*  ARToolKitUWP
*
*  This work is a modified version of the original "ARMarkerSquare.cpp" of
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


/* The original copyright information: *//*
*  ARMarkerSquare.cpp
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
#include <ARMarkerSquare.h>
#include <ARController.h>

#ifndef MAX
#  define MAX(x,y) (x > y ? x : y)
#endif


ARMarkerSquare::ARMarkerSquare() : ARMarker(SINGLE),
									m_loaded(false),
									m_arPattHandle(NULL),
									m_cf(0.0f),
									m_cfMin(AR_CONFIDENCE_CUTOFF_DEFAULT),
									patt_id(-1),
									patt_type(-1),
									useContPoseEstimation(true)
{
}


ARMarkerSquare::~ARMarkerSquare()
{
	if (m_loaded) unload();
}



bool ARMarkerSquare::unload()
{
	if (m_loaded) {
		freePatterns();
		if (patt_type == AR_PATTERN_TYPE_TEMPLATE && patt_id != -1) {
			if (m_arPattHandle) {
				arPattFree(m_arPattHandle, patt_id);
				m_arPattHandle = NULL;
			}
		}
		patt_id = patt_type = -1;
		m_cf = 0.0f;
		m_width = 0.0f;
		m_loaded = false;
	}

	return (true);
}


bool ARMarkerSquare::initWithPatternFile(const char* path, ARdouble width, ARPattHandle *arPattHandle)
{
	// Ensure the pattern string is valid
	if (!path || !arPattHandle) return false;

	if (m_loaded) unload();

	ARController::logv(AR_LOG_LEVEL_INFO, "Loading single AR marker from file '%s', width %f.", path, width);

	m_arPattHandle = arPattHandle;
	patt_id = arPattLoad(m_arPattHandle, path);
	if (patt_id < 0) {
		ARController::logv(AR_LOG_LEVEL_ERROR, "Error: unable to load single AR marker from file '%s'.", path);
		arPattHandle = NULL;
		return false;
	}

	patt_type = AR_PATTERN_TYPE_TEMPLATE;
	m_width = width;

	visible = visiblePrev = false;

	// An ARPattern to hold an image of the pattern for display to the user.
	allocatePatterns(1);
	patterns[0]->loadTemplate(patt_id, m_arPattHandle, (float)m_width);

	m_loaded = true;
	return true;
}

bool ARMarkerSquare::initWithPatternFromBuffer(const char* buffer, ARdouble width, ARPattHandle *arPattHandle)
{
	// Ensure the pattern string is valid
	if (!buffer || !arPattHandle) return false;

	if (m_loaded) unload();

	ARController::logv(AR_LOG_LEVEL_INFO, "Loading single AR marker from buffer, width %f.", width);

	m_arPattHandle = arPattHandle;
	patt_id = arPattLoadFromBuffer(m_arPattHandle, buffer);
	if (patt_id < 0) {
		ARController::logv(AR_LOG_LEVEL_ERROR, "Error: unable to load single AR marker from buffer.");
		return false;
	}

	patt_type = AR_PATTERN_TYPE_TEMPLATE;
	m_width = width;

	visible = visiblePrev = false;

	// An ARPattern to hold an image of the pattern for display to the user.
	allocatePatterns(1);
	patterns[0]->loadTemplate(patt_id, arPattHandle, (float)m_width);

	m_loaded = true;
	return true;
}

bool ARMarkerSquare::initWithBarcode(int barcodeID, ARdouble width)
{
	if (barcodeID < 0) return false;

	if (m_loaded) unload();

	ARController::logv(AR_LOG_LEVEL_INFO, "Adding single AR marker with barcode %d, width %f.", barcodeID, width);

	patt_id = barcodeID;

	patt_type = AR_PATTERN_TYPE_MATRIX;
	m_width = width;

	visible = visiblePrev = false;

	// An ARPattern to hold an image of the pattern for display to the user.
	allocatePatterns(1);
	patterns[0]->loadMatrix(patt_id, AR_MATRIX_CODE_3x3, (float)m_width); // FIXME: need to determine actual matrix code type.

	m_loaded = true;
	return true;
}

ARdouble ARMarkerSquare::getConfidence()
{
	return (m_cf);
}

ARdouble ARMarkerSquare::getConfidenceCutoff()
{
	return (m_cfMin);
}

void ARMarkerSquare::setConfidenceCutoff(ARdouble value)
{
	if (value >= AR_CONFIDENCE_CUTOFF_DEFAULT && value <= 1.0f) {
		m_cfMin = value;
	}
}

bool ARMarkerSquare::updateWithDetectedMarkers(ARMarkerInfo* markerInfo, int markerNum, AR3DHandle *ar3DHandle) {

	ARController::logv(AR_LOG_LEVEL_DEBUG, "ARMarkerSquare::update(), id: %d\n", patt_id);

	if (patt_id < 0) return false;	// Can't update if no pattern loaded

	visiblePrev = visible;

	if (markerInfo) {

		int k = -1;
		if (patt_type == AR_PATTERN_TYPE_TEMPLATE) {
			// Iterate over all detected markers.
			for (int j = 0; j < markerNum; j++) {
				if (patt_id == markerInfo[j].idPatt) {
					// The pattern of detected trapezoid matches marker[k].
					if (k == -1) {
						if (markerInfo[j].cfPatt > m_cfMin) k = j; // Count as a match if match confidence exceeds cfMin.
					}
					else if (markerInfo[j].cfPatt > markerInfo[k].cfPatt) k = j; // Or if it exceeds match confidence of a different already matched trapezoid (i.e. assume only one instance of each marker).
				}
			}
			if (k != -1) {
				markerInfo[k].id = markerInfo[k].idPatt;
				markerInfo[k].cf = markerInfo[k].cfPatt;
				markerInfo[k].dir = markerInfo[k].dirPatt;
			}
		}
		else {
			for (int j = 0; j < markerNum; j++) {
				if (patt_id == markerInfo[j].idMatrix) {
					if (k == -1) {
						if (markerInfo[j].cfMatrix >= m_cfMin) k = j; // Count as a match if match confidence exceeds cfMin.
					}
					else if (markerInfo[j].cfMatrix > markerInfo[k].cfMatrix) k = j; // Or if it exceeds match confidence of a different already matched trapezoid (i.e. assume only one instance of each marker).
				}
			}
			if (k != -1) {
				markerInfo[k].id = markerInfo[k].idMatrix;
				markerInfo[k].cf = markerInfo[k].cfMatrix;
				markerInfo[k].dir = markerInfo[k].dirMatrix;
			}
		}

		// Consider marker visible if a match was found.
		if (k != -1) {
			visible = true;
			m_cf = markerInfo[k].cf;
			// If the model is visible, update its transformation matrix
			if (visiblePrev && useContPoseEstimation) {
				// If the marker was visible last time, use "cont" version of arGetTransMatSquare
				arGetTransMatSquareCont(ar3DHandle, &(markerInfo[k]), trans, m_width, trans);
			}
			else {
				// If the marker wasn't visible last time, use normal version of arGetTransMatSquare
				arGetTransMatSquare(ar3DHandle, &(markerInfo[k]), m_width, trans);
			}
		}
		else {
			visible = false;
			m_cf = 0.0f;
		}

	}
	else {
		visible = false;
		m_cf = 0.0f;
	}

	return (ARMarker::update()); // Parent class will finish update.
}

