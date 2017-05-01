/*
 *  arImageProc.c
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

#include <string.h> // memset(), memcpy()
#include <AR/arImageProc.h>
#if AR_IMAGEPROC_USE_VIMAGE
#  include <Accelerate/Accelerate.h>
#endif

#ifdef HAVE_ARM_NEON
#  include <arm_neon.h>
#  ifdef ANDROID
#    include "cpu-features.h"
#  endif
#endif

#ifdef HAVE_ARM_NEON
static void arImageProcBGRAtoL_ARM_neon_asm(uint8_t * __restrict dest, uint8_t * __restrict src, int numPixels);
static void arImageProcRGBAtoL_ARM_neon_asm(uint8_t * __restrict dest, uint8_t * __restrict src, int numPixels);
static void arImageProcABGRtoL_ARM_neon_asm(uint8_t * __restrict dest, uint8_t * __restrict src, int numPixels);
static void arImageProcARGBtoL_ARM_neon_asm(uint8_t * __restrict dest, uint8_t * __restrict src, int numPixels);
#endif

ARImageProcInfo *arImageProcInit(const int xsize, const int ysize, const AR_PIXEL_FORMAT pixFormat, int alwaysCopy)
{
    ARImageProcInfo *ipi = (ARImageProcInfo *)malloc(sizeof(ARImageProcInfo));
    if (ipi) {
        ipi->pixFormat = pixFormat;
        if (alwaysCopy || (pixFormat != AR_PIXEL_FORMAT_MONO && pixFormat != AR_PIXEL_FORMAT_420v && pixFormat != AR_PIXEL_FORMAT_420f && pixFormat != AR_PIXEL_FORMAT_NV21)) {
            ipi->image = (unsigned char *)malloc(xsize * ysize * sizeof(unsigned char));
            if (!ipi->image) goto bail;
            ipi->imageWasAllocated = TRUE;
        } else {
            ipi->imageWasAllocated = FALSE;
        }
        ipi->alwaysCopy = alwaysCopy;
        ipi->image2 = NULL;
        ipi->imageX = xsize;
        ipi->imageY = ysize;
#if AR_IMAGEPROC_USE_VIMAGE
        ipi->tempBuffer = NULL;
#endif
#ifdef HAVE_ARM_NEON
        ipi->fastPath = (ipi->imageX * ipi->imageY % 8 == 0
                         && (pixFormat == AR_PIXEL_FORMAT_RGBA
                             || pixFormat == AR_PIXEL_FORMAT_BGRA
                             || pixFormat == AR_PIXEL_FORMAT_ABGR
                             ||pixFormat == AR_PIXEL_FORMAT_ARGB
                             )
                         );
#  ifdef ANDROID
        // Not all Android devices with ARMv7 are guaranteed to have NEON, so check.
        uint64_t features = android_getCpuFeatures();
        ipi->fastPath = ipi->fastPath && (features & ANDROID_CPU_ARM_FEATURE_ARMv7) && (features & ANDROID_CPU_ARM_FEATURE_NEON);
#  endif
        if (ipi->fastPath) ARLOGd("arImageProc will use ARM NEON acceleration.\n");
#endif
    }
    return (ipi);
    
bail:
    free(ipi);
    return (NULL);
}

void arImageProcFinal(ARImageProcInfo *ipi)
{
    if (!ipi) return;
    if (ipi->imageWasAllocated) free (ipi->image);
    if (ipi->image2) free (ipi->image2);
#if AR_IMAGEPROC_USE_VIMAGE
    if (ipi->tempBuffer) free (ipi->tempBuffer);
#endif
    free (ipi);
}

int arImageProcLuma(ARImageProcInfo *ipi, const ARUint8 *__restrict dataPtr)
{
    unsigned int p, q;

    AR_PIXEL_FORMAT pixFormat = ipi->pixFormat;
#ifdef HAVE_ARM_NEON
    if (ipi->fastPath) {
        if (pixFormat == AR_PIXEL_FORMAT_BGRA) {
            arImageProcBGRAtoL_ARM_neon_asm(ipi->image, (unsigned char *__restrict)dataPtr, ipi->imageX*ipi->imageY);
        } else if (pixFormat == AR_PIXEL_FORMAT_RGBA) {
            arImageProcRGBAtoL_ARM_neon_asm(ipi->image, (unsigned char *__restrict)dataPtr, ipi->imageX*ipi->imageY);
        } else if (pixFormat == AR_PIXEL_FORMAT_ABGR) {
            arImageProcABGRtoL_ARM_neon_asm(ipi->image, (unsigned char *__restrict)dataPtr, ipi->imageX*ipi->imageY);
        } else /*(pixFormat == AR_PIXEL_FORMAT_ARGB)*/ {
            arImageProcARGBtoL_ARM_neon_asm(ipi->image, (unsigned char *__restrict)dataPtr, ipi->imageX*ipi->imageY);
        }
        return (0);
    }
#endif
    if (pixFormat == AR_PIXEL_FORMAT_MONO || pixFormat == AR_PIXEL_FORMAT_420v || pixFormat == AR_PIXEL_FORMAT_420f || pixFormat == AR_PIXEL_FORMAT_NV21) {
        if (!ipi->alwaysCopy) {
            ipi->image = (unsigned char *__restrict)dataPtr;
        } else {
            memcpy(ipi->image, dataPtr, ipi->imageX * ipi->imageY);
        }
    } else {
        q = 0;
        if (pixFormat == AR_PIXEL_FORMAT_RGBA || pixFormat == AR_PIXEL_FORMAT_BGRA) {
            for (p = 0; p < ipi->imageX * ipi->imageY; p++) {
                ipi->image[p] = (dataPtr[q + 0] + dataPtr[q + 1] + dataPtr[q + 2]) / 3;
                q += 4;
            }
        } else if (pixFormat == AR_PIXEL_FORMAT_ABGR || pixFormat == AR_PIXEL_FORMAT_ARGB) {
            for (p = 0; p < ipi->imageX * ipi->imageY; p++) {
                ipi->image[p] = (dataPtr[q + 1] + dataPtr[q + 2] + dataPtr[q + 3]) / 3;
                q += 4;
            }
        } else if (pixFormat == AR_PIXEL_FORMAT_RGB || pixFormat == AR_PIXEL_FORMAT_BGR) {
            for (p = 0; p < ipi->imageX * ipi->imageY; p++) {
                ipi->image[p] = (dataPtr[q + 0] + dataPtr[q + 1] + dataPtr[q + 2]) / 3;
                q += 3;
            }
        } else if (pixFormat == AR_PIXEL_FORMAT_yuvs) {
            for (p = 0; p < ipi->imageX * ipi->imageY; p++) {
                ipi->image[p] = dataPtr[q + 0];
                q += 2;
            }
        } else if (pixFormat == AR_PIXEL_FORMAT_2vuy) {
            for (p = 0; p < ipi->imageX * ipi->imageY; p++) {
                ipi->image[p] = dataPtr[q + 1];
                q += 2;
            }
        } else if (pixFormat == AR_PIXEL_FORMAT_RGB_565) {
            for (p = 0; p < ipi->imageX * ipi->imageY; p++) {
                ipi->image[p] = ((dataPtr[q + 0] & 0xf8) + ((dataPtr[q + 0] & 0x07) << 5) + ((dataPtr[q + 1] & 0xe0) >> 3) + ((dataPtr[q + 1] & 0x1f) << 3) + 10) / 3;
                q += 2;
            }
        } else if (pixFormat == AR_PIXEL_FORMAT_RGBA_5551) {
            for (p = 0; p < ipi->imageX * ipi->imageY; p++) {
                ipi->image[p] = ((dataPtr[q + 0] & 0xf8) + ((dataPtr[q + 0] & 0x07) << 5) + ((dataPtr[q + 1] & 0xc0) >> 3) + ((dataPtr[q + 1] & 0x3e) << 2) + 12) / 3;
                q += 2;
            }
        } else if (pixFormat == AR_PIXEL_FORMAT_RGBA_4444) {
            for (p = 0; p < ipi->imageX * ipi->imageY; p++) {
                ipi->image[p] = ((dataPtr[q + 0] & 0xf0) + ((dataPtr[q + 0] & 0x0f) << 4) + (dataPtr[q + 1] & 0xf0) + 24) / 3;
                q += 2;
            }
        } else {
            ARLOGe("Error: Unsupported pixel format passed to arImageProcHist().\n");
            return (-1);
        }
    }
    return (0);
}

int arImageProcLumaHist(ARImageProcInfo *ipi, const ARUint8 *__restrict dataPtr)
{
	if (!ipi || !dataPtr) return (-1);

    if (arImageProcLuma(ipi, dataPtr) < 0) {
        return (-1);
    }
    
#ifdef AR_IMAGEPROC_USE_VIMAGE
    vImage_Error err;
    vImage_Buffer buf = {(void *)ipi->image, ipi->imageY, ipi->imageX, ipi->imageX};
    if ((err = vImageHistogramCalculation_Planar8(&buf, ipi->histBins, 0)) != kvImageNoError) {
        ARLOGe("arImageProcLumaHist(): vImageHistogramCalculation_Planar8 error %ld.\n", err);
        return (-1);
    }
#else
    unsigned char *p;
    memset(ipi->histBins, 0, sizeof(ipi->histBins));
    for (p = ipi->image; p < ipi->image + ipi->imageX*ipi->imageY; p++) ipi->histBins[*p]++;
#endif // AR_IMAGEPROC_USE_VIMAGE
    
    return (0);
}

// Returns a pointer to a buffer containing a 256x256 8-bit grayscale texture.
// The texture can be uploaded to an OpenGL texture with the calls:
//     buf = arImageProcGetHistImage(ipi);
//     if (buf) {
//         glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 256, 256, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, buf);
//         free(buf);
//     }
// After uploading, the texture can be drawn as with any other OpenGL texture.
unsigned char *arImageProcGetHistImage(ARImageProcInfo *ipi)
{
    int i, j, y;
    unsigned long peak = 0;
    float scalef;
    
    if (!ipi) return (NULL);
    
    unsigned char *buf = (unsigned char *)calloc(1, 256*256*sizeof(unsigned char));
    if (buf) {
        for (i = 0; i < 256; i++) if (ipi->histBins[i] > peak) peak = ipi->histBins[i]; // Find value of mode.
        scalef = 256.0f / (float)peak;
        for (i = 0; i < 256; i++) {
            y = (int)((float)ipi->histBins[i] * scalef);
            if (y > 256) y = 256; // Safety in case of FP rounding errors etc.
            for (j = 0; j < y; j++) buf[256*j + i] = 0xff;
        }
    }
    return (buf);
}

int arImageProcLumaHistAndCDF(ARImageProcInfo *ipi, const ARUint8 *__restrict dataPtr)
{
    unsigned long cdfCurrent;
	unsigned char i;

	int ret = arImageProcLumaHist(ipi, dataPtr);
    if (ret < 0) return (ret);

    cdfCurrent = 0;
    i = 0;
    do {
        ipi->cdfBins[i] = cdfCurrent + ipi->histBins[i];
        cdfCurrent = ipi->cdfBins[i];
        i++;
    } while (i != 0);
    return (0);
}

int arImageProcLumaHistAndCDFAndPercentile(ARImageProcInfo *ipi, const ARUint8 *__restrict dataPtr, const float percentile, unsigned char *value_p)
{
	int ret;
	unsigned int requiredCD;
	unsigned char i, j;

    if (percentile < 0.0f || percentile > 1.0f) return (-1);
    
    ret = arImageProcLumaHistAndCDF(ipi, dataPtr);
    if (ret < 0) return (ret);
    
    requiredCD = (unsigned int)(ipi->imageX * ipi->imageY * percentile);
    i = 0;
    while (ipi->cdfBins[i] < requiredCD) i++; // cdfBins[i] >= requiredCD
    j = i;
    while (ipi->cdfBins[j] == requiredCD) j++; // cdfBins[j] > requiredCD    
    *value_p = (unsigned char)((i + j) / 2);
    return (0);
}

int arImageProcLumaHistAndCDFAndMedian(ARImageProcInfo *ipi, const ARUint8 *__restrict dataPtr, unsigned char *value_p)
{
    return (arImageProcLumaHistAndCDFAndPercentile(ipi, dataPtr, 0.5f, value_p));
}

// Implementation of Otsu's Method of binarization threshold determination.
// See http://en.wikipedia.org/wiki/Otsu's_method fore more information.
int arImageProcLumaHistAndOtsu(ARImageProcInfo *ipi, const ARUint8 *__restrict dataPtr, unsigned char *value_p)
{
    int ret;
    unsigned char i;
    
    ret = arImageProcLumaHist(ipi, dataPtr);
    if (ret < 0) return (ret);
    
    float sum = 0.0f;
    i = 1;
    do {
        sum += ipi->histBins[i] * i;
        i++;
    } while (i != 0);
    
    float count = (float)(ipi->imageX * ipi->imageY);
    float sumB = 0.0f;
    float wB = 0.0f;
    float wF = 0.0f;
    float varMax = 0.0f;
    unsigned char threshold = 0;
    i = 0;
    do {
        wB += ipi->histBins[i];          // Weight background.
        if (wB != 0.0f) {
            wF = count - wB;                 // Weight foreground.
            if (wF == 0.0f) break;
            
            sumB += (float)(i * ipi->histBins[i]);
            
            float mB = sumB / wB;            // Mean background.
            float mF = (sum - sumB) / wF;    // Mean foreground.
            
            // Calculate between-class variance.
            float varBetween = wB * wF * (mB - mF) * (mB - mF);
            
            // Check if new maximum found.
            if (varBetween > varMax) {
                varMax = varBetween;
                threshold = i;
            }
        }
        i++;
    } while (i != 0);
    
    *value_p = threshold;
    return (0);
}

#if !AR_DISABLE_THRESH_MODE_AUTO_ADAPTIVE
int arImageProcLumaHistAndBoxFilterWithBias(ARImageProcInfo *ipi, const ARUint8 *__restrict dataPtr, const int boxSize, const int bias)
{
    int ret, i;
#if !AR_IMAGEPROC_USE_VIMAGE
    int j, kernelSizeHalf;
#endif
    
    ret = arImageProcLumaHist(ipi, dataPtr);
    if (ret < 0) return (ret);
    
    if (!ipi->image2) {
        ipi->image2 = (unsigned char *)malloc(ipi->imageX * ipi->imageY * sizeof(unsigned char));
        if (!ipi->image2) return (-1);
    }
#if AR_IMAGEPROC_USE_VIMAGE
    vImage_Error err;
    vImage_Buffer src = {ipi->image, ipi->imageY, ipi->imageX, ipi->imageX};
    vImage_Buffer dest = {ipi->image2, ipi->imageY, ipi->imageX, ipi->imageX};
    if (!ipi->tempBuffer) {
        // Request size of buffer, and allocate.
        if ((err = vImageBoxConvolve_Planar8(&src, &dest, NULL, 0, 0, boxSize, boxSize, '\0', kvImageTruncateKernel | kvImageGetTempBufferSize)) < 0) return (-1);
        if (!(ipi->tempBuffer = malloc(err))) return (-1);
    }
    err = vImageBoxConvolve_Planar8(&src, &dest, ipi->tempBuffer, 0, 0, boxSize, boxSize, '\0', kvImageTruncateKernel);
    if (err != kvImageNoError) {
        ARLOGe("Error %ld in vImageBoxConvolve_Planar8().\n", err);
        return (-1);
    }
#else
    kernelSizeHalf = boxSize >> 1;
    for (j = 0; j < ipi->imageY; j++) {
        for (i = 0; i < ipi->imageX; i++) {
            int val, count, kernel_i, kernel_j, ii, jj;
            val = count = 0;
            for (kernel_j = -kernelSizeHalf; kernel_j <= kernelSizeHalf; kernel_j++) {
                jj = j + kernel_j;
                if (jj < 0 || jj >= ipi->imageY) continue;
                for (kernel_i = -kernelSizeHalf; kernel_i <= kernelSizeHalf; kernel_i++) {
                    ii = i + kernel_i;
                    if (ii < 0 || ii >= ipi->imageX) continue;
                    val += ipi->image[ii + jj*(ipi->imageX)];
                    count++;
                }
            }
            ipi->image2[i + j*(ipi->imageX)] = val / count;
        }
    }
#endif
    if (bias) for (i = 0; i < ipi->imageX*ipi->imageY; i++) ipi->image2[i] += bias;
    return (0);
}
#endif // !AR_DISABLE_THRESH_MODE_AUTO_ADAPTIVE

int arImageProcLumaHistAndCDFAndLevels(ARImageProcInfo *ipi, const ARUint8 *__restrict dataPtr)
{
	unsigned char l;
	unsigned int maxCD;

    int ret = arImageProcLumaHistAndCDF(ipi, dataPtr);
    if (ret < 0) return (ret);

    // Find min and max values.
    l = 0;
    while (ipi->cdfBins[l] == 0) l++;
    ipi->min = l;
    maxCD = ipi->imageX*ipi->imageY;
    while (ipi->cdfBins[l] < maxCD) l++;
    ipi->max = l;
    
    return (0);
}

//
// Methods from http://computer-vision-talks.com/2011/02/a-very-fast-bgra-to-grayscale-conversion-on-iphone/
//
#ifdef HAVE_ARM_NEON
#if 0
static void arImageProcBGRAtoL_ARM_neon(uint8_t * __restrict dest, uint8_t * __restrict src, int numPixels)
{
	int i;
	uint8x8_t rfac = vdup_n_u8 (77); // CCIR 601 recommended values. See http://www.poynton.com/notes/colour_and_gamma/ColorFAQ.html#RTFToC11.
	uint8x8_t gfac = vdup_n_u8 (150);
	uint8x8_t bfac = vdup_n_u8 (29);
	int n = numPixels / 8;
    
	// Convert per eight pixels.
	for (i = 0; i < n; i++) {
		uint16x8_t  temp;
		uint8x8x4_t rgb  = vld4_u8 (src);
		uint8x8_t result;
        
		temp = vmull_u8 (rgb.val[0],      bfac);
		temp = vmlal_u8 (temp,rgb.val[1], gfac);
		temp = vmlal_u8 (temp,rgb.val[2], rfac);
        
		result = vshrn_n_u16 (temp, 8);
		vst1_u8 (dest, result);
		src  += 8*4;
		dest += 8;
	}
}
#endif


static void arImageProcBGRAtoL_ARM_neon_asm(uint8_t * __restrict dest, uint8_t * __restrict src, int numPixels)
{
	__asm__ volatile("lsr          %2, %2, #3      \n" // Divide arg 2 (numPixels) by 8.
					 "# build the three constants: \n"
					 "mov         r4, #29          \n" // Blue channel multiplier.
					 "mov         r5, #150         \n" // Green channel multiplier.
					 "mov         r6, #77          \n" // Red channel multiplier.
					 "vdup.8      d4, r4           \n"
					 "vdup.8      d5, r5           \n"
					 "vdup.8      d6, r6           \n"
					 "0:						   \n"
					 "# load 8 pixels:             \n"
					 "vld4.8      {d0-d3}, [%1]!   \n" // B into d0, G into d1, R into d2, A into d3.
					 "# do the weight average:     \n"
					 "vmull.u8    q7, d0, d4       \n"
					 "vmlal.u8    q7, d1, d5       \n"
					 "vmlal.u8    q7, d2, d6       \n"
					 "# shift and store:           \n"
					 "vshrn.u16   d7, q7, #8       \n" // Divide q3 by 256 and store in the d7.
					 "vst1.8      {d7}, [%0]!      \n"
					 "subs        %2, %2, #1       \n" // Decrement iteration count.
					 "bne         0b            \n" // Repeat unil iteration count is not zero.
					 :
					 : "r"(dest), "r"(src), "r"(numPixels)
					 : "cc", "r4", "r5", "r6"
					 );
}

static void arImageProcRGBAtoL_ARM_neon_asm(uint8_t * __restrict dest, uint8_t * __restrict src, int numPixels)
{
	__asm__ volatile("lsr          %2, %2, #3      \n" // Divide arg 2 (numPixels) by 8.
					 "# build the three constants: \n"
					 "mov         r4, #77          \n" // Red channel multiplier.
					 "mov         r5, #150         \n" // Green channel multiplier.
					 "mov         r6, #29          \n" // Blue channel multiplier.
					 "vdup.8      d4, r4           \n"
					 "vdup.8      d5, r5           \n"
					 "vdup.8      d6, r6           \n"
					 "0:						   \n"
					 "# load 8 pixels:             \n"
					 "vld4.8      {d0-d3}, [%1]!   \n" // R into d0, G into d1, B into d2, A into d3.
					 "# do the weight average:     \n"
					 "vmull.u8    q7, d0, d4       \n"
					 "vmlal.u8    q7, d1, d5       \n"
					 "vmlal.u8    q7, d2, d6       \n"
					 "# shift and store:           \n"
					 "vshrn.u16   d7, q7, #8       \n" // Divide q3 by 256 and store in the d7.
					 "vst1.8      {d7}, [%0]!      \n"
					 "subs        %2, %2, #1       \n" // Decrement iteration count.
					 "bne         0b            \n" // Repeat unil iteration count is not zero.
					 :
					 : "r"(dest), "r"(src), "r"(numPixels)
					 : "cc", "r4", "r5", "r6"
					 );
}

static void arImageProcABGRtoL_ARM_neon_asm(uint8_t * __restrict dest, uint8_t * __restrict src, int numPixels)
{
	__asm__ volatile("lsr          %2, %2, #3      \n" // Divide arg 2 (numPixels) by 8.
					 "# build the three constants: \n"
					 "mov         r4, #29          \n" // Blue channel multiplier.
					 "mov         r5, #150         \n" // Green channel multiplier.
					 "mov         r6, #77          \n" // Red channel multiplier.
					 "vdup.8      d4, r4           \n"
					 "vdup.8      d5, r5           \n"
					 "vdup.8      d6, r6           \n"
					 "0:						   \n"
					 "# load 8 pixels:             \n"
					 "vld4.8      {d0-d3}, [%1]!   \n" // A into d0, B into d1, G into d2, R into d3.
					 "# do the weight average:     \n"
					 "vmull.u8    q7, d1, d4       \n"
					 "vmlal.u8    q7, d2, d5       \n"
					 "vmlal.u8    q7, d3, d6       \n"
					 "# shift and store:           \n"
					 "vshrn.u16   d7, q7, #8       \n" // Divide q3 by 256 and store in the d7.
					 "vst1.8      {d7}, [%0]!      \n"
					 "subs        %2, %2, #1       \n" // Decrement iteration count.
					 "bne         0b            \n" // Repeat unil iteration count is not zero.
					 :
					 : "r"(dest), "r"(src), "r"(numPixels)
					 : "cc", "r4", "r5", "r6"
					 );
}

static void arImageProcARGBtoL_ARM_neon_asm(uint8_t * __restrict dest, uint8_t * __restrict src, int numPixels)
{
	__asm__ volatile("lsr          %2, %2, #3      \n" // Divide arg 2 (numPixels) by 8.
					 "# build the three constants: \n"
					 "mov         r4, #77          \n" // Red channel multiplier.
					 "mov         r5, #150         \n" // Green channel multiplier.
					 "mov         r6, #29          \n" // Blue channel multiplier.
					 "vdup.8      d4, r4           \n"
					 "vdup.8      d5, r5           \n"
					 "vdup.8      d6, r6           \n"
					 "0:						   \n"
					 "# load 8 pixels:             \n"
					 "vld4.8      {d0-d3}, [%1]!   \n" // A into d0, R into d1, G into d2, B into d3.
					 "# do the weight average:     \n"
					 "vmull.u8    q7, d1, d4       \n"
					 "vmlal.u8    q7, d2, d5       \n"
					 "vmlal.u8    q7, d3, d6       \n"
					 "# shift and store:           \n"
					 "vshrn.u16   d7, q7, #8       \n" // Divide q3 by 256 and store in the d7.
					 "vst1.8      {d7}, [%0]!      \n"
					 "subs        %2, %2, #1       \n" // Decrement iteration count.
					 "bne         0b            \n" // Repeat unil iteration count is not zero.
					 :
					 : "r"(dest), "r"(src), "r"(numPixels)
					 : "cc", "r4", "r5", "r6"
					 );
}
#endif // HAVE_ARM_NEON

