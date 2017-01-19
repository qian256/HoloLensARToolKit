/*
 *  videoLinuxV4L.h
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
 *  Author(s): Hirokazu Kato
 *
 */
/*******************************************************
 *
 * Author: Hirokazu Kato
 *
 *         kato@sys.im.hiroshima-cu.ac.jp
 *
 * Revision: 5.4
 * Date: 2004/01/01
 *
 *******************************************************/

#ifndef AR_VIDEO_LINUX_V4L_H
#define AR_VIDEO_LINUX_V4L_H

#include <stdlib.h>
#include <linux/types.h>
//#include <linux/videodev.h>
#include <libv4l1-videodev.h>
#include <pthread.h>
#include <AR/ar.h>
#include <AR/videoConfig.h>

#ifdef  __cplusplus
extern "C" {
#endif

#define   AR2VIDEO_V4L_STATUS_IDLE    0
#define   AR2VIDEO_V4L_STATUS_RUN     1
#define   AR2VIDEO_V4L_STATUS_STOP    2

typedef struct {
    AR2VideoBufferT     in;
    AR2VideoBufferT     wait;
    AR2VideoBufferT     out;
    pthread_mutex_t     mutex;
} AR2VideoBufferV4LT;

typedef struct {
    char                   dev[256];
    int                    width;
    int                    height;
    int                    channel;
    int                    mode;
    AR_PIXEL_FORMAT        format;
    int                    debug;
    double                 contrast;
    double                 brightness;
    double                 hue;
    double                 whiteness;
    double                 color;

    int                    fd;
    int                    status;
    int                    video_cont_num;
    ARUint8                *map;
    struct video_mbuf      vm;
    struct video_mmap      vmm;
    pthread_t              capture;
    AR2VideoBufferV4LT     buffer;
} AR2VideoParamV4LT;


int                  ar2VideoDispOptionV4L     ( void );
AR2VideoParamV4LT   *ar2VideoOpenV4L           ( const char *config );
int                  ar2VideoCloseV4L          ( AR2VideoParamV4LT *vid );
int                  ar2VideoGetIdV4L          ( AR2VideoParamV4LT *vid, ARUint32 *id0, ARUint32 *id1 );
int                  ar2VideoGetSizeV4L        ( AR2VideoParamV4LT *vid, int *x,int *y );
AR_PIXEL_FORMAT      ar2VideoGetPixelFormatV4L ( AR2VideoParamV4LT *vid );
AR2VideoBufferT     *ar2VideoGetImageV4L       ( AR2VideoParamV4LT *vid );
int                  ar2VideoCapStartV4L       ( AR2VideoParamV4LT *vid );
int                  ar2VideoCapStopV4L        ( AR2VideoParamV4LT *vid );

int                    ar2VideoGetParamiV4L    ( AR2VideoParamV4LT *vid, int paramName, int *value );
int                    ar2VideoSetParamiV4L    ( AR2VideoParamV4LT *vid, int paramName, int  value );
int                    ar2VideoGetParamdV4L    ( AR2VideoParamV4LT *vid, int paramName, double *value );
int                    ar2VideoSetParamdV4L    ( AR2VideoParamV4LT *vid, int paramName, double  value );


#ifdef  __cplusplus
}
#endif
#endif
