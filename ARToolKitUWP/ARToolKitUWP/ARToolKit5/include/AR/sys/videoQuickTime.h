/*
 *	videoQuickTime.h
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
 *  Copyright 2002-2015 ARToolworks, Inc.
 *
 *  Author(s): Kiyoshi Kiyokawa, Hirokazu Kato, Philip Lamb
 *
 *	Rev		Date		Who		Changes
 *	1.1.0	2003-09-09	PRL		Based on Apple "Son of MungGrab" sample code for QuickTime 6.
 *								Added config option "-fps" to superimpose frame counter on video.
 *								Returns aligned data in ARGB pixel format.
 *  1.2.0   2004-04-28  PRL		Now one thread per video source. Versions of QuickTime
 *								prior to 6.4 are NOT thread safe, and with these earlier
 *								versions, QuickTime toolbox access will be serialised.
 *	1.2.1   2004-06-28  PRL		Support for 2vuy and yuvs pixel formats.
 *  1.3.0   2004-07-13  PRL		Code from Daniel Heckenberg to directly access vDig.
 *  1.3.1   2004-12-07  PRL		Added config option "-pixelformat=" to support pixel format
 *								specification at runtime, with default determined at compile time.
 *	1.4.0	2005-03-08	PRL		Video input settings now saved and restored.
 *  1.4.1   2005-03-15  PRL     QuickTime 6.4 or newer is now required by default. In order
 *								to allow earlier versions, AR_VIDEO_SUPPORT_OLD_QUICKTIME must
 *								be uncommented at compile time.
 *
 */
/*
 *	Video capture subrutine for Linux/libdc1394 devices
 *	Author: Kiyoshi Kiyokawa ( kiyo@crl.go.jp )
 *	        Hirokazu Kato ( kato@sys.im.hiroshima-cu.ac.jp )
 *
 *	Revision: 1.0   Date: 2002/01/01
 *
 */

#ifndef AR_VIDEO_QUICKTIME_H
#define AR_VIDEO_QUICKTIME_H

#include <AR/sys/videoQuickTimeMovie.h>

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct _AR2VideoParamQuickTimeT AR2VideoParamQuickTimeT;

AR_DLL_API int                       ar2VideoDispOptionQuickTime     ( void );
AR_DLL_API AR2VideoParamQuickTimeT  *ar2VideoOpenQuickTime           ( const char *config );
AR_DLL_API int                       ar2VideoCloseQuickTime          ( AR2VideoParamQuickTimeT *vid );
AR_DLL_API int                       ar2VideoGetIdQuickTime          ( AR2VideoParamQuickTimeT *vid, ARUint32 *id0, ARUint32 *id1 );
AR_DLL_API int                       ar2VideoGetSizeQuickTime        ( AR2VideoParamQuickTimeT *vid, int *x,int *y );
AR_DLL_API AR_PIXEL_FORMAT           ar2VideoGetPixelFormatQuickTime ( AR2VideoParamQuickTimeT *vid );
AR_DLL_API AR2VideoBufferT          *ar2VideoGetImageQuickTime       ( AR2VideoParamQuickTimeT *vid );
AR_DLL_API int                       ar2VideoCapStartQuickTime       ( AR2VideoParamQuickTimeT *vid );
AR_DLL_API int                       ar2VideoCapStopQuickTime        ( AR2VideoParamQuickTimeT *vid );
AR_DLL_API int                       ar2VideoGetParamiQuickTime      ( AR2VideoParamQuickTimeT *vid, int paramName, int *value );
AR_DLL_API int                       ar2VideoSetParamiQuickTime      ( AR2VideoParamQuickTimeT *vid, int paramName, int  value );
AR_DLL_API int                       ar2VideoGetParamdQuickTime      ( AR2VideoParamQuickTimeT *vid, int paramName, double *value );
AR_DLL_API int                       ar2VideoSetParamdQuickTime      ( AR2VideoParamQuickTimeT *vid, int paramName, double  value );

AR_DLL_API AR_VIDEO_QUICKTIME_MOVIE_t ar2VideoGetMovieQuickTime( AR2VideoParamQuickTimeT *vid );

#ifdef  __cplusplus
}
#endif
#endif // AR_VIDEO_QUICKTIME_H
