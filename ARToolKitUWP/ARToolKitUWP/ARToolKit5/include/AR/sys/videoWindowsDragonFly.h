/*
 *	videoWindowsDragonFly.h
 *  ARToolKit5
 *
 *  DragonFly/FlyCapture video capture module.
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
 *  Author(s): Hirokazu Kato, Philip Lamb
 *
 */
/*******************************************************
 *
 * Author: Hirokazu Kato
 *
 *         kato@sys.im.hiroshima-cu.ac.jp
 *
 * Revision: 2.1
 * Date: 2004/01/01
 * Revised 2012-08-05, Philip Lamb.
 *
 *******************************************************/

#ifndef AR_VIDEO_WINDOWS_DRAGON_FLY_H
#define AR_VIDEO_WINDOWS_DRAGON_FLY_H

#include <windows.h>
#include <stdlib.h>
#include <stddef.h>
#include <process.h>
#include <AR/ar.h>
#include <AR/videoConfig.h>

#ifdef  __cplusplus
extern "C" {
#endif

#define   AR2VIDEO_WINDF_STATUS_IDLE    0
#define   AR2VIDEO_WINDF_STATUS_RUN     1
#define   AR2VIDEO_WINDF_STATUS_STOP    2
#define   AR2VIDEO_WINDF_STATUS_STOP2   3

typedef struct {
    AR2VideoBufferT     in;
    AR2VideoBufferT     wait;
    AR2VideoBufferT     out;
	HANDLE              buffMutex;
} AR2VideoBufferWinDFT;

typedef struct _AR2VideoParamWinDFT AR2VideoParamWinDFT;

int                   ar2VideoDispOptionWinDF     ( void );
AR2VideoParamWinDFT   *ar2VideoOpenWinDF           ( const char *config );
int                   ar2VideoCloseWinDF          ( AR2VideoParamWinDFT *vid );
int                   ar2VideoGetIdWinDF          ( AR2VideoParamWinDFT *vid, ARUint32 *id0, ARUint32 *id1 );
int                   ar2VideoGetSizeWinDF        ( AR2VideoParamWinDFT *vid, int *x,int *y );
AR_PIXEL_FORMAT       ar2VideoGetPixelFormatWinDF ( AR2VideoParamWinDFT *vid );
AR2VideoBufferT      *ar2VideoGetImageWinDF       ( AR2VideoParamWinDFT *vid );
int                   ar2VideoCapStartWinDF       ( AR2VideoParamWinDFT *vid );
int                   ar2VideoCapStopWinDF        ( AR2VideoParamWinDFT *vid );

int                    ar2VideoGetParamiWinDF     ( AR2VideoParamWinDFT *vid, int paramName, int *value );
int                    ar2VideoSetParamiWinDF     ( AR2VideoParamWinDFT *vid, int paramName, int  value );
int                    ar2VideoGetParamdWinDF     ( AR2VideoParamWinDFT *vid, int paramName, double *value );
int                    ar2VideoSetParamdWinDF     ( AR2VideoParamWinDFT *vid, int paramName, double  value );

#ifdef  __cplusplus
}
#endif
#endif
