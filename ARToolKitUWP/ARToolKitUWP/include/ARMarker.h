/*
*  ARMarker.h
*  ARToolKitUWP
*
*  This work is a modified version of the original "ARMarker.h" of
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
*  ARMarker.h
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
*  Author(s): Julian Looser, Philip Lamb
*
*/

#pragma once
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


