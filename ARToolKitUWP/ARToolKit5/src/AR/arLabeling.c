/*
 *  arLabeling.c
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
 * Revision: 4.0
 * Date: 03/08/13
 *
 *******************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <AR/ar.h>
#include <AR/config.h>
#include "arLabelingSub/arLabelingPrivate.h"

int arLabeling( ARUint8 *image, int xsize, int ysize, int pixFormat,
                int debugMode, int labelingMode, int labelingThresh, int imageProcMode,
                ARLabelInfo *labelInfo, ARUint8 *image_thresh )
{
#if !AR_DISABLE_LABELING_DEBUG_MODE
    if( debugMode == AR_DEBUG_DISABLE ) {
#endif
        if( labelingMode == AR_LABELING_BLACK_REGION ) {
#if !AR_DISABLE_THRESH_MODE_AUTO_ADAPTIVE
            if (image_thresh) return arLabelingSubDBZ(image, xsize, ysize, image_thresh, labelInfo);
#endif
            if( imageProcMode == AR_IMAGE_PROC_FRAME_IMAGE ) {
                if( pixFormat == AR_PIXEL_FORMAT_RGB || pixFormat == AR_PIXEL_FORMAT_BGR )
                    return arLabelingSubDBR3C(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_RGBA || pixFormat == AR_PIXEL_FORMAT_BGRA )
                    return arLabelingSubDBR3CA(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_ABGR || pixFormat == AR_PIXEL_FORMAT_ARGB )
                    return arLabelingSubDBRA3C(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_MONO || pixFormat == AR_PIXEL_FORMAT_420v || pixFormat == AR_PIXEL_FORMAT_420f || pixFormat == AR_PIXEL_FORMAT_NV21 )
                    return arLabelingSubDBRC(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_yuvs )
                     return arLabelingSubDBRYC(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_2vuy )
                     return arLabelingSubDBRCY(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_RGB_565 )
                    return arLabelingSubDBR3C565(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_RGBA_5551 )
                    return arLabelingSubDBR3CA5551(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_RGBA_4444 )
                    return arLabelingSubDBR3CA4444(image, xsize, ysize, labelingThresh, labelInfo);
                else exit(0);
            }
            else if( imageProcMode == AR_IMAGE_PROC_FIELD_IMAGE ) {
                if( pixFormat == AR_PIXEL_FORMAT_RGB || pixFormat == AR_PIXEL_FORMAT_BGR )
                    return arLabelingSubDBI3C(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_RGBA || pixFormat == AR_PIXEL_FORMAT_BGRA )
                    return arLabelingSubDBI3CA(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_ABGR || pixFormat == AR_PIXEL_FORMAT_ARGB )
                    return arLabelingSubDBIA3C(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_MONO || pixFormat == AR_PIXEL_FORMAT_420v || pixFormat == AR_PIXEL_FORMAT_420f || pixFormat == AR_PIXEL_FORMAT_NV21 )
                    return arLabelingSubDBIC(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_yuvs )
                    return arLabelingSubDBIYC(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_2vuy )
                    return arLabelingSubDBICY(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_RGB_565 )
                    return arLabelingSubDBI3C565(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_RGBA_5551 )
                    return arLabelingSubDBI3CA5551(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_RGBA_4444 )
                    return arLabelingSubDBI3CA4444(image, xsize, ysize, labelingThresh, labelInfo);
                else exit(0);
            }
            else exit(0);
        }
        else if( labelingMode == AR_LABELING_WHITE_REGION ) {
#if !AR_DISABLE_THRESH_MODE_AUTO_ADAPTIVE
            if (image_thresh) return arLabelingSubDWZ(image, xsize, ysize, image_thresh, labelInfo);
#endif
            if( imageProcMode == AR_IMAGE_PROC_FRAME_IMAGE ) {
                if( pixFormat == AR_PIXEL_FORMAT_RGB || pixFormat == AR_PIXEL_FORMAT_BGR )
                    return arLabelingSubDWR3C(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_RGBA || pixFormat == AR_PIXEL_FORMAT_BGRA )
                    return arLabelingSubDWR3CA(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_ABGR || pixFormat == AR_PIXEL_FORMAT_ARGB )
                    return arLabelingSubDWRA3C(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_MONO || pixFormat == AR_PIXEL_FORMAT_420v || pixFormat == AR_PIXEL_FORMAT_420f || pixFormat == AR_PIXEL_FORMAT_NV21 )
                    return arLabelingSubDWRC(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_yuvs )
                    return arLabelingSubDWRYC(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_2vuy )
                    return arLabelingSubDWRCY(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_RGB_565 )
                    return arLabelingSubDWR3C565(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_RGBA_5551 )
                    return arLabelingSubDWR3CA5551(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_RGBA_4444 )
                    return arLabelingSubDWR3CA4444(image, xsize, ysize, labelingThresh, labelInfo);
                else exit(0);
            }
            else if( imageProcMode == AR_IMAGE_PROC_FIELD_IMAGE ) {
                if( pixFormat == AR_PIXEL_FORMAT_RGB || pixFormat == AR_PIXEL_FORMAT_BGR )
                    return arLabelingSubDWI3C(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_RGBA || pixFormat == AR_PIXEL_FORMAT_BGRA )
                    return arLabelingSubDWI3CA(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_ABGR || pixFormat == AR_PIXEL_FORMAT_ARGB )
                    return arLabelingSubDWIA3C(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_MONO || pixFormat == AR_PIXEL_FORMAT_420v || pixFormat == AR_PIXEL_FORMAT_420f || pixFormat == AR_PIXEL_FORMAT_NV21 )
                    return arLabelingSubDWIC(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_yuvs )
                    return arLabelingSubDWIYC(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_2vuy )
                    return arLabelingSubDWICY(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_RGB_565 )
                    return arLabelingSubDWI3C565(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_RGBA_5551 )
                    return arLabelingSubDWI3CA5551(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_RGBA_4444 )
                    return arLabelingSubDWI3CA4444(image, xsize, ysize, labelingThresh, labelInfo);
                else exit(0);
            }
            else exit(0);
        }
        else exit(0);
#if !AR_DISABLE_LABELING_DEBUG_MODE
    }
    else if( debugMode == AR_DEBUG_ENABLE ) {
        if( labelingMode == AR_LABELING_BLACK_REGION ) {
#if !AR_DISABLE_THRESH_MODE_AUTO_ADAPTIVE
            if (image_thresh) return arLabelingSubEBZ(image, xsize, ysize, image_thresh, labelInfo);
#endif
            if( imageProcMode == AR_IMAGE_PROC_FRAME_IMAGE ) {
                if( pixFormat == AR_PIXEL_FORMAT_RGB || pixFormat == AR_PIXEL_FORMAT_BGR )
                    return arLabelingSubEBR3C(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_RGBA || pixFormat == AR_PIXEL_FORMAT_BGRA )
                    return arLabelingSubEBR3CA(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_ABGR || pixFormat == AR_PIXEL_FORMAT_ARGB )
                    return arLabelingSubEBRA3C(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_MONO || pixFormat == AR_PIXEL_FORMAT_420v || pixFormat == AR_PIXEL_FORMAT_420f || pixFormat == AR_PIXEL_FORMAT_NV21 )
                    return arLabelingSubEBRC(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_yuvs )
                    return arLabelingSubEBRYC(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_2vuy )
                    return arLabelingSubEBRCY(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_RGB_565 )
                    return arLabelingSubEBR3C565(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_RGBA_5551 )
                    return arLabelingSubEBR3CA5551(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_RGBA_4444 )
                    return arLabelingSubEBR3CA4444(image, xsize, ysize, labelingThresh, labelInfo);
                else exit(0);
            }
            else if( imageProcMode == AR_IMAGE_PROC_FIELD_IMAGE ) {
                if( pixFormat == AR_PIXEL_FORMAT_RGB || pixFormat == AR_PIXEL_FORMAT_BGR )
                    return arLabelingSubEBI3C(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_RGBA || pixFormat == AR_PIXEL_FORMAT_BGRA )
                    return arLabelingSubEBI3CA(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_ABGR || pixFormat == AR_PIXEL_FORMAT_ARGB )
                    return arLabelingSubEBIA3C(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_MONO || pixFormat == AR_PIXEL_FORMAT_420v || pixFormat == AR_PIXEL_FORMAT_420f || pixFormat == AR_PIXEL_FORMAT_NV21 )
                    return arLabelingSubEBIC(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_yuvs )
                    return arLabelingSubEBIYC(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_2vuy )
                    return arLabelingSubEBICY(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_RGB_565 )
                    return arLabelingSubEBI3C565(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_RGBA_5551 )
                    return arLabelingSubEBI3CA5551(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_RGBA_4444 )
                    return arLabelingSubEBI3CA4444(image, xsize, ysize, labelingThresh, labelInfo);
                else exit(0);
            }
            else exit(0);
        }
        else if( labelingMode == AR_LABELING_WHITE_REGION ) {
#if !AR_DISABLE_THRESH_MODE_AUTO_ADAPTIVE
            if (image_thresh) return arLabelingSubEWZ(image, xsize, ysize, image_thresh, labelInfo);
#endif
            if( imageProcMode == AR_IMAGE_PROC_FRAME_IMAGE ) {
                if( pixFormat == AR_PIXEL_FORMAT_RGB || pixFormat == AR_PIXEL_FORMAT_BGR )
                    return arLabelingSubEWR3C(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_RGBA || pixFormat == AR_PIXEL_FORMAT_BGRA )
                    return arLabelingSubEWR3CA(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_ABGR || pixFormat == AR_PIXEL_FORMAT_ARGB )
                    return arLabelingSubEWRA3C(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_MONO || pixFormat == AR_PIXEL_FORMAT_420v || pixFormat == AR_PIXEL_FORMAT_420f || pixFormat == AR_PIXEL_FORMAT_NV21 )
                    return arLabelingSubEWRC(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_yuvs )
                    return arLabelingSubEWRYC(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_2vuy )
                    return arLabelingSubEWRCY(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_RGB_565 )
                    return arLabelingSubEWR3C565(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_RGBA_5551 )
                    return arLabelingSubEWR3CA5551(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_RGBA_4444 )
                    return arLabelingSubEWR3CA4444(image, xsize, ysize, labelingThresh, labelInfo);
                else exit(0);
            }
            else if( imageProcMode == AR_IMAGE_PROC_FIELD_IMAGE ) {
                if( pixFormat == AR_PIXEL_FORMAT_RGB || pixFormat == AR_PIXEL_FORMAT_BGR )
                    return arLabelingSubEWI3C(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_RGBA || pixFormat == AR_PIXEL_FORMAT_BGRA )
                    return arLabelingSubEWI3CA(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_ABGR || pixFormat == AR_PIXEL_FORMAT_ARGB )
                    return arLabelingSubEWIA3C(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_MONO || pixFormat == AR_PIXEL_FORMAT_420v || pixFormat == AR_PIXEL_FORMAT_420f || pixFormat == AR_PIXEL_FORMAT_NV21 )
                    return arLabelingSubEWIC(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_yuvs )
                    return arLabelingSubEWIYC(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_2vuy )
                    return arLabelingSubEWICY(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_RGB_565 )
                    return arLabelingSubEWI3C565(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_RGBA_5551 )
                    return arLabelingSubEWI3CA5551(image, xsize, ysize, labelingThresh, labelInfo);
                else if( pixFormat == AR_PIXEL_FORMAT_RGBA_4444 )
                    return arLabelingSubEWI3CA4444(image, xsize, ysize, labelingThresh, labelInfo);
                else exit(0);
            }
            else exit(0);
        }
        else exit(0);
    }
    else exit(0);
#endif
}
