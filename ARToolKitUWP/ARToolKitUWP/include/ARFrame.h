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
