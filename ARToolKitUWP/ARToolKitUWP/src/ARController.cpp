/*
*  ARController.cpp
*  ARToolKitUWP
*
*  This work is a modified version of the original "ARController.cpp" of
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
*  ARController.cpp
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

#include "pch.h"
#include <ARController.h>
#include <algorithm>
#include <string>
#include "trackingSub.h"
#include <stdarg.h>


ARController::ARController() :
	state(NOTHING_INITIALISED),
	versionString(NULL),
	threshold(AR_DEFAULT_LABELING_THRESH),
	thresholdMode(AR_LABELING_THRESH_MODE_DEFAULT),
	imageProcMode(AR_DEFAULT_IMAGE_PROC_MODE),
	labelingMode(AR_DEFAULT_LABELING_MODE),
	pattRatio(AR_PATT_RATIO),
	patternDetectionMode(AR_DEFAULT_PATTERN_DETECTION_MODE),
	matrixCodeType(AR_MATRIX_CODE_TYPE_DEFAULT),
	markers(),
	doMarkerDetection(false),
	m_arHandle(NULL),
	m_arPattHandle(NULL),
	m_ar3DHandle(NULL),
    doNFTMarkerDetection(false),
    m_nftMultiMode(false),
    m_kpmRequired(true),
    m_kpmBusy(false),
	trackingThreadHandle(NULL),
    m_ar2Handle(NULL),
	m_kpmHandle(NULL),
	frameWidth(896),
	frameHeight(504),
	pixelFormat(AR_PIXEL_FORMAT_RGBA)
{
	for (int i = 0; i < PAGES_MAX; i++) surfaceSet[i] = NULL;
}

ARController::ARController(int width, int height, int format) :
	state(NOTHING_INITIALISED),
	versionString(NULL),
	threshold(AR_DEFAULT_LABELING_THRESH),
	thresholdMode(AR_LABELING_THRESH_MODE_DEFAULT),
	imageProcMode(AR_DEFAULT_IMAGE_PROC_MODE),
	labelingMode(AR_DEFAULT_LABELING_MODE),
	pattRatio(AR_PATT_RATIO),
	patternDetectionMode(AR_DEFAULT_PATTERN_DETECTION_MODE),
	matrixCodeType(AR_MATRIX_CODE_TYPE_DEFAULT),
	markers(),
	doMarkerDetection(false),
	m_arHandle(NULL),
	m_arPattHandle(NULL),
	m_ar3DHandle(NULL),
	doNFTMarkerDetection(false),
    m_nftMultiMode(false),
    m_kpmRequired(true),
    m_kpmBusy(false),
	trackingThreadHandle(NULL),
    m_ar2Handle(NULL),
	m_kpmHandle(NULL),
	frameWidth(width),
	frameHeight(height),
	pixelFormat(AR_PIXEL_FORMAT(format))
{
	for (int i = 0; i < PAGES_MAX; i++) surfaceSet[i] = NULL;
}


ARController::~ARController()
{
	shutdown();
	if (versionString) free(versionString);
}


const char* ARController::getARToolKitVersion()
{
	if (!versionString) arGetVersion(&versionString);
	return versionString;
}



bool ARController::initialiseBase(const int patternSize, const int patternCountMax)
{
	logv(AR_LOG_LEVEL_DEBUG, "ARWrapper::ARController::initialiseBase(): called");
	if (state != NOTHING_INITIALISED) {
		logv(AR_LOG_LEVEL_ERROR, "Initialise called while already initialised. Will shutdown first, exiting, returning false");
		if (!shutdown())
			return false;
	}
	logv(AR_LOG_LEVEL_INFO, "ARWrapper::ARController::initialiseBase(): Initialising...");
	
	// Create pattern handle so markers can begin to be added.
	if ((m_arPattHandle = arPattCreateHandle2(patternSize, patternCountMax)) == NULL) {
		logv(AR_LOG_LEVEL_ERROR, "Error: arPattCreateHandle2, exiting, returning false");
		return false;
	}

	state = BASE_INITIALISED;

	logv(AR_LOG_LEVEL_DEBUG, "ARWrapper::ARController::initialiseBase(): Initialised, exiting, returning true");
	return true;
}


void ARController::setFrameWidthHeight(int width, int height) {
	frameWidth = width;
	frameHeight = height;
}

void ARController::setPixelFormat(int format) {
	pixelFormat = (AR_PIXEL_FORMAT)format;
}


bool ARController::initARMore(void)
{
	logv(AR_LOG_LEVEL_INFO, "ARController::initARMore() called");

	// Create AR handle
	if ((m_arHandle = arCreateHandle(frameSource->getCameraParameters())) == NULL) {
		logv(AR_LOG_LEVEL_ERROR, "ARController::initARMore(): Error: arCreateHandle()");
		goto bail;
	}

	// Set the pixel format
	if (arSetPixelFormat(m_arHandle, frameSource->getPixelFormat()) < 0) {
		logv(AR_LOG_LEVEL_ERROR, "ARController::initARMore(): Error: arSetPixelFormat");
		goto bail1;
	}

	arPattAttach(m_arHandle, m_arPattHandle);

	// Set initial configuration. One call for each configuration option.
	arSetLabelingThresh(m_arHandle, threshold);
	arSetLabelingThreshMode(m_arHandle, thresholdMode);
	arSetImageProcMode(m_arHandle, imageProcMode);
	arSetLabelingMode(m_arHandle, labelingMode);
	arSetPattRatio(m_arHandle, pattRatio);
	arSetPatternDetectionMode(m_arHandle, patternDetectionMode);
	arSetMatrixCodeType(m_arHandle, matrixCodeType);

	// Create 3D handle
	if ((m_ar3DHandle = ar3DCreateHandle(&frameSource->getCameraParameters()->param)) == NULL) {
		logv(AR_LOG_LEVEL_ERROR, "ARController::initARMore(): Error: ar3DCreateHandle");
		goto bail1;
	}

	logv(AR_LOG_LEVEL_DEBUG, "ARController::initARMore() exiting, returning true");
	return true;

bail1:
	arDeleteHandle(m_arHandle);
	m_arHandle = NULL;
bail:
	logv(AR_LOG_LEVEL_ERROR, "ARController::initARMore() exiting, returning false");
	return false;
}


bool ARController::startRunning(const char* cparaName, const char* cparaBuff, const long cparaBuffLen)
{
	logv(AR_LOG_LEVEL_INFO, "ARController::startRunning(): called, start running");

	// Check for initialization before starting video
	if (state != BASE_INITIALISED) {
		logv(AR_LOG_LEVEL_ERROR, "ARController::startRunning(): Error: not initialized, exiting, returning false");
		return false;
	}

	frameSource = FrameSource::newFrameSource(frameWidth, frameHeight, pixelFormat);
	if (!frameSource) {
		logv(AR_LOG_LEVEL_ERROR, "ARController::startRunning(): Error: no frame source, exiting, returning false");
		return false;
	}

	frameSource->configure(cparaName, cparaBuff, cparaBuffLen);

	if (!frameSource->open()) {
		delete frameSource;
		frameSource = NULL;
		return false;
	}

	// v0.2
	initARMore();

	// v0.2
	state = DETECTION_RUNNING;

	logv(AR_LOG_LEVEL_DEBUG, "ARController::startRunning(): exiting, returning true");
	return true;
}


bool ARController::stopRunning()
{
	logv(AR_LOG_LEVEL_DEBUG, "ARController::stopRunning(): called");
	if (state != DETECTION_RUNNING ) {
		logv(AR_LOG_LEVEL_ERROR, "ARController::stopRunning(): Error: Not running.");
		return false;
	}
	
	// Tracking thread is holding a reference to the camera parameters. Closing the
    // video source will dispose of the camera parameters, thus invalidating this reference.
    // So must stop tracking before closing the video source.
    if (trackingThreadHandle) {
        logv(AR_LOG_LEVEL_DEBUG, "ARController::stopRunning(): calling unloadNFTData()");
        unloadNFTData();
    }
	

	if (frameSource) {
		logv(AR_LOG_LEVEL_DEBUG, "ARController::stopRunning(): if (frameSource) true");
		logv(AR_LOG_LEVEL_DEBUG, "ARController::stopRunning(): calling frameSource->close()");
		frameSource->close();
		delete frameSource;
		frameSource = NULL;
	}

    // NFT cleanup.
    //logv("Cleaning up ARToolKit NFT handles.");
    if (m_ar2Handle) {
        logv(AR_LOG_LEVEL_DEBUG, "ARController::stopRunning(): calling ar2DeleteHandle(&m_ar2Handle)");
        ar2DeleteHandle(&m_ar2Handle); // Sets m_ar2Handle to NULL.
    }
	
    if (m_kpmHandle) {
        logv(AR_LOG_LEVEL_DEBUG, "ARController::stopRunning(): calling kpmDeleteHandle(&m_kpmHandle)");
        kpmDeleteHandle(&m_kpmHandle); // Sets m_kpmHandle to NULL.
    }
	
	if (m_ar3DHandle) {
		logv(AR_LOG_LEVEL_DEBUG, "ARController::stopRunning(): calling ar3DDeleteHandle(&m_ar3DHandle)");
		ar3DDeleteHandle(&m_ar3DHandle); // Sets ar3DHandle0 to NULL.
	}

	if (m_arHandle) {
		logv(AR_LOG_LEVEL_DEBUG, "ARController::stopRunning(): if (m_arHandle0) true");
		arPattDetach(m_arHandle);
		arDeleteHandle(m_arHandle);
		m_arHandle = NULL;
	}

	state = BASE_INITIALISED;

	logv(AR_LOG_LEVEL_DEBUG, "ARController::stopRunning(): exiting, returning true");
	return true;
}


bool ARController::update(ARUint8* frame)
{
	//
	// check ARController state
	//
	if (state != DETECTION_RUNNING) {
		// v0.2
		// State is NOTHING_INITIALISED or BASE_INITIALISED.
		logv(AR_LOG_LEVEL_ERROR, "ARController::update(): Error-if (state != WAITING_FOR_VIDEO) true, exiting returning false");
		return false;
	}
	
	//
	// check frame and frameSource
	//
	if (!frame) {
		logv(AR_LOG_LEVEL_ERROR, "ARController::update(): no frame parsed, exiting returning true");
		return false;
	}
	if (!frameSource) {
		logv(AR_LOG_LEVEL_ERROR, "ARController::update(): Error-no video source or video source is closed, exiting returning false");
		return false;
	}
	else {
		frameSource->setFrame(frame);
	}

	//
	// Detect markers.
	//
	if (doMarkerDetection) {
		logv(AR_LOG_LEVEL_DEBUG, "ARController::update(): if (doMarkerDetection) true");

		ARMarkerInfo *markerInfo = NULL;
		int markerNum = 0;

		if (!m_arHandle) {
			// v0.2
			//if (!initARMore()) {
			logv(AR_LOG_LEVEL_ERROR, "ARController::update(): Error initialising AR, exiting returning false");
			return false;
			//}
		}

		if (m_arHandle) {
			if (arDetectMarker(m_arHandle, frame) < 0) {
				logv(AR_LOG_LEVEL_ERROR, "ARController::update(): Error: arDetectMarker(), exiting returning false");
				return false;
			}
			markerInfo = arGetMarker(m_arHandle);
			markerNum = arGetMarkerNum(m_arHandle);
			logv(AR_LOG_LEVEL_DEBUG, "ARController::update(): number of detected markers: %d", markerNum);
			for (int i = 0; i < markerNum; i++) {
				logv(AR_LOG_LEVEL_DEBUG, "ARController::update(): marker %d with id: %d", i, markerInfo[i].idMatrix);
			}
		}

		// Update square markers.
		bool success = true;
			for (std::vector<ARMarker *>::iterator it = markers.begin(); it != markers.end(); ++it) {
				if ((*it)->type == ARMarker::SINGLE) {
					success &= ((ARMarkerSquare *)(*it))->updateWithDetectedMarkers(markerInfo, markerNum, m_ar3DHandle);
				}
				else if ((*it)->type == ARMarker::MULTI) {
					success &= ((ARMarkerMulti *)(*it))->updateWithDetectedMarkers(markerInfo, markerNum, m_ar3DHandle);
				}
			}
	} // doMarkerDetection

	if (doNFTMarkerDetection) {
        logv(AR_LOG_LEVEL_DEBUG, "ARController::update(): if (doNFTMarkerDetection) true");
        
        if (!m_kpmHandle || !m_ar2Handle) {
            if (!initNFT()) {
                logv(AR_LOG_LEVEL_ERROR, "ARController::update(): Error initialising NFT, exiting returning false");
                return false;
            }
        }
		
        if (!trackingThreadHandle) {
            loadNFTData();
        }
        
        if (trackingThreadHandle) {            
            // Do KPM tracking.
            float err;
            float trackingTrans[3][4];
            
            if (m_kpmRequired) {
                if (!m_kpmBusy) {
                    trackingInitStart(trackingThreadHandle, frame);
                    m_kpmBusy = true;
                } else {
                    int ret;
                    int pageNo;
                    ret = trackingInitGetResult(trackingThreadHandle, trackingTrans, &pageNo);
                    if (ret != 0) {
                        m_kpmBusy = false;
                        if (ret == 1) {
                            if (pageNo >= 0 && pageNo < PAGES_MAX) {
								if (surfaceSet[pageNo]->contNum < 1) {
									//logv("Detected page %d.\n", pageNo);
									ar2SetInitTrans(surfaceSet[pageNo], trackingTrans); // Sets surfaceSet[page]->contNum = 1.
								}
                            } else {
                                logv(AR_LOG_LEVEL_ERROR, "ARController::update(): Detected bad page %d", pageNo);
                            }
                        } else /*if (ret < 0)*/ {
                            //logv("No page detected.");
                        }
                    }
                }
            }
            
            // Do AR2 tracking and update NFT markers.
            int page = 0;
            int pagesTracked = 0;
            bool success = true;
            ARdouble *transL2R = NULL;

            for (std::vector<ARMarker *>::iterator it = markers.begin(); it != markers.end(); ++it) {
                if ((*it)->type == ARMarker::NFT) {
                    
                    if (surfaceSet[page]->contNum > 0) {
                        if (ar2Tracking(m_ar2Handle, surfaceSet[page], frame, trackingTrans, &err) < 0) {
                            //logv("Tracking lost on page %d.", page);
                            success &= ((ARMarkerNFT *)(*it))->updateWithNFTResults(-1, NULL);
                        } else {
                            //logv("Tracked page %d (pos = {% 4f, % 4f, % 4f}).\n", page, trackingTrans[0][3], trackingTrans[1][3], trackingTrans[2][3]);
                            success &= ((ARMarkerNFT *)(*it))->updateWithNFTResults(page, trackingTrans);
                            pagesTracked++;
                        }
                    }
                    
                    page++;
                }
            }
            
            m_kpmRequired = (pagesTracked < (m_nftMultiMode ? page : 1));
            
        } // trackingThreadHandle
    } // doNFTMarkerDetection

	logv(AR_LOG_LEVEL_DEBUG, "ARController::update(): exiting, returning true");

	return true;
}

bool ARController::initNFT(void)
{
 	logv(AR_LOG_LEVEL_INFO, "ARController::initNFT(): called, initialising NFT");
    //
    // NFT init.
    //
    
    // KPM init.
    m_kpmHandle = kpmCreateHandle(frameSource->getCameraParameters(), frameSource->getPixelFormat());
    if (!m_kpmHandle) {
        logv(AR_LOG_LEVEL_ERROR, "ARController::initNFT(): Error: kpmCreatHandle, exiting, returning false");
        return (false);
    }
    //kpmSetProcMode( m_kpmHandle, KpmProcHalfSize );
    
    // AR2 init.
    if( (m_ar2Handle = ar2CreateHandle(frameSource->getCameraParameters(), frameSource->getPixelFormat(), AR2_TRACKING_DEFAULT_THREAD_NUM)) == NULL ) {
        logv(AR_LOG_LEVEL_ERROR, "ARController::initNFT(): Error: ar2CreateHandle, exiting, returning false");
        kpmDeleteHandle(&m_kpmHandle);
        return (false);
    }
    if (threadGetCPU() <= 1) {
        logv(AR_LOG_LEVEL_INFO, "Using NFT tracking settings for a single CPU");
        // Settings for devices with single-core CPUs.
        ar2SetTrackingThresh( m_ar2Handle, 5.0 );
        ar2SetSimThresh( m_ar2Handle, 0.50 );
        ar2SetSearchFeatureNum(m_ar2Handle, 16);
        ar2SetSearchSize(m_ar2Handle, 6);
        ar2SetTemplateSize1(m_ar2Handle, 6);
        ar2SetTemplateSize2(m_ar2Handle, 6);
    } else {
        logv(AR_LOG_LEVEL_INFO, "Using NFT tracking settings for more than one CPU");
        // Settings for devices with dual/multi-core CPUs.
        ar2SetTrackingThresh( m_ar2Handle, 5.0 );
        ar2SetSimThresh( m_ar2Handle, 0.50 );
        ar2SetSearchFeatureNum(m_ar2Handle, 16);
        ar2SetSearchSize(m_ar2Handle, 12);
        ar2SetTemplateSize1(m_ar2Handle, 6);
        ar2SetTemplateSize2(m_ar2Handle, 6);
    }
    logv(AR_LOG_LEVEL_DEBUG, "ARController::initNFT(): NFT initialisation complete, exiting, returning true");
    return (true);    
}

bool ARController::unloadNFTData(void)
{
    int i;
    
    if (trackingThreadHandle) {
        logv(AR_LOG_LEVEL_INFO, "Stopping NFT tracking thread.");
        trackingInitQuit(&trackingThreadHandle);
        m_kpmBusy = false;
    }
    for (i = 0; i < PAGES_MAX; i++) surfaceSet[i] = NULL; // Discard weak-references.
    m_kpmRequired = true;
    
    return true;
}

bool ARController::loadNFTData(void)
{
    // If data was already loaded, stop KPM tracking thread and unload previously loaded data.
    if (trackingThreadHandle) {
        logv(AR_LOG_LEVEL_INFO, "Reloading NFT data");
        unloadNFTData();
    } else {
        logv(AR_LOG_LEVEL_INFO, "Loading NFT data");
    }
    
    KpmRefDataSet *refDataSet = NULL;
    int pageCount = 0;
    
    for (std::vector<ARMarker *>::iterator it = markers.begin(); it != markers.end(); ++it) {
		if ((*it)->type == ARMarker::NFT) {
            // Load KPM data.
            KpmRefDataSet *refDataSet2;
            logv(AR_LOG_LEVEL_INFO, "Reading %s.fset3", ((ARMarkerNFT *)(*it))->datasetPathname);
            if (kpmLoadRefDataSet(((ARMarkerNFT *)(*it))->datasetPathname, "fset3", &refDataSet2) < 0) {
                logv(AR_LOG_LEVEL_ERROR, "Error reading KPM data from %s.fset3", ((ARMarkerNFT *)(*it))->datasetPathname);
                ((ARMarkerNFT *)(*it))->pageNo = -1;
                continue;
            }
            ((ARMarkerNFT *)(*it))->pageNo = pageCount;
            logv(AR_LOG_LEVEL_INFO, "  Assigned page no. %d.", pageCount);
            if (kpmChangePageNoOfRefDataSet(refDataSet2, KpmChangePageNoAllPages, pageCount) < 0) {
                logv(AR_LOG_LEVEL_ERROR, "ARController::loadNFTData(): Error: kpmChangePageNoOfRefDataSet, exit(-1)");
                exit(-1);
            }
            if (kpmMergeRefDataSet(&refDataSet, &refDataSet2) < 0) {
                logv(AR_LOG_LEVEL_ERROR, "ARController::loadNFTData(): Error: kpmMergeRefDataSet, exit(-1)");
                exit(-1);
            }
            logv(AR_LOG_LEVEL_INFO, "Done");
            
            // For convenience, create a weak reference to the AR2 data.
            surfaceSet[pageCount] = ((ARMarkerNFT *)(*it))->surfaceSet;
            
            pageCount++;
            if (pageCount == PAGES_MAX) {
                logv(AR_LOG_LEVEL_ERROR, "Maximum number of NFT pages (%d) loaded", PAGES_MAX);
                break;
            }
        }
    }
    if (kpmSetRefDataSet(m_kpmHandle, refDataSet) < 0) {
        logv(AR_LOG_LEVEL_ERROR, "ARController::loadNFTData(): Error: kpmSetRefDataSet, exit(-1)");
        exit(-1);
    }
    kpmDeleteRefDataSet(&refDataSet);
    
    // Start the KPM tracking thread.
    logv(AR_LOG_LEVEL_INFO, "Starting NFT tracking thread.");
    trackingThreadHandle = trackingInitInit(m_kpmHandle);
    if (!trackingThreadHandle) {
        logv(AR_LOG_LEVEL_ERROR, "ARController::loadNFTData(): trackingInitInit(), exit(-1)");
        exit(-1);
    }
    
    logv(AR_LOG_LEVEL_DEBUG, "Loading of NFT data complete, exiting, return true");
    return true;
}

bool ARController::shutdown()
{
	logv(AR_LOG_LEVEL_DEBUG, "ARController::shutdown(): called");
	do {
		switch (state) {
		case DETECTION_RUNNING:
		// v0.2
		//case WAITING_FOR_FRAME:
			logv(AR_LOG_LEVEL_DEBUG, "ARController::shutdown(): DETECTION_RUNNING or WAITING_FOR_FRAME, forcing stop.");
			stopRunning();
			break;

		case BASE_INITIALISED:
			if (countMarkers() > 0) {
				logv(AR_LOG_LEVEL_DEBUG, "ARController::shutdown(): BASE_INITIALISED, cleaning up markers.");
				removeAllMarkers();
			}

			if (m_arPattHandle) {
				arPattDeleteHandle(m_arPattHandle);
				m_arPattHandle = NULL;
			}

			state = NOTHING_INITIALISED;
			// Fall though.
		case NOTHING_INITIALISED:
			logv(AR_LOG_LEVEL_DEBUG, "ARController::shutdown(): NOTHING_INITIALISED, complete");
			break;
		}
	} while (state != NOTHING_INITIALISED);

	logv(AR_LOG_LEVEL_DEBUG, "ARController::shutdown(): exiting, returning true");
	return true;
}


bool ARController::canAddMarker()
{
	// Check we are in a valid state to add a marker (i.e. base initialisation has occurred)
	return (state != NOTHING_INITIALISED);
}

bool ARController::isRunning()
{
	return state == DETECTION_RUNNING;
}

bool ARController::frameParameters(int *width, int *height, AR_PIXEL_FORMAT *pixelFormat)
{
	FrameSource *fs = frameSource;
	if (!fs) return false;

	if (width) *width = fs->getFrameWidth();
	if (height) *height = fs->getFrameHeight();
	if (pixelFormat) *pixelFormat = fs->getPixelFormat();

	return true;
}


void ARController::setImageProcMode(int mode) {
	imageProcMode = mode;

	if (m_arHandle) {
		if (arSetImageProcMode(m_arHandle, mode) == 0) {
			logv(AR_LOG_LEVEL_INFO, "Image proc. mode set to %d.", imageProcMode);
		}
	}
}

int ARController::getImageProcMode() const
{
	return imageProcMode;
}

void ARController::setThreshold(int thresh)
{
	if (thresh < 0 || thresh > 255) return;
	threshold = thresh;
	if (m_arHandle) {
		if (arSetLabelingThresh(m_arHandle, threshold) == 0) {
			logv(AR_LOG_LEVEL_INFO, "Threshold set to %d", threshold);
		}
	}
}

int ARController::getThreshold() const
{
	return threshold;
}

void ARController::setThresholdMode(int mode)
{
	thresholdMode = (AR_LABELING_THRESH_MODE)mode;
	if (m_arHandle) {
		if (arSetLabelingThreshMode(m_arHandle, thresholdMode) == 0) {
			logv(AR_LOG_LEVEL_INFO, "Threshold mode set to %d", (int)thresholdMode);
		}
	}
}

int ARController::getThresholdMode() const
{
	return (int)thresholdMode;
}

void ARController::setLabelingMode(int mode)
{
	labelingMode = mode;
	if (m_arHandle) {
		if (arSetLabelingMode(m_arHandle, labelingMode) == 0) {
			logv(AR_LOG_LEVEL_INFO, "Labeling mode set to %d", labelingMode);
		}
	}
}

int ARController::getLabelingMode() const
{
	return labelingMode;
}

void ARController::setPatternDetectionMode(int mode)
{
	patternDetectionMode = mode;
	if (m_arHandle) {
		if (arSetPatternDetectionMode(m_arHandle, patternDetectionMode) == 0) {
			logv(AR_LOG_LEVEL_INFO, "Pattern detection mode set to %d.", patternDetectionMode);
		}
	}
}

int ARController::getPatternDetectionMode() const
{
	return patternDetectionMode;
}

void ARController::setPattRatio(ARdouble ratio)
{
	if (ratio <= 0.0f || ratio >= 1.0f) return;
	pattRatio = ratio;
	if (m_arHandle) {
		if (arSetPattRatio(m_arHandle, pattRatio) == 0) {
			logv(AR_LOG_LEVEL_INFO, "Pattern ratio size set to %f.", pattRatio);
		}
	}
}

ARdouble ARController::getPattRatio() const
{
	return pattRatio;
}

void ARController::setMatrixCodeType(int type)
{
	matrixCodeType = (AR_MATRIX_CODE_TYPE)type;
	if (m_arHandle) {
		if (arSetMatrixCodeType(m_arHandle, matrixCodeType) == 0) {
			logv(AR_LOG_LEVEL_INFO, "Matrix code type set to %d.", matrixCodeType);
		}
	}
}

int ARController::getMatrixCodeType() const
{
	return (int)matrixCodeType;
}

void ARController::setNFTMultiMode(bool on)
{
    m_nftMultiMode = on;
}

bool ARController::getNFTMultiMode() const
{
	return m_nftMultiMode;
}

int ARController::addMarker(const char* cfg)
{
	if (!canAddMarker()) {
		logv(AR_LOG_LEVEL_ERROR, "Error: Cannot add marker. ARToolKit not initialised");
		return -1;
	}

	ARMarker *marker = ARMarker::newWithConfig(cfg, m_arPattHandle);
	if (!marker) {
		logv(AR_LOG_LEVEL_ERROR, "Error: Failed to load marker.");
		return -1;
	}
	if (!addMarker(marker)) {
		return -1;
	}
	return marker->UID;
}


bool ARController::addMarker(ARMarker* marker)
{
	logv(AR_LOG_LEVEL_DEBUG, "ARController::addMarker(): called");
	if (!canAddMarker()) {
		logv(AR_LOG_LEVEL_ERROR, "Error: Cannot add marker. ARToolKit not initialised, exiting, returning false");
		return false;
	}

	if (!marker) {
		logv(AR_LOG_LEVEL_ERROR, "Error: Cannot add a NULL marker, exiting, returning false");
		return false;
	}

	markers.push_back(marker);

	if (marker->type == ARMarker::NFT) {
        if (!doNFTMarkerDetection)
            logv(AR_LOG_LEVEL_INFO, "First NFT marker added; enabling NFT marker detection.");
        doNFTMarkerDetection = true;
        if (trackingThreadHandle) {
            unloadNFTData(); // loadNFTData() will be called on next update().
        }
    } else {
        doMarkerDetection = true;
    }

	logv(AR_LOG_LEVEL_INFO, "Added marker (UID=%d), total markers loaded: %d.", marker->UID, countMarkers());
	return true;
}

bool ARController::removeMarker(int UID)
{
	ARMarker *marker = findMarker(UID);
	if (!marker) {
		logv(AR_LOG_LEVEL_ERROR, "ARController::removeMarker(): could not find marker (UID=%d).");
		return (false);
	}
	return removeMarker(marker);
}

bool ARController::removeMarker(ARMarker* marker)
{
	logv(AR_LOG_LEVEL_DEBUG, "ARController::removeMarker(): called");
	if (!marker) {
		logv(AR_LOG_LEVEL_ERROR, "ARController::removeMarker(): no marker specified, exiting, returning false");
		return false;
	}

	int UID = marker->UID;
	std::vector<ARMarker *>::iterator position = std::find(markers.begin(), markers.end(), marker);
	bool found = (position != markers.end());
	if (!found) {
		logv(AR_LOG_LEVEL_ERROR, "ARController::removeMarker(): Could not find marker (UID=%d), exiting, returning false", UID);
		return false;
	}
	if (marker->type == ARMarker::NFT && trackingThreadHandle) {
        unloadNFTData(); // If at least 1 NFT marker remains, loadNFTData() will be called on next update().
    }

	delete marker; // std::vector does not call destructor if it's a raw pointer being stored, so explicitly delete it.
	markers.erase(position);

	// Count each type of marker.
    int nftMarkerCount = 0;
    int squareMarkerCount = 0;
    std::vector<ARMarker *>::const_iterator it = markers.begin();
    while (it != markers.end()) {
        if ((*it)->type == ARMarker::NFT ) nftMarkerCount++;
        else squareMarkerCount++;
        ++it;
    }
    if (nftMarkerCount == 0) {
        if (doNFTMarkerDetection)
            logv(AR_LOG_LEVEL_INFO, "Last NFT marker removed; disabling NFT marker detection.");
        doNFTMarkerDetection = false;
    }
    if (squareMarkerCount == 0) {
        if (doMarkerDetection)
            logv(AR_LOG_LEVEL_INFO, "Last square marker removed; disabling square marker detection.");
        doMarkerDetection = false;
    }
    int markerCount = nftMarkerCount + squareMarkerCount;

	logv(AR_LOG_LEVEL_INFO, "Removed marker (UID=%d), now %d markers loaded", UID, markerCount);
	logv(AR_LOG_LEVEL_DEBUG, "ARController::removeMarker(): exiting, returning %s", ((found) ? "true" : "false"));
	return (found);
}

int ARController::removeAllMarkers()
{
	int count = countMarkers();

	if (trackingThreadHandle) {
        unloadNFTData();
    }
	markers.clear();
	doMarkerDetection = false;
	doNFTMarkerDetection = false;
	logv(AR_LOG_LEVEL_INFO, "Removed all %d markers.", count);

	return count;
}

int ARController::countMarkers()
{
	return (int)(markers.size());
}


ARMarker* ARController::findMarker(int UID)
{
	std::vector<ARMarker *>::const_iterator it = markers.begin();
	while (it != markers.end()) {
		if ((*it)->UID == UID) return (*it);
		++it;
	}
	return NULL;
}



// logging

static const char LOG_TAG[] = "ARController (native)";
PFN_LOGCALLBACK ARController::logCallback = NULL;


/*private: static*/void ARController::logvBuf(va_list args, const char* format, char** bufPtr, int* lenPtr)
{
#   ifdef _WIN32
	*lenPtr = _vscprintf(format, args);
	if (0 <= *lenPtr) {
		*bufPtr = (char *)malloc((*lenPtr + 1) * sizeof(char)); // +1 for nul-term.
		// The "+1" at the end is the fix by LQ
		// because vsnprintf is only copying n-1 bytes, as documented by cplusplus
		// addressing issue https://github.com/artoolkit/artoolkit5/issues/244
		vsnprintf(*bufPtr, *lenPtr+1, format, args);
		(*bufPtr)[*lenPtr] = '\0'; // nul-terminate.
	}
#   else
	*lenPtr = vasprintf(bufPtr, format, args) + 1;
#   endif
}

/*private: static*/void ARController::logvWriteBuf(char* buf, int len, const int logLevel)
{
	// Pre-pend a tag onto the message to identify the source as the C++ wrapper
	std::string ErrOrWarnBuf;
	if (AR_LOG_LEVEL_ERROR == logLevel)
		ErrOrWarnBuf = "[error]";
	else if (AR_LOG_LEVEL_WARN == logLevel)
		ErrOrWarnBuf = "[warning]";
	else if (AR_LOG_LEVEL_INFO == logLevel)
		ErrOrWarnBuf = "[info]";
	else if (AR_LOG_LEVEL_DEBUG == logLevel)
		ErrOrWarnBuf = "[debug]";
	else
		ErrOrWarnBuf = "";

	len += sizeof(LOG_TAG) + 2; // +2 for ": ".
	char* Out = (char *)malloc(sizeof(char) * ((len + 1) + strlen(ErrOrWarnBuf.c_str()))); // +1 for nul.
	if (NULL != Out) {
		sprintf(Out, "%s: %s%s", LOG_TAG, ErrOrWarnBuf.c_str(), buf);
		// Pass message to log callback if it's valid
		logCallback(Out);
		free(Out);
	}
}

/*public: static*/void ARController::logv(const int logLevel, const char* format, ...)
{
	// Check input for NULL
	if (logLevel < arLogLevel) return;
	if (!format) return;
	if (!logCallback) return;

	// Unpack msg formatting.
	char *Buf = NULL;
	int Len;
	va_list Ap;

	va_start(Ap, format);
	logvBuf(Ap, format, &Buf, &Len);
	va_end(Ap);

	if (Len >= 0)
		logvWriteBuf(Buf, Len, logLevel);

	if (NULL != Buf)
		free(Buf);
}

/*public: static*/void ARController::logv(const char* format, ...)
{
	// Check input for NULL
	if (!format) return;
	if (!logCallback) return;

	// Unpack msg formatting.
	char* Buf = NULL;
	int Len;
	va_list Ap;

	va_start(Ap, format);
	logvBuf(Ap, format, &Buf, &Len);
	va_end(Ap);

	if (Len >= 0)
		logvWriteBuf(Buf, Len, AR_LOG_LEVEL_ERROR);

	if (NULL != Buf)
		free(Buf);
}




