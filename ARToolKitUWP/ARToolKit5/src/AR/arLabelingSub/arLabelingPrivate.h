/*
 *  arLabelingPrivate.c
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

#ifndef AR_LABELING_PRIVATE_H
#define AR_LABELING_PRIVATE_H

#include <AR/config.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
	Function naming convention:
	(E|D) - DEBUG_ENABLE|!DEBUG_ENABLE
	(W|B) - WHITE_REGION|!WHITE_REGION
    (Z|   - ADAPTIVE|!ADAPTIVE
	   (R|I) - FRAME_IMAGE|!FRAME_IMAGE
	   (3C|3CA|A3C|C|YC|CY|3C565|3CA5551|3CA4444) - pixel format
                                                 )
 */

/*	CCC pixel format */	

int arLabelingSubDBI3C( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubDBR3C( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubDWI3C( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubDWR3C( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
#if !AR_DISABLE_LABELING_DEBUG_MODE
int arLabelingSubEBI3C( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubEBR3C( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubEWI3C( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubEWR3C( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
#endif

/*	CCCA pixel format */	

int arLabelingSubDBI3CA( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubDBR3CA( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubDWI3CA( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubDWR3CA( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
#if !AR_DISABLE_LABELING_DEBUG_MODE
int arLabelingSubEBI3CA( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubEBR3CA( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubEWI3CA( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubEWR3CA( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
#endif

/*	ACCC pixel format */	

int arLabelingSubDBIA3C( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubDBRA3C( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubDWIA3C( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubDWRA3C( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
#if !AR_DISABLE_LABELING_DEBUG_MODE
int arLabelingSubEBIA3C( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubEBRA3C( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubEWIA3C( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubEWRA3C( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
#endif

/*	C pixel format */	

int arLabelingSubDBIC( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubDBRC( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubDWIC( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubDWRC( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
#if !AR_DISABLE_LABELING_DEBUG_MODE
int arLabelingSubEBIC( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubEBRC( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubEWIC( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubEWRC( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
#endif

/*	YC pixel format */	

int arLabelingSubDBIYC( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubDBRYC( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubDWIYC( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubDWRYC( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
#if !AR_DISABLE_LABELING_DEBUG_MODE
int arLabelingSubEBIYC( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubEBRYC( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubEWIYC( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubEWRYC( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
#endif

/*	CY pixel format */	

int arLabelingSubDBICY( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubDBRCY( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubDWICY( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubDWRCY( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
#if !AR_DISABLE_LABELING_DEBUG_MODE
int arLabelingSubEBICY( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubEBRCY( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubEWICY( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubEWRCY( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
#endif

/*	CCC_565 pixel format */	

int arLabelingSubDBI3C565( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubDBR3C565( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubDWI3C565( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubDWR3C565( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
#if !AR_DISABLE_LABELING_DEBUG_MODE
int arLabelingSubEBI3C565( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubEBR3C565( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubEWI3C565( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubEWR3C565( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
#endif

/*	CCCA_5551 pixel format */	

int arLabelingSubDBI3CA5551( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubDBR3CA5551( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubDWI3CA5551( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubDWR3CA5551( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
#if !AR_DISABLE_LABELING_DEBUG_MODE
int arLabelingSubEBI3CA5551( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubEBR3CA5551( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubEWI3CA5551( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubEWR3CA5551( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
#endif

/*	CCCA_4444 pixel format */	

int arLabelingSubDBI3CA4444( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubDBR3CA4444( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubDWI3CA4444( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubDWR3CA4444( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
#if !AR_DISABLE_LABELING_DEBUG_MODE
int arLabelingSubEBI3CA4444( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubEBR3CA4444( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubEWI3CA4444( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
int arLabelingSubEWR3CA4444( ARUint8 *image, int xsize, int ysize, int labelingThresh, ARLabelInfo *labelInfo );
#endif

/*  Adaptive */

#if !AR_DISABLE_THRESH_MODE_AUTO_ADAPTIVE
int arLabelingSubDBZ( ARUint8 *image, const int xsize, const int ysize, ARUint8* image_thresh, ARLabelInfo *labelInfo );
int arLabelingSubDWZ( ARUint8 *image, const int xsize, const int ysize, ARUint8* image_thresh, ARLabelInfo *labelInfo );
int arLabelingSubEBZ( ARUint8 *image, const int xsize, const int ysize, ARUint8* image_thresh, ARLabelInfo *labelInfo );
int arLabelingSubEWZ( ARUint8 *image, const int xsize, const int ysize, ARUint8* image_thresh, ARLabelInfo *labelInfo );
#endif

#ifdef __cplusplus
}
#endif
#endif
