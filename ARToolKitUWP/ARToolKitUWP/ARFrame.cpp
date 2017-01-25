/*
*  ARFrame.cpp
*  ARToolKitUWP
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

#include "pch.h"
#include <ARFrame.h>
#include <ARController.h>

#define MAX(x,y) (x > y ? x : y)
#define MIN(x,y) (x < y ? x : y)
#define CLAMP(x,r1,r2) (MIN(MAX(x,r1),r2))

FrameSource* FrameSource::newFrameSource(int width, int height, AR_PIXEL_FORMAT format) {
	return new FrameSource(width, height, format);
}


FrameSource::FrameSource(int width, int height, AR_PIXEL_FORMAT format) :
	cameraParam(NULL),
	cameraParamBuffer(NULL),
	cameraParamBufferLen(0L),
	cparamLT(NULL),
	frameWidth(width),
	frameHeight(height),
	pixelFormat(format),
	frameBuffer(NULL),
	frameStamp(0),
	cameraParamLoaded(false)
{
	;
}


FrameSource::~FrameSource() {	
	if (cameraParam) {
		free(cameraParam);
		cameraParam = NULL;
	}
	if (cameraParamBuffer) {
		free(cameraParamBuffer);
		cameraParamBuffer = NULL;
	}
	cameraParamLoaded = false;
}


void FrameSource::configure(const char* cparaName, const char* cparaBuff, size_t cparaBuffLen) {
	ARController::logv(AR_LOG_LEVEL_DEBUG, "FrameSource::configure(): called");

	if (cparaName) {
		size_t len = strlen(cparaName);
		cameraParam = (char*)malloc(sizeof(char) * len + 1);
		strcpy(cameraParam, cparaName);
		ARController::logv(AR_LOG_LEVEL_INFO, "Settting camera parameters file '%s'.", cameraParam);
		cameraParamLoaded = false;
	}

	if (cparaBuff) {
		cameraParamBufferLen = cparaBuffLen;
		cameraParamBuffer = (char*)malloc(sizeof(char) * cameraParamBufferLen);
		memcpy(cameraParamBuffer, cparaBuff, cameraParamBufferLen);
		ARController::logv(AR_LOG_LEVEL_INFO, "Settting camera parameters buffer: %ld bytes.", cameraParamBufferLen);
		cameraParamLoaded = false;
	}
	ARController::logv(AR_LOG_LEVEL_DEBUG, "FrameSource::configure(): exiting");
}


bool FrameSource::open()
{
	ARParam cparam;
	const char cparam_name_default[] = "camera_para.dat"; // Default name for the camera parameters.
	if (cameraParamBuffer) {
		if (arParamLoadFromBuffer(cameraParamBuffer, cameraParamBufferLen, &cparam) < 0) {
			ARController::logv(AR_LOG_LEVEL_ERROR, "FrameSource::open2: error-failed to load camera parameters from buffer, calling close(), exiting returning false");
			this->close();
			return false;
		}
		else {
			ARController::logv(AR_LOG_LEVEL_INFO, "Camera parameters loaded from buffer.");
		}
	}
	else {
		if (arParamLoad((cameraParam ? cameraParam : cparam_name_default), 1, &cparam) < 0) {
			ARController::logv(AR_LOG_LEVEL_ERROR, "ARWrapper::ARToolKitVideoSource::open2(): error-failed to load camera parameters %s, calling close(), exiting returning false",
				(cameraParam ? cameraParam : cparam_name_default));
			this->close();
			return false;
		}
		else {
			ARController::logv(AR_LOG_LEVEL_INFO, "Camera parameters loaded from file '%s'.", (cameraParam ? cameraParam : cparam_name_default));
		}
	}

	if (cparam.xsize != frameWidth || cparam.ysize != frameHeight) {
		ARController::logv(AR_LOG_LEVEL_DEBUG, "*** Camera Parameter resized from %d, %d. ***", cparam.xsize, cparam.ysize);
		arParamChangeSize(&cparam, frameWidth, frameHeight, &cparam);
	}
	if (!(cparamLT = arParamLTCreate(&cparam, AR_PARAM_LT_DEFAULT_OFFSET))) {
		ARController::logv(AR_LOG_LEVEL_ERROR, "Error: Failed to create camera parameters lookup table.");
		this->close();
		return false;
	}
	cameraParamLoaded = true;
	return cameraParamLoaded;
}

bool FrameSource::close()
{
	ARController::logv(AR_LOG_LEVEL_DEBUG, "FrameSource::close(): called");
	cameraParamLoaded = false;
	if (cparamLT) arParamLTFree(&cparamLT);
	frameBuffer = NULL;
	return true;
}



ARParamLT* FrameSource::getCameraParameters() {
	return cparamLT;
}

int FrameSource::getFrameWidth() {
	return frameWidth;
}

int FrameSource::getFrameHeight() {
	return frameHeight;
}


AR_PIXEL_FORMAT FrameSource::getPixelFormat() {
	return pixelFormat;
}

ARUint8* FrameSource::getFrame() {
	return frameBuffer;
}

void FrameSource::setFrame(ARUint8* frame) {
	frameBuffer = frame;
	frameStamp++;
}

int FrameSource::getFrameStamp() {
	return frameStamp;
}

const char * FrameSource::getName()
{
	return "Frame Source for UWP (HoloLens)";
}




