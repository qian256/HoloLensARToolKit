/*
 *	videoWindowsDSVideoLib.h
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
 *  Copyright 2004-2015 ARToolworks, Inc.
 *
 *  Author(s): Thomas Pintaric, Philip Lamb
 *
 *	Rev		Date		Who		Changes
 *	2.68.2	2004-07-20	PRL		Rewrite for ARToolKit 2.68.2
 *	2.71.0	2005-08-05	PRL		Incorporate DSVL-0.0.8b
 *
 */
/*
	========================================================================
	PROJECT: DirectShow Video Processing Library
	Version: 0.0.8 (05/04/2005)
	========================================================================
	Author:  Thomas Pintaric, Vienna University of Technology
	Contact: pintaric@ims.tuwien.ac.at http://ims.tuwien.ac.at/~thomas
	=======================================================================
	
	Copyright (C) 2005  Vienna University of Technology
	
	For further information please contact Thomas Pintaric under
	<pintaric@ims.tuwien.ac.at> or write to Thomas Pintaric,
	Vienna University of Technology, Favoritenstr. 9-11/E188/2, A-1040
	Vienna, Austria.
	========================================================================
 */

#ifndef AR_VIDEO_WIN_DSVL_H
#define AR_VIDEO_WIN_DSVL_H

#include <windows.h>
#include <stdlib.h>
#include <stddef.h>
#include <process.h>
#include <AR/ar.h>

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct _AR2VideoParamWinDSVLT AR2VideoParamWinDSVLT;

int                    ar2VideoDispOptionWinDSVL   ( void );
AR2VideoParamWinDSVLT *ar2VideoOpenWinDSVL         ( const char *config );
int                    ar2VideoCloseWinDSVL        ( AR2VideoParamWinDSVLT *vid );
int                    ar2VideoGetIdWinDSVL        ( AR2VideoParamWinDSVLT *vid, ARUint32 *id0, ARUint32 *id1 );
int                    ar2VideoGetSizeWinDSVL      ( AR2VideoParamWinDSVLT *vid, int *x,int *y );
AR_PIXEL_FORMAT        ar2VideoGetPixelFormatWinDSVL ( AR2VideoParamWinDSVLT *vid );
AR2VideoBufferT       *ar2VideoGetImageWinDSVL     ( AR2VideoParamWinDSVLT *vid );
int                    ar2VideoCapStartWinDSVL     ( AR2VideoParamWinDSVLT *vid );
int                    ar2VideoCapStopWinDSVL      ( AR2VideoParamWinDSVLT *vid );

int                    ar2VideoGetParamiWinDSVL    ( AR2VideoParamWinDSVLT *vid, int paramName, int *value );
int                    ar2VideoSetParamiWinDSVL    ( AR2VideoParamWinDSVLT *vid, int paramName, int  value );
int                    ar2VideoGetParamdWinDSVL    ( AR2VideoParamWinDSVLT *vid, int paramName, double *value );
int                    ar2VideoSetParamdWinDSVL    ( AR2VideoParamWinDSVLT *vid, int paramName, double  value );

#ifdef  __cplusplus
}
#endif
#endif
