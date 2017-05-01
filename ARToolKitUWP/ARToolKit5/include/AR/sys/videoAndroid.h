/*
 *	videoAndroid.h
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
 *  Copyright 2014-2015 ARToolworks, Inc.
 *
 *  Author(s): Philip Lamb
 *
 */

#ifndef AR_VIDEO_ANDROID_H
#define AR_VIDEO_ANDROID_H

#define AR_VIDEO_ANDROID_ENABLE_NATIVE_CAMERA 0

#include <AR/ar.h>
#include <AR/video.h>

#ifdef  __cplusplus
extern "C" {
#endif

enum {
    AR_VIDEO_PARAM_ANDROID_DEVICEID,        // s, readonly
#if !AR_VIDEO_ANDROID_ENABLE_NATIVE_CAMERA
    AR_VIDEO_PARAM_ANDROID_WIDTH,           // i
    AR_VIDEO_PARAM_ANDROID_HEIGHT,          // i
    AR_VIDEO_PARAM_ANDROID_PIXELFORMAT,     // i (cast to AR_PIXEL_FORMAT)
#endif
    AR_VIDEO_PARAM_ANDROID_CAMERA_INDEX,    // i
    AR_VIDEO_PARAM_ANDROID_CAMERA_FACE,     // i
    AR_VIDEO_PARAM_ANDROID_FOCAL_LENGTH,    // d
    AR_VIDEO_PARAM_ANDROID_INTERNET_STATE   // i
};

enum {
    AR_VIDEO_ANDROID_CAMERA_FACE_REAR = 0,
    AR_VIDEO_ANDROID_CAMERA_FACE_FRONT,
};

typedef struct _AR2VideoParamAndroidT AR2VideoParamAndroidT;

#define AR_VIDEO_ANDROID_FOCAL_LENGTH_DEFAULT 0.3 // Metres.

//
// At present, videoAndroid provides only some of the usual APIs, and doesn't support frame retrieval.
// Most of the video functionality on Android is handled in separate Android-specific classes.
// For Java (JDK) Android apps, the classes are in ARBaseLib.
// For Native (NDK) Android apps, the classes are in the 'CameraSurface' class (part of the example code).
//

int                    ar2VideoDispOptionAndroid     ( void );
AR2VideoParamAndroidT *ar2VideoOpenAndroid           ( const char *config );
int                    ar2VideoCloseAndroid          ( AR2VideoParamAndroidT *vid );
int                    ar2VideoGetIdAndroid          ( AR2VideoParamAndroidT *vid, ARUint32 *id0, ARUint32 *id1 );
int                    ar2VideoGetSizeAndroid        ( AR2VideoParamAndroidT *vid, int *x,int *y );
AR_PIXEL_FORMAT        ar2VideoGetPixelFormatAndroid ( AR2VideoParamAndroidT *vid );

#if AR_VIDEO_ANDROID_ENABLE_NATIVE_CAMERA
AR2VideoBufferT       *ar2VideoGetImageAndroid       ( AR2VideoParamAndroidT *vid );
int                    ar2VideoCapStartAndroid       ( AR2VideoParamAndroidT *vid );
int                    ar2VideoCapStartAsyncAndroid  ( AR2VideoParamAndroidT *vid, AR_VIDEO_FRAME_READY_CALLBACK callback, void *userdata );
int                    ar2VideoCapStopAndroid        ( AR2VideoParamAndroidT *vid );
#endif

int                    ar2VideoGetParamiAndroid      ( AR2VideoParamAndroidT *vid, int paramName, int *value );
int                    ar2VideoSetParamiAndroid      ( AR2VideoParamAndroidT *vid, int paramName, int  value );
int                    ar2VideoGetParamdAndroid      ( AR2VideoParamAndroidT *vid, int paramName, double *value );
int                    ar2VideoSetParamdAndroid      ( AR2VideoParamAndroidT *vid, int paramName, double  value );
int                    ar2VideoGetParamsAndroid      ( AR2VideoParamAndroidT *vid, const int paramName, char **value );
int                    ar2VideoSetParamsAndroid      ( AR2VideoParamAndroidT *vid, const int paramName, const char  *value );

//int ar2VideoSetBufferSizeAndroid(AR2VideoParamAndroidT *vid, const int width, const int height);
//int ar2VideoGetBufferSizeAndroid(AR2VideoParamAndroidT *vid, int *width, int *height);

int                    ar2VideoGetCParamAsyncAndroid (AR2VideoParamAndroidT *vid, void (*callback)(const ARParam *, void *), void *userdata);

#ifdef  __cplusplus
}
#endif
#endif
