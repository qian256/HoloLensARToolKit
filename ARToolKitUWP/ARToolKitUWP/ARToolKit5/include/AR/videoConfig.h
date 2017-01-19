/*
 *	videoConfig.h
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
 *  Author(s): Hirokazu Kato, Philip Lamb
 *
 */

#ifndef AR_CONFIG_H
#define AR_CONFIG_H

#include <AR/config.h>

#ifdef AR_INPUT_V4L
#define   AR_VIDEO_V4L_MODE_PAL              0
#define   AR_VIDEO_V4L_MODE_NTSC             1
#define   AR_VIDEO_V4L_MODE_SECAM            2
#define   AR_VIDEO_V4L_DEFAULT_DEVICE        "/dev/video0"
#define   AR_VIDEO_V4L_DEFAULT_WIDTH         640
#define   AR_VIDEO_V4L_DEFAULT_HEIGHT        480
#define   AR_VIDEO_V4L_DEFAULT_CHANNEL       3
#define   AR_VIDEO_V4L_DEFAULT_MODE          AR_VIDEO_V4L_MODE_NTSC
#endif

#ifdef AR_INPUT_V4L2
#define   AR_VIDEO_V4L2_MODE_PAL              0
#define   AR_VIDEO_V4L2_MODE_NTSC             1
#define   AR_VIDEO_V4L2_MODE_SECAM            2
#define   AR_VIDEO_V4L2_DEFAULT_DEVICE        "/dev/video0"
#define   AR_VIDEO_V4L2_DEFAULT_WIDTH         640
#define   AR_VIDEO_V4L2_DEFAULT_HEIGHT        480
#define   AR_VIDEO_V4L2_DEFAULT_CHANNEL       0
#define   AR_VIDEO_V4L2_DEFAULT_MODE          AR_VIDEO_V4L2_MODE_NTSC
#endif


#ifdef AR_INPUT_1394CAM
#define   AR_VIDEO_1394_MODE_320x240_YUV422            32
#define   AR_VIDEO_1394_MODE_640x480_YUV411            33
#define   AR_VIDEO_1394_MODE_640x480_YUV411_HALF       34
#define   AR_VIDEO_1394_MODE_640x480_YUV422            35
#define   AR_VIDEO_1394_MODE_640x480_RGB               36
#define   AR_VIDEO_1394_MODE_640x480_MONO              37
#define   AR_VIDEO_1394_MODE_640x480_MONO_COLOR        38
#define   AR_VIDEO_1394_MODE_640x480_MONO_COLOR_HALF   39
#define   AR_VIDEO_1394_MODE_640x480_MONO_COLOR2       40
#define   AR_VIDEO_1394_MODE_640x480_MONO_COLOR_HALF2  41
#define   AR_VIDEO_1394_MODE_640x480_MONO_COLOR3       42
#define   AR_VIDEO_1394_MODE_640x480_MONO_COLOR_HALF3  43
#define   AR_VIDEO_1394_MODE_1024x768_MONO             44
#define   AR_VIDEO_1394_MODE_1024x768_MONO_COLOR       45
#define   AR_VIDEO_1394_MODE_1024x768_MONO_COLOR_HALF  46
#define   AR_VIDEO_1394_MODE_1024x768_MONO_COLOR2      47
#define   AR_VIDEO_1394_MODE_1024x768_MONO_COLOR_HALF2 48
#define   AR_VIDEO_1394_MODE_1024x768_MONO_COLOR3      49
#define   AR_VIDEO_1394_MODE_1024x768_MONO_COLOR_HALF3 50
#define   AR_VIDEO_1394_MODE_1280x720_MONO             51
#define   AR_VIDEO_1394_MODE_1280x720_MONO_COLOR       52
#define   AR_VIDEO_1394_MODE_1280x720_MONO_COLOR_HALF  53
#define   AR_VIDEO_1394_MODE_1280x720_MONO_COLOR2      54
#define   AR_VIDEO_1394_MODE_1280x720_MONO_COLOR_HALF2 55
#define   AR_VIDEO_1394_MODE_1280x720_MONO_COLOR3      56
#define   AR_VIDEO_1394_MODE_1280x720_MONO_COLOR_HALF3 57
#define   AR_VIDEO_1394_MODE_1600x900_MONO             58
#define   AR_VIDEO_1394_MODE_1600x900_MONO_COLOR       59
#define   AR_VIDEO_1394_MODE_1600x900_MONO_COLOR_HALF  60
#define   AR_VIDEO_1394_MODE_1600x900_MONO_COLOR2      61
#define   AR_VIDEO_1394_MODE_1600x900_MONO_COLOR_HALF2 62
#define   AR_VIDEO_1394_MODE_1600x900_MONO_COLOR3      63
#define   AR_VIDEO_1394_MODE_1600x900_MONO_COLOR_HALF3 64
#define   AR_VIDEO_1394_MODE_1600x1200_MONO             65
#define   AR_VIDEO_1394_MODE_1600x1200_MONO_COLOR       66
#define   AR_VIDEO_1394_MODE_1600x1200_MONO_COLOR_HALF  67
#define   AR_VIDEO_1394_MODE_1600x1200_MONO_COLOR2      68
#define   AR_VIDEO_1394_MODE_1600x1200_MONO_COLOR_HALF2 69
#define   AR_VIDEO_1394_MODE_1600x1200_MONO_COLOR3      70
#define   AR_VIDEO_1394_MODE_1600x1200_MONO_COLOR_HALF3 71
#define   AR_VIDEO_1394_FRAME_RATE_1_875                1
#define   AR_VIDEO_1394_FRAME_RATE_3_75                 2
#define   AR_VIDEO_1394_FRAME_RATE_7_5                  3
#define   AR_VIDEO_1394_FRAME_RATE_15                   4
#define   AR_VIDEO_1394_FRAME_RATE_30                   5
#define   AR_VIDEO_1394_FRAME_RATE_60                   6
#define   AR_VIDEO_1394_FRAME_RATE_120                  7
#define   AR_VIDEO_1394_FRAME_RATE_240                  8
#define   AR_VIDEO_1394_SPEED_400                       1
#define   AR_VIDEO_1394_SPEED_800                       2

#if AR_INPUT_1394CAM_DEFAULT_PIXEL_FORMAT == AR_PIXEL_FORMAT_MONO
#if defined(AR_INPUT_1394CAM_USE_FLEA_XGA)
#define   AR_VIDEO_1394_DEFAULT_MODE                   AR_VIDEO_1394_MODE_1024x768_MONO
#else
#define   AR_VIDEO_1394_DEFAULT_MODE                   AR_VIDEO_1394_MODE_640x480_MONO
#endif
#else
#if defined(AR_INPUT_1394CAM_USE_DRAGONFLY)
#define   AR_VIDEO_1394_DEFAULT_MODE                   AR_VIDEO_1394_MODE_640x480_MONO_COLOR
#elif defined(AR_INPUT_1394CAM_USE_DF_EXPRESS)
#define   AR_VIDEO_1394_DEFAULT_MODE                   AR_VIDEO_1394_MODE_640x480_MONO_COLOR2
#elif defined(AR_INPUT_1394CAM_USE_FLEA)
#define   AR_VIDEO_1394_DEFAULT_MODE                   AR_VIDEO_1394_MODE_640x480_MONO_COLOR2
#elif defined(AR_INPUT_1394CAM_USE_FLEA_XGA)
#define   AR_VIDEO_1394_DEFAULT_MODE                   AR_VIDEO_1394_MODE_1024x768_MONO_COLOR
#elif defined(AR_INPUT_1394CAM_USE_DFK21AF04)
#define   AR_VIDEO_1394_DEFAULT_MODE                   AR_VIDEO_1394_MODE_640x480_MONO_COLOR3
#else
#define   AR_VIDEO_1394_DEFAULT_MODE                   AR_VIDEO_1394_MODE_640x480_YUV411
#endif
#endif
#define   AR_VIDEO_1394_DEFAULT_FRAME_RATE             AR_VIDEO_1394_FRAME_RATE_30
#define   AR_VIDEO_1394_DEFAULT_SPEED                  AR_VIDEO_1394_SPEED_400
#define   AR_VIDEO_1394_DEFAULT_PORT                   0
#endif


#ifdef AR_INPUT_WINDOWS_DIRECTSHOW
#endif

#ifdef AR_INPUT_WINDOWS_DRAGONFLY
#endif


#ifdef AR_INPUT_QUICKTIME
#endif

#endif // !AR_CONFIG_H
