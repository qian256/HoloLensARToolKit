/*
 *  gsubConfig.h
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

#ifndef AR_GSUB_CONFIG_H
#define AR_GSUB_CONFIG_H

/*   NUM of TEXTURE MESH   */
#define  AR_GL_TEXTURE_MESH_NUM                   40

/*   MIN_LINE_SEGMENT      */
#define  AR_GL_MIN_LINE_SEGMENT                   10

/*   dispMethod    */
#define  AR_GL_DISP_METHOD_GL_DRAW_PIXELS          0
#define  AR_GL_DISP_METHOD_TEXTURE_MAPPING_FRAME   1
#define  AR_GL_DISP_METHOD_TEXTURE_MAPPING_FIELD   2
#define  AR_GL_DEFAULT_DISP_METHOD                 AR_GL_DISP_METHOD_TEXTURE_MAPPING_FRAME

/*    dispMode     */
#define  AR_GL_DISP_MODE_FIT_TO_VIEWPORT                            0
#define  AR_GL_DISP_MODE_FIT_TO_VIEWPORT_KEEP_ASPECT_RATIO          1
#define  AR_GL_DISP_MODE_FIT_TO_VIEWPORT_HEIGHT_KEEP_ASPECT_RATIO   2
#define  AR_GL_DISP_MODE_FIT_TO_VIEWPORT_WIDTH_KEEP_ASPECT_RATIO    3
#define  AR_GL_DISP_MODE_USE_SPECIFIED_SCALE                        4 
#define  AR_GL_DEFAULT_DISP_MODE                                    AR_GL_DISP_MODE_FIT_TO_VIEWPORT

#ifndef AR_IOS
/*    flipMode     */
#define  AR_GL_FLIP_H                              1
#define  AR_GL_FLIP_V                              2
#define  AR_GL_FLIP_DISABLE                        0
#define  AR_GL_DEFAULT_FLIP_MODE                   AR_GL_FLIP_DISABLE
#endif

/*    distortionMode    */
#define   AR_GL_DISTORTION_COMPENSATE_DISABLE      0
#define   AR_GL_DISTORTION_COMPENSATE_ENABLE       1
#define   AR_GL_DEFAULT_DISTORTION_MODE            AR_GL_DISTORTION_COMPENSATE_ENABLE

/*    clipPlane  */
#define   AR_GL_DEFAULT_CLIP_NEAR               50.0
#define   AR_GL_DEFAULT_CLIP_FAR              5000.0

/*    scaleLimit */
#define   AR_GL_DISP_SCALE_MAX                  5.0
#define   AR_GL_DISP_SCALE_MIN                  0.3

#endif
