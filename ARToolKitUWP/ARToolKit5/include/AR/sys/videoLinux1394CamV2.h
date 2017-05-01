/*
 *	videoLinux1394CamV2.h
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
 *  Copyright 2009-2015 ARToolworks, Inc.
 *
 *  Author(s): Hirokazu Kato
 *
 */

#ifndef AR_VIDEO_LINUX_1394CAM2_H
#define AR_VIDEO_LINUX_1394CAM2_H

#include <stdlib.h>
//#include <dc1394/utils.h>
#include <dc1394/control.h>
#include <pthread.h>
#include <AR/ar.h>
#include <AR/videoConfig.h>

#ifdef  __cplusplus
extern "C" {
#endif


#define   AR2VIDEO_1394_STATUS_IDLE    0
#define   AR2VIDEO_1394_STATUS_RUN     1
#define   AR2VIDEO_1394_STATUS_STOP    2

typedef struct {
    AR2VideoBufferT     in;
    AR2VideoBufferT     wait;
    AR2VideoBufferT     out;
    pthread_mutex_t     mutex;
} AR2VideoBuffer1394T;

typedef struct {
    int                    mode;
    int                    rate;
    int                    speed;
    int                    format7;
    int                    debug;

    uint32_t               guid[2];
    int                    width;
    int                    height;
    int                    bits;
    int                    dma_buf_num;
    int                    internal_id;
    int                    status;
    dc1394camera_t        *camera;
    dc1394video_mode_t     int_mode;
    dc1394video_modes_t    modes;
    dc1394framerate_t      int_rate;
    dc1394framerates_t     rates;
    dc1394speed_t          int_speed;
    dc1394operation_mode_t int_opmode;
    dc1394featureset_t     features;
    pthread_t              capture;
    AR2VideoBuffer1394T    buffer;
} AR2VideoParam1394T;


int                   ar2VideoDispOption1394     ( void );
AR2VideoParam1394T   *ar2VideoOpen1394           ( const char *config );
int                   ar2VideoClose1394          ( AR2VideoParam1394T *vid );
int                   ar2VideoGetId1394          ( AR2VideoParam1394T *vid, ARUint32 *id0, ARUint32 *id1 );
int                   ar2VideoGetSize1394        ( AR2VideoParam1394T *vid, int *x,int *y );
AR_PIXEL_FORMAT       ar2VideoGetPixelFormat1394 ( AR2VideoParam1394T *vid );
AR2VideoBufferT      *ar2VideoGetImage1394       ( AR2VideoParam1394T *vid );
int                   ar2VideoCapStart1394       ( AR2VideoParam1394T *vid );
int                   ar2VideoCapStop1394        ( AR2VideoParam1394T *vid );

int                    ar2VideoGetParami1394     ( AR2VideoParam1394T *vid, int paramName, int *value );
int                    ar2VideoSetParami1394     ( AR2VideoParam1394T *vid, int paramName, int  value );
int                    ar2VideoGetParamd1394     ( AR2VideoParam1394T *vid, int paramName, double *value );
int                    ar2VideoSetParamd1394     ( AR2VideoParam1394T *vid, int paramName, double  value );

int                    ar2VideoGetValue1394                   ( AR2VideoParam1394T *vid, int paramName, int *value );
int                    ar2VideoSetValue1394                   ( AR2VideoParam1394T *vid, int paramName, int  value );
int                    ar2VideoGetAutoOn1394                  ( AR2VideoParam1394T *vid, int paramName, int *value );
int                    ar2VideoSetAutoOn1394                  ( AR2VideoParam1394T *vid, int paramName, int  value );
int                    ar2VideoGetFeatureOn1394               ( AR2VideoParam1394T *vid, int paramName, int *value );
int                    ar2VideoSetFeatureOn1394               ( AR2VideoParam1394T *vid, int paramName, int  value );
int                    ar2VideoGetMaxValue1394                ( AR2VideoParam1394T *vid, int paramName, int *value );
int                    ar2VideoGetMinValue1394                ( AR2VideoParam1394T *vid, int paramName, int *value );

int                    ar2VideoGetAbsValue1394                ( AR2VideoParam1394T *vid, int paramName, ARdouble *value );
int                    ar2VideoSetAbsValue1394                ( AR2VideoParam1394T *vid, int paramName, ARdouble  value );
int                    ar2VideoGetAbsMaxValue1394             ( AR2VideoParam1394T *vid, int paramName, ARdouble *value );
int                    ar2VideoGetAbsMinValue1394             ( AR2VideoParam1394T *vid, int paramName, ARdouble *value );

int                    ar2VideoSaveParam1394                  ( AR2VideoParam1394T *vid, char *filename );
int                    ar2VideoLoadParam1394                  ( AR2VideoParam1394T *vid, char *filename );
#ifdef  __cplusplus
}
#endif
#endif
