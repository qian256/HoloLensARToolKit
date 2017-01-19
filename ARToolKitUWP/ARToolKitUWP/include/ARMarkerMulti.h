#pragma once

#include <ARMarker.h>
#include <AR/arMulti.h>


/**
* Multiple marker type of ARMarker.
*/
class ARMarkerMulti : public ARMarker {

private:
	bool m_loaded;

protected:
	bool unload();

public:

	ARMultiMarkerInfoT *config;							///< Structure holding information about the multimarker patterns
	bool robustFlag;									///< Flag specifying which pose estimation approach to use

	ARMarkerMulti();
	~ARMarkerMulti();

	bool load(const char *multiConfig, ARPattHandle *arPattHandle);

	/**
	* Updates the marker with new tracking info.
	* Then calls ARMarker::update()
	* @param markerInfo		Array containing detected marker information
	* @param markerNum			Number of items in the array
	* @param ar3DHandle        AR3DHandle used to extract marker pose.
	*/
	bool updateWithDetectedMarkers(ARMarkerInfo *markerInfo, int markerNum, AR3DHandle *ar3DHandle);
};



