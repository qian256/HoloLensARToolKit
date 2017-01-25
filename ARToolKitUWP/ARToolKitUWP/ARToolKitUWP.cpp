/*
*  ARToolKitUWP.cpp
*  ARToolKitUWP
*
*  This work is a modified version of the original "ARToolKitWrapperExportedAPI.cpp" of
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
*  ARToolKitWrapperExportedAPI.cpp
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
*  Author(s): Philip Lamb, Julian Looser.
*
*/

#include "pch.h"
#include <ARToolKitUWP.h>


static ARController *gARTK = NULL;
void CALL_CONV log(const char *msg);


EXPORT_API bool aruwpInitialiseAR(int width, int height, int pixelSize)
{
	if (!gARTK) gARTK = new ARController(width, height, pixelSize);
	gARTK->logCallback = log;
	return gARTK->initialiseBase();
}

EXPORT_API bool aruwpInitialiseARWithOptions(int width, int height, int pixelSize, const int pattSize, const int pattCountMax)
{
	if (!gARTK) gARTK = new ARController(width, height, pixelSize);
	gARTK->logCallback = log;
	return gARTK->initialiseBase(pattSize, pattCountMax);
}

EXPORT_API bool aruwpGetARToolKitVersion(char *buffer, int length)
{
	if (!buffer) return false;
	if (!gARTK) return false;

	if (const char *version = gARTK->getARToolKitVersion()) {
		strncpy(buffer, version, length - 1); buffer[length - 1] = '\0';
		return true;
	}
	return false;
}

EXPORT_API bool aruwpStartRunning(const char *cparaName)
{
	if (!gARTK) return false;
	return gARTK->startRunning(cparaName, NULL, 0);
}

EXPORT_API bool aruwpStartRunningBuffer(const char *cparaBuff, const int cparaBuffLen)
{
	if (!gARTK) return false;
	return gARTK->startRunning(NULL, cparaBuff, cparaBuffLen);
}


EXPORT_API bool aruwpIsRunning()
{
	if (!gARTK) return false;
	return gARTK->isRunning();
}

EXPORT_API bool aruwpStopRunning()
{
	if (!gARTK) return false;
	return gARTK->stopRunning();
}

EXPORT_API bool aruwpShutdownAR()
{
	if (gARTK) {
		delete gARTK;
		gARTK = NULL;
	}
	return (true);
}

EXPORT_API bool aruwpGetFrameParams(int *width, int *height, int *pixelSize, char *pixelFormatStringBuffer, int pixelFormatStringBufferLen)
{
	AR_PIXEL_FORMAT pf;

	if (!gARTK) return false;
	if (!gARTK->frameParameters(width, height, &pf)) return false;
	if (pixelSize) *pixelSize = arUtilGetPixelSize(pf);
	if (pixelFormatStringBuffer && pixelFormatStringBufferLen > 0) {
		strncpy(pixelFormatStringBuffer, arUtilGetPixelFormatName(pf), pixelFormatStringBufferLen);
		pixelFormatStringBuffer[pixelFormatStringBufferLen - 1] = '\0';
	}
	return true;
}

EXPORT_API bool aruwpUpdate(ARUint8* frame)
{
	if (!gARTK) return false;
	return gARTK->update(frame);
}


EXPORT_API void aruwpSetVideoThreshold(int threshold)
{
	if (!gARTK) return;
	gARTK->setThreshold(threshold);
}

EXPORT_API int aruwpGetVideoThreshold()
{
	if (!gARTK) return 0;
	return gARTK->getThreshold();
}

EXPORT_API void aruwpSetVideoThresholdMode(int mode)
{
	if (!gARTK) return;
	gARTK->setThresholdMode(mode);
}

EXPORT_API int aruwpGetVideoThresholdMode()
{
	if (!gARTK) return 0;
	return gARTK->getThresholdMode();
}

EXPORT_API void aruwpSetLabelingMode(int mode)
{
	if (!gARTK) return;
	gARTK->setLabelingMode(mode);
}

EXPORT_API int aruwpGetLabelingMode()
{
	if (!gARTK) return 0;
	return gARTK->getLabelingMode();
}

EXPORT_API void aruwpSetPatternDetectionMode(int mode)
{
	if (!gARTK) return;
	gARTK->setPatternDetectionMode(mode);
}

EXPORT_API int aruwpGetPatternDetectionMode()
{
	if (!gARTK) return 0;
	return gARTK->getPatternDetectionMode();
}

EXPORT_API void aruwpSetBorderSize(ARdouble size)
{
	if (!gARTK) return;
	gARTK->setPattRatio(1.0f - 2.0f*size);
}

EXPORT_API float aruwpGetBorderSize()
{
	if (!gARTK) return 0.0f;
	return ((1.0f - gARTK->getPattRatio()) * 0.5f);
}

EXPORT_API void aruwpSetMatrixCodeType(int type)
{
	if (!gARTK) return;
	gARTK->setMatrixCodeType(type);
}

EXPORT_API int aruwpGetMatrixCodeType()
{
	if (!gARTK) return 0;
	return gARTK->getMatrixCodeType();
}

EXPORT_API void aruwpSetImageProcMode(int mode)
{
	if (!gARTK) return;
	gARTK->setImageProcMode(mode);
}

EXPORT_API int aruwpGetImageProcMode()
{
	if (!gARTK) return 0;
	return gARTK->getImageProcMode();
}

EXPORT_API int aruwpAddMarker(const char *cfg)
{
	if (!gARTK) return -1;
	return gARTK->addMarker(cfg);
}

EXPORT_API bool aruwpRemoveMarker(int markerUID)
{
	if (!gARTK) return false;
	return gARTK->removeMarker(markerUID);
}

EXPORT_API int aruwpRemoveAllMarkers()
{
	if (!gARTK) return 0;
	return gARTK->removeAllMarkers();
}

EXPORT_API bool aruwpQueryMarkerVisibility(int markerUID)
{
	ARMarker *marker;

	if (!gARTK) return false;
	if (!(marker = gARTK->findMarker(markerUID))) {
		gARTK->logv(AR_LOG_LEVEL_ERROR, "arwQueryMarkerVisibility(): Couldn't locate marker with UID %d.", markerUID);
		return false;
	}
	return marker->visible;
}

EXPORT_API bool aruwpQueryMarkerTransformation(int markerUID, ARdouble trans[12])
{
	ARMarker *marker;

	if (!gARTK) return false;
	if (!(marker = gARTK->findMarker(markerUID))) {
		gARTK->logv(AR_LOG_LEVEL_ERROR, "arwQueryMarkerTransformation(): Couldn't locate marker with UID %d.", markerUID);
		return false;
	}
	memcpy(trans, marker->trans, sizeof(ARdouble) * 12);
	return marker->visible;
}


EXPORT_API int aruwpGetMarkerPatternCount(int markerUID)
{
	ARMarker *marker;

	if (!gARTK) return 0;
	if (!(marker = gARTK->findMarker(markerUID))) {
		gARTK->logv(AR_LOG_LEVEL_ERROR, "arwQueryMarkerTransformation(): Couldn't locate marker with UID %d.", markerUID);
		return 0;
	}
	return marker->patternCount;
}

EXPORT_API bool aruwpGetMarkerPatternConfig(int markerUID, int patternID, ARdouble matrix[16], ARdouble *width, ARdouble *height, int *imageSizeX, int *imageSizeY)
{
	ARMarker *marker;
	ARPattern *p;

	if (!gARTK) return false;
	if (!(marker = gARTK->findMarker(markerUID))) {
		gARTK->logv(AR_LOG_LEVEL_ERROR, "arwGetMarkerPatternConfig(): Couldn't locate marker with UID %d.", markerUID);
		return false;
	}

	if (!(p = marker->getPattern(patternID))) {
		gARTK->logv(AR_LOG_LEVEL_ERROR, "arwGetMarkerPatternConfig(): Marker with UID %d has no pattern with ID %d.", markerUID, patternID);
		return false;
	}

	if (matrix) {
		memcpy(matrix, p->m_matrix, sizeof(ARdouble) * 16);
	}
	if (width) *width = (ARdouble)p->m_width;
	if (height) *height = (ARdouble)p->m_height;
	if (imageSizeX) *imageSizeX = p->m_imageSizeX;
	if (imageSizeY) *imageSizeY = p->m_imageSizeY;
	return true;
}


EXPORT_API bool aruwpGetMarkerOptionBool(int markerUID, int option)
{
	ARMarker *marker;

	if (!gARTK) return false;
	if (!(marker = gARTK->findMarker(markerUID))) {
		gARTK->logv(AR_LOG_LEVEL_ERROR, "arwGetMarkerOptionBool(): Couldn't locate marker with UID %d.", markerUID);
		return false;
	}

	switch (option) {
	case ARUWP_MARKER_OPTION_FILTERED:
		return(marker->isFiltered());
		break;
	case ARUWP_MARKER_OPTION_SQUARE_USE_CONT_POSE_ESTIMATION:
		if (marker->type == ARMarker::SINGLE) return (((ARMarkerSquare *)marker)->useContPoseEstimation);
		break;
	default:
		gARTK->logv(AR_LOG_LEVEL_ERROR, "arwGetMarkerOptionBool(): Unrecognised option %d.", option);
		break;
	}
	return(false);
}

EXPORT_API void aruwpSetMarkerOptionBool(int markerUID, int option, int value)
{
	ARMarker *marker;

	if (!gARTK) return;
	if (!(marker = gARTK->findMarker(markerUID))) {
		gARTK->logv(AR_LOG_LEVEL_ERROR, "arwSetMarkerOptionBool(): Couldn't locate marker with UID %d.", markerUID);
		return;
	}

	switch (option) {
	case ARUWP_MARKER_OPTION_FILTERED:
		marker->setFiltered((value!=0));
		break;
	case ARUWP_MARKER_OPTION_SQUARE_USE_CONT_POSE_ESTIMATION:
		if (marker->type == ARMarker::SINGLE) ((ARMarkerSquare *)marker)->useContPoseEstimation = (value!=0);
		break;
	default:
		gARTK->logv(AR_LOG_LEVEL_ERROR, "arwSetMarkerOptionBool(): Unrecognised option %d.", option);
		break;
	}
}

EXPORT_API int aruwpGetMarkerOptionInt(int markerUID, int option)
{
	ARMarker *marker;

	if (!gARTK) return INT_MIN;
	if (!(marker = gARTK->findMarker(markerUID))) {
		gARTK->logv(AR_LOG_LEVEL_ERROR, "arwGetMarkerOptionBool(): Couldn't locate marker with UID %d.", markerUID);
		return (INT_MIN);
	}

	switch (option) {
	case ARUWP_MARKER_OPTION_MULTI_MIN_SUBMARKERS:
		if (marker->type == ARMarker::MULTI) return ((ARMarkerMulti *)marker)->config->min_submarker;
		break;
	default:
		gARTK->logv(AR_LOG_LEVEL_ERROR, "arwGetMarkerOptionInt(): Unrecognised option %d.", option);
		break;
	}
	return (INT_MIN);
}

EXPORT_API void aruwpSetMarkerOptionInt(int markerUID, int option, int value)
{
	ARMarker *marker;

	if (!gARTK) return;
	if (!(marker = gARTK->findMarker(markerUID))) {
		gARTK->logv(AR_LOG_LEVEL_ERROR, "arwSetMarkerOptionInt(): Couldn't locate marker with UID %d.", markerUID);
		return;
	}

	switch (option) {
	case ARUWP_MARKER_OPTION_MULTI_MIN_SUBMARKERS:
		if (marker->type == ARMarker::MULTI) ((ARMarkerMulti *)marker)->config->min_submarker = value;
		break;
	default:
		gARTK->logv(AR_LOG_LEVEL_ERROR, "arwSetMarkerOptionInt(): Unrecognised option %d.", option);
		break;
	}
}

EXPORT_API ARdouble aruwpGetMarkerOptionFloat(int markerUID, int option)
{
	ARMarker *marker;

	if (!gARTK) return (NAN);
	if (!(marker = gARTK->findMarker(markerUID))) {
		gARTK->logv(AR_LOG_LEVEL_ERROR, "arwGetMarkerOptionBool(): Couldn't locate marker with UID %d.", markerUID);
		return (NAN);
	}

	switch (option) {
	case ARUWP_MARKER_OPTION_FILTER_SAMPLE_RATE:
		return ((ARdouble)marker->filterSampleRate());
		break;
	case ARUWP_MARKER_OPTION_FILTER_CUTOFF_FREQ:
		return ((ARdouble)marker->filterCutoffFrequency());
		break;
	case ARUWP_MARKER_OPTION_SQUARE_CONFIDENCE:
		if (marker->type == ARMarker::SINGLE) return ((ARdouble)((ARMarkerSquare *)marker)->getConfidence());
		else return (NAN);
		break;
	case ARUWP_MARKER_OPTION_SQUARE_CONFIDENCE_CUTOFF:
		if (marker->type == ARMarker::SINGLE) return ((ARdouble)((ARMarkerSquare *)marker)->getConfidenceCutoff());
		else return (NAN);
		break;
	case ARUWP_MARKER_OPTION_NFT_SCALE:
		return (NAN);
		break;
	case ARUWP_MARKER_OPTION_MULTI_MIN_CONF_MATRIX:
		if (marker->type == ARMarker::MULTI) return (ARdouble)((ARMarkerMulti *)marker)->config->cfMatrixCutoff;
		else return (NAN);
		break;
	case ARUWP_MARKER_OPTION_MULTI_MIN_CONF_PATTERN:
		if (marker->type == ARMarker::MULTI) return (ARdouble)((ARMarkerMulti *)marker)->config->cfPattCutoff;
		else return (NAN);
		break;
	default:
		gARTK->logv(AR_LOG_LEVEL_ERROR, "arwGetMarkerOptionFloat(): Unrecognised option %d.", option);
		break;
	}
	return (NAN);
}

EXPORT_API void aruwpSetMarkerOptionFloat(int markerUID, int option, ARdouble value)
{
	ARMarker *marker;

	if (!gARTK) return;
	if (!(marker = gARTK->findMarker(markerUID))) {
		gARTK->logv(AR_LOG_LEVEL_ERROR, "arwSetMarkerOptionFloat(): Couldn't locate marker with UID %d.", markerUID);
		return;
	}

	switch (option) {
	case ARUWP_MARKER_OPTION_FILTER_SAMPLE_RATE:
		marker->setFilterSampleRate(value);
		break;
	case ARUWP_MARKER_OPTION_FILTER_CUTOFF_FREQ:
		marker->setFilterCutoffFrequency(value);
		break;
	case ARUWP_MARKER_OPTION_SQUARE_CONFIDENCE_CUTOFF:
		if (marker->type == ARMarker::SINGLE) ((ARMarkerSquare *)marker)->setConfidenceCutoff(value);
		break;
	case ARUWP_MARKER_OPTION_NFT_SCALE:
		break;
	case ARUWP_MARKER_OPTION_MULTI_MIN_CONF_MATRIX:
		if (marker->type == ARMarker::MULTI) ((ARMarkerMulti *)marker)->config->cfMatrixCutoff = value;
		break;
	case ARUWP_MARKER_OPTION_MULTI_MIN_CONF_PATTERN:
		if (marker->type == ARMarker::MULTI) ((ARMarkerMulti *)marker)->config->cfPattCutoff = value;
		break;
	default:
		gARTK->logv(AR_LOG_LEVEL_ERROR, "arwSetMarkerOptionFloat(): Unrecognised option %d.", option);
		break;
	}
}





static PFN_LOGCALLBACK logCallback = NULL;
static DWORD logThreadID = 0;
static int logDumpedWrongThreadCount = 0;


// ----------------------------------------------------------------------------------------------------

// When handed a logging callback, install it for use by our own log function,
// and pass our own log function as the callback instead.
// This allows us to use buffering to ensure that logging occurs only on the
// same thread that registered the log callback, as required e.g. by C# interop.

void CALL_CONV log(const char *msg)
{
	if (logCallback) {
		if (GetCurrentThreadId() != logThreadID){
			logDumpedWrongThreadCount++;
			return;
		}
		if (logDumpedWrongThreadCount) {
			char s[80];
			sprintf(s, "%d log messages on non-main thread were dumped.\n", logDumpedWrongThreadCount);
			logDumpedWrongThreadCount = 0;
			logCallback(s);
		}
		logCallback(msg);
	}
	else {
		LOGE("%s\n", msg);
	}
}

EXPORT_API void aruwpRegisterLogCallback(PFN_LOGCALLBACK callback)
{
	logCallback = callback;
	arLogSetLogger(callback, 1); // 1 -> only callback on same thread.
	logThreadID = GetCurrentThreadId();
	logDumpedWrongThreadCount = 0;
	//char buf[256];
	//_snprintf(buf, 256, "Registering log callback on thread %d.\n", logThreadID);
	//log(buf);
}

EXPORT_API void aruwpSetLogLevel(const int logLevel)
{
	if (logLevel >= 0) {
		arLogLevel = logLevel;
	}
}

