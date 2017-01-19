#pragma once
#define ARDOUBLE_IS_FLOAT
#include <AR/ar.h>
#include <AR/arFilterTransMat.h>
#include <ARPattern.h>

class ARMarker {
public:
	enum MarkerType {
		SINGLE,		// standard single square marker
		MULTI		// composite marker made up of multiple single markers
	};
	MarkerType type;
	int UID;		// internal unique ID, not same as pattern ID

	static ARMarker* newWithConfig(const char* cfg, ARPattHandle *arPattHandle);

	bool visiblePrev;           // Whether or not the marker was visible prior to last update.
	bool visible;				// Whether or not the marker is visible at current time.
	
	ARdouble transformationMatrix[16];

	int patternCount;			// number of patterns in this marker (1 for single marker)
	ARPattern** patterns;		// array of patterns in this marker

	/**
	* Constructor takes the type of this marker.
	*/
	ARMarker(MarkerType type);
	virtual ~ARMarker();


	void setPositionScalefactor(ARdouble scale);
	ARdouble positionScalefactor();


	/**
	* Completes an update begun in the parent class, performing filtering
	* Subclasses should first do their required updates, set visible, visiblePrev,
	* then call ARMarker::update().
	* @return true if successful, false if an error occurred
	*/
	virtual bool update();

	/**
	* Returns the specified pattern within this marker.
	* @param n		The pattern to retrieve
	*/
	ARPattern* getPattern(int n);


	// Filter control.
	void setFiltered(bool flag);
	bool isFiltered();
	ARdouble filterSampleRate();
	void setFilterSampleRate(ARdouble rate);
	ARdouble filterCutoffFrequency();
	void setFilterCutoffFrequency(ARdouble freq);

	ARdouble trans[3][4];		// transformation from camera to this marker

protected:
	/**
	* Allocates space for patterns within this marker.
	* @param count	The number of patterns to allocate
	*/
	void allocatePatterns(int count);

	/**
	* Frees allocated patterns and resets the pattern count to zero.
	*/
	void freePatterns();

	ARdouble m_positionScaleFactor;




private:
	ARFilterTransMatInfo *m_ftmi;
	ARdouble   m_filterCutoffFrequency;
	ARdouble   m_filterSampleRate;
};


