/*
 *  ar.h
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
/*******************************************************
 *
 * Author: Hirokazu Kato
 *
 *         kato@sys.im.hiroshima-cu.ac.jp
 *
 * Revision: 4.01
 * Date: 2003/11/07
 *
 *******************************************************/

/*!
	@header ar
	@abstract ARToolKit core routines.
	@discussion
        This header declares essential types and API for the entire
        ARToolKit SDK.

        For compile-time per-machine configuration, see &lt;AR/config.h&gt;.<br>
        For compile-time ARToolKit configuration, see &lt;AR/arConfig.h&gt;.
	@copyright 2015 Daqri, LLC.
 */

#ifndef AR_H
#define AR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#ifndef _WIN32 // errno is defined in stdlib.h on Windows.
#  ifdef EMSCRIPTEN // errno is not in sys/
#    include <errno.h>
#  else
#    include <sys/errno.h>
#  endif
#endif
#include <AR/config.h>
#include <AR/arConfig.h>
#ifdef __ANDROID__
#  include <jni.h>
#  include <android/log.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

//
// Logging.
//

#ifdef __ANDROID__
#  define ARLOG(...)  __android_log_print(ANDROID_LOG_INFO, "libar", __VA_ARGS__)
#else
#  define ARLOG(...)  printf(__VA_ARGS__)
#endif

/*!
    @var
    @abstract   Sets the severity level. Log messages below the set severity level are not logged.
	@discussion
        All calls to ARToolKit's logging facility include a "log level" parameter, which specifies
        the severity of the log message. (The severities are defined in %lt;ar/config.h&gt;.)
        Setting this global allows for filtering of log messages. All log messages lower than
        the set level will not be logged by arLog().
    @seealso arLog arLog
*/
extern int arLogLevel;

/*!
    @function
    @abstract   Write a string to the current logging facility.
	@discussion
        The default logging facility varies by platform, but on Unix-like platforms is typically
        the standard error file descriptor. However, logging may be redirected to some other
        facility by arLogSetLogger.

        Newlines are not automatically appended to log output.
    @param      logLevel The severity of the log message. Defined in %lt;ar/config.h&gt;.
        Log output is written to the logging facility provided the logLevel meets or
        exceeds the minimum level specified in global arLogLevel.
    @param      format Log format string, in the form of printf().
    @seealso arLogLevel arLogLevel
    @seealso arLogSetLogger arLogSetLogger
*/

void arLog(const int logLevel, const char *format, ...);

typedef void (AR_CALLBACK *AR_LOG_LOGGER_CALLBACK)(const char *logMessage);

/*!
    @function
    @abstract   Divert logging to a callback, or revert to default logging.
	@discussion
        The default logging facility varies by platform, but on Unix-like platforms is typically
        the standard error file descriptor. However, logging may be redirected to some other
        facility by this function.
    @param      callback The function which will be called with the log output, or NULL to
        cancel redirection.
    @param      callBackOnlyIfOnSameThread If non-zero, then the callback will only be called
        if the call to arLog is made on the same thread as the thread which called this function,
        and if the arLog call is made on a different thread, log output will be buffered until
        the next call to arLog on the original thread.

        The purpose of this is to prevent logging from secondary threads in cases where the
        callback model of the target platform precludes this.
    @seealso arLog arLog
*/
void arLogSetLogger(AR_LOG_LOGGER_CALLBACK callback, int callBackOnlyIfOnSameThread);

// FN(x) allows for a function-like macro x which expands to a single statement.
#define FN(x) do {x} while (0)

#ifdef DEBUG
#  define ARLOGd(...) arLog(AR_LOG_LEVEL_DEBUG, __VA_ARGS__)
#else
#  define ARLOGd(...)
#endif
#define ARLOGi(...) arLog(AR_LOG_LEVEL_INFO, __VA_ARGS__)
#define ARLOGw(...) arLog(AR_LOG_LEVEL_WARN, __VA_ARGS__)
#define ARLOGe(...) arLog(AR_LOG_LEVEL_ERROR, __VA_ARGS__)
#define ARLOGperror(s) arLog(AR_LOG_LEVEL_ERROR, (s ? "%s: %s\n" : "%s%s\n"), (s ? s : ""), strerror(errno))


#define arMalloc(V,T,S)  \
{ if( ((V) = (T *)malloc( sizeof(T) * (S) )) == NULL ) \
{ARLOGe("Out of memory!!\n"); exit(1);} }

#define arMallocClear(V,T,S)  \
{ if( ((V) = (T *)calloc( (S), sizeof(T) )) == NULL ) \
{ARLOGe("Out of memory!!\n"); exit(1);} }

typedef char              ARInt8;
typedef short             ARInt16;
typedef int               ARInt32;
typedef unsigned char     ARUint8;
typedef unsigned short    ARUint16;
typedef unsigned int      ARUint32;
typedef float             ARfloat;
#ifdef ARDOUBLE_IS_FLOAT
typedef float             ARdouble;
#else
typedef double            ARdouble;
#endif

#ifndef TRUE
#  define TRUE 1
#endif
#ifndef FALSE
#  define FALSE 0
#endif

#ifdef __cplusplus
}
#endif

#include <AR/matrix.h>
#include <AR/icp.h>

#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------------------------*/

typedef enum {
    AR_LABELING_THRESH_MODE_MANUAL = 0,
    AR_LABELING_THRESH_MODE_AUTO_MEDIAN,
    AR_LABELING_THRESH_MODE_AUTO_OTSU,
    AR_LABELING_THRESH_MODE_AUTO_ADAPTIVE,
    AR_LABELING_THRESH_MODE_AUTO_BRACKETING
} AR_LABELING_THRESH_MODE;

/*!
    @typedef ARMarkerInfo2
    @abstract   (description)
    @discussion (description)
    @field      area (description)
	@field      pos (description)
	@field      coord_num (description)
	@field      x_coord (description)
	@field      y_coord (description)
	@field      vertex (description)
*/
typedef struct {
    int             area;
    ARdouble        pos[2];
    int             coord_num;
    int             x_coord[AR_CHAIN_MAX];
    int             y_coord[AR_CHAIN_MAX];
    int             vertex[5];
} ARMarkerInfo2;

/*!
    @typedef
    @abstract Result codes returned by arDetectMarker to report state of individual detected trapezoidal regions.
    @discussion
        When detecting markers, all trapezoidal regions in the incoming image are considered for
        marker matching. Various heuristics are used to reject regions judged to be non-markers.
        The code will, as far as possible, report rejection by placing one of these constants
        into the ARMarkerInfo.cutoffPhase field of regions rejected during the arDetectMarker() call.
        Note that the ARMarkerInfo.id of such rejected regions will be -1.
    @constant AR_MARKER_INFO_CUTOFF_PHASE_NONE                Marker OK.
    @constant AR_MARKER_INFO_CUTOFF_PHASE_PATTERN_EXTRACTION  Failure during pattern extraction.
    @constant AR_MARKER_INFO_CUTOFF_PHASE_MATCH_GENERIC       Generic error during matching phase.
    @constant AR_MARKER_INFO_CUTOFF_PHASE_MATCH_CONTRAST      Insufficient contrast during matching.
    @constant AR_MARKER_INFO_CUTOFF_PHASE_MATCH_BARCODE_NOT_FOUND Barcode matching could not find correct barcode locator pattern.
    @constant AR_MARKER_INFO_CUTOFF_PHASE_MATCH_BARCODE_EDC_FAIL  Barcode matching error detection/correction found unrecoverable error.
    @constant AR_MARKER_INFO_CUTOFF_PHASE_MATCH_CONFIDENCE    Matching confidence cutoff value not reached.
    @constant AR_MARKER_INFO_CUTOFF_PHASE_POSE_ERROR          Maximum allowable pose error exceeded.
    @constant AR_MARKER_INFO_CUTOFF_PHASE_POSE_ERROR_MULTI    Multi-marker pose error value exceeded.
    @constant AR_MARKER_INFO_CUTOFF_PHASE_HEURISTIC_TROUBLESOME_MATRIX_CODES Heuristic-based rejection of troublesome matrix code which is often generated in error.
 */
typedef enum {
    AR_MARKER_INFO_CUTOFF_PHASE_NONE,
    AR_MARKER_INFO_CUTOFF_PHASE_PATTERN_EXTRACTION,
    AR_MARKER_INFO_CUTOFF_PHASE_MATCH_GENERIC,
    AR_MARKER_INFO_CUTOFF_PHASE_MATCH_CONTRAST,
    AR_MARKER_INFO_CUTOFF_PHASE_MATCH_BARCODE_NOT_FOUND,
    AR_MARKER_INFO_CUTOFF_PHASE_MATCH_BARCODE_EDC_FAIL,
    AR_MARKER_INFO_CUTOFF_PHASE_MATCH_CONFIDENCE,
    AR_MARKER_INFO_CUTOFF_PHASE_POSE_ERROR,
    AR_MARKER_INFO_CUTOFF_PHASE_POSE_ERROR_MULTI,
    AR_MARKER_INFO_CUTOFF_PHASE_HEURISTIC_TROUBLESOME_MATRIX_CODES
} AR_MARKER_INFO_CUTOFF_PHASE;

#define AR_MARKER_INFO_CUTOFF_PHASE_DESCRIPTION_COUNT 10
extern const char *arMarkerInfoCutoffPhaseDescriptions[AR_MARKER_INFO_CUTOFF_PHASE_DESCRIPTION_COUNT];

/*!
    @typedef ARMarkerInfo
    @abstract   Describes a detected trapezoidal area (a candidate for a marker match).
    @discussion
        After marker detection, a number of trapezoidal areas in the camera image will have been identified. An
        ARMarkerInfo struct is returned for each area so matched. Trapezoidal areas which have been matched
        with marker images (in pattern mode) or barcodes (in matrix mode) will have valid values assigned to the
        appropriate id field.
    @field      area Area in pixels of the largest connected region, comprising the marker border and regions connected to it. Note that this is
        not the same as the actual onscreen area inside the marker border.
	@field      id If pattern detection mode is either pattern mode OR matrix but not both, will be marker ID (>= 0) if marker is valid, or -1 if invalid.
	@field      idPatt If pattern detection mode includes a pattern mode, will be marker ID (>= 0) if marker is valid, or -1 if invalid.
    @field      idMatrix If pattern detection mode includes a matrix mode, will be marker ID (>= 0) if marker is valid, or -1 if invalid.
	@field      dir If pattern detection mode is either pattern mode OR matrix but not both, and id != -1, will be marker direction (range 0 to 3, inclusive).
	@field      dirPatt If pattern detection mode includes a pattern mode, and id != -1, will be marker direction (range 0 to 3, inclusive).
	@field      dirMatrix If pattern detection mode includes a matrix mode, and id != -1, will be marker direction (range 0 to 3, inclusive).
	@field      cf If pattern detection mode is either pattern mode OR matrix but not both, will be marker matching confidence (range 0.0 to 1.0 inclusive) if marker is valid, or -1.0 if marker is invalid.
	@field      cfPatt If pattern detection mode includes a pattern mode, will be marker matching confidence (range 0.0 to 1.0 inclusive) if marker is valid, or -1.0 if marker is invalid.
	@field      cfMatrix If pattern detection mode includes a matrix mode, will be marker matching confidence (range 0.0 to 1.0 inclusive) if marker is valid, or -1.0 if marker is invalid.
	@field      pos 2D position (in camera image coordinates, origin at top-left) of the centre of the marker.
	@field      line Line equations for the 4 sides of the marker.
	@field      vertex 2D positions (in camera image coordinates, origin at top-left) of the corners of the marker. vertex[(4 - dir)%4][] is the top-left corner of the marker. Other vertices proceed clockwise from this. These are idealised coordinates (i.e. the onscreen position aligns correctly with the undistorted camera image.)
	@field      markerInfo2Ptr (description)
    @field      cutoffPhase If a trapezoidal region is detected, but is eliminated from the candidates for tracking,
        this field is filled out with the tracking phase at which the marker was cut off. An English-language
        description of the phase can be obtained by indexing into the C-string array arMarkerInfoCutoffPhaseDescriptions[].
    @field      globalID If arPattDetectionMode is a matrix mode, matrixCodeType is AR_MATRIX_CODE_GLOBAL_ID, and idMatrix >= 0, will contain the globalID.
*/
typedef struct {
    int             area;
    int             id;
    int             idPatt;
    int             idMatrix;
    int             dir;
    int             dirPatt;
    int             dirMatrix;
    ARdouble        cf;
    ARdouble        cfPatt;
    ARdouble        cfMatrix;
    ARdouble        pos[2];
    ARdouble        line[4][3];
    ARdouble        vertex[4][2];
    ARMarkerInfo2  *markerInfo2Ptr;
    AR_MARKER_INFO_CUTOFF_PHASE cutoffPhase;
    int             errorCorrected;
    uint64_t        globalID;
} ARMarkerInfo;

/*!
	@typedef ARTrackingHistory
	@abstract   (description)
	@discussion (description)
	@field      marker (description)
	@field      count (description)
 */
typedef struct {
    ARMarkerInfo    marker;
    int             count;
} ARTrackingHistory;

/*!
	@typedef ARLabelInfo
	@abstract   (description)
	@discussion (description)
	@field      labelImage (description)
	@field      bwImage (description)
	@field      label_num (description)
	@field      area (description)
	@field      clip (description)
	@field      pos (description)
	@field      work (description)
	@field      work2 (description)
 */
typedef struct {
    AR_LABELING_LABEL_TYPE *labelImage;
#if !AR_DISABLE_LABELING_DEBUG_MODE
    ARUint8        *bwImage;
#endif
    int             label_num;
    int             area[AR_LABELING_WORK_SIZE];
    int             clip[AR_LABELING_WORK_SIZE][4];
    ARdouble        pos[AR_LABELING_WORK_SIZE][2];
    int             work[AR_LABELING_WORK_SIZE];
    int             work2[AR_LABELING_WORK_SIZE*7]; // area, pos[2], clip[4].
} ARLabelInfo;

/* --------------------------------------------------*/

/*!
    @typedef ARPattHandle
    @abstract   A structure which holds descriptions of trained patterns for template matching.
    @discussion Template (picture)-based pattern matching requires details of the pattern
        to be supplied to the matching functions. This structure holds such details. It is
        generally setup by loading pattern files from disk.
    @field      patt_num Number of valid patterns in the structure.
	@field      pattf Flag: 0 = no pattern loaded at this position. 1 = pattern loaded and activated. 2 = pattern loaded but deactivated.
	@field      patt Array of 4 different orientations of each pattern's colour values, in 1-byte per component BGR order.
	@field      pattpow Root-mean-square of the pattern intensities.
	@field      pattBW Array of 4 different orientations of each pattern's 1-byte luminosity values.
	@field      pattpowBW  Root-mean-square of the pattern intensities.
*/
typedef struct {
    int             patt_num;
    int             patt_num_max;
    int            *pattf;
    int           **patt;
    ARdouble       *pattpow;
    int           **pattBW;
    ARdouble       *pattpowBW;
    //ARdouble        pattRatio;
    int             pattSize;
} ARPattHandle;

/*!
    @typedef ARPattRectInfo
    @abstract Defines a pattern rectangle as a sub-portion of a marker image.
    @discussion A complete marker image has coordinates {0.0f, 0.0f, 1.0f, 1.0f}.
        A standard ARToolKit marker with a pattern ratio of 0.5 has coordinates
        {0.25f, 0.25f, 0.75f, 0.75f}.
    @field      topLeftX Horizontal coordinate of the top left corner of the pattern space, in range 0.0f-1.0f.
    @field      topLeftY Vertical coordinate of the top left corner of the pattern space, in range 0.0f-1.0f.
    @field      bottomRightX Horizontal coordinate of the bottom right corner of the pattern space, in range 0.0f-1.0f.
    @field      bottomRightY Vertical coordinate of the bottom right corner of the pattern space, in range 0.0f-1.0f.
 */
typedef struct {
    float   topLeftX;
    float   topLeftY;
    float   bottomRightX;
    float   bottomRightY;
} ARPattRectInfo;

/* --------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#include <AR/param.h>
#include <AR/arImageProc.h>

#ifdef __cplusplus
extern "C" {
#endif

#define AR_MATRIX_CODE_TYPE_SIZE_MASK 0x000000ff
#define AR_MATRIX_CODE_TYPE_ECC_NONE 0x00000000
#define AR_MATRIX_CODE_TYPE_ECC_PARITY 0x00000100 // Single-bit parity.
#define AR_MATRIX_CODE_TYPE_ECC_HAMMING 0x00000200 // Hamming code with Hamming distance of 3.
#define AR_MATRIX_CODE_TYPE_ECC_BCH___3 0x00000300 // BCH code with Hamming distance of 3.
#define AR_MATRIX_CODE_TYPE_ECC_BCH___5 0x00000400 // BCH code with Hamming distance of 5.
#define AR_MATRIX_CODE_TYPE_ECC_BCH___7 0x00000500 // BCH code with Hamming distance of 7.
#define AR_MATRIX_CODE_TYPE_ECC_BCH___9 0x00000600 // BCH code with Hamming distance of 9.
#define AR_MATRIX_CODE_TYPE_ECC_BCH___11 0x00000700 // BCH code with Hamming distance of 11.
#define AR_MATRIX_CODE_TYPE_ECC_BCH___19 0x00000b00 // BCH code with Hamming distance of 19.

typedef enum {
    AR_MATRIX_CODE_3x3 = 0x03,                                                  // Matrix code in range 0-63.
    AR_MATRIX_CODE_3x3_PARITY65 = 0x03 | AR_MATRIX_CODE_TYPE_ECC_PARITY,        // Matrix code in range 0-31.
    AR_MATRIX_CODE_3x3_HAMMING63 = 0x03 | AR_MATRIX_CODE_TYPE_ECC_HAMMING,      // Matrix code in range 0-7.
    AR_MATRIX_CODE_4x4 = 0x04,                                                  // Matrix code in range 0-8191.
    AR_MATRIX_CODE_4x4_BCH_13_9_3 = 0x04 | AR_MATRIX_CODE_TYPE_ECC_BCH___3,     // Matrix code in range 0-511.
    AR_MATRIX_CODE_4x4_BCH_13_5_5 = 0x04 | AR_MATRIX_CODE_TYPE_ECC_BCH___5,     // Matrix code in range 0-31.
    AR_MATRIX_CODE_5x5_BCH_22_12_5 = 0x05 | AR_MATRIX_CODE_TYPE_ECC_BCH___5,    // Matrix code in range 0-4095.
    AR_MATRIX_CODE_5x5_BCH_22_7_7 = 0x05 | AR_MATRIX_CODE_TYPE_ECC_BCH___7,     // Matrix code in range 0-127.
    AR_MATRIX_CODE_5x5 = 0x05,                                                  // Matrix code in range 0-4194303.
    AR_MATRIX_CODE_6x6 = 0x06,                                                  // Matrix code in range 0-8589934591.
    AR_MATRIX_CODE_GLOBAL_ID = 0x0e | AR_MATRIX_CODE_TYPE_ECC_BCH___19
} AR_MATRIX_CODE_TYPE;

/*!
    @typedef ARHandle
    @abstract   (description)
    @discussion (description)
	@field		arDebug (description)
	@field		arPixelFormat (description)
	@field		arPixelSize (description)
	@field		arLabelingMode (description)
	@field		arLabelingThresh (description)
	@field		arImageProcMode
		To query this value, call arGetImageProcMode(). To set this value, call arSetImageProcMode().
	@field		arPatternDetectionMode (description)
	@field		arMarkerExtractionMode (description)
	@field		arParamLT (description)
	@field		marker_num (description)
	@field		markerInfo (description)
	@field		marker2_num (description)
	@field		markerInfo2 (description)
	@field		history_num (description)
	@field		history (description)
	@field		labelInfo (description)
	@field		pattHandle (description)
    @field      pattRatio A value between 0.0 and 1.0, representing the proportion of the marker width which constitutes the pattern. In earlier versions, this value was fixed at 0.5.
    @field      matrixCodeType When matrix code pattern detection mode is active, indicates the type of matrix code to detect.
 */
typedef struct {
    int                arDebug;
    AR_PIXEL_FORMAT    arPixelFormat;
    int                arPixelSize;
    int                arLabelingMode;
    int                arLabelingThresh;
    int                arImageProcMode;
    int                arPatternDetectionMode;
    int                arMarkerExtractionMode;
    ARParamLT         *arParamLT;
    int                xsize;
    int                ysize;
    int                marker_num;
    ARMarkerInfo       markerInfo[AR_SQUARE_MAX];
    int                marker2_num;
    ARMarkerInfo2      markerInfo2[AR_SQUARE_MAX];
    int                history_num;
    ARTrackingHistory  history[AR_SQUARE_MAX];
    ARLabelInfo        labelInfo;
    ARPattHandle      *pattHandle;
    AR_LABELING_THRESH_MODE arLabelingThreshMode;
    int                arLabelingThreshAutoInterval;
    int                arLabelingThreshAutoIntervalTTL;
    int                arLabelingThreshAutoBracketOver;
    int                arLabelingThreshAutoBracketUnder;
    ARImageProcInfo   *arImageProcInfo;
    ARdouble           pattRatio;
    AR_MATRIX_CODE_TYPE matrixCodeType;
} ARHandle;


/* --------------------------------------------------*/

/*!
    @typedef
    @abstract   (description)
    @discussion (description)
    @field      icpHandle (description)
*/
typedef struct {
    ICPHandleT          *icpHandle;
} AR3DHandle;

#define   AR_TRANS_MAT_IDENTITY            ICP_TRANS_MAT_IDENTITY

/*!
    @typedef
    @abstract   (description)
    @discussion (description)
    @field      icpStereoHandle (description)
*/
typedef struct {
    ICPStereoHandleT    *icpStereoHandle;
} AR3DStereoHandle;


/***********************************/
/*                                 */
/*    For square detection         */
/*                                 */
/***********************************/

/*!
	@functiongroup "Square detection".
 */
/*!
    @function
    @abstract   Create a handle to hold settings for an ARToolKit tracker instance.
    @discussion
        ARHandle is the primary structure holding the settings for a single ARToolKit
        square marker tracking instance. Settings include expected video stream image
        size and pixel format, tracking modes, loaded markers and more.

        Expected video stream image size is taken directly from the supplied ARParamLT
        structure's xsize and ysize fields. Video stream image pixel format defaults to
        AR_DEFAULT_PIXEL_FORMAT, which is platform and video-module dependent. Usually
        a call to arSetPixelFormat() is advisable to set the correct format.

        After creation of the ARHandle, tracking settings should be set via appropriate
        calls to other arSet*() functions.

        The ARHandle should be disposed of via a call to arDeleteHandle when tracking
        with this instance is complete.
    @param      paramLT The created handle will hold a pointer to the calibrated
		camera parameters specified by this parameter. This parameter uses the new
        lookup-table based form of the camera parameters introduced in ARToolKit v5.
        An ARParamLT structure may be created from an ARParam structure via the
        call:
        <code>ARParamLT *paramLT = arParamLTCreate(&param, AR_PARAM_LT_DEFAULT_OFFSET);</code>
        Note that the pointer is only copied, and so the ARParamLT structure must remain
        valid until the ARHandle is disposed of by calling arDeleteHandle.
    @result     An ARHandle which should be passed to other functions which
		deal with the operations of the ARToolKit tracker.
    @seealso arSetPixelFormat arSetPixelFormat
    @seealso arDeleteHandle arDeleteHandle
*/
ARHandle      *arCreateHandle( ARParamLT *paramLT );

/*!
    @function
    @abstract   Delete a handle which holds settings for an ARToolKit tracker instance.
	@discussion The calibrated camera parameters pointed to by the handle are
		NOT deleted by this operation.
    @param      handle The handle to delete, as created by arCreateHandle();
    @result     0 if no error occured.
    @seealso arCreateHandle arCreateHandle
*/
int            arDeleteHandle( ARHandle *handle );

/*!
    @function
    @abstract   Enable or disable ARToolKit's debug mode.
    @discussion In debug mode, ARToolKit offers additional error reporting. Use
        this function to enable or disable debug mode at runtime.

        Additionally, in debug mode, ARToolKit creates a mono (8-bit grayscale)
        image of the thresholded video input, and makes this available
        through the field ARHandle->labelInfo.bwImage.
    @param      handle An ARHandle referring to the current AR tracker
		to be queried for its debug mode.
	@param      mode
		Options for this field are:
		AR_DEBUG_DISABLE
		AR_DEBUG_ENABLE
		The default mode is AR_DEBUG_DISABLE.
    @result     0 if no error occured.
*/
int            arSetDebugMode( ARHandle *handle, int mode );

/*!
    @function
    @abstract   Find out whether ARToolKit's debug mode is enabled.
    @discussion See arSetDebugMode() for more info.
    @param      handle An ARHandle referring to the current AR tracker
		to be queried for its mode.
	@param		mode Pointer into which will be placed the
		value representing the mode.
    @result     0 if no error occured.
*/
int            arGetDebugMode( ARHandle *handle, int *mode );

/*!
	@function
	@abstract   Select between detection of black markers and white markers.
	@discussion
        ARToolKit's labelling algorithm can work with both black-bordered
        markers on a white background (AR_LABELING_BLACK_REGION) or
        white-bordered markers on a black background (AR_LABELING_WHITE_REGION).
        This function allows you to specify the type of markers to look for.
        Note that this does not affect the pattern-detection algorith
        which works on the interior of the marker.
	@param      handle An ARHandle referring to the current AR tracker
		to have its labeling mode set.
	@param      mode
		Options for this field are:
		AR_LABELING_WHITE_REGION
		AR_LABELING_BLACK_REGION
		The default mode is AR_LABELING_BLACK_REGION.
    @result     0 if no error occured.
 */
int            arSetLabelingMode( ARHandle *handle, int mode );

/*!
    @function
    @abstract   Enquire whether detection is looking for black markers or white markers.
    @discussion See discussion for arSetLabelingMode.
    @param      handle An ARHandle referring to the current AR tracker
		to be queried for its labeling mode.
	@param		mode Pointer into which will be placed the
		value representing the mode.
    @result     0 if no error occured.
*/
int            arGetLabelingMode( ARHandle *handle, int *mode );

/*!
    @function
    @abstract   Set the labeling threshhold.
    @discussion
        This function forces sets the threshold value.
        The default value is AR_DEFAULT_LABELING_THRESH which is 100,
        unless edited in arConfig.h.

        The current threshold mode is not affected by this call.
        Typically, this function is used when labeling threshold mode
        is AR_LABELING_THRESH_MODE_MANUAL.

        The threshold value is not relevant if threshold mode is
        AR_LABELING_THRESH_MODE_AUTO_ADAPTIVE.

        Background: The labeling threshold is the value which
		the AR library uses to differentiate between black and white
		portions of an ARToolKit marker. Since the actual brightness,
		contrast, and gamma of incoming images can vary signficantly
		between different cameras and lighting conditions, this
		value typically needs to be adjusted dynamically to a
		suitable midpoint between the observed values for black
		and white portions of the markers in the image.
	@param      handle An ARHandle referring to the current AR tracker
		to have its labeling threshold value set.
	@param      thresh An integer in the range [0,255] (inclusive).
    @result     0 if no error occured.
*/
int            arSetLabelingThresh( ARHandle *handle, int thresh );

/*!
    @function
    @abstract   Get the current labeling threshold.
    @discussion
        This function queries the current labeling threshold. For,
        AR_LABELING_THRESH_MODE_AUTO_MEDIAN, AR_LABELING_THRESH_MODE_AUTO_OTSU,
        and AR_LABELING_THRESH_MODE_AUTO_BRACKETING
        the threshold value is only valid until the next auto-update.

        The current threshold mode is not affected by this call.

        The threshold value is not relevant if threshold mode is
        AR_LABELING_THRESH_MODE_AUTO_ADAPTIVE.
    @param      handle An ARHandle referring to the current AR tracker
		to be queried for its labeling threshold value.
	@param		thresh Pointer into which will be placed the
		value of the labeling threshhold.
		An integer in the range [0,255] (inclusive)
    @result     0 if no error occured.
    @seealso arSetLabelingThresh arSetLabelingThresh
*/
int            arGetLabelingThresh( ARHandle *handle, int *thresh );

/*!
    @function
    @abstract   Set the labeling threshold mode (auto/manual).
    @discussion
    @param      handle An ARHandle referring to the current AR tracker
        to be queried for its labeling threshold mode.
    @param		mode An integer specifying the mode. One of:
        AR_LABELING_THRESH_MODE_MANUAL,
        AR_LABELING_THRESH_MODE_AUTO_MEDIAN,
        AR_LABELING_THRESH_MODE_AUTO_OTSU,
        AR_LABELING_THRESH_MODE_AUTO_ADAPTIVE,
        AR_LABELING_THRESH_MODE_AUTO_BRACKETING
    @result     0 if no error occured.
    @seealso arSetLabelingThresh arSetLabelingThresh
 */
int arSetLabelingThreshMode(ARHandle *handle, const AR_LABELING_THRESH_MODE mode);

/*!
    @function
    @abstract   Get the labeling threshold mode (auto/manual).
    @discussion
    @param      handle An ARHandle referring to the current AR tracker
        to be queried for its labeling threshold value.
    @param		mode_p Pointer into which will be placed the
        value of the labeling threshold mode, one of:
        AR_LABELING_THRESH_MODE_MANUAL,
        AR_LABELING_THRESH_MODE_AUTO_MEDIAN,
        AR_LABELING_THRESH_MODE_AUTO_OTSU,
        AR_LABELING_THRESH_MODE_AUTO_ADAPTIVE,
        AR_LABELING_THRESH_MODE_AUTO_BRACKETING
    @result     0 if no error occured.
    @seealso arSetLabelingThresh arSetLabelingThresh
 */
int arGetLabelingThreshMode(const ARHandle *handle, AR_LABELING_THRESH_MODE *mode_p);

/*!
    @function
    @abstract   Set the number of frames between auto-threshold calculations.
    @discussion
        This is the number of frames BETWEEN calculations, meaning that the
        calculation occurs every (interval + 1) frames.
    @param      handle An ARHandle referring to the current AR tracker
        for which the labeling threshold auto interval will be set.
    @param		interval The interval, specifying the number of frames between
        automatic updates to the threshold.
        An integer in the range [0,INT_MAX] (inclusive). Default
        value is AR_LABELING_THRESH_AUTO_INTERVAL_DEFAULT.
    @result     0 if no error occured.
    @seealso arGetLabelingThreshModeAutoInterval arGetLabelingThreshModeAutoInterval
 */
int arSetLabelingThreshModeAutoInterval(ARHandle *handle, const int interval);

/*!
    @function
    @abstract   Get the number of frames between auto-threshold calculations.
    @discussion
        This is the number of frames BETWEEN calculations, meaning that the
        calculation occurs every (interval + 1) frames.
    @param      handle An ARHandle referring to the current AR tracker
        to be queried for its labeling threshold auto interval value.
    @param		interval_p Pointer into which will be placed the
        value of the labeling threshhold auto interval.
        An integer in the range [0,INT_MAX] (inclusive)
    @result     0 if no error occured.
    @seealso arSetLabelingThreshModeAutoInterval arSetLabelingThreshModeAutoInterval
 */
int arGetLabelingThreshModeAutoInterval(const ARHandle *handle, int *interval_p);

/*!
    @function
    @abstract   Set the image processing mode.
    @discussion
        When ARthe image processing mode is AR_IMAGE_PROC_FRAME_IMAGE,
        ARToolKit processes all pixels in each incoming image
        to locate markers. When the mode is AR_IMAGE_PROC_FIELD_IMAGE,
        ARToolKit processes pixels in only every second pixel row and
        column. This is useful both for handling images from interlaced
        video sources (where alternate lines are assembled from alternate
        fields and thus have one field time-difference, resulting in a
        "comb" effect) such as Digital Video cameras.
        The effective reduction by 75% in the pixels processed also
        has utility in accelerating tracking by effectively reducing
        the image size to one quarter size, at the cost of pose accuraccy.
	@param      handle An ARHandle referring to the current AR tracker
		to have its mode set.
    @param      mode
		Options for this field are:
		AR_IMAGE_PROC_FRAME_IMAGE
		AR_IMAGE_PROC_FIELD_IMAGE
		The default mode is AR_IMAGE_PROC_FRAME_IMAGE.
    @result     0 if no error occured.
*/
int            arSetImageProcMode( ARHandle *handle, int mode );
/*!
    @function
    @abstract   Get the image processing mode.
    @discussion
		See arSetImageProcMode() for a complete description.
    @param      handle An ARHandle referring to the current AR tracker
		to be queried for its mode.
	@param		mode Pointer into which will be placed the
		value representing the current image processing mode.
    @result     0 if no error occured.
*/
int            arGetImageProcMode( ARHandle *handle, int *mode );

/*!
    @function
    @abstract   Set the pattern detection mode
    @discussion
        The pattern detection determines the method by which ARToolKit
        matches detected squares in the video image to marker templates
        and/or IDs. ARToolKit v4.x can match against pictorial "template" markers,
        whose pattern files are created with the mk_patt utility, in either colour
        or mono, and additionally can match against 2D-barcode-type "matrix"
        markers, which have an embedded marker ID. Two different two-pass modes
        are also available, in which a matrix-detection pass is made first,
        followed by a template-matching pass.
	@param      handle An ARHandle referring to the current AR tracker
		to have its mode set.
	@param      mode
		Options for this field are:
		AR_TEMPLATE_MATCHING_COLOR
		AR_TEMPLATE_MATCHING_MONO
		AR_MATRIX_CODE_DETECTION
		AR_TEMPLATE_MATCHING_COLOR_AND_MATRIX
		AR_TEMPLATE_MATCHING_MONO_AND_MATRIX
		The default mode is AR_TEMPLATE_MATCHING_COLOR.
    @result     0 if no error occured.
*/
int            arSetPatternDetectionMode( ARHandle *handle, int mode );

/*!
    @function
    @abstract   Get the pattern detection mode
    @discussion
        See arSetPatternDetectionMode() for a complete description.
    @param      handle An ARHandle referring to the current AR tracker
		to be queried for its mode.
	@param		mode Pointer into which will be placed the
		value representing the mode.
    @result     0 if no error occured.
*/
int            arGetPatternDetectionMode( ARHandle *handle, int *mode );

/*!
    @function
    @abstract   Set the size and ECC algorithm to be used for matrix code (2D barcode) marker detection.
    @discussion
        When matrix-code (2D barcode) marker detection is enabled (see arSetPatternDetectionMode)
        then the size of the barcode pattern and the type of error checking and correction (ECC)
        with which the markers were produced can be set via this function.

        This setting is global to a given ARHandle; It is not possible to have two different matrix
        code types in use at once.
    @param      handle An ARHandle referring to the current AR tracker to have its mode set.
    @param      type The type of matrix code (2D barcode) in use. Options include:
        AR_MATRIX_CODE_3x3
        AR_MATRIX_CODE_3x3_HAMMING63
        AR_MATRIX_CODE_3x3_PARITY65
        AR_MATRIX_CODE_4x4
        AR_MATRIX_CODE_4x4_BCH_13_9_3
        AR_MATRIX_CODE_4x4_BCH_13_5_5
        The default mode is AR_MATRIX_CODE_3x3.
    @seealso arSetPatternDetectionMode arSetPatternDetectionMode
    @seealso arGetMatrixCodeType arGetMatrixCodeType
    @result     0 if no error occured.
*/
int            arSetMatrixCodeType(ARHandle *handle, const AR_MATRIX_CODE_TYPE type);

/*!
    @function
    @abstract   Get the size and ECC algorithm being used for matrix code (2D barcode) marker detection.
    @discussion See the description for arSetMatrixCodeType().
    @param      handle An ARHandle referring to the current AR tracker to be queried for its mode.
    @param		type_p Pointer into which will be placed the value representing the mode.
    @seealso    arGetPatternDetectionMode arGetPatternDetectionMode
    @seealso    arSetMatrixCodeType arSetMatrixCodeType
    @result     0 if no error occured.
*/
int            arGetMatrixCodeType(ARHandle *handle, AR_MATRIX_CODE_TYPE *type_p);

/*!
    @function
    @abstract   Set the marker extraction mode
    @discussion (description)
	@param      handle An ARHandle referring to the current AR tracker to have its mode set.
	@param      mode
		Options for this field are:
		AR_USE_TRACKING_HISTORY
		AR_NOUSE_TRACKING_HISTORY
		AR_USE_TRACKING_HISTORY_V2
		The default mode is AR_USE_TRACKING_HISTORY_V2.
    @result     0 if no error occured.
 */
int            arSetMarkerExtractionMode( ARHandle *handle, int mode );

/*!
    @function
    @abstract   Get the marker extraction mode
    @discussion (description)
    @param      handle An ARHandle referring to the current AR tracker to be queried for its mode.
	@param		mode Pointer into which will be placed the value representing the mode.
    @result     0 if no error occured.
 */
int            arGetMarkerExtractionMode( ARHandle *handle, int *mode );

/*!
     @function
     @abstract   Set the border size.
     @discussion N.B. Deprecated in favour of arSetPattRatio(), but retained for
        backwards compatibility.
     @param      handle An ARHandle referring to the current AR tracker
        to have its border size set.
     @param      borderSize The border size. To set the default, pass (1.0 - 2*AR_PATT_RATIO).
        If compatibility with ARToolKit verions 1.0 through 4.4 is required, this value
        must be 0.25.
     @result     0 if no error occured.
 */
int            arSetBorderSize( ARHandle *handle, const ARdouble borderSize );

/*!
    @function
    @abstract   Get the border size.
    @discussion N.B. Deprecated in favour of arGetPattRatio(), but retained for
        backwards compatibility.
    @param      handle An ARHandle referring to the current AR tracker
        to be queried for its border size.
    @param		borderSize Pointer into which will be placed the
        value representing the border size. The default border size for newly-created
        ARHandle structures is AR_BORDER_SIZE_DEFAULT.
    @result     0 if no error occured.
 */
int            arGetBorderSize( ARHandle *handle, ARdouble *borderSize );

/*!
    @function
    @abstract   Set the width/height of the marker pattern space, as a proportion of marker width/height.
    @discussion N.B. Supercedes arSetBorderSize().
    @param      handle An ARHandle referring to the current AR tracker to be modified.
    @param		pattRatio The the width/height of the marker pattern space, as a proportion of marker
        width/height. To set the default, pass AR_PATT_RATIO.
        If compatibility with ARToolKit verions 1.0 through 4.4 is required, this value
        must be 0.5.
    @result     0 if no error occured.
 */
int            arSetPattRatio( ARHandle *handle, const ARdouble pattRatio );

/*!
    @function
    @abstract   Get the width/height of the marker pattern space, as a proportion of marker width/height.
    @discussion N.B. Supercedes arGetBorderSize().
    @param      handle An ARHandle referring to the current AR tracker to be queried.
    @param		pattRatio Pointer into which will be placed the
        value representing the width/height of the marker pattern space, as a proportion of marker
        width/height. The default border size for newly-created ARHandle structures is AR_PATT_RATIO.
    @result     0 if no error occured.
 */
int            arGetPattRatio( ARHandle *handle, ARdouble *pattRatio );

/*!
    @function
    @abstract   Set the expected pixel format for video frames being passed to arDetectMarker
    @discussion
        This function should be used at least once after creation of an ARHandle, to set the pixel
        format with which images will be passed to arDetectMarker(). If not called, the default value
        AR_DEFAULT_PIXEL_FORMAT will be used. Note that AR_DEFAULT_PIXEL_FORMAT varies depending on
        platform and video module. If the pixel format of incoming video images changes, this value
        must also be changed.
    @param      handle Handle to settings structure in which to set the pixel format.
    @param      pixFormat Value representing the format of pixels to be
		processed by the ARToolKit detection routines. See AR_PIXEL_FORMAT
		reference for more information.
    @result     0 if no error occured.
    @seealso arGetPixelFormat arGetPixelFormat
    @seealso arCreateHandle arCreateHandle
    @seealso arDetectMarker arDetectMarker
 */
int            arSetPixelFormat( ARHandle *handle, AR_PIXEL_FORMAT pixFormat );

/*!
    @function
    @abstract   Get the expected pixel format for video frames being passed to arDetectMarker
    @discussion
        See discussion for arSetPixelFormat().
    @param      handle Handle to AR settings structure from which to retrieve the pixel format.
	@param		pixFormat Pointer into which will be placed the
		value representing the format of pixels being
		processed by the ARToolKit detection routines. See AR_PIXEL_FORMAT
		reference for more information.
    @result     0 if no error occured.
    @seealso arSetPixelFormat arSetPixelFormat
    @seealso arCreateHandle arCreateHandle
    @seealso arDetectMarker arDetectMarker
 */
int            arGetPixelFormat( ARHandle *handle, AR_PIXEL_FORMAT *pixFormat );


/*!
    @function
    @abstract   Detect markers in a video frame.
    @discussion
		This is the core ARToolKit marker detection function. It calls through to a set of
		internal functions to perform the key marker detection steps of binarization and
		labelling, contour extraction, and template matching and/or matrix code extraction.

        Typically, the resulting set of detected markers is retrieved by calling arGetMarkerNum
        to get the number of markers detected and arGetMarker to get an array of ARMarkerInfo
        structures with information on each detected marker, followed by a step in which
        detected markers are possibly examined for some measure of goodness of match (e.g. by
        examining the match confidence value) and pose extraction.

    @param      arHandle Handle to initialised settings, including camera parameters,
        incoming video image size and pixel format, markers, detection modes and other information.
	@param		dataPtr Pointer to the first byte of a block of memory containing pixel
		data for an image which is to be processed for marker detection. The format of
		pixels in this image is specified by arSetPixelFormat(). The width and height of
		the image are specified by the xsize and ysize parameters of the camera parameters
		held in arHandle.
    @result     0 if the function proceeded without error, or a value less than 0 in case of error.
		A result of 0 does not however, imply any markers were detected.
    @seealso arCreateHandle arCreateHandle
    @seealso arGetMarkerNum arGetMarkerNum
    @seealso arGetMarker arGetMarker
 */
int            arDetectMarker( ARHandle *arHandle, ARUint8 *dataPtr );

/*!
    @function
    @abstract   Get the number of markers detected in a video frame.
    @discussion

    @result     The number of detected markers in the most recent image passed to arDetectMarker.
        Note that this is actually a count, not an index. A better name for this function would be
        arGetDetectedMarkerCount, but the current name lives on for historical reasons.
    @param      arHandle Handle upon which arDetectMarker has been called.
    @seealso arGetMarker arGetMarker
    @seealso ARMarkerInfo ARMarkerInfo
    @seealso arDetectMarker arDetectMarker
 */
int            arGetMarkerNum( ARHandle *arHandle );

/*!
    @function
    @abstract   Get information on the markers detected in a video frame.
    @discussion
    @result     An array (of length arGetMarkerNum(arHandle)) of ARMarkerInfo structs.
        A better name for this function would be arGetDetectedMarkerInfo, but the current name lives
        on for historical reasons.
    @param      arHandle Handle upon which arDetectMarker has been called.
    @seealso arGetMarkerNum arGetMarkerNum
    @seealso ARMarkerInfo ARMarkerInfo
    @seealso arDetectMarker arDetectMarker
 */
ARMarkerInfo  *arGetMarker( ARHandle *arHandle );

/* ------------------------------ */

int            arLabeling( ARUint8 *image, int xsize, int ysize, int pixelFormat,
                           int debugMode, int labelingMode, int labelingThresh, int imageProcMode,
                           ARLabelInfo *labelInfo, ARUint8 *image_thresh );
int            arDetectMarker2( int xsize, int ysize, ARLabelInfo *labelInfo, int imageProcMode,
                                int areaMax, int areaMin, ARdouble squareFitThresh,
                                ARMarkerInfo2 *markerInfo2, int *marker2_num );
/*!
    @function
    @abstract   Examine a set of detected squares for match with known markers.
    @discussion
        Performs the intermediate marker-detection stage of taking detected squares in a processed image, and
        matching the interior of these squares against known marker templates, or extracting matrix codes from
        the interior of the square.
    @param      image Image in which squares were detected.
    @param      xsize Horizontal dimension of image, in pixels.
    @param      ysize Vertical dimension of image, in pixels.
    @param      pixelFormat Format of pixels in image. See &lt;AR/config.h&gt; for values.
    @param      markerInfo2 Pointer to an array of ARMarkerInfo2 structures holding information on detected squares which are candidates for marker matching.
    @param      marker2_num Size of markerInfo2 array.
    @param      pattHandle Handle to loaded patterns for template matching against detected squares.
    @param      imageProcMode Indicates whether square detection was performed treating the image as a frame or a field.
    @param      pattDetectMode Whether to perform color/mono template matching, matrix code detection, or both.
    @param      arParamLTf Lookup table for the camera parameters for the optical source from which the image was acquired. See arParamLTCreate.
    @param      pattRatio A value between 0.0 and 1.0, representing the proportion of the marker width which constitutes the pattern. In earlier versions, this value was fixed at 0.5.
    @param      markerInfo Output: Pointer to an array of ARMarkerInfo structures holding information on successful matches.
    @param      marker_num Output: Size of markerInfo array.
    @param      matrixCodeType When matrix code pattern detection mode is active, indicates the type of matrix code to detect.
    @result     0 in case of no error, or -1 otherwise.
    @seealso    arParamLTCreate arParamLTCreate
 */
int            arGetMarkerInfo( ARUint8 *image, int xsize, int ysize, int pixelFormat,
                                ARMarkerInfo2 *markerInfo2, int marker2_num,
                                ARPattHandle *pattHandle, int imageProcMode, int pattDetectMode, ARParamLTf *arParamLTf, ARdouble pattRatio,
                                ARMarkerInfo *markerInfo, int *marker_num,
                                const AR_MATRIX_CODE_TYPE matrixCodeType );

int            arGetContour( AR_LABELING_LABEL_TYPE *lImage, int xsize, int ysize, int *label_ref, int label,
                             int clip[4], ARMarkerInfo2 *marker_info2 );
int            arGetLine( int x_coord[], int y_coord[], int coord_num, int vertex[], ARParamLTf *paramLTf,
                          ARdouble line[4][3], ARdouble v[4][2] );


/***********************************/
/*                                 */
/*    For pattern identification   */
/*                                 */
/***********************************/

/*!
	@functiongroup "Pattern identification".
 */
/*!
    @function
    @abstract   Allocate a pattern handle.
    @discussion Allocates an empty pattern handle, into which patterns can
		be loaded by calling arPattLoad().
		When the pattern handle is no longer needed, it should be
		freed by calling arPattDeleteHandle().

        Note that a pattern handle is NOT required when using only matrix-
        code (2D barcode) markers.
    @seealso    arPattLoad arPattLoad
    @seealso    arPattDeleteHandle arPattDeleteHandle
    @result     The created pattern handle, or NULL in case of error.
*/
ARPattHandle  *arPattCreateHandle(void);

/*!
    @function
    @abstract   Allocate a pattern handle and set pattern template size and maximum number of patterns loadable.
    @discussion Allocates an empty pattern handle, into which patterns can
		be loaded by calling arPattLoad().
		When the pattern handle is no longer needed, it should be
		freed by calling arPattDeleteHandle().

        Note that a pattern handle is NOT required when using only matrix-
        code (2D barcode) markers.
    @param pattSize For any square template (pattern) markers, the number of rows and
        columns in the template. May not be less than 16 or more than AR_PATT_SIZE1_MAX.

        Pass AR_PATT_SIZE1 for the same behaviour as arPattCreateHandle().
    @param patternCountMax For any square template (pattern) markers, the maximum number of
        markers that may be loaded for a single matching pass. Must be > 0.

        Pass AR_PATT_NUM_MAX for the same behaviour as arPattCreateHandle().
    @seealso    arPattLoad arPattLoad
    @seealso    arPattDeleteHandle arPattDeleteHandle
    @result     The created pattern handle, or NULL in case of error.
*/

ARPattHandle *arPattCreateHandle2(const int pattSize, const int patternCountMax);

/*!
    @function
    @abstract   Free all loaded patterns and pattern handle.
    @discussion Frees a pattern handle, freeing (unloading)
		any patterns loaded into the handle in the process.
    @param      pattHandle The handle to free.
    @result     0 on success, or -1 if trying to free a NULL handle.
*/
int arPattDeleteHandle(ARPattHandle *pattHandle);

/*!
    @function
    @abstract   Load a pattern file into a pattern handle.
    @discussion
        This function loads a pattern template from a file on disk, and attaches
        it to the given ARPattHandle so making it available for future pattern-matching.
        Additional patterns can be loaded by calling again with the same
        ARPattHandle (however no more than AR_PATT_NUM_MAX patterns can be attached
        to a single ARPattHandle). Patterns are initially loaded
		in an active state.

        Note that matrix-code (2D barcode) markers do not have any associated
        pattern file and do not need to be loaded.
    @param      pattHandle Pattern handle, as generated by arPattCreateHandle(),
		into which the pattern file infomation will be loaded.
	@param      filename Pathname of pattern file to load. The pattern file
		is typically generated by the make_patt program. The pathname is
		relative to the current working directory, which is operating system-
		specific.
    @seealso arPattCreateHandle arPattCreateHandle
    @seealso arPattActivate arPattActivate
    @seealso arPattDeactivate arPattDeactivate
    @seealso arPattFree arPattFree
    @result     Returns the index number of the loaded pattern, in the range
		[0, AR_PATT_NUM_MAX - 1], or -1 if the pattern could not be loaded
		because the maximum number of patterns (AR_PATT_NUM_MAX) has already been
		loaded already into this handle.
*/
int            arPattLoad( ARPattHandle *pattHandle, const char *filename );

int            arPattLoadFromBuffer(ARPattHandle *pattHandle, const char *buffer);

/*!
    @function
    @abstract   Save a pattern to a pattern file.
    @discussion This function is used by the make_patt utility. See the
		sourcecode to mk_patt for usage.
    @param      image (description)
	@param      xsize (description)
	@param      ysize (description)
	@param      pixelFormat (description)
	@param      paramLTf (description)
	@param      imageProcMode (description)
	@param      marker_info (description)
    @param      pattRatio A value between 0.0 and 1.0, representing the proportion of the marker width which constitutes the pattern. In earlier versions, this value was fixed at 0.5.
    @param      pattSize The number of rows and columns to create in the pattern. Normally AR_PATT_SIZE1.
	@param      filename (description)
    @result     (description)
 */
int            arPattSave( ARUint8 *image, int xsize, int ysize, int pixelFormat, ARParamLTf *paramLTf,
                           int imageProcMode, ARMarkerInfo *marker_info, ARdouble pattRatio, int pattSize, const char *filename );

/*!
    @function
    @abstract   Frees (unloads) a pattern file from memory.
    @discussion Unloads a pattern from a pattern handle, freeing that
		slot for another pattern to be loaded, if necessary.
    @param      pattHandle The pattern handle to unload from.
	@param		patno The index into the pattern handle's array of
		patterns to the pattern to be unloaded.
    @result     0 if the pattern was successfully unloaded, or -1
		if there was no pattern loaded.
    @seealso    arPattLoad arPattLoad
 */
int            arPattFree( ARPattHandle *pattHandle, int patno );

/*!
    @function
    @abstract   Activate a previously deactivated pattern.
    @discussion When a pattern is activated, is becomes available
		for recognition in a scene. This is the default state
		for a loaded pattern.
    @param      pattHandle The handle holding the loaded pattern
		which is to be reactivated.
	@param		patno The index into the pattern handle's array of
		patterns to the pattern to be reactivated.
	@result     0 on success, or -1 if the pattern was already
		activated or no pattern was loaded.
    @seealso    arPattDeactivate arPattDeactivate
*/
int            arPattActivate( ARPattHandle *pattHandle, int patno );

/*!
    @function
	@abstract   Deactivate a previously activated pattern.
	@discussion When a pattern is activated, is becomes unavailable
		for recognition in a scene. Deactivating unused patterns
		can speed up recognition time and accuracy when there are
		multiple patterns in a scene, and it is also useful for
		controlling interactivity in a scene.
	@param      pattHandle The handle holding the loaded pattern
		which is to be deactivated.
	@param		patno The index into the pattern handle's array of
		patterns to the pattern to be deactivated.
    @result     0 on success, or -1 if the pattern was already
		deactivated or no pattern was loaded.
    @seealso    arPattActivate arPattActivate
*/
int            arPattDeactivate(ARPattHandle *pattHandle, int patno);

/*!
    @function
    @abstract	Associate a set of patterns with an ARHandle.
    @discussion Associating a set of patterns with an ARHandle makes
		the patterns the set which will be searched when marker
		identification is performed on an image associated with the
		same ARHandle.
    @param      arHandle (description)
	@param      pattHandle (description)
    @seealso    arPattDetach arPattDetach
    @result     Returns 0 in the case of success, or -1 if the specified
        ARHandle already has an ARPattHandle attached, or if arHandle is NULL.
*/
int            arPattAttach(ARHandle *arHandle, ARPattHandle *pattHandle);

/*!
    @function
    @abstract   Reset an ARHandle to no pattern association.
    @discussion See arPattAttach() for more information.
    @param      arHandle (description)
    @seealso    arPattAttach arPattAttach
    @result     Returns 0 in the case of success, or -1 if the specified
        ARHandle has no ARPattHandle attached, or if arHandle is NULL.
*/
int            arPattDetach(ARHandle *arHandle);

//int            arPattGetPattRatio( ARPattHandle *pattHandle, float *ratio );
//int            arPattSetPattRatio( ARPattHandle *pattHandle, float  ratio );

/* ------------------------------ */

#if !AR_DISABLE_NON_CORE_FNS
int            arPattGetID( ARPattHandle *pattHandle, int imageProcMode, int pattDetectMode,
                            ARUint8 *image, int xsize, int ysize, AR_PIXEL_FORMAT pixelFormat,
                            int *x_coord, int *y_coord, int *vertex, ARdouble pattRatio,
                            int *code, int *dir, ARdouble *cf, const AR_MATRIX_CODE_TYPE matrixCodeType );
int            arPattGetImage( int imageProcMode, int pattDetectMode, int patt_size, int sample_size,
                              ARUint8 *image, int xsize, int ysize, AR_PIXEL_FORMAT pixelFormat,
                              int *x_coord, int *y_coord, int *vertex, ARdouble pattRatio,
                              ARUint8 *ext_patt );

/*!
    @function
    @abstract   Match the interior of a detected square against known patterns.
    @discussion
    @param      pattHandle Handle contained details of known patterns, i.e. loaded templates, or valid barcode IDs.
    @param      imageProcMode See discussion of arSetImageProcMode().
    @param      pattDetectMode See discussion of arSetPatternDetectionMode().
    @param      image Pointer to packed raw image data.
    @param      xsize Horizontal pixel dimension of raw image data.
    @param      ysize Vertical pixel dimension of raw image data.
    @param      pixelFormat Pixel format of raw image data.
    @param      arParamLTf Lookup table for the camera parameters for the optical source from which the image was acquired. See arParamLTCreate.
    @param      vertex 4x2 array of points which correspond to the x and y locations of the corners of the detected marker square.
    @param      pattRatio A value between 0.0 and 1.0, representing the proportion of the marker width which constitutes the pattern. In earlier versions, this value was fixed at 0.5.
    @param      codePatt Where the pattern matching mode includes template (picture) matching, and a valid template is matched, the ID of the pattern from pattHandle, or -1 if not identified.
    @param      dirPatt Where the pattern matching mode includes template (picture) matching, and a valid template is matched, the direction (up, right, down, left) of the pattern from pattHandle.
    @param      cfPatt Where the pattern matching mode includes template (picture) matching, and a valid template is matched, the confidence factor of the match (range [0.0 - 1.0]).
    @param      codeMatrix Where the pattern matching mode includes matrix (barcode) matching, and a valid matrix is matched, the ID of the pattern, or -1 if not identified.
    @param      dirMatrix Where the pattern matching mode includes matrix (barcode) matching, and a valid matrix is matched, the direction (up, right, down, left) of the pattern.
    @param      cfMatrix Where the pattern matching mode includes matrix (barcode) matching, and a valid matrix is matched, the confidence factor of the match (range [0.0 - 1.0]).
    @param      matrixCodeType When matrix code pattern detection mode is active, indicates the type of matrix code to detect.
    @result     0 if the function was able to correctly match, or -1 in case of error or no match.
    @seealso    arParamLTCreate arParamLTCreate
 */
int            arPattGetID2( ARPattHandle *pattHandle, int imageProcMode, int pattDetectMode,
                             ARUint8 *image, int xsize, int ysize, AR_PIXEL_FORMAT pixelFormat, ARParamLTf *arParamLTf, ARdouble vertex[4][2], ARdouble pattRatio,
                             int *codePatt, int *dirPatt, ARdouble *cfPatt, int *codeMatrix, int *dirMatrix, ARdouble *cfMatrix,
                             const AR_MATRIX_CODE_TYPE matrixCodeType );
#endif // !AR_DISABLE_NON_CORE_FNS

/*!
    @function
    @abstract   Match the interior of a detected square against known patterns with variable border width.
    @discussion
    @param      pattHandle Handle contained details of known patterns, i.e. loaded templates, or valid barcode IDs.
    @param      imageProcMode See discussion of arSetImageProcMode().
    @param      pattDetectMode See discussion of arSetPatternDetectionMode().
    @param      image Pointer to packed raw image data.
    @param      xsize Horizontal pixel dimension of raw image data.
    @param      ysize Vertical pixel dimension of raw image data.
    @param      pixelFormat Pixel format of raw image data.
    @param      arParamLTf Lookup table for the camera parameters for the optical source from which the image was acquired. See arParamLTCreate.
    @param      vertex 4x2 array of points which correspond to the x and y locations of the corners of the detected marker square.
    @param      pattRatio A value between 0.0 and 1.0, representing the proportion of the marker width which constitutes the pattern. In earlier versions, this value was fixed at 0.5.
    @param      codePatt Where the pattern matching mode includes template (picture) matching, and a valid template is matched, the ID of the pattern from pattHandle, or -1 if not identified.
    @param      dirPatt Where the pattern matching mode includes template (picture) matching, and a valid template is matched, the direction (up, right, down, left) of the pattern from pattHandle.
    @param      cfPatt Where the pattern matching mode includes template (picture) matching, and a valid template is matched, the confidence factor of the match (range [0.0 - 1.0]).
    @param      codeMatrix Where the pattern matching mode includes matrix (barcode) matching, and a valid matrix is matched, the ID of the pattern, or -1 if not identified.
    @param      dirMatrix Where the pattern matching mode includes matrix (barcode) matching, and a valid matrix is matched, the direction (up, right, down, left) of the pattern.
    @param      cfMatrix Where the pattern matching mode includes matrix (barcode) matching, and a valid matrix is matched, the confidence factor of the match (range [0.0 - 1.0]).
    @param      matrixCodeType When matrix code pattern detection mode is active, indicates the type of matrix code to detect.
    @param      errorCorrected Pointer to an integer which will be filled out with the number of errors detected and corrected during marker identification, or NULL if this information is not required.
    @param      codeGlobalID_p Pointer to uint64_t which will be filled out with the global ID, or NULL if this value is not required.
    @result     0 if the function was able to correctly match, or -1 in case of error or no match.
    @seealso    arParamLTCreate arParamLTCreate
 */
int arPattGetIDGlobal( ARPattHandle *pattHandle, int imageProcMode, int pattDetectMode,
              ARUint8 *image, int xsize, int ysize, AR_PIXEL_FORMAT pixelFormat, ARParamLTf *arParamLTf, ARdouble vertex[4][2], ARdouble pattRatio,
              int *codePatt, int *dirPatt, ARdouble *cfPatt, int *codeMatrix, int *dirMatrix, ARdouble *cfMatrix,
              const AR_MATRIX_CODE_TYPE matrixCodeType, int *errorCorrected, uint64_t *codeGlobalID_p );

/*!
    @function
    @abstract   Extract the image (i.e. locate and unwarp) of the pattern-space portion of a detected square.
    @discussion
    @param      imageProcMode See discussion of arSetImageProcMode().
    @param      pattDetectMode See discussion of arSetPatternDetectionMode().
    @param      patt_size The number of horizontal and vertical units to subdivide the pattern-space into.
    @param      sample_size At present, must always be the square of patt_size.
    @param      image Pointer to packed raw image data.
    @param      xsize Horizontal pixel dimension of raw image data.
    @param      ysize Vertical pixel dimension of raw image data.
    @param      pixelFormat Pixel format of raw image data.
    @param      arParamLTf Lookup table for the camera parameters for the optical source from which the image was acquired. See arParamLTCreate.
    @param      vertex 4x2 array of points which correspond to the x and y locations of the corners of the detected marker square.
    @param      pattRatio A value between 0.0 and 1.0, representing the proportion of the marker width which constitutes the pattern. In earlier versions, this value was fixed at 0.5.
    @param      ext_patt Pointer to an array of appropriate size (i.e. patt_size*patt_size*3), which will be filled with the extracted image. Where a colour image is available, it will be supplied in BGR byte order.
    @result     0 if the function was able to correctly get the image, or -1 in case of error or no match.
    @seealso    arParamLTCreate arParamLTCreate
 */
int            arPattGetImage2( int imageProcMode, int pattDetectMode, int patt_size, int sample_size,
                                ARUint8 *image, int xsize, int ysize, AR_PIXEL_FORMAT pixelFormat, ARParamLTf *arParamLTf,
                                ARdouble vertex[4][2], ARdouble pattRatio, ARUint8 *ext_patt );

/*!
    @function
    @abstract   Extract the image (i.e. locate and unwarp) of an arbitrary portion of a detected square.
    @discussion Use this function to obtain an image of the marker pattern space for display to the user.
    @param      arHandle The ARHandle structure associated with the current tracking data.
    @param      markerNo The marker number (in range 0 to arHandle->marker_num - 1, inclusive) from which to extract the pattern.
    @param      image The source video image.
    @param      rect Pointer to an ARPattRectInfo structure which defines the portion of the marker image to extract.
    @param      xsize Width of the output image, in pixels.
    @param      ysize Height of the output image, in pixels.
    @param      overSampleScale Number of samples to acquire per destination pixel, e.g. 2.
    @param      outImage Pointer to a buffer, at least xsize*ysize*arUtilGetPixelSize(arHandle->arPixelFormat) bytes in size, which will be filled out with the marker image.
    @result     0 if the function was able to correctly get the image, or -1 in case of error or no match.
    @seealso    ARPattRectInfo ARPattRectInfo
 */
int            arPattGetImage3( ARHandle *arHandle, int markerNo, ARUint8 *image, ARPattRectInfo *rect, int xsize, int ysize,
                                int overSampleScale, ARUint8 *outImage );


/***********************************/
/*                                 */
/*    For 3D calculation           */
/*                                 */
/***********************************/

/*!
	@functiongroup "3D calculation".
 */

/*!
    @function
    @abstract   Create handle used for 3D calculation from calibrated camera parameters.
    @discussion
        An AR3DHandle holds data structures used in calculating the 3D pose of a
        marker from the 2D location of its corners (i.e. pose estimation).
    @param      arParam (description)
    @result     The handle. When no more ar3D*() functions need be called, the handle should be deleted
        by calling ar3DDeleteHandle().
    @seealso    ar3DCreateHandle2 ar3DCreateHandle2
    @seealso    ar3DDeleteHandle ar3DDeleteHandle
*/
AR3DHandle    *ar3DCreateHandle(ARParam *arParam);

/*!
    @function
    @abstract   Create handle used for 3D calculation from an intrinsic parameters matrix.
    @discussion
        An AR3DHandle holds data structures used in calculating the 3D pose of a
        marker from the 2D location of its corners (i.e. pose estimation).
    @param      cpara (description)
    @result     The handle. When no more ar3D*() functions need be called, the handle should be deleted
        by calling ar3DDeleteHandle().
    @seealso    ar3DCreateHandle ar3DCreateHandle
    @seealso    ar3DDeleteHandle ar3DDeleteHandle
*/
AR3DHandle    *ar3DCreateHandle2(ARdouble cpara[3][4]);

/*!
    @function
    @abstract   Delete handle used for 3D calculation.
    @discussion When no more ar3D*() functions need be called, the handle should be deleted
        by calling ar3DDeleteHandle().
    @param      handle A pointer to the 3D handle. On success, the contents of this location will be set to NULL.
    @result     0 if the handle was successfully deleted, -1 otherwise.
    @seealso    ar3DDeleteHandle ar3DDeleteHandle
*/
int            ar3DDeleteHandle( AR3DHandle **handle );

/*!
    @function
    @abstract   (description)
    @discussion (description)
    @param      handle (description)
    @param      cpara (description)
    @result     (description)
*/
int            ar3DChangeCpara( AR3DHandle *handle, ARdouble cpara[3][4] );

/*!
    @function
    @abstract   (description)
    @discussion (description)
    @param      handle (description)
    @param      maxLoopCount (description)
    @result     (description)
*/
int            ar3DChangeMaxLoopCount( AR3DHandle *handle, int maxLoopCount );

/*!
    @function
    @abstract   (description)
    @discussion (description)
    @param      handle (description)
    @param      loopBreakThresh (description)
    @result     (description)
*/
int            ar3DChangeLoopBreakThresh( AR3DHandle *handle, ARdouble loopBreakThresh );

/*!
    @function
    @abstract   (description)
    @discussion (description)
    @param      handle (description)
    @param      loopBreakThreshRatio (description)
    @result     (description)
*/
int            ar3DChangeLoopBreakThreshRatio( AR3DHandle *handle, ARdouble loopBreakThreshRatio );

/*!
    @function
    @abstract   (description)
    @discussion (description)
    @param      handle (description)
    @param      marker_info (description)
    @param      width (description)
    @param      conv (description)
    @result     (description)
*/
ARdouble         arGetTransMatSquare( AR3DHandle *handle, ARMarkerInfo *marker_info,
                                    ARdouble width, ARdouble conv[3][4] );

/*!
    @function
    @abstract   (description)
    @discussion (description)
    @param      handle (description)
    @param      marker_info (description)
    @param      initConv (description)
    @param      width (description)
    @param      conv (description)
    @result     (description)
*/
ARdouble         arGetTransMatSquareCont( AR3DHandle *handle, ARMarkerInfo *marker_info,
                                        ARdouble initConv[3][4],
                                        ARdouble width, ARdouble conv[3][4] );

/*!
    @function
    @abstract   (description)
    @discussion (description)
    @param      handle (description)
    @param      handle (description)
    @param      initConv (description)
    @param      pos2d (description)
    @param      pos3d (description)
    @param      num (description)
    @param      conv (description)
    @result     (description)
*/
ARdouble         arGetTransMat( AR3DHandle *handle, ARdouble initConv[3][4],
                              ARdouble pos2d[][2], ARdouble pos3d[][3], int num,
                              ARdouble conv[3][4] );

/*!
    @function
    @abstract   (description)
    @discussion (description)
    @param      handle (description)
    @param      initConv (description)
    @param      pos2d (description)
    @param      pos3d (description)
    @param      num (description)
    @param      conv (description)
    @result     (description)
*/
ARdouble         arGetTransMatRobust( AR3DHandle *handle, ARdouble initConv[3][4],
                                    ARdouble pos2d[][2], ARdouble pos3d[][3], int num,
                                    ARdouble conv[3][4] );


/***********************************/
/*                                 */
/*    For 3D calculation by Stereo */
/*                                 */
/***********************************/

/*!
	@functiongroup "3D calculation by Stereo".
 */

AR3DStereoHandle    *ar3DStereoCreateHandle(ARParam *arParamL, ARParam *arParamR, ARdouble transL[3][4], ARdouble transR[3][4]);
AR3DStereoHandle    *ar3DStereoCreateHandle2(ARdouble cparaL[3][4], ARdouble cparaR[3][4], ARdouble transL[3][4], ARdouble transR[3][4]);
int                  ar3DStereoDeleteHandle( AR3DStereoHandle **handle );
int                  ar3DStereoChangeMaxLoopCount( AR3DStereoHandle *handle, int maxLoopCount );
int                  ar3DStereoChangeLoopBreakThresh( AR3DStereoHandle *handle, ARdouble loopBreakThresh );
int                  ar3DStereoChangeLoopBreakThreshRatio( AR3DStereoHandle *handle, ARdouble loopBreakThreshRatio );
int                  ar3DStereoChangeCpara( AR3DStereoHandle *handle, ARdouble cparaL[3][4], ARdouble cparaR[3][4] );
int                  ar3DStereoChangeTransMat( AR3DStereoHandle *handle, ARdouble transL[3][4], ARdouble transR[3][4] );

ARdouble             arGetTransMatSquareStereo( AR3DStereoHandle *handle,
                                                ARMarkerInfo *marker_infoL, ARMarkerInfo *marker_infoR,
                                                ARdouble width, ARdouble conv[3][4] );
ARdouble             arGetTransMatSquareContStereo( AR3DStereoHandle *handle,
                                                    ARMarkerInfo *marker_infoL, ARMarkerInfo *marker_infoR,
                                                    ARdouble prev_conv[3][4],
                                                    ARdouble width, ARdouble conv[3][4] );
ARdouble             arGetTransMatStereo( AR3DStereoHandle *handle, ARdouble initConv[3][4],
                                          ARdouble pos2dL[][2], ARdouble pos3dL[][3], int numL,
                                          ARdouble pos2dR[][2], ARdouble pos3dR[][3], int numR,
                                          ARdouble conv[3][4] );
ARdouble             arGetTransMatStereoRobust( AR3DStereoHandle *handle, ARdouble initConv[3][4],
                                                ARdouble pos2dL[][2], ARdouble pos3dL[][3], int numL,
                                                ARdouble pos2dR[][2], ARdouble pos3dR[][3], int numR,
                                                ARdouble conv[3][4] );

ARdouble             arGetStereoMatchingErrorSquare( AR3DStereoHandle *handle,
                                                     ARMarkerInfo *marker_infoL,
                                                     ARMarkerInfo *marker_infoR );
ARdouble             arGetStereoMatchingError( AR3DStereoHandle *handle,
                                               ARdouble pos2dL[2], ARdouble pos2dR[2] );
int                  arGetStereoMatching( AR3DStereoHandle *handle,
                                          ARdouble pos2dL[2], ARdouble pos2dR[2], ARdouble pos3d[3] );



/***********************************/
/*                                 */
/*    Utility                      */
/*                                 */
/***********************************/

/*!
	@functiongroup "Utility".
 */

/*!
    @function
    @abstract   Get the ARToolKit version information in numberic and string format.
    @discussion
		As of version 2.72, ARToolKit now allows querying of the version number
		of the toolkit available at runtime. It is highly recommended that
		any calling program that depends on features in a certain
		ARToolKit version, check at runtime that it is linked to a version
		of ARToolKit that can supply those features. It is NOT sufficient
		to check the ARToolKit SDK header versions, since with ARToolKit implemented
		in dynamically-loaded libraries, there is no guarantee that the
		version of ARToolKit installed on the machine at run-time will be as
		recent as the version of the ARToolKit SDK which the host
		program was compiled against.

		The version information is reported in binary-coded decimal format,
		and optionally in an ASCII string.

        A increase in the major version number indicates the removal of functionality
        previously provided in the API. An increase in the minor version number
        indicates that new functionality has been added. A change in the tiny version
        number indicates changes (e.g. bug fixes) which do not affect the API. See
        the comments in the config.h header for more discussion of the definition of
        major, minor, tiny and build version numbers.

	@param      versionStringRef
		If non-NULL, the location pointed to will be filled
		with a pointer to a string containing the version information.
		Fields in the version string are separated by spaces. As of version
		2.72.0, there is only one field implemented, and this field
		contains the major, minor and tiny version numbers
		in dotted-decimal format. The string is guaranteed to contain
		at least this field in all future versions of the toolkit.
		Later versions of the toolkit may add other fields to this string
		to report other types of version information. The storage for the
		string is malloc'ed inside the function. The caller is responsible
		for free'ing the string.
    @result
		Returns the full version number of the ARToolKit in
		binary coded decimal (BCD) format.
		BCD format allows simple tests of version number in the caller
		e.g. if ((arGetVersion(NULL) >> 16) > 0x0272) printf("This release is later than 2.72\n");
		The major version number is encoded in the most-significant byte
		(bits 31-24), the minor version number in the second-most-significant
		byte (bits 23-16), the tiny version number in the third-most-significant
		byte (bits 15-8), and the build version number in the least-significant
		byte (bits 7-0).
 */
ARUint32 arGetVersion(char **versionStringRef);

int            arUtilMatInv( const ARdouble s[3][4], ARdouble d[3][4] );
int            arUtilMatMul( const ARdouble s1[3][4], const ARdouble s2[3][4], ARdouble d[3][4] );

#ifdef ARDOUBLE_IS_FLOAT
#define arUtilMatInvf arUtilMatInv
#define arUtilMatMulf arUtilMatMul
#define arUtilMatMuldff arUtilMatMul
#else
int            arUtilMatInvf( const float s[3][4], float d[3][4] );
int            arUtilMatMulf( const float s1[3][4], const float s2[3][4], float d[3][4] );
int            arUtilMatMuldff( const ARdouble s1[3][4], const float s2[3][4], float d[3][4] );
#endif
int            arUtilMat2QuatPos( const ARdouble m[3][4], ARdouble q[4], ARdouble p[3] );
int            arUtilQuatPos2Mat( const ARdouble q[4], const ARdouble p[3], ARdouble m[3][4] );
int            arUtilQuatNorm(ARdouble q[4]);

double         arUtilTimer(void);
void           arUtilTimerReset(void);

#ifndef _WINRT
/*!
    @function
    @abstract   Relinquish CPU to the system for specified number of milliseconds.
    @discussion
        This function calls the native system-provided sleep routine to relinquish
        CPU control to the system for the specified time.
    @param      msec Sleep time in milliseconds (thousandths of a second).
    @availability Not available on Windows Runtime (WinRT).
 */
void           arUtilSleep( int msec );
#endif // !_WINRT

int            arUtilReplaceExt( char *filename, int n, char *ext );
int            arUtilRemoveExt ( char *filename );
int            arUtilDivideExt ( const char *filename, char *s1, char *s2 );

int            arUtilGetSquareCenter( ARdouble vertex[4][2], ARdouble *x, ARdouble *y );

int            arUtilSortLabel( int mask[], int m, int n,
                                ARdouble pos[][2], int area[], int label_num,
                                int l1, int x1, int y1,
                                int l2, int x2, int y2,
                                int label[] );

/*!
    @function
    @abstract   Get the size in bytes of a single pixel for a given pixel format.
    @discussion
        Different pixel formats have different sizes in bytes, and therefore
        different storage requirements per row of pixels. Use this function
        to calculate the number of bytes required to store a single pixel
        of the given type.
    @param      arPixelFormat The pixel type whose size is to be measured.
    @result     Number of bytes required to store 1 pixel of the given type.
*/
int            arUtilGetPixelSize( const AR_PIXEL_FORMAT arPixelFormat );

/*!
    @function
    @abstract   Get a string holding a descriptive name for a given pixel format enumeration.
    @discussion
        On occasions it can be useful to display to the user the format of the pixels
        which ARToolKit is processing. This funtion converts a pixel-format number
        into a human-readable string description.
    @param      arPixelFormat Enumerated pixel format number for which to retrieve a name.
    @result     A constant c-string holding a descriptive name for the pixel format.
        The string returned matches the constants used in the definition of the type
        AR_PIXEL_FORMAT, e.g. "AR_PIXEL_FORMAT_RGB".
*/
const char *arUtilGetPixelFormatName(const AR_PIXEL_FORMAT arPixelFormat);

char          *arUtilGetMachineType(void);

/*
    @function
    @abstract Get the filename portion of a full pathname.
    @discussion
        Given a full or partial pathname passed in string path,
        returns a pointer to the first char of the filename
        portion of path.
 */
const char *arUtilGetFileNameFromPath(const char *path);


/*!
    @function
    @abstract Get file base name from a path.
    @discussion
        Given a full or partial pathname passed in string path,
        returns a string with the base name portion of path,
        i.e. the text between the rightmost path separator and the
        the rightmost '.' character, if any.
        If the filename contains no '.', returns the filename.
	@param path Full or partial pathname.
    @param convertToLowercase If convertToLowercase is TRUE, uppercase
        ASCII characters in the basename will be converted to lowercase.
    @result A string with the basename portion of path.
        NB: The returned string must be freed by the caller.
 */
char *arUtilGetFileBasenameFromPath(const char *path, const int convertToLowercase);

/*!
    @function
    @abstract Get file extension from a path.
    @discussion
        Given a full or partial pathname passed in string path,
        returns a string with the extension portion of path,
        i.e. the text after the rightmost '.' character, if any.
        If the filename contains no '.', NULL is returned.
	@param path Full or partial pathname.
    @param convertToLowercase If convertToLowercase is TRUE, uppercase
        ASCII characters in the extension will be converted to lowercase.
    @result A string with the extension portion of path.
        NB: The returned string must be freed by the caller.
 */
char *arUtilGetFileExtensionFromPath(const char *path, const int convertToLowercase);

    /*
     @function
     @abstract Get the directory portion of a full pathname.
     @discussion
        Given a full or partial pathname passed in string path,
        returns a string with the directory name portion of path.
        The string is placed into the buffer of size n pointed to by dir.
        If the string and its terminating null byte cannot be accomodated by the
        buffer, NULL is returned, otherwise dir is returned.
        The string is terminated by the directory separator if addSeparator != 0.
 */
char *arUtilGetDirectoryNameFromPath(char *dir, const char *path, const size_t n, const int addSeparator);

/*!
    @function
    @abstract Get a path as a file URI.
    @discussion
        Given a full or partial pathname passed in string path,
        returns a string with the file URI for that path.

        Partial pathnames are handled by concatening with the
        process's current working directory.
	@param path Full or partial pathname.

        On Windows, both partial pathnames, full pathnames including
        the drive letter, or UNC pathnames (beginning with "\\" are
        all OK.
    @result A string with the the file URI for that path, or NULL
        in the case of error.
        NB: The returned string must be freed by the caller (by
        calling free() once its use is complete).
 */
char *arUtilGetFileURI(const char *path);

/*!
    @typedef
    @abstract Options for controlling the behavior of arUtilGetResourcesDirectoryPath and arUtilChangeToResourcesDirectory.
    @constant AR_UTIL_RESOURCES_DIRECTORY_BEHAVIOR_BEST          Use a platform-dependent recommended-best option.
        Note the this behavior is subject to change in future versions of ARToolKit.
        At present, on Mac OS X and iOS, this will change to the Apple-provided resources directory inside the application bundle.
        At present, on other platforms, this will change to the same directory as the executable.
    @constant AR_UTIL_RESOURCES_DIRECTORY_BEHAVIOR_USE_CWD Use the current working directory. For arUtilChangeToResourcesDirectory, this will leave the current working directory unchanged.
    @constant AR_UTIL_RESOURCES_DIRECTORY_BEHAVIOR_USE_SUPPLIED_PATH Change to the working directory specified.
    @constant AR_UTIL_RESOURCES_DIRECTORY_BEHAVIOR_USE_EXECUTABLE_DIR Change to the same directory as the executable.
		On OS X and iOS, this corresponds to the directory of the binary executable inside the app bundle, not ythe directory containing the app bundle.
    @constant AR_UTIL_RESOURCES_DIRECTORY_BEHAVIOR_USE_USER_ROOT Change to the root of the implementation-dependent user-writable root.
        On OS X and Linux, this is equivalent to the "~" user home. On Android, this is the root of the "external" storage (e.g. an SD card).
        On Windows, this is the user home directory, typically "C:\Documents and Settings\USERNAME".
    @constant AR_UTIL_RESOURCES_DIRECTORY_BEHAVIOR_USE_APP_CACHE_DIR Change to a writable cache directory, i.e. a directory which is not normally shown to the user, in which files which may be subject to deletion by the system or the user.
        On Android, change to the applications's (internal) cache directory, and a valid instance of Android/Context must be passed in the instanceofAndroidContext parameter.
	@constant AR_UTIL_RESOURCES_DIRECTORY_BEHAVIOR_USE_APP_DATA_DIR Change to a writable data directory, i.e. a directory which is not normally shown to the user, but in which files are retained permanently.
	@seealso arUtilGetResourcesDirectoryPath arUtilGetResourcesDirectoryPath
	@seealso arUtilChangeToResourcesDirectory arUtilChangeToResourcesDirectory
 */
typedef enum {
    AR_UTIL_RESOURCES_DIRECTORY_BEHAVIOR_BEST = 0,
    AR_UTIL_RESOURCES_DIRECTORY_BEHAVIOR_USE_CWD,
    AR_UTIL_RESOURCES_DIRECTORY_BEHAVIOR_USE_SUPPLIED_PATH,
    AR_UTIL_RESOURCES_DIRECTORY_BEHAVIOR_USE_EXECUTABLE_DIR,
    AR_UTIL_RESOURCES_DIRECTORY_BEHAVIOR_USE_BUNDLE_RESOURCES_DIR,
    AR_UTIL_RESOURCES_DIRECTORY_BEHAVIOR_USE_USER_ROOT,
    AR_UTIL_RESOURCES_DIRECTORY_BEHAVIOR_USE_APP_CACHE_DIR,
	AR_UTIL_RESOURCES_DIRECTORY_BEHAVIOR_USE_APP_DATA_DIR
} AR_UTIL_RESOURCES_DIRECTORY_BEHAVIOR;

/*!
    @function
    @abstract   Get the path to the resources directory using the specified behavior.
    @discussion
    	ARToolKit uses relative paths to locate several types of resources, including
    	camera parameter files, pattern files, multimarker files and others.
    	This function provides the convenience of finding an appropriate value for your
        application.

        On Android only, the function has an optional parameter 'instanceOfAndroidContext'.
        If behavior is AR_UTIL_RESOURCES_DIRECTORY_BEHAVIOR_USE_APP_CACHE_DIR, this
        parameter must be an instance of a class derived from android/content/Context.
        In all other cases, pass NULL for this parameter.
    @param behavior See AR_UTIL_RESOURCES_DIRECTORY_BEHAVIOR type for allowed values.
    @result NULL in the case of error, or the path otherwise. Must be free()d by the caller.
 */
#ifdef ANDROID
char *arUtilGetResourcesDirectoryPath(AR_UTIL_RESOURCES_DIRECTORY_BEHAVIOR behavior, jobject instanceOfAndroidContext);
#else
char *arUtilGetResourcesDirectoryPath(AR_UTIL_RESOURCES_DIRECTORY_BEHAVIOR behavior);
#endif

#ifndef _WINRT
/*!
    @function
    @abstract   Change to the resources directory using the specified behavior.
    @discussion
    	ARToolKit uses relative paths to locate several types of resources, including
    	camera parameter files, pattern files, multimarker files and others.
    	This function provides the convenience of setting the current process
    	working directory to the appropriate value for your application.

        On Android only, the function has an optional parameter 'instanceOfAndroidContext'.
        If behavior is AR_UTIL_RESOURCES_DIRECTORY_BEHAVIOR_USE_APP_CACHE_DIR, this
        parameter must be an instance of a class derived from android/content/Context.
        In all other cases, pass NULL for this parameter.
    @param behavior See AR_UTIL_RESOURCES_DIRECTORY_BEHAVIOR type for allowed values.
    @param path When behavior is AR_UTIL_RESOURCES_DIRECTORY_BEHAVIOR_USE_SUPPLIED_PATH,
    	the path to change to (absolute or relative to current working directory). In all
        other cases, if this parameter is non-NULL, it will be taken as a subdirectory
        of the desired path and to which the working directory should be changed.
    @result -1 in the case of error, or 0 otherwise.
	@availability Not available on Windows Runtime (WinRT).
 */
#ifdef ANDROID
int arUtilChangeToResourcesDirectory(AR_UTIL_RESOURCES_DIRECTORY_BEHAVIOR behavior, const char *path, jobject instanceOfAndroidContext);
#else
int arUtilChangeToResourcesDirectory(AR_UTIL_RESOURCES_DIRECTORY_BEHAVIOR behavior, const char *path);
#endif
#endif // !_WINRT

/*!
    @function
    @abstract   Prints a transformation matrix via ARLOG(...).
    @param trans The transformation matrix to print.
 */
void arUtilPrintTransMat(const ARdouble trans[3][4]);

/*!
    @function
    @abstract   Prints a 4x4 row-major matrix via ARLOG(...).
    @param mtx16 The matrix to print.
 */
void arUtilPrintMtx16(const ARdouble mtx16[16]);

#ifdef ANDROID
    //Call from native code to do the following in Java source:
    //    import android.provider.Settings.Secure;
    //    private String android_id = Secure.getString(getContext().getContentResolver(),
    //                                                 Secure.ANDROID_ID);
    char* arUtilGetAndroidDevID();
#endif //#ifdef ANDROID

#ifdef __cplusplus
}
#endif //#ifdef __cplusplus

#endif //#ifndef AR_H
