/*
 *  arImageProc.h
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
 */

#ifndef AR_IMAGEPROC_H
#define AR_IMAGEPROC_H

#ifndef TRUE
#  define TRUE 1
#endif
#ifndef FALSE
#  define FALSE 0
#endif

#include <AR/config.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __APPLE__
#  if !AR_DISABLE_THRESH_MODE_AUTO_ADAPTIVE
#    define AR_IMAGEPROC_USE_VIMAGE 1
#  endif
#endif

struct _ARImageProcInfo {
    unsigned char *__restrict image; // Buffer holds result of conversion to luminance image (8 bit grayscale).
    unsigned char *__restrict image2; // Extra buffer, allocated as required.
    int imageX; // Width of image buffer.
    int imageY; // Height of image buffer.
    unsigned long histBins[256]; // Luminance histogram.
    unsigned long cdfBins[256]; // Luminance cumulative density function.
    unsigned char min; // Minimum luminance.
    unsigned char max; // Maximum luminance.
#if AR_IMAGEPROC_USE_VIMAGE
    void *tempBuffer;
#endif
    AR_PIXEL_FORMAT pixFormat; // Expected pixel format of incoming images.
    int alwaysCopy;
    int imageWasAllocated;
#ifdef HAVE_ARM_NEON
    int fastPath;
#endif
};
typedef struct _ARImageProcInfo ARImageProcInfo;
    
#ifdef __cplusplus
}
#endif

#include <AR/ar.h>

#ifdef __cplusplus
extern "C" {
#endif

ARImageProcInfo *arImageProcInit(const int xsize, const int ysize, const AR_PIXEL_FORMAT pixFormat, int alwaysCopy);
void arImageProcFinal(ARImageProcInfo *ipi);
int arImageProcLuma(ARImageProcInfo *ipi, const ARUint8 *__restrict dataPtr);
int arImageProcLumaHist(ARImageProcInfo *ipi, const ARUint8 *__restrict dataPtr);
unsigned char *arImageProcGetHistImage(ARImageProcInfo *ipi);
int arImageProcLumaHistAndCDF(ARImageProcInfo *ipi, const ARUint8 *__restrict dataPtr);
int arImageProcLumaHistAndCDFAndPercentile(ARImageProcInfo *ipi, const ARUint8 *__restrict dataPtr, const float percentile, unsigned char *value_p);
int arImageProcLumaHistAndCDFAndMedian(ARImageProcInfo *ipi, const ARUint8 *__restrict dataPtr, unsigned char *value_p);
int arImageProcLumaHistAndOtsu(ARImageProcInfo *ipi, const ARUint8 *__restrict dataPtr, unsigned char *value_p);
#if !AR_DISABLE_THRESH_MODE_AUTO_ADAPTIVE
int arImageProcLumaHistAndBoxFilterWithBias(ARImageProcInfo *ipi, const ARUint8 *__restrict dataPtr, const int boxSize, const int bias);
#endif
int arImageProcLumaHistAndCDFAndLevels(ARImageProcInfo *ipi, const ARUint8 *__restrict dataPtr);

#ifdef __cplusplus
}
#endif

#endif // !AR_IMAGEPROC_H