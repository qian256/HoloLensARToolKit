/*
*  ARMarkerMulti.h
*  ARToolKitUWP
*
*  This work is a modified version of the original "ARMarkerMulti.h" of
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
*  ARMarkerMulti.h
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
*  Author(s): Philip Lamb.
*
*/

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



