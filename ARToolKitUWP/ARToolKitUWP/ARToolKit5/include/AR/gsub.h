/*
 *  gsub.h
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
 *  Copyright 2003-2015 ARToolworks, Inc.
 *
 *  Author(s): Hirokazu Kato
 *
 */

#ifndef AR_GSUB_H
#define AR_GSUB_H

#ifndef __APPLE__
#  ifdef _WIN32
#    include <windows.h>
#  endif
#  include <GL/glut.h>
#  ifdef GL_VERSION_1_2
#    include <GL/glext.h>
#  endif
#else
#  include <GLUT/glut.h>
#  include <OpenGL/glext.h>
#endif
#include <AR/ar.h>
#include <AR/config.h>
#include <AR/gsubConfig.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int                 sx;
    int                 sy;
    int                 xsize;
    int                 ysize;
} ARGViewport;

typedef struct {
    int   clipSx;
    int   clipSy;
    int   clipWidth;
    int   clipHeight;
} ARGImageClip;

typedef struct {
    ARdouble               nearClip;
    ARdouble               farClip;
} ARGClipPlane;

typedef struct {
#ifdef AR_IOS
    int                 imageVBOFlag;
    GLuint              imageVBO;
    GLuint              imageTBO;
    GLuint              imageIBO;
    int                 texIdFlag;
    GLuint              texId;
    int                 texIdYUVFlag;
    GLuint              texIdYUV1;
    GLuint              texIdYUV2;
    GLuint              texIdYUV3;
    ARUint8            *imageUV1;
    ARUint8            *imageUV2;
#else
    int                 texId;
    int                 listIndex;
#endif
} ARGDisplayList;

typedef struct {
    ARGViewport         viewport;
    ARParam            *cparam;
    ARGClipPlane        clipPlane;
    int                 dispMethod;
    int                 dispMode;
#ifndef AR_IOS
    int                 flipMode;
#endif
    int                 distortionMode;
    int                 pixFormat;
    ARdouble            scale;
    ARGImageClip        imageClip;
    ARGDisplayList      dispList;
    ARGDisplayList      dispListHalf;
} ARGViewportHandle;


#ifndef AR_IOS
typedef struct {
    int                 winID;
    int                 xsize;
    int                 ysize;
    int                 possibleTextureRectangle;
    GLint               maxTextureRectangleSize;
} ARGWindowHandle;
#endif


#ifndef AR_IOS
int                 argSetWindow                     ( ARGWindowHandle *w );
int                 argInitWindowPos                 ( int xpos, int ypos );
ARGWindowHandle    *argCreateWindow                  ( int xsize, int ysize );
ARGWindowHandle    *argCreateFullWindow              ( void );
int                 argGetScreenSize                 ( int *xsize, int *ysize );
int                 argSetWindowSize                 ( int  xsize, int  ysize );
#endif
int                 argGetWindowSize                 ( int *xsize, int *ysize );

ARGViewportHandle  *argCreateViewport                ( ARGViewport *pos );
int                 argDeleteViewport                ( ARGViewportHandle **argVPhandle );
int                 argViewportSetViewport           ( ARGViewportHandle *argVPhandle, ARGViewport *viewport );
int                 argViewportSetCparam             ( ARGViewportHandle *argVPhandle, ARParam *cparam );
int                 argViewportSetImageSize          ( ARGViewportHandle *argVPhandle, int xsize, int ysize );
int                 argViewportSetClipPlane          ( ARGViewportHandle *argVPhandle, ARGClipPlane *clipPlane );
int                 argViewportSetDispMethod         ( ARGViewportHandle *argVPhandle, int dispMethod );
int                 argViewportSetDispMode           ( ARGViewportHandle *argVPhandle, int dispMode );
int                 argViewportSetFlipMode           ( ARGViewportHandle *argVPhandle, int flipMode );
int                 argViewportSetDistortionMode     ( ARGViewportHandle *argVPhandle, int distortionMode );
int                 argViewportSetPixFormat          ( ARGViewportHandle *argVPhandle, int pixFormat );
int                 argViewportSetDispScale          ( ARGViewportHandle *argVPhandle, ARdouble scale );
int                 argViewportSetImageClip          ( ARGViewportHandle *argVPhandle, ARGImageClip *imageClip );
int                 argViewportSetImageClip2         ( ARGViewportHandle *argVPhandle, int *centerX, int *centerY, ARdouble *scale );

#ifndef AR_IOS
int                 argCleanup                       ( void );
int                 argSwapBuffers                   ( void );

int                 argSetDispFunc                   ( void (*mainFunc)(void), int idleFlag );
int                 argSetKeyFunc                    ( void (*keyFunc)(unsigned char key, int x, int y) );
int                 argSetMouseFunc                  ( void (*mouseFunc)(int button, int state, int x, int y) );
int                 argSetMotionFunc                 ( void (*motionFunc)(int x, int y) );
void                argMainLoop                      ( void );
void                argDefaultKeyFunc                ( unsigned char key, int x, int y );
#endif

void                argConvGlpara                    ( ARdouble para[3][4], ARdouble gl_para[16] );
int                 argDrawMode2D                    ( ARGViewportHandle *argVPhandle );
int                 argDrawMode3D                    ( ARGViewportHandle *argVPhandle );
int                 argDrawModeHomography            ( ARGViewportHandle *vp, ARdouble z );


int                 argDrawSquareByIdealPos          ( ARdouble vertIdeal[4][2] );
int                 argDrawSquareByObservedPos       ( ARdouble vertObserved[4][2] );
int                 argDrawSquare2ByIdealPos         ( ARdouble vertIdeal[4][2] );
int                 argDrawSquare2ByObservedPos      ( ARdouble vertObserved[4][2] );
int                 argDrawLineByIdealPos            ( ARdouble x1, ARdouble y1, ARdouble x2, ARdouble y2 );
int                 argDrawLineByObservedPos         ( ARdouble x1, ARdouble y1, ARdouble x2, ARdouble y2 );
int                 argDrawPointByIdealPos           ( ARdouble x, ARdouble y );
int                 argDrawPointByObservedPos        ( ARdouble x, ARdouble y );
int                 argDrawStringsByIdealPos         ( char *string, ARdouble sx, ARdouble sy );
int                 argDrawStringsByObservedPos( char *string, ARdouble sx, ARdouble sy );

int                 argDrawImage                     ( ARUint8 *image );
int                 argDrawImageHalf                 ( ARUint8 *image );
int                 argDrawImageYUV                  ( ARUint8 *image, ARUint8 *imageUV);

#ifdef AR_IOS
ARGViewportHandle  *argGetCurrentVPHandle            ( void );
int                 argSetCurrentVPHandle            ( ARGViewportHandle *vp );
int                 argClearDisplayList              ( ARGDisplayList *dispList );
int                 argGetCurrentScale               ( ARGViewportHandle *vp, ARdouble *sx, ARdouble *sy, ARdouble *offx, ARdouble *offy );
#endif

#ifdef __cplusplus
}
#endif
#endif
