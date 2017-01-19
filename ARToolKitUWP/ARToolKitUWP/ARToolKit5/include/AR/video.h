/*
 *	video.h
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
 *  Author(s): Hirokazu Kato, Atsishi Nakazawa, Philip Lamb
 *
 */
/*******************************************************
 *
 * Author: Hirokazu Kato, Atsishi Nakazawa
 *
 *         kato@sys.im.hiroshima-cu.ac.jp
 *         nakazawa@inolab.sys.es.osaka-u.ac.jp
 *
 * Revision: 4.3
 * Date: 2002/01/01
 *
 *******************************************************/

#ifndef AR_VIDEO_H
#define AR_VIDEO_H

#include <AR/ar.h>
#include <AR/videoConfig.h>
#include <limits.h>

#ifdef  __cplusplus
extern "C" {
#endif

#define  AR_VIDEO_DEVICE_DUMMY              0
#define  AR_VIDEO_DEVICE_V4L                1
#define  AR_VIDEO_DEVICE_DV                 2
#define  AR_VIDEO_DEVICE_1394CAM            3
#define  AR_VIDEO_DEVICE_SGI                4
#define  AR_VIDEO_DEVICE_WINDOWS_DIRECTSHOW 5
#define  AR_VIDEO_DEVICE_WINDOWS_DRAGONFLY  6
#define  AR_VIDEO_DEVICE_RESERVED1          7
#define  AR_VIDEO_DEVICE_RESERVED2          8
#define  AR_VIDEO_DEVICE_QUICKTIME          9
#define  AR_VIDEO_DEVICE_WINDOWS_DSVIDEOLIB 10
#define  AR_VIDEO_DEVICE_GSTREAMER          11
#define  AR_VIDEO_DEVICE_IPHONE             12
#define  AR_VIDEO_DEVICE_QUICKTIME7         13
#define  AR_VIDEO_DEVICE_IMAGE              14
#define  AR_VIDEO_DEVICE_ANDROID            15
#define  AR_VIDEO_DEVICE_WINDOWS_MEDIA_FOUNDATION 16
#define  AR_VIDEO_DEVICE_WINDOWS_MEDIA_CAPTURE 17
#define  AR_VIDEO_DEVICE_V4L2               18
#define  AR_VIDEO_DEVICE_MAX                18


#define  AR_VIDEO_1394_BRIGHTNESS                      65
#define  AR_VIDEO_1394_BRIGHTNESS_FEATURE_ON           66
#define  AR_VIDEO_1394_BRIGHTNESS_AUTO_ON              67
#define  AR_VIDEO_1394_BRIGHTNESS_MAX_VAL              68
#define  AR_VIDEO_1394_BRIGHTNESS_MIN_VAL              69
#define  AR_VIDEO_1394_EXPOSURE                        70
#define  AR_VIDEO_1394_EXPOSURE_FEATURE_ON             71
#define  AR_VIDEO_1394_EXPOSURE_AUTO_ON                72
#define  AR_VIDEO_1394_EXPOSURE_MAX_VAL                73
#define  AR_VIDEO_1394_EXPOSURE_MIN_VAL                74
#define  AR_VIDEO_1394_WHITE_BALANCE                   75
#define  AR_VIDEO_1394_WHITE_BALANCE_UB                76
#define  AR_VIDEO_1394_WHITE_BALANCE_VR                77
#define  AR_VIDEO_1394_WHITE_BALANCE_FEATURE_ON        78
#define  AR_VIDEO_1394_WHITE_BALANCE_AUTO_ON           79
#define  AR_VIDEO_1394_WHITE_BALANCE_MAX_VAL           80
#define  AR_VIDEO_1394_WHITE_BALANCE_MIN_VAL           81
#define  AR_VIDEO_1394_SHUTTER_SPEED                   82
#define  AR_VIDEO_1394_SHUTTER_SPEED_FEATURE_ON        83
#define  AR_VIDEO_1394_SHUTTER_SPEED_AUTO_ON           84
#define  AR_VIDEO_1394_SHUTTER_SPEED_MAX_VAL           85
#define  AR_VIDEO_1394_SHUTTER_SPEED_MIN_VAL           86
#define  AR_VIDEO_1394_GAIN                            87
#define  AR_VIDEO_1394_GAIN_FEATURE_ON                 88
#define  AR_VIDEO_1394_GAIN_AUTO_ON                    89
#define  AR_VIDEO_1394_GAIN_MAX_VAL                    90
#define  AR_VIDEO_1394_GAIN_MIN_VAL                    91
#define  AR_VIDEO_1394_FOCUS                           92
#define  AR_VIDEO_1394_FOCUS_FEATURE_ON                93
#define  AR_VIDEO_1394_FOCUS_AUTO_ON                   94
#define  AR_VIDEO_1394_FOCUS_MAX_VAL                   95
#define  AR_VIDEO_1394_FOCUS_MIN_VAL                   96
#define  AR_VIDEO_1394_GAMMA                           97
#define  AR_VIDEO_1394_GAMMA_FEATURE_ON                98
#define  AR_VIDEO_1394_GAMMA_AUTO_ON                   99
#define  AR_VIDEO_1394_GAMMA_MAX_VAL                  100
#define  AR_VIDEO_1394_GAMMA_MIN_VAL                  101

#define  AR_VIDEO_WINDS_SHOW_PROPERTIES			      129

#define  AR_VIDEO_FOCUS_MODE                          301 // i
#define  AR_VIDEO_FOCUS_MANUAL_DISTANCE               302 // d
#define  AR_VIDEO_FOCUS_POINT_OF_INTEREST_X           303 // d
#define  AR_VIDEO_FOCUS_POINT_OF_INTEREST_Y           304 // d

#define  AR_VIDEO_GET_VERSION                     INT_MAX

#define  AR_VIDEO_FOCUS_MODE_FIXED                    0
#define  AR_VIDEO_FOCUS_MODE_AUTO                     1
#define  AR_VIDEO_FOCUS_MODE_POINT_OF_INTEREST        2
#define  AR_VIDEO_FOCUS_MODE_MANUAL                   3

/*!
    @typedef
    @asbtract Structure which carries information about a frame retrieved by the video library.
    @field buff         A pointer to the packed video data for this video frame. The video data pixel
                        format is as specified by arVideoGetPixelFormat(). For multi-planar frames,
                        this pointer is a copy of bufPlanes[0].
    @field bufPlanes    For multi-planar video frames, this must be an array of length bufPlaneCount
                        of (ARUint8*), into which will be copied pointers to the packed video data
                        for each plane. For single-plane formats, this will be NULL.
    @field bufPlaneCount For multi-planar video frames, this is the number of frame planes. For
                        single-plane formats, this will be 0.
    @field fillFlag     Set non-zero when buff is valid.
    @field time_sec     Seconds portion of the time at which buff was filled. Epoch is OS-specific.
    @field time_usec    Microseconds portion of the time at which buff was filled. Epoch is OS-specific.
    @seealso arVideoGetPixelFormat arVideoGetPixelFormat
 */
typedef struct {
    ARUint8            *buff;
    ARUint8           **bufPlanes;
    unsigned int        bufPlaneCount;
    int                 fillFlag;
    ARUint32            time_sec;
    ARUint32            time_usec;
} AR2VideoBufferT;

#define AR_VIDEO_POSITION_UNKNOWN     0x0000 // Camera physical position on device unknown.
#define AR_VIDEO_POSITION_FRONT       0x0008 // Camera is on front of device pointing towards user.
#define AR_VIDEO_POSITION_BACK        0x0010 // Camera is on back of device pointing away from user.
#define AR_VIDEO_POSITION_LEFT        0x0018 // Camera is on left of device pointing to user's left.
#define AR_VIDEO_POSITION_RIGHT       0x0020 // Camera is on right of device pointing to user's right.
#define AR_VIDEO_POSITION_TOP         0x0028 // Camera is on top of device pointing toward ceiling when device is held upright.
#define AR_VIDEO_POSITION_BOTTOM      0x0030 // Camera is on bottom of device pointing towards floor when device is held upright.
#define AR_VIDEO_POSITION_OTHER       0x0038 // Camera physical position on device is known but none of the above.

#define AR_VIDEO_STEREO_MODE_MONO                        0x0000
#define AR_VIDEO_STEREO_MODE_LEFT                        0x0040
#define AR_VIDEO_STEREO_MODE_RIGHT                       0x0080
#define AR_VIDEO_STEREO_MODE_FRAME_SEQUENTIAL            0x00C0
#define AR_VIDEO_STEREO_MODE_SIDE_BY_SIDE                0x0100
#define AR_VIDEO_STEREO_MODE_OVER_UNDER                  0x0140
#define AR_VIDEO_STEREO_MODE_HALF_SIDE_BY_SIDE           0x0180
#define AR_VIDEO_STEREO_MODE_OVER_UNDER_HALF_HEIGHT      0x01C0
#define AR_VIDEO_STEREO_MODE_ROW_INTERLACED              0x0200
#define AR_VIDEO_STEREO_MODE_COLUMN_INTERLACED           0x0240
#define AR_VIDEO_STEREO_MODE_ROW_AND_COLUMN_INTERLACED   0x0280
#define AR_VIDEO_STEREO_MODE_ANAGLYPH_RG                 0x02C0
#define AR_VIDEO_STEREO_MODE_ANAGLYPH_RB                 0x0300
#define AR_VIDEO_STEREO_MODE_RESERVED0                   0x0340
#define AR_VIDEO_STEREO_MODE_RESERVED1                   0x0380
#define AR_VIDEO_STEREO_MODE_RESERVED2                   0x03C0
    
#define AR_VIDEO_SOURCE_INFO_FLAG_OFFLINE       0x0001      // 0 = unknown or not offline, 1 = offline.
#define AR_VIDEO_SOURCE_INFO_FLAG_IN_USE        0x0002      // 0 = unknown or not in use, 1 = in use.
#define AR_VIDEO_SOURCE_INFO_FLAG_OPEN_ASYNC    0x0004      // 0 = open normally, 1 = open async.
#define AR_VIDEO_SOURCE_INFO_POSITION_MASK      0x0038      // compare (value & AR_VIDEO_SOURCE_INFO_POSITION_MASK) against enums.
#define AR_VIDEO_SOURCE_INFO_STEREO_MODE_MASK   0x03C0      // compare (value & AR_VIDEO_SOURCE_INFO_STEREO_MODE_MASK) against enums.
    
typedef struct {
    char *name;             // UTF-8 encoded string.
    char *UID;              // UTF-8 encoded string.
    unsigned int flags;
} ARVideoSourceInfoT;

typedef struct {
    int count;
    ARVideoSourceInfoT *info;
} ARVideoSourceInfoListT;

typedef void (*AR_VIDEO_FRAME_READY_CALLBACK)(void *);
    
#ifdef _WIN32
#  ifndef LIBARVIDEO_STATIC
#    ifdef LIBARVIDEO_EXPORTS
#      define AR_DLL_API __declspec(dllexport)
#    else
#      define AR_DLL_API __declspec(dllimport)
#    endif
#  else
#    define AR_DLL_API extern
#  endif
#else
#  define AR_DLL_API
#endif

#ifdef AR_INPUT_DUMMY
#include <AR/sys/videoDummy.h>
#endif
#ifdef AR_INPUT_V4L
#include <AR/sys/videoLinuxV4L.h>
#endif
#ifdef AR_INPUT_V4L2
#include <AR/sys/videoLinuxV4L2.h>
#endif
#ifdef AR_INPUT_DV
#include <AR/sys/videoLinuxDV.h>
#endif
#ifdef AR_INPUT_1394CAM
#include <AR/sys/videoLinux1394Cam.h>
#endif
#ifdef AR_INPUT_SGI
#include <AR/sys/videoSGI.h>
#endif
#ifdef AR_INPUT_WINDOWS_DIRECTSHOW
#include <AR/sys/videoWindowsDirectShow.h>
#endif
#ifdef AR_INPUT_WINDOWS_DSVIDEOLIB
#include <AR/sys/videoWindowsDSVideoLib.h>
#endif
#ifdef AR_INPUT_WINDOWS_DRAGONFLY
#include <AR/sys/videoWindowsDragonFly.h>
#endif
#ifdef AR_INPUT_QUICKTIME
#include <AR/sys/videoQuickTime.h>
#endif
#ifdef AR_INPUT_GSTREAMER
#include <AR/sys/videoGStreamer.h>
#endif
#ifdef AR_INPUT_IPHONE
#include <AR/sys/videoiPhone.h>
#endif
#ifdef AR_INPUT_QUICKTIME7
#include <AR/sys/videoQuickTime7.h>
#endif
#ifdef AR_INPUT_IMAGE
#include <AR/sys/videoImage.h>
#endif
#ifdef AR_INPUT_ANDROID
#include <AR/sys/videoAndroid.h>
#endif
#ifdef AR_INPUT_WINDOWS_MEDIA_FOUNDATION
#include <AR/sys/videoWindowsMediaFoundation.h>
#endif
#ifdef AR_INPUT_WINDOWS_MEDIA_CAPTURE
#include <AR/sys/videoWindowsMediaCapture.h>
#endif
    

typedef union {
#ifdef AR_INPUT_DUMMY
    AR2VideoParamDummyT         *dummy;
#endif
#ifdef AR_INPUT_V4L
    AR2VideoParamV4LT           *v4l;
#endif
#ifdef AR_INPUT_V4L2
    AR2VideoParamV4L2T          *v4l2;
#endif
#ifdef AR_INPUT_DV
    AR2VideoParamDVT            *dv;
#endif
#ifdef AR_INPUT_1394CAM
    AR2VideoParam1394T          *cam1394;
#endif
#ifdef AR_INPUT_SGI
    AR2VideoParamSGIT           *sgi;
#endif
#ifdef AR_INPUT_WINDOWS_DIRECTSHOW
    AR2VideoParamWinDST         *winDS;
#endif
#ifdef AR_INPUT_WINDOWS_DSVIDEOLIB
    AR2VideoParamWinDSVLT       *winDSVL;
#endif
#ifdef AR_INPUT_WINDOWS_DRAGONFLY
    AR2VideoParamWinDFT         *winDF;
#endif
#ifdef AR_INPUT_QUICKTIME
    AR2VideoParamQuickTimeT     *quickTime;
#endif
#ifdef AR_INPUT_GSTREAMER
    AR2VideoParamGStreamerT     *gstreamer;
#endif
#ifdef AR_INPUT_IPHONE
    AR2VideoParamiPhoneT        *iPhone;
#endif
#ifdef AR_INPUT_QUICKTIME7
    AR2VideoParamQuickTime7T    *quickTime7;
#endif
#ifdef AR_INPUT_IMAGE
    AR2VideoParamImageT         *image;
#endif
#ifdef AR_INPUT_ANDROID
    AR2VideoParamAndroidT       *android;
#endif
#ifdef AR_INPUT_WINDOWS_MEDIA_FOUNDATION
    AR2VideoParamWinMFT         *winMF;
#endif
#ifdef AR_INPUT_WINDOWS_MEDIA_CAPTURE
    AR2VideoParamWinMCT         *winMC;
#endif
} AR2VideoDeviceHandleT;

typedef struct {
    int                    deviceType;
    AR2VideoDeviceHandleT  device;
} AR2VideoParamT;

AR_DLL_API int               arVideoGetDefaultDevice(void);
AR_DLL_API int               arVideoOpen            (const char *config);
AR_DLL_API int               arVideoOpenAsync       (const char *config, void (*callback)(void *), void *userdata);
AR_DLL_API int               arVideoClose           (void);
AR_DLL_API int               arVideoDispOption      (void);
AR_DLL_API int               arVideoGetDevice       (void);
AR_DLL_API int               arVideoGetId           (ARUint32 *id0, ARUint32 *id1);
AR_DLL_API int               arVideoGetSize         (int *x, int *y);
AR_DLL_API int               arVideoGetPixelSize    (void);
AR_DLL_API AR_PIXEL_FORMAT   arVideoGetPixelFormat  (void);
AR_DLL_API ARUint8          *arVideoGetImage        (void);
AR_DLL_API int               arVideoCapStart        (void);
AR_DLL_API int               arVideoCapStartAsync   (AR_VIDEO_FRAME_READY_CALLBACK callback, void *userdata);
AR_DLL_API int               arVideoCapStop         (void);
AR_DLL_API int               arVideoGetParami       (int paramName, int *value);
AR_DLL_API int               arVideoSetParami       (int paramName, int  value);
AR_DLL_API int               arVideoGetParamd       (int paramName, double *value);
AR_DLL_API int               arVideoSetParamd       (int paramName, double  value);
AR_DLL_API int               arVideoGetParams       (const int paramName, char *value);
AR_DLL_API int               arVideoSetParams       (const int paramName, const char  *value);
AR_DLL_API int               arVideoSaveParam       (char *filename);
AR_DLL_API int               arVideoLoadParam       (char *filename);
AR_DLL_API int               arVideoSetBufferSize   (const int width, const int height);
AR_DLL_API int               arVideoGetBufferSize   (int *width, int *height);
AR_DLL_API int               arVideoGetCParam       (ARParam *cparam);
AR_DLL_API int               arVideoGetCParamAsync  (void (*callback)(const ARParam *, void *), void *userdata);
    
AR_DLL_API int               arVideoUtilGetPixelSize(const AR_PIXEL_FORMAT arPixelFormat);
AR_DLL_API const char       *arVideoUtilGetPixelFormatName(const AR_PIXEL_FORMAT arPixelFormat);
#if !AR_ENABLE_MINIMIZE_MEMORY_FOOTPRINT
AR_DLL_API int               arVideoSaveImageJPEG(int w, int h, AR_PIXEL_FORMAT pixFormat, ARUint8 *pixels, const char *filename, const int quality /* 0 to 100 */, const int flipV);
#endif // !AR_ENABLE_MINIMIZE_MEMORY_FOOTPRINT

typedef enum {
    AR_VIDEO_ASPECT_RATIO_1_1,       // 1.0:
    AR_VIDEO_ASPECT_RATIO_11_9,      // 1.222: 176x144 (QCIF), 352x288 (CIF)
    AR_VIDEO_ASPECT_RATIO_5_4,       // 1.25:  1280x1024 (SXGA), 2560x2048
    AR_VIDEO_ASPECT_RATIO_4_3,       // 1.333: 320x240 (QVGA), 480x360, 640x480 (VGA), 768x576 (576p), 800x600 (SVGA), 960x720, 1024x768 (XGA), 1152x864, 1280x960, 1400x1050, 1600x1200, 2048x1536
    AR_VIDEO_ASPECT_RATIO_3_2,       // 1.5:   240x160, 480x320, 960x640, 720x480 (480p), 1152x768, 1280x854, 1440x960
    AR_VIDEO_ASPECT_RATIO_14_9,      // 1.556:
    AR_VIDEO_ASPECT_RATIO_8_5,       // 1.6:   320x200, 1280x800, 1440x900, 1680x1050, 1920x1200, 2560x1600

    AR_VIDEO_ASPECT_RATIO_5_3,       // 1.667: 800x480, 1280x768, 1600x960
    AR_VIDEO_ASPECT_RATIO_16_9,      // 1.778: 1280x720 (720p), 1920x1080 (1080p)
    AR_VIDEO_ASPECT_RATIO_9_5,       // 1.8:   864x480
    AR_VIDEO_ASPECT_RATIO_17_9,      // 1.889: 2040x1080
    AR_VIDEO_ASPECT_RATIO_UNIQUE
} AR_VIDEO_ASPECT_RATIO;

/*!
    @function
    @abstract Determine the approximate aspect ratio for a given image resolution.
    @discussion
        A convenience method which makes it easy to determine the approximate aspect ratio
        of an image with the given resolution (expressed in pixel width and height).
        Returns a symbolic constant for the aspect ratio, which makes it easy to determine
        whether two different resolutions have the same aspect ratio. Assumes square pixels.
    @param w Width in pixels
    @param h Height in pixels
    @result If a matching commonly-used aspect ratio can be found, returns symbolic constant for that aspect ratio.
*/
AR_VIDEO_ASPECT_RATIO arVideoUtilFindAspectRatio(int w, int h);

/*!
    @function
    @abstract Determine the approximate aspect ratio for a given image resolution.
    @discussion
        A convenience method which makes it easy to determine the approximate aspect ratio
        of an image with the given resolution (expressed in pixel width and height).
        Returns a string for the aspect ratio. Assumes square pixels.
    @param w Width in pixels
    @param h Height in pixels
    @result If a matching commonly-used aspect ratio can be found, returns string name for that aspect ratio. This string must be free'd when finished with.
*/
char *arVideoUtilFindAspectRatioName(int w, int h);

/*!
    @function   arVideoGetVersion
    @abstract   Get the version of ARToolKit with which the arVideo library was built.
    @discussion
        It is highly recommended that
        any calling program that depends on features in a certain
        ARToolKit version, check at runtime that it is linked to a version
        of ARToolKit that can supply those features. It is NOT sufficient
        to check the ARToolKit SDK header versions, since with ARToolKit implemented
        in dynamically-loaded libraries, there is no guarantee that the
        version of ARToolKit installed on the machine at run-time will be as
        recent as the version of the ARToolKit SDK which the host
        program was compiled against.
    @result
        Returns the full version number of the ARToolKit version corresponding
        to this video library, in binary coded decimal (BCD) format.
 
        BCD format allows simple tests of version number in the caller
        e.g. if ((arGetVersion(NULL) >> 16) > 0x0272) printf("This release is later than 2.72\n");
 
        The major version number is encoded in the most-significant byte
        (bits 31-24), the minor version number in the second-most-significant
        byte (bits 23-16), the tiny version number in the third-most-significant
        byte (bits 15-8), and the build version number in the least-significant
        byte (bits 7-0).
 
        If the returned value is equal to -1, it can be assumed that the actual
        version is in the range 0x04000000 to 0x04040100.
    @availability Available in ARToolKit v4.4.2 and later. The underlying
        functionality can also be used when compiling against earlier versions of
        ARToolKit (v4.0 to 4.4.1) with a slightly different syntax: replace
        "arVideoGetVersion()" in your code with "arVideoGetParami(AR_VIDEO_GET_VERSION, NULL)".
 */
#define  arVideoGetVersion() arVideoGetParami(AR_VIDEO_GET_VERSION, NULL)

AR_DLL_API ARVideoSourceInfoListT *ar2VideoCreateSourceInfoList(const char *config);
AR_DLL_API void              ar2VideoDeleteSourceInfoList(ARVideoSourceInfoListT **p);
AR_DLL_API AR2VideoParamT   *ar2VideoOpen            (const char *config);
AR_DLL_API AR2VideoParamT   *ar2VideoOpenAsync       (const char *config, void (*callback)(void *), void *userdata);
AR_DLL_API int               ar2VideoClose           (AR2VideoParamT *vid);
AR_DLL_API int               ar2VideoDispOption      (AR2VideoParamT *vid);
AR_DLL_API int               ar2VideoGetDevice       (AR2VideoParamT *vid);
AR_DLL_API int               ar2VideoGetId           (AR2VideoParamT *vid, ARUint32 *id0, ARUint32 *id1);
AR_DLL_API int               ar2VideoGetSize         (AR2VideoParamT *vid, int *x,int *y);
AR_DLL_API int               ar2VideoGetPixelSize    (AR2VideoParamT *vid);
AR_DLL_API AR_PIXEL_FORMAT   ar2VideoGetPixelFormat  (AR2VideoParamT *vid);
AR_DLL_API AR2VideoBufferT  *ar2VideoGetImage        (AR2VideoParamT *vid);
AR_DLL_API int               ar2VideoCapStart        (AR2VideoParamT *vid);
AR_DLL_API int               ar2VideoCapStartAsync   (AR2VideoParamT *vid, AR_VIDEO_FRAME_READY_CALLBACK callback, void *userdata);
AR_DLL_API int               ar2VideoCapStop         (AR2VideoParamT *vid);
AR_DLL_API int               ar2VideoGetParami       (AR2VideoParamT *vid, int paramName, int *value);
AR_DLL_API int               ar2VideoSetParami       (AR2VideoParamT *vid, int paramName, int  value);
AR_DLL_API int               ar2VideoGetParamd       (AR2VideoParamT *vid, int paramName, double *value);
AR_DLL_API int               ar2VideoSetParamd       (AR2VideoParamT *vid, int paramName, double  value);
AR_DLL_API int               ar2VideoGetParams       (AR2VideoParamT *vid, const int paramName, char **value);
AR_DLL_API int               ar2VideoSetParams       (AR2VideoParamT *vid, const int paramName, const char  *value);
AR_DLL_API int               ar2VideoSaveParam       (AR2VideoParamT *vid, char *filename);
AR_DLL_API int               ar2VideoLoadParam       (AR2VideoParamT *vid, char *filename);
AR_DLL_API int               ar2VideoSetBufferSize   (AR2VideoParamT *vid, const int width, const int height);
AR_DLL_API int               ar2VideoGetBufferSize   (AR2VideoParamT *vid, int *width, int *height);
AR_DLL_API int               ar2VideoGetCParam       (AR2VideoParamT *vid, ARParam *cparam);
AR_DLL_API int               ar2VideoGetCParamAsync  (AR2VideoParamT *vid, void (*callback)(const ARParam *, void *), void *userdata);
#ifdef  __cplusplus
}
#endif
#endif
