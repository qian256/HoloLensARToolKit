/*
 *	videoiPhone.h
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
 *  Copyright 2008-2015 ARToolworks, Inc.
 *
 *  Author(s): Philip Lamb
 *
 *	Rev		Date		Who		Changes
 *	1.0.0	2008-05-04	PRL		Written.
 *
 */

#ifndef AR_VIDEO_IPHONE_H
#define AR_VIDEO_IPHONE_H

// This header is only ever included by other headers, so
// doesn't need to include <AR/ar.h> or <AR/video.h>.

#ifdef  __cplusplus
extern "C" {
#endif

enum {
    AR_VIDEO_PARAM_IPHONE_WILL_CAPTURE_NEXT_FRAME,
    AR_VIDEO_PARAM_IPHONE_IS_USING_COREVIDEO,
    AR_VIDEO_PARAM_IOS_DEVICE,
    AR_VIDEO_PARAM_IOS_FOCUS,
    AR_VIDEO_PARAM_IOS_RECOMMENDED_CPARAM_NAME, // No longer supported. Use arVideoGetCParam() instead.
    AR_VIDEO_PARAM_IOS_ASYNC,
    AR_VIDEO_PARAM_IOS_CAMERA_POSITION
};

typedef enum {
    AR_VIDEO_IOS_FOCUS_NONE = 0,
    AR_VIDEO_IOS_FOCUS_MACRO,
    AR_VIDEO_IOS_FOCUS_0_3M,
    AR_VIDEO_IOS_FOCUS_1_0M,
    AR_VIDEO_IOS_FOCUS_INF
} AR_VIDEO_IOS_FOCUS;

typedef enum {
    AR_VIDEO_IOS_CAMERA_POSITION_UNKNOWN = -1,
    AR_VIDEO_IOS_CAMERA_POSITION_UNSPECIFIED = 0,
    AR_VIDEO_IOS_CAMERA_POSITION_REAR,
    AR_VIDEO_IOS_CAMERA_POSITION_FRONT,
    AR_VIDEO_IOS_CAMERA_POSITION_REAR_STEREO_LEFT,
    AR_VIDEO_IOS_CAMERA_POSITION_REAR_STEREO_RIGHT,
    AR_VIDEO_IOS_CAMERA_POSITION_FRONT_STEREO_LEFT,
    AR_VIDEO_IOS_CAMERA_POSITION_FRONT_STEREO_RIGHT
} AR_VIDEO_IOS_CAMERA_POSITION;
    
typedef enum {
    AR_VIDEO_IOS_DEVICE_IPHONE3G = 1,
    AR_VIDEO_IOS_DEVICE_IPHONE3GS,
    AR_VIDEO_IOS_DEVICE_IPHONE4,
    AR_VIDEO_IOS_DEVICE_IPODTOUCH4,
    AR_VIDEO_IOS_DEVICE_IPAD2,
    AR_VIDEO_IOS_DEVICE_IPAD3,
    AR_VIDEO_IOS_DEVICE_IPHONE_GENERIC,
    AR_VIDEO_IOS_DEVICE_IPOD_GENERIC,
    AR_VIDEO_IOS_DEVICE_IPAD_GENERIC,
    AR_VIDEO_IOS_DEVICE_IPHONE4S,
    AR_VIDEO_IOS_DEVICE_APPLETV_GENERIC,
    AR_VIDEO_IOS_DEVICE_IPHONE5,
    AR_VIDEO_IOS_DEVICE_IPODTOUCH5,
    AR_VIDEO_IOS_DEVICE_IPAD4,
    AR_VIDEO_IOS_DEVICE_IPADMINI,
    AR_VIDEO_IOS_DEVICE_IPHONE5C,
    AR_VIDEO_IOS_DEVICE_IPHONE5S,
    AR_VIDEO_IOS_DEVICE_IPADAIR,
    AR_VIDEO_IOS_DEVICE_IPADMINI2,
    AR_VIDEO_IOS_DEVICE_IPADMINI3,
    AR_VIDEO_IOS_DEVICE_IPADMINI4,
    AR_VIDEO_IOS_DEVICE_IPADAIR2,
    AR_VIDEO_IOS_DEVICE_IPHONE6,
    AR_VIDEO_IOS_DEVICE_IPHONE6PLUS,
    AR_VIDEO_IOS_DEVICE_IPHONE6S,
    AR_VIDEO_IOS_DEVICE_IPHONE6SPLUS
} AR_VIDEO_IOS_DEVICE;
    
    
typedef struct _AR2VideoParamiPhoneT AR2VideoParamiPhoneT;


int                    ar2VideoDispOptioniPhone     ( void );
AR2VideoParamiPhoneT  *ar2VideoOpeniPhone           ( const char *config );
int                    ar2VideoCloseiPhone          ( AR2VideoParamiPhoneT *vid );
int                    ar2VideoGetIdiPhone          ( AR2VideoParamiPhoneT *vid, ARUint32 *id0, ARUint32 *id1 );
int                    ar2VideoGetSizeiPhone        ( AR2VideoParamiPhoneT *vid, int *x,int *y );
int                    ar2VideoGetPixelFormatiPhone ( AR2VideoParamiPhoneT *vid );
AR2VideoBufferT       *ar2VideoGetImageiPhone       ( AR2VideoParamiPhoneT *vid );
int                    ar2VideoCapStartiPhone       ( AR2VideoParamiPhoneT *vid );
int                    ar2VideoCapStopiPhone        ( AR2VideoParamiPhoneT *vid );

int                    ar2VideoGetParamiiPhone      ( AR2VideoParamiPhoneT *vid, int paramName, int *value );
int                    ar2VideoSetParamiiPhone      ( AR2VideoParamiPhoneT *vid, int paramName, int  value );
int                    ar2VideoGetParamdiPhone      ( AR2VideoParamiPhoneT *vid, int paramName, double *value );
int                    ar2VideoSetParamdiPhone      ( AR2VideoParamiPhoneT *vid, int paramName, double  value );
int                    ar2VideoGetParamsiPhone      ( AR2VideoParamiPhoneT *vid, const int paramName, char **value );
int                    ar2VideoSetParamsiPhone      ( AR2VideoParamiPhoneT *vid, const int paramName, const char  *value );

int ar2VideoSetBufferSizeiPhone(AR2VideoParamiPhoneT *vid, const int width, const int height);
int ar2VideoGetBufferSizeiPhone(AR2VideoParamiPhoneT *vid, int *width, int *height);
void ar2VideoSetGotImageFunctioniPhone(AR2VideoParamiPhoneT *vid, void (*gotImageFunc)(AR2VideoBufferT *, void *), void *userData);
void (*ar2VideoGetGotImageFunctioniPhone(AR2VideoParamiPhoneT *vid))(AR2VideoBufferT *, void *);

int                    ar2VideoGetCParamiPhone      (AR2VideoParamiPhoneT *vid, ARParam *cparam);
AR2VideoParamiPhoneT  *ar2VideoOpenAsynciPhone      (const char *config, void (*callback)(void *), void *userdata);

// videoiPhone uses underlying native classes (CameraVideo, MovieVideo).
// This function retrieves a pointer, allowing direct access to the underlying native
// object instance.
#ifdef __OBJC__
id ar2VideoGetNativeVideoInstanceiPhone(AR2VideoParamiPhoneT *vid); 
#endif

#ifdef  __cplusplus
}
#endif
#endif
