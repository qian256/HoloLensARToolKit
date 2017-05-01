/*
 *  arCreateHandle.c
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
 *  Copyright 2003-2015 ARToolworks, Inc.
 *
 *  Author(s): Hirokazu Kato, Philip Lamb
 *
 */
/*******************************************************
 *
 * Author: Hirokazu Kato
 *
 *         kato@sys.im.hiroshima-cu.ac.jp
 *
 * Revision: 5.1
 * Date: 03/08/13
 *
 *******************************************************/

#include <AR/ar.h>
#include <stdio.h>
#include <math.h>

ARHandle *arCreateHandle( ARParamLT *paramLT )
{
    ARHandle   *handle;

    arMalloc( handle, ARHandle, 1 );

    handle->arDebug                 = AR_DEBUG_DISABLE;
#if !AR_DISABLE_LABELING_DEBUG_MODE
    handle->labelInfo.bwImage       = NULL;
#endif
    handle->arImageProcInfo         = NULL;
    handle->arPixelFormat           = AR_PIXEL_FORMAT_INVALID;
    handle->arPixelSize             = 0;
    handle->arLabelingMode          = AR_DEFAULT_LABELING_MODE;
    handle->arLabelingThresh        = AR_DEFAULT_LABELING_THRESH;
    handle->arImageProcMode         = AR_DEFAULT_IMAGE_PROC_MODE;
    handle->arPatternDetectionMode  = AR_DEFAULT_PATTERN_DETECTION_MODE;
    handle->arMarkerExtractionMode  = AR_DEFAULT_MARKER_EXTRACTION_MODE;
    handle->pattRatio               = AR_PATT_RATIO;
    handle->matrixCodeType          = AR_MATRIX_CODE_TYPE_DEFAULT;

    handle->arParamLT           = paramLT;
    handle->xsize               = paramLT->param.xsize;
    handle->ysize               = paramLT->param.ysize;

    handle->marker_num          = 0;
    handle->marker2_num         = 0;
    handle->labelInfo.label_num = 0;
    handle->history_num         = 0;

    arMalloc( handle->labelInfo.labelImage, AR_LABELING_LABEL_TYPE, handle->xsize*handle->ysize );
    
    handle->pattHandle = NULL;
    
    arSetPixelFormat(handle, AR_DEFAULT_PIXEL_FORMAT);

    arSetDebugMode(handle, AR_DEFAULT_DEBUG_MODE);
    
    handle->arLabelingThreshMode = -1;
    arSetLabelingThreshMode(handle, AR_LABELING_THRESH_MODE_DEFAULT);
    arSetLabelingThreshModeAutoInterval(handle, AR_LABELING_THRESH_AUTO_INTERVAL_DEFAULT);
    
    return handle;
}

int arDeleteHandle( ARHandle *handle )
{
    if( handle == NULL ) return -1;

    if (handle->arImageProcInfo) {
        arImageProcFinal(handle->arImageProcInfo);
        handle->arImageProcInfo = NULL;
    }
    
    //if( handle->arParamLT != NULL ) arParamLTFree( &handle->arParamLT );
    free( handle->labelInfo.labelImage );
#if !AR_DISABLE_LABELING_DEBUG_MODE
    if (handle->labelInfo.bwImage) free( handle->labelInfo.bwImage );
#endif
    free( handle );

    return 0;
}

int arSetDebugMode( ARHandle *handle, int mode )
{
    if( handle == NULL ) return -1;

    if (handle->arDebug != mode) {
        handle->arDebug = mode;
#if !AR_DISABLE_LABELING_DEBUG_MODE
        if (mode == AR_DEBUG_DISABLE) {
            free(handle->labelInfo.bwImage);
            handle->labelInfo.bwImage = NULL;
        } else {
            arMalloc(handle->labelInfo.bwImage, ARUint8, handle->xsize * handle->ysize);
        }
#endif
    }
    return 0;
}

int arGetDebugMode( ARHandle *handle, int *mode )
{
    if (!handle || !mode) return -1;
    *mode = handle->arDebug;

    return 0;
}

int arSetLabelingMode( ARHandle *handle, int mode )
{
    if( handle == NULL ) return -1;

    switch( mode ) {
        case AR_LABELING_WHITE_REGION:
        case AR_LABELING_BLACK_REGION:
            break;
        default:
            return -1;
    }

    handle->arLabelingMode = mode;

    return 0;
}

int arGetLabelingMode( ARHandle *handle, int *mode )
{
    if (!handle || !mode) return -1;
    *mode = handle->arLabelingMode;

    return 0;
}

int arSetLabelingThresh( ARHandle *handle, int thresh )
{
    if( handle == NULL ) return -1;

    if( thresh < 0 || thresh > 255 ) return -1;

    handle->arLabelingThresh = thresh;
    
    return 0;
}

int arGetLabelingThresh( ARHandle *handle, int *thresh )
{
    if (!handle || !thresh) return -1;
    *thresh = handle->arLabelingThresh;

    return 0;
}

int arSetLabelingThreshMode(ARHandle *handle, const AR_LABELING_THRESH_MODE mode)
{
    AR_LABELING_THRESH_MODE mode1;

	if (!handle) return (-1);
    if (handle->arLabelingThreshMode != mode) {
        if (handle->arImageProcInfo) {
            arImageProcFinal(handle->arImageProcInfo);
            handle->arImageProcInfo = NULL;
        }

        mode1 = mode;
        switch (mode) {
            case AR_LABELING_THRESH_MODE_AUTO_MEDIAN:
            case AR_LABELING_THRESH_MODE_AUTO_OTSU:
#if !AR_DISABLE_THRESH_MODE_AUTO_ADAPTIVE
            case AR_LABELING_THRESH_MODE_AUTO_ADAPTIVE:
#endif
                handle->arImageProcInfo = arImageProcInit(handle->xsize, handle->ysize, handle->arPixelFormat, 0);
                break;
            case AR_LABELING_THRESH_MODE_AUTO_BRACKETING:
                handle->arLabelingThreshAutoBracketOver = handle->arLabelingThreshAutoBracketUnder = 1;
                break;
            case AR_LABELING_THRESH_MODE_MANUAL:
                break; // Do nothing.
            default:
                ARLOGe("Unknown or unsupported labeling threshold mode requested. Set to manual.\n");
                mode1 = AR_LABELING_THRESH_MODE_MANUAL;
        }
        handle->arLabelingThreshMode = mode1;
        if (handle->arDebug == AR_DEBUG_ENABLE) {
            const char *modeDescs[] = {
                "MANUAL",
                "AUTO_MEDIAN",
                "AUTO_OTSU",
                "AUTO_ADAPTIVE"
                "AUTO_BRACKETING"
            };
            ARLOGe("Labeling threshold mode set to %s.\n", modeDescs[mode1]);
        }
    }
    return (0);
}

int arGetLabelingThreshMode(const ARHandle *handle, AR_LABELING_THRESH_MODE *mode_p)
{
    if (!handle || !mode_p) return (-1);
    *mode_p = handle->arLabelingThreshMode;
    return (0);
}

int arSetLabelingThreshModeAutoInterval(ARHandle *handle, const int interval)
{
    if (!handle) return (-1);
    handle->arLabelingThreshAutoInterval = interval;
    handle->arLabelingThreshAutoIntervalTTL = 0;
    return (0);
}

int arGetLabelingThreshModeAutoInterval(const ARHandle *handle, int *interval_p)
{
    if (!handle || !interval_p) return (-1);
    *interval_p = handle->arLabelingThreshAutoInterval;
    return (0);
}

int arSetImageProcMode( ARHandle *handle, int mode )
{
    if( handle == NULL ) return -1;

    switch( mode ) {
        case AR_IMAGE_PROC_FRAME_IMAGE:
        case AR_IMAGE_PROC_FIELD_IMAGE:
            break;
        default:
            return -1;
    }

    handle->arImageProcMode = mode;

    return 0;
}

int arGetImageProcMode( ARHandle *handle, int *mode )
{
    if( handle == NULL ) return -1;
    *mode = handle->arImageProcMode;

    return 0;
}

int arSetPatternDetectionMode( ARHandle *handle, int mode )
{
    if( handle == NULL ) return -1;

    switch( mode ) {
        case AR_TEMPLATE_MATCHING_COLOR:
        case AR_TEMPLATE_MATCHING_MONO:
        case AR_MATRIX_CODE_DETECTION:
        case AR_TEMPLATE_MATCHING_COLOR_AND_MATRIX:
        case AR_TEMPLATE_MATCHING_MONO_AND_MATRIX:
            break;
        default:
            return -1;
    }

    handle->arPatternDetectionMode = mode;

    return 0;
}
 
int  arSetMatrixCodeType(ARHandle *handle, const AR_MATRIX_CODE_TYPE type)
{
    if (!handle) return (-1);
    handle->matrixCodeType = type;
    return (0);
}

int  arGetMatrixCodeType(ARHandle *handle, AR_MATRIX_CODE_TYPE *type_p)
{
    if (!handle || !type_p) return (-1);
    *type_p = handle->matrixCodeType;
    return (0);
}

int arGetPatternDetectionMode( ARHandle *handle, int *mode )
{
    if( handle == NULL ) return -1;
    *mode = handle->arPatternDetectionMode;

    return 0;
}

int arSetMarkerExtractionMode( ARHandle *handle, int mode )
{
    if( handle == NULL ) return -1;

    switch( mode ) {
        case AR_USE_TRACKING_HISTORY:
        case AR_NOUSE_TRACKING_HISTORY:
        case AR_USE_TRACKING_HISTORY_V2:
            break;
        default:
            return -1;
    }

    handle->arMarkerExtractionMode = mode;

    return 0;
}

int arGetMarkerExtractionMode( ARHandle *handle, int *mode )
{
    if( handle == NULL ) return -1;
    *mode = handle->arMarkerExtractionMode;

    return 0;
}

int arSetBorderSize( ARHandle *handle, const ARdouble borderSize )
{
    if( handle == NULL ) return -1;
    if (borderSize <= 0.0 || borderSize >= 0.5) return (-1);
    
    handle->pattRatio = 1.0 - 2.0*borderSize;
    return 0;    
}

int arGetBorderSize( ARHandle *handle, ARdouble *borderSize )
{
    if( handle == NULL ) return -1;
    *borderSize = (1.0 - handle->pattRatio)*0.5;
    
    return 0;
}

int arSetPattRatio( ARHandle *handle, const ARdouble pattRatio )
{
    if( handle == NULL ) return -1;
    if (pattRatio <= 0.0 || pattRatio >= 1.0) return (-1);
    
    handle->pattRatio = pattRatio;
    return 0;
}

int arGetPattRatio( ARHandle *handle, ARdouble *pattRatio )
{
    if( handle == NULL ) return -1;
    *pattRatio = handle->pattRatio;
    
    return 0;
}

int arSetPixelFormat( ARHandle *handle, AR_PIXEL_FORMAT pixFormat )
{
    int monoFormat;
    
    if (handle == NULL) return (-1);
    if (pixFormat == handle->arPixelFormat) return (0);

    switch( pixFormat ) {
        case AR_PIXEL_FORMAT_RGB:
        case AR_PIXEL_FORMAT_BGR:
        case AR_PIXEL_FORMAT_RGBA:
        case AR_PIXEL_FORMAT_BGRA:
        case AR_PIXEL_FORMAT_ABGR:
        case AR_PIXEL_FORMAT_ARGB:
        case AR_PIXEL_FORMAT_2vuy:
        case AR_PIXEL_FORMAT_yuvs:
        case AR_PIXEL_FORMAT_RGB_565:
        case AR_PIXEL_FORMAT_RGBA_5551:
        case AR_PIXEL_FORMAT_RGBA_4444:
            monoFormat = FALSE;
            break;
        case AR_PIXEL_FORMAT_MONO:
        case AR_PIXEL_FORMAT_420v:
        case AR_PIXEL_FORMAT_420f:
        case AR_PIXEL_FORMAT_NV21:
            monoFormat = TRUE;
            break;
        default:
            ARLOGe("Error: Unsupported pixel format (%d) requested.\n", pixFormat);
            return -1;
    }

    handle->arPixelFormat = pixFormat;
    handle->arPixelSize   = arUtilGetPixelSize(handle->arPixelFormat);
    
    // Update handle settings that depend on pixel format.
    if (handle->arImageProcInfo) {
        arImageProcFinal(handle->arImageProcInfo);
        handle->arImageProcInfo = arImageProcInit(handle->xsize, handle->ysize, handle->arPixelFormat, 0);
    }
    
    // If template matching, automatically switch to these most suitable colour template matching mode.
    if (monoFormat) {
        if (handle->arPatternDetectionMode == AR_TEMPLATE_MATCHING_COLOR) handle->arPatternDetectionMode = AR_TEMPLATE_MATCHING_MONO;
        else if (handle->arPatternDetectionMode == AR_TEMPLATE_MATCHING_COLOR_AND_MATRIX) handle->arPatternDetectionMode = AR_TEMPLATE_MATCHING_MONO_AND_MATRIX;
    } else {
        if (handle->arPatternDetectionMode == AR_TEMPLATE_MATCHING_MONO) handle->arPatternDetectionMode = AR_TEMPLATE_MATCHING_MONO_AND_MATRIX;
        else if (handle->arPatternDetectionMode == AR_TEMPLATE_MATCHING_MONO_AND_MATRIX) handle->arPatternDetectionMode = AR_TEMPLATE_MATCHING_COLOR_AND_MATRIX;
    }
    
    return 0;
}

int arGetPixelFormat( ARHandle *handle, AR_PIXEL_FORMAT *pixFormat )
{
    if( handle == NULL ) return -1;
    *pixFormat = handle->arPixelFormat;

    return 0;
}


int arGetMarkerNum( ARHandle *handle )
{
    if( handle == NULL ) return -1;

    return handle->marker_num;
}

ARMarkerInfo *arGetMarker( ARHandle *handle )
{
    if( handle == NULL ) return NULL;

    if( handle->marker_num <= 0 ) return NULL;

    return &(handle->markerInfo[0]);
}
