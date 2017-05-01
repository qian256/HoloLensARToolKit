/*
*  ARToolKitUWP.h
*  ARToolKitUWP
*
*  This work is a modified version of the original "ARToolKitWrapperExportedAPI.h" of
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
*  ARToolKitWrapperExportedAPI.h
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


#pragma once
#include <AR/ar.h>
#include <ARController.h>
#define EXPORT_API __declspec(dllexport)


extern "C" {



	EXPORT_API bool aruwpInitialiseAR(int width, int height, int pixelSize);
	EXPORT_API bool aruwpInitialiseARWithOptions(int width, int height, int pixelSize, const int pattSize, const int pattCountMax);
	EXPORT_API bool aruwpGetARToolKitVersion(char *buffer, int length);
	EXPORT_API bool aruwpStartRunning(const char *cparaName);
	EXPORT_API bool aruwpStartRunningBuffer(const char *cparaBuff, const int cparaBuffLen);

	EXPORT_API bool aruwpIsRunning();
	EXPORT_API bool aruwpStopRunning();
	EXPORT_API bool aruwpShutdownAR();

	EXPORT_API bool aruwpGetFrameParams(int *width, int *height, int *pixelSize, char *pixelFormatStringBuffer, int pixelFormatStringBufferLen);

	/**
	* Performs detection and marker updates. The newest frame from the video source is retrieved and
	* analysed. All loaded markers are updated.
	* @return			true if successful, false if an error occurred
	*/
	EXPORT_API bool aruwpUpdate(ARUint8* frame);

	// setter and getter
	EXPORT_API void aruwpSetVideoThreshold(int threshold);
	EXPORT_API int aruwpGetVideoThreshold();
	EXPORT_API void aruwpSetVideoThresholdMode(int mode);
	EXPORT_API int aruwpGetVideoThresholdMode();
	EXPORT_API void aruwpSetLabelingMode(int mode);
	EXPORT_API int aruwpGetLabelingMode();
	EXPORT_API void aruwpSetPatternDetectionMode(int mode);
	EXPORT_API int aruwpGetPatternDetectionMode();
	EXPORT_API void aruwpSetBorderSize(ARdouble size);
	EXPORT_API ARdouble aruwpGetBorderSize();
	EXPORT_API void aruwpSetMatrixCodeType(int type);
	EXPORT_API int aruwpGetMatrixCodeType();
	EXPORT_API void aruwpSetImageProcMode(int mode);
	EXPORT_API int aruwpGetImageProcMode();

	// marker management
	/**
	* Adds a marker as specified in the given configuration string. The format of the string can be
	* one of:
	* - Single marker:		"single;pattern_file;pattern_width", e.g. "single;data/hiro.patt;80"
	* - Multi marker:		"multi;config_file", e.g. "multi;data/multi/marker.dat"
	* @param cfg		The configuration string
	* @return			The unique identifier (UID) of the marker instantiated based on the configuration string, or -1 if an error occurred
	*/
	EXPORT_API int aruwpAddMarker(const char *cfg);
	/**
	* Removes the marker with the given unique identifier (UID).
	* @param markerUID	The unique identifier (UID) of the marker to remove
	* @return			true if the marker was removed, false if an error occurred
	*/
	EXPORT_API bool aruwpRemoveMarker(int markerUID);
	/**
	* Clears the collection of markers.
	* @return			The number of markers removed
	*/
	EXPORT_API int aruwpRemoveAllMarkers();

	/**
	* Returns the visibility status of the specified marker. After a call to aruwpUpdate, all marker
	* information will be current. Any marker can then be checked for visibility in the current frame.
	* @param markerUID	The unique identifier (UID) of the marker to query
	* @return			true if the specified marker is visible, false if not, or an error occurred
	*/
	EXPORT_API bool aruwpQueryMarkerVisibility(int markerUID);
	/**
	* Populates the provided float array with the current transformation for the specified marker. After
	* a call to aruwpUpdate, all marker information will be current. Marker transformations can then be
	* checked. If the specified marker is not found the last good transformation is used, and false is
	* returned.
	* @param markerUID	The unique identifier (UID) of the marker to query
	* @param matrix	The float array to populate with an OpenGL compatible transformation matrix
	* @return			true if the specified marker is visible, false if not, or an error occurred
	*/
	EXPORT_API bool aruwpQueryMarkerTransformation(int markerUID, ARdouble trans[12]);
	/**
	* Returns the number of pattern images associated with the specified marker. A single marker has one pattern
	* image. A multimarker has one or more pattern images.
	* Images of NFT markers are not currently supported, so at present this function will return 0 for NFT markers.
	* @param markerUID	The unique identifier (UID) of the marker
	* @return			The number of pattern images
	*/
	EXPORT_API int aruwpGetMarkerPatternCount(int markerUID);
	/**
	* Gets configuration of a pattern associated with a marker.
	* @param markerUID	The unique identifier (UID) of the marker
	* @param patternID	The id of the pattern within the marker, in range from 0 to aruwpGetMarkerPatternCount() - 1, inclusive. Ignored for single markers and NFT markers (i.e. 0 assumed).
	* @param matrix	The float array to populate with the 4x4 transformation matrix of the pattern (column-major order).
	* @param width		Float value to set to the width of the pattern
	* @param height	Float value to set to the height of the pattern.
	* @param imageSizeX Int value to set to the width of the pattern image (in pixels).
	* @param imageSizeY Int value to set to the height of the pattern image (in pixels).
	* @return			true if successful, false if an error occurred
	*/
	EXPORT_API bool aruwpGetMarkerPatternConfig(int markerUID, int patternID, ARdouble matrix[16], ARdouble *width, ARdouble *height, int *imageSizeX, int *imageSizeY);


	/**
	* Constants for use with marker option setters/getters.
	*/
	enum {
		ARUWP_MARKER_OPTION_FILTERED = 1,                         ///< bool, true for filtering enabled.
		ARUWP_MARKER_OPTION_FILTER_SAMPLE_RATE = 2,               ///< float, sample rate for filter calculations.
		ARUWP_MARKER_OPTION_FILTER_CUTOFF_FREQ = 3,               ///< float, cutoff frequency of filter.
		ARUWP_MARKER_OPTION_SQUARE_USE_CONT_POSE_ESTIMATION = 4,  ///< bool, true to use continuous pose estimate.
		ARUWP_MARKER_OPTION_SQUARE_CONFIDENCE = 5,                ///< float, confidence value of most recent marker match
		ARUWP_MARKER_OPTION_SQUARE_CONFIDENCE_CUTOFF = 6,         ///< float, minimum allowable confidence value used in marker matching.
		ARUWP_MARKER_OPTION_NFT_SCALE = 7,                        ///< float, scale factor applied to NFT marker size.
		ARUWP_MARKER_OPTION_MULTI_MIN_SUBMARKERS = 8,             ///< int, minimum number of submarkers for tracking to be valid.
		ARUWP_MARKER_OPTION_MULTI_MIN_CONF_MATRIX = 9,            ///< float, minimum confidence value for submarker matrix tracking to be valid.
		ARUWP_MARKER_OPTION_MULTI_MIN_CONF_PATTERN = 10,          ///< float, minimum confidence value for submarker pattern tracking to be valid.
	};

	// Bool is treated as integer 
	EXPORT_API void aruwpSetMarkerOptionBool(int markerUID, int option, int value);
	EXPORT_API bool aruwpGetMarkerOptionBool(int markerUID, int option);

	EXPORT_API void aruwpSetMarkerOptionInt(int markerUID, int option, int value);
	EXPORT_API void aruwpSetMarkerOptionFloat(int markerUID, int option, ARdouble value);
	EXPORT_API int aruwpGetMarkerOptionInt(int markerUID, int option);
	EXPORT_API ARdouble aruwpGetMarkerOptionFloat(int markerUID, int option);







	/**
	* Registers a callback function to use when a message is logged.
	* If the callback is to become invalid, be sure to call this function with NULL
	* first so that the callback is unregistered.
	*/
	EXPORT_API void aruwpRegisterLogCallback(PFN_LOGCALLBACK callback);

	EXPORT_API void aruwpSetLogLevel(const int logLevel);
}



