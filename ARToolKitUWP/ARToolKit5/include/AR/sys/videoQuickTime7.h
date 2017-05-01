/*
 *	videoQuickTime7.h
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
 *  Author(s): Philip Lamb
 *
 *	Rev		Date		Who		Changes
 *	1.0.0	2010-07-01	PRL		Written.
 *
 */

#ifndef AR_VIDEO_QUICKTIME7_H
#define AR_VIDEO_QUICKTIME7_H


#include <AR/ar.h>
#include <AR/video.h>

#ifdef  __cplusplus
extern "C" {
#endif

    
    
typedef struct _AR2VideoParamQuickTime7T AR2VideoParamQuickTime7T;


int                    ar2VideoDispOptionQuickTime7     ( void );
AR2VideoParamQuickTime7T  *ar2VideoOpenQuickTime7       ( const char *config );
int                    ar2VideoCloseQuickTime7          ( AR2VideoParamQuickTime7T *vid );
int                    ar2VideoGetIdQuickTime7          ( AR2VideoParamQuickTime7T *vid, ARUint32 *id0, ARUint32 *id1 );
int                    ar2VideoGetSizeQuickTime7        ( AR2VideoParamQuickTime7T *vid, int *x,int *y );
AR_PIXEL_FORMAT        ar2VideoGetPixelFormatQuickTime7 ( AR2VideoParamQuickTime7T *vid );
AR2VideoBufferT       *ar2VideoGetImageQuickTime7       ( AR2VideoParamQuickTime7T *vid );
int                    ar2VideoCapStartQuickTime7       ( AR2VideoParamQuickTime7T *vid );
int                    ar2VideoCapStopQuickTime7        ( AR2VideoParamQuickTime7T *vid );

int                    ar2VideoGetParamiQuickTime7      ( AR2VideoParamQuickTime7T *vid, int paramName, int *value );
int                    ar2VideoSetParamiQuickTime7      ( AR2VideoParamQuickTime7T *vid, int paramName, int  value );
int                    ar2VideoGetParamdQuickTime7      ( AR2VideoParamQuickTime7T *vid, int paramName, double *value );
int                    ar2VideoSetParamdQuickTime7      ( AR2VideoParamQuickTime7T *vid, int paramName, double  value );
int                    ar2VideoGetParamsQuickTime7      ( AR2VideoParamQuickTime7T *vid, const int paramName, char **value );
int                    ar2VideoSetParamsQuickTime7      ( AR2VideoParamQuickTime7T *vid, const int paramName, const char  *value );

//int ar2VideoSetBufferSizeQuickTime7(AR2VideoParamQuickTime7T *vid, const int width, const int height);
//int ar2VideoGetBufferSizeQuickTime7(AR2VideoParamQuickTime7T *vid, int *width, int *height);

void ar2VideoSetGotImageFunctionQuickTime7(AR2VideoParamQuickTime7T *vid, void (*gotImageFunc)(AR2VideoBufferT *, void *), void *userData);
void (*ar2VideoGetGotImageFunctionQuickTime7(AR2VideoParamQuickTime7T *vid))(AR2VideoBufferT *, void *);
    

// videoQuickTime7 uses an underlying native classe (QTKitVideo).
// This function retrieves a pointer, allowing direct access to the underlying native
// object instance.
#ifdef __OBJC__
id ar2VideoGetQTKitVideoInstanceQuickTime7(AR2VideoParamQuickTime7T *vid); 
#endif

#ifdef  __cplusplus
}
#endif
#endif
