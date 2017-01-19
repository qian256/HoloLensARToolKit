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


/**
* Wrapper for ARToolKit functionality. This class handles ARToolKit initialisation, updates,
* and cleanup. It maintains a collection of markers, providing methods to add and remove them.
*/
class ARController {
private:
	typedef enum {
		NOTHING_INITIALISED,			///< No initialisation yet and no resources allocated.
		BASE_INITIALISED,				///< Marker management initialised, markers can be added.
		WAITING_FOR_FRAME,				///< Marker added done, waiting for frame source to become ready.
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


public:
	ARController();
	ARController(int width, int height, int pixelSize);

	/**
	* Destructor.
	*/
	~ARController();
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
