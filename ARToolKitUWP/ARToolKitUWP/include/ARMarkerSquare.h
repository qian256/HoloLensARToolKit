#pragma once
#include <ARMarker.h>

#define    AR_PATTERN_TYPE_TEMPLATE    0
#define    AR_PATTERN_TYPE_MATRIX      1


/**
* Single marker type of ARMarker.
*/
class ARMarkerSquare : public ARMarker {

private:
	bool m_loaded;

protected:
	ARPattHandle *m_arPattHandle;
	ARdouble m_width;
	ARdouble m_cf;
	ARdouble m_cfMin;

	bool unload();

public:

	int patt_id;							///< Unique pattern ID provided by ARToolKit
	int patt_type;

	ARMarkerSquare();
	~ARMarkerSquare();

	bool useContPoseEstimation;
	ARdouble getConfidence();
	ARdouble getConfidenceCutoff();
	void setConfidenceCutoff(ARdouble value);

	bool initWithPatternFile(const char* path, ARdouble width, ARPattHandle *arPattHandle);
	bool initWithPatternFromBuffer(const char* buffer, ARdouble width, ARPattHandle *arPattHandle);
	bool initWithBarcode(int barcodeID, ARdouble width);

	/**
	* Updates the marker with new tracking info.
	* Then calls ARMarker::update()
	* @param markerInfo		Array containing detected marker information
	* @param markerNum			Number of items in the array
	* @param ar3DHandle        AR3DHandle used to extract marker pose.
	*/
	bool updateWithDetectedMarkers(ARMarkerInfo* markerInfo, int markerNum, AR3DHandle *ar3DHandle);
};

