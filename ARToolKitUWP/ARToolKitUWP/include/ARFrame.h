/*
*  ARFrame.h
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

#pragma once

#include <AR/ar.h>

class FrameSource {
private:
	bool cameraParamLoaded;

	// camera parameters
	char* cameraParam;					///< Camera parameter filename
	char* cameraParamBuffer;
	size_t cameraParamBufferLen;
	ARParamLT *cparamLT;				///< Camera paramaters

	int frameWidth;						///< Width of the frame in pixels
	int frameHeight;					///< Height of the frame in pixels
	AR_PIXEL_FORMAT pixelFormat;		///< Pixel format from ARToolKit enumeration.

	ARUint8 *frameBuffer;               ///< Pointer to latest frame. Set by concrete subclass to point to frame data.
	int frameStamp;						///< Latest framestamp. Incremented in the concrete subclass when a new frame arrives.

public:
	static FrameSource* newFrameSource(int width, int height, AR_PIXEL_FORMAT format);

	FrameSource(int width, int height, AR_PIXEL_FORMAT format);
	~FrameSource();


	void configure(const char* cparaName, const char* cparaBuff, size_t cparaBuffLen);
	// actual loading of camera parameters
	bool open();
	bool close();

	ARParamLT* getCameraParameters();

	int getFrameWidth();
	int getFrameHeight();
	AR_PIXEL_FORMAT getPixelFormat();

	ARUint8* getFrame();
	void setFrame(ARUint8* frame);
	int getFrameStamp();
	
	const char* getName();
};
