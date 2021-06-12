/*
 *  ARMarkerNFT.cpp
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

#include "pch.h"
#include <ARMarkerNFT.h>

ARMarkerNFT::ARMarkerNFT() : ARMarker(NFT),
    m_loaded(false),
    m_nftScale(1.0f),
    pageNo(-1),
    datasetPathname(NULL)
{
}

ARMarkerNFT::~ARMarkerNFT()
{
	if (m_loaded) unload();
}

bool ARMarkerNFT::load(const char* dataSetPathname_in)
{
    if (m_loaded) unload();
    
	visible = visiblePrev = false;
	
    // Load AR2 data.
    ARController::logv(AR_LOG_LEVEL_INFO, "Loading %s.fset.", dataSetPathname_in);
    if ((surfaceSet = ar2ReadSurfaceSet(dataSetPathname_in, "fset", NULL)) == NULL) {
        ARController::logv(AR_LOG_LEVEL_ERROR, "Error reading data from %s.fset", dataSetPathname_in);
        return (false);
    }
 	datasetPathname = strdup(dataSetPathname_in);
    
    allocatePatterns(1);
    patterns[0]->loadISet(surfaceSet->surface[0].imageSet, m_nftScale);
   
    m_loaded = true;
    
	return true;
}

bool ARMarkerNFT::unload()
{
    if (m_loaded) {
        freePatterns();
        pageNo = -1;
        if (surfaceSet) {
            ARController::logv(AR_LOG_LEVEL_INFO, "Unloading %s.fset.", datasetPathname);
            ar2FreeSurfaceSet(&surfaceSet); // Sets surfaceSet to NULL.
        }
        if (datasetPathname) {
            free(datasetPathname);
            datasetPathname = NULL;
        }
        
        m_loaded = false;
    }
	return true;
}

bool ARMarkerNFT::updateWithNFTResults(int detectedPage, float trackingTrans[3][4])
{
    if (!m_loaded) return false;
    
	visiblePrev = visible;
    
    // The marker will only have a pageNo if the data has actually been loaded by a call to ARController::loadNFTData().
	if (pageNo >= 0 && pageNo == detectedPage) {
        visible = true;
        for (int j = 0; j < 3; j++) {
            trans[j][0] = (ARdouble)trackingTrans[j][0];
            trans[j][1] = (ARdouble)trackingTrans[j][1];
            trans[j][2] = (ARdouble)trackingTrans[j][2];
            trans[j][3] = (ARdouble)(trackingTrans[j][3] * m_nftScale);
        }
	} else visible = false;

	return (ARMarker::update()); // Parent class will finish update.
}

void ARMarkerNFT::setNFTScale(const float scale)
{
    m_nftScale = scale;
    patterns[0]->loadISet(surfaceSet->surface[0].imageSet, m_nftScale);
}

float ARMarkerNFT::getNFTScale()
{
    return (m_nftScale);
}
