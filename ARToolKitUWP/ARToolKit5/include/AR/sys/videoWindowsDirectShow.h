/*
 *	videoWindowsDirectShow.h
 *  ARToolKit5
 *
 *  DirectShow video capture module.
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
 *  Copyright 2005-2015 ARToolworks, Inc.
 *
 *  Author(s): Hirokazu Kato, Philip Lamb
 *
 */
/*******************************************************
 *
 * Author: Hirokazu Kato
 *
 *         kato@sys.es.osaka-u.ac.jp
 *
 * Revision: 4.1
 * Date: 2005/08/26
 *
 *******************************************************/

#ifndef AR_VIDEO_WIN_DS_H
#define AR_VIDEO_WIN_DS_H

#include <windows.h>
#include <stdlib.h>
#include <stddef.h>
#include <process.h>
#include <AR/ar.h>


#ifdef  __cplusplus
extern "C" {
#endif

typedef void AR2VideoParamWinDST;

int                    ar2VideoDispOptionWinDS     ( void );
ARVideoSourceInfoListT *ar2VideoCreateSourceInfoListWinDS(const char *config);
AR2VideoParamWinDST   *ar2VideoOpenWinDS           ( const char *config );
int                    ar2VideoCloseWinDS          ( AR2VideoParamWinDST *vid );
int                    ar2VideoGetIdWinDS          ( AR2VideoParamWinDST *vid, ARUint32 *id0, ARUint32 *id1 );
int                    ar2VideoGetSizeWinDS        ( AR2VideoParamWinDST *vid, int *x,int *y );
AR_PIXEL_FORMAT        ar2VideoGetPixelFormatWinDS ( AR2VideoParamWinDST *vid );
AR2VideoBufferT       *ar2VideoGetImageWinDS       ( AR2VideoParamWinDST *vid );
int                    ar2VideoCapStartWinDS       ( AR2VideoParamWinDST *vid );
int                    ar2VideoCapStopWinDS        ( AR2VideoParamWinDST *vid );

int                    ar2VideoGetParamiWinDS      ( AR2VideoParamWinDST *vid, int paramName, int *value );
int                    ar2VideoSetParamiWinDS      ( AR2VideoParamWinDST *vid, int paramName, int  value );
int                    ar2VideoGetParamdWinDS      ( AR2VideoParamWinDST *vid, int paramName, double *value );
int                    ar2VideoSetParamdWinDS      ( AR2VideoParamWinDST *vid, int paramName, double  value );
 
#ifdef  __cplusplus
}
#endif
#endif
