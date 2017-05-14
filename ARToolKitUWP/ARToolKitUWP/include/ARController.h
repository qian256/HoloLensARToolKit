/*
*  ARController.h
*  ARToolKitUWP
*  
*  This work is a modified version of the original "ARController.h" of
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
*  ARController.h
*  ARToolKit5
*
*  A C++ class encapsulating core controller functionality of ARToolKit.
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

#pragma once

#include <AR/ar.h>
#include <AR/arMulti.h>
#include <ARMarker.h>
#include <ARMarkerSquare.h>
#include <ARMarkerMulti.h>
#include <ARFrame.h>

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>



#define CALL_CONV __stdcall
#define LOGI(...) fprintf(stdout, __VA_ARGS__)
#define LOGE(...) fprintf(stderr, __VA_ARGS__)
typedef void (CALL_CONV *PFN_LOGCALLBACK)(const char* msg);


/**
* Wrapper for ARToolKit functionality. This class handles ARToolKit initialisation, updates,
* and cleanup. It maintains a collection of markers, providing methods to add and remove them.
*/
class ARController {
private:
	typedef enum {
		NOTHING_INITIALISED,			///< No initialisation yet and no resources allocated.
		BASE_INITIALISED,				///< Marker management initialised, markers can be added.
		// WAITING_FOR_FRAME,				///< Marker added done, waiting for frame source to become ready.
		DETECTION_RUNNING				///< Frame running, additional initialisation occurred, marker detection running.
	} ARToolKitState;

	ARToolKitState state;				///< Current state of operation, progress through initialisation
	char* versionString;				///< ARToolKit version string
	
	// ARToolKit configuration. These allow for configuration prior to starting ARToolKit.
	int threshold;
	AR_LABELING_THRESH_MODE thresholdMode;
	int imageProcMode;
	int labelingMode;
	ARdouble pattRatio;
	int patternDetectionMode;
	AR_MATRIX_CODE_TYPE matrixCodeType;

	std::vector<ARMarker *> markers;    ///< List of markers.

	FrameSource* frameSource;

	bool doMarkerDetection;				// whether there is a marker
	// ARToolKit data.
	ARHandle *m_arHandle;				///< Structure containing general ARToolKit tracking information
	ARPattHandle *m_arPattHandle;			///< Structure containing information about trained patterns
	AR3DHandle *m_ar3DHandle;		    ///< Structure used to compute 3D poses from tracking data
	
	int frameWidth, frameHeight;
	AR_PIXEL_FORMAT pixelFormat;

	//
	// Internal marker management.
	//


	bool addMarker(ARMarker* marker);
	bool removeMarker(ARMarker* marker);

	//
	// Convenience initialisers.
	//

	bool initARMore(void);


	static void logvBuf(va_list args, const char* format, char **bufPtr, int* lenPtr);
	static void logvWriteBuf(char* buf, int len, const int logLevel);


public:
	ARController();
	ARController(int width, int height, int pixelSize);

	/**
	* Destructor.
	*/
	~ARController();


	static void logv(const int logLevel, const char* format, ...);

	/**
	* If a log callback has been set, then this passes the formatted message to it.
	* If no log callback has been set, then the message is discarded.
	* @param msg		The message to output. Follows the same formatting rules as printf().
	*/
	static void logv(const char* msg, ...);


	static PFN_LOGCALLBACK logCallback;		///< Callback where log messages are passed to

	/**
	* Returns a string containing the ARToolKit version, such as "4.5.1".
	* @return		The ARToolKit version
	*/
	const char* getARToolKitVersion();

	/**
	* Start marker management so markers can be added and removed.
	* @param patternSize For any square template (pattern) markers, the number of rows and columns in the template.
	* @param patternCountMax For any square template (pattern) markers, the maximum number of markers that may be loaded for a single matching pass. Must be > 0.
	* @return       true if initialisation was OK, false if an error occured.
	*/
	bool initialiseBase(const int patternSize = AR_PATT_SIZE1, const int patternCountMax = AR_PATT_NUM_MAX);

	/**
	* Report whether a marker can be added. Markers can be added once basic
	* initialisation has occurred.
	* @return       true if adding a marker is currently possible
	*/
	bool canAddMarker();

	bool isRunning();

	/**
	* Load camera parameters and start marker detection. (AR/NFT initialisation will begin on a subsequent call to update().)
	* @param cparaName		Camera parameters filename, or NULL if camera parameters file not being used.
	* @param cparaBuff		A byte-buffer containing contents of a camera parameters file, or NULL if a camera parameters file is being used.
	* @param cparaBuffLen	Length (in bytes) of cparaBuffLen, or 0 if a camera parameters file is being used.
	* @return				true if video capture and marker detection was started, otherwise false.
	*/
	bool startRunning(const char* cparaName, const char* cparaBuff, const long cparaBuffLen);
	bool stopRunning();

	bool frameParameters(int *width, int *height, AR_PIXEL_FORMAT *pixelFormat);
	void setFrameWidthHeight(int width, int height);
	void setPixelFormat(int format);
	bool shutdown();

	int addMarker(const char* cfg);
	bool removeMarker(int UID);
	int removeAllMarkers();
	int countMarkers();
	ARMarker* findMarker(int UID);


	/**
	* Performs marker detection and updates all markers. The latest frame from the current
	* video source is retrieved and analysed. Each marker in the collection is updated with
	* new tracking information. The marker info array is
	* iterated over, and detected markers are matched up with those in the marker collection. Each matched
	* marker is updated with visibility and transformation information. Any markers not detected are considered
	* not currently visible.
	*
	* @return				true if update completed successfully, false if an error occurred
	*/
	bool update(ARUint8* frame);


	// setter and getter
	void setThreshold(int thresh);
	int getThreshold() const;

	void setThresholdMode(int mode);
	int getThresholdMode() const;

	void setLabelingMode(int mode);
	int getLabelingMode() const;

	void setPatternDetectionMode(int mode);
	int getPatternDetectionMode() const;

	void setPattRatio(ARdouble ratio);
	ARdouble getPattRatio() const;

	void setMatrixCodeType(int type);
	int getMatrixCodeType() const;


	void setImageProcMode(int mode);
	int getImageProcMode() const;
	
};
