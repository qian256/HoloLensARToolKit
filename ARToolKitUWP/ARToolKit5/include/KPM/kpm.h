/*
 *  kpm.h
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
 *  Copyright 2015 Daqri, LLC. All rights reserved.
 *  Copyright 2006-2015 ARToolworks, Inc. All rights reserved.
 *  Author(s): Hirokazu Kato, Philip Lamb
 *
 */

/*!
	@header kpm
	@abstract libKPM NFT image recognition and tracking initialisation routines.
	@discussion
        This header declares types and API for an NFT tracker,
        in particular those routines involved in recognising a texture page and
        initialising the tracking for use by the texture tracker.
    @copyright 2006-2015 ARToolworks, Inc.
 */

#ifndef KPM_H
#define KPM_H

#define     BINARY_FEATURE    1

#include <AR/ar.h>
#include <KPM/kpmType.h>

#ifdef __cplusplus
extern "C" {
#endif
    
#define   KpmPose6DOF            1
#define   KpmPoseHomography      2

typedef enum {
    KpmProcFullSize        = 1,
    KpmProcHalfSize        = 2,
    KpmProcQuatSize        = 3,
    KpmProcOneThirdSize    = 4,
    KpmProcTwoThirdSize    = 5
} KPM_PROC_MODE;
#define   KpmDefaultProcMode     KpmProcFullSize

#define   KpmCompNull            0
#define   KpmCompX               1
#define   KpmCompY               2
#define   KpmDefaultComp         KpmCompNull

#define   KpmChangePageNoAllPages (-1)

typedef struct {
    float             x;
    float             y;
} KpmCoord2D;

typedef struct {
    int               width;
    int               height;
    int               imageNo;
} KpmImageInfo;

typedef struct {
    KpmImageInfo     *imageInfo;
    int               imageNum;
    int               pageNo;
} KpmPageInfo;

typedef struct _KpmRefData {
    KpmCoord2D        coord2D;
    KpmCoord2D        coord3D;      // millimetres.
#if BINARY_FEATURE
    FreakFeature      featureVec;
#else
    SurfFeature       featureVec;
#endif
    int               pageNo;
    int               refImageNo;
} KpmRefData;

/*!
    @typedef    KpmRefDataSet
    @abstract   A loaded dataset for KPM tracking.
    @discussion
        Key point matching takes as input a reference data set of points. This structure holds a set of points in memory prior to loading into the tracker.
	@field		refPoint Tracking reference points.
	@field		num Number of refPoints in the dataset.
	@field		pageInfo Array of info about each page in the dataset. One entry per page.
	@field		pageNum Number of pages in the dataset (i.e. a count, not an index).
 */
typedef struct {
    KpmRefData       *refPoint;
    int               num;
    KpmPageInfo      *pageInfo;
    int               pageNum;
} KpmRefDataSet;

/*!
    @typedef    KpmInputDataSet
    @abstract   Data describing the number and location of keypoints in an input image to be matched against a loaded data set.
    @discussion
        Key point matching occurs between a loaded data set and a set of keypoints extracted from an input image. This structure
        holds the number and pixel location of keypoints in the input image. The keypoints themselves are an array of 'num'
        KpmRefData structures.
	@field		coord Array of pixel locations of the keypoints in an input image.
	@field		num Number of coords in the array.
  */
typedef struct {
    KpmCoord2D       *coord;
    int               num;
} KpmInputDataSet;

#if !BINARY_FEATURE
typedef struct {
    int               refIndex;
    int               inIndex;
} KpmMatchData;

typedef struct {
    KpmMatchData     *match;
    int               num;
} KpmMatchResult;
#endif

typedef struct {
    float                     camPose[3][4];
    int                       pageNo;
    float                     error;
    int                       inlierNum;
    int                       camPoseF;
    int                       skipF;
} KpmResult;

typedef struct _KpmHandle KpmHandle;


/*!
    @function
    @abstract Allocate and initialise essential structures for KPM tracking, using full six degree-of-freedom tracking.
    @discussion 
    @param cparamLT Pointer to an ARParamLT structure holding camera parameters in lookup-table form.
        The pointer only is copied, and the ARParamLT structure itself is NOT copied, and must remain
        valid for the lifetime of the KpmHandle.
        This structure also specifies the size of video frames which will be later supplied to the
        kpmMatching() function as cparamLT->param.xsize and cparamLT->param.ysize.
    @param pixFormat Pixel format of video frames which will be later supplied to the kpmMatching() function.
    @result Pointer to a newly-allocated KpmHandle structure. This structure must be deallocated
        via a call to kpmDeleteHandle() when no longer needed.
    @seealso kpmCreateHandleHomography kpmCreateHandleHomography
    @seealso kpmDeleteHandle kpmDeleteHandle
 */
KpmHandle  *kpmCreateHandle ( ARParamLT *cparamLT, AR_PIXEL_FORMAT pixFormat );
#define     kpmCreatHandle kpmCreateHandle

KpmHandle  *kpmCreateHandle2( int xsize, int ysize, AR_PIXEL_FORMAT pixFormat );
#define     kpmCreatHandle2 kpmCreateHandle2

/*!
    @function
    @abstract Allocate and initialise essential structures for KPM tracking, using homography-only tracking.
    @discussion
        Homography tracking assumes that the camera has zero lens-distortion, and this does not depend on
        camera parameters. It is therefore unable to provide correctly calibrated position measurements,
        but the resulting pose is suitable for visual overlay purposes.
    @param xsize Width of video frames which will be later supplied to the kpmMatching() function.
    @param ysize Height of video frames which will be later supplied to the kpmMatching() function.
    @param pixFormat Pixel format of video frames which will be later supplied to the kpmMatching() function.
    @result Pointer to a newly-allocated KpmHandle structure. This structure must be deallocated
        via a call to kpmDeleteHandle() when no longer needed.
    @seealso kpmCreateHandle kpmCreateHandle
    @seealso kpmDeleteHandle kpmDeleteHandle
 */
KpmHandle  *kpmCreateHandleHomography( int xsize, int ysize, AR_PIXEL_FORMAT pixFormat );
#define     kpmCreatHandleHomography kpmCreateHandleHomography

/*!
    @function
    @abstract Finalise and dispose of structures for KPM tracking.
    @discussion
        Once KPM processing has completed, this routine should be called to
        dispose of memory allocated.
    @param kpmHandle Pointer to a location which holds a pointer to a KpmHandle structure.
        On return, the location pointed to will be set to NULL.
    @result 0 if successful, or value &lt;0 in case of error.
    @seealso kpmCreateHandle kpmCreateHandle
    @seealso kpmCreateHandleHomography kpmCreateHandleHomography
 */
int         kpmDeleteHandle( KpmHandle **kpmHandle );

int         kpmHandleGetXSize(const KpmHandle *kpmHandle);
int         kpmHandleGetYSize(const KpmHandle *kpmHandle);
AR_PIXEL_FORMAT kpmHandleGetPixelFormat(const KpmHandle *kpmHandle);
    
int         kpmSetProcMode( KpmHandle *kpmHandle, KPM_PROC_MODE  procMode );
int         kpmGetProcMode( KpmHandle *kpmHandle, KPM_PROC_MODE *procMode );
int         kpmSetDetectedFeatureMax( KpmHandle *kpmHandle, int  detectedMaxFeature );
int         kpmGetDetectedFeatureMax( KpmHandle *kpmHandle, int *detectedMaxFeature );
int         kpmSetSurfThreadNum( KpmHandle *kpmHandle, int surfThreadNum );

/*!
    @function
    @abstract Load a reference data set into the key point matcher for tracking.
    @discussion
        This function takes a reference data set already in memory and makes it the current
        dataset for key point matching.
    @param kpmHandle Handle to the current KPM tracker instance, as generated by kpmCreateHandle or kpmCreateHandleHomography.
    @param refDataSet The reference data set to load into the KPM handle. The operation takes
        a copy of the data required from this dataset, thus unless the need for a further load
        at a later time is required, the dataset can be disposed of by calling kpmDeleteRefDataSet
        after this operation succeeds.
    @result 0 if successful, or value &lt;0 in case of error.
    @seealso kpmCreateHandle kpmCreateHandle
    @seealso kpmCreateHandleHomography kpmCreateHandleHomography
    @seealso kpmDeleteRefDataSet kpmDeleteRefDataSet
 */
int         kpmSetRefDataSet( KpmHandle *kpmHandle, KpmRefDataSet *refDataSet );

/*!
    @function
    @abstract
        Loads a reference data set from a file into the KPM tracker.
    @discussion
        This is a convenience method which performs a sequence of kpmLoadRefDataSet, followed
        by kpmSetRefDataSet and finally kpmDeleteRefDataSet. When tracking from a single
        reference dataset file, this is the simplest means to start.
    @param kpmHandle Handle to the current KPM tracker instance, as generated by kpmCreateHandle or kpmCreateHandleHomography.
    @param filename Path to the dataset. Either full path, or a relative path if supported by
        the operating system.
    @param ext If non-NULL, a '.' charater and this string will be appended to 'filename'.
        Often, this parameter is a pointer to the string "fset3".
    @result Returns 0 if successful, or value &lt;0 in case of error.
    @seealso kpmLoadRefDataSet kpmLoadRefDataSet
    @seealso kpmSetRefDataSet kpmSetRefDataSet
    @seealso kpmDeleteRefDataSet kpmDeleteRefDataSet
 */
int         kpmSetRefDataSetFile( KpmHandle *kpmHandle, const char *filename, const char *ext );

int         kpmSetRefDataSetFileOld( KpmHandle *kpmHandle, const char *filename, const char *ext );

/*!
    @function
    @abstract Perform key-point matching on an image.
    @discussion 
    @param kpmHandle
    @param inImage Source image containing the pixels which will be searched for features.
        Typically, this is one frame from a video stream. The dimensions and pixel format
        of this image must match the values specified at the time of creation of the KPM handle.
    @result 0 if successful, or value &lt;0 in case of error.
    @seealso kpmCreateHandle kpmCreateHandle
    @seealso kpmCreateHandleHomography kpmCreateHandleHomography
 */
int         kpmMatching( KpmHandle *kpmHandle, ARUint8 *inImage );

int         kpmSetMatchingSkipPage( KpmHandle *kpmHandle, int *skipPages, int num );
#if !BINARY_FEATURE
int         kpmSetMatchingSkipRegion( KpmHandle *kpmHandle, SurfSubRect *skipRegion, int regionNum);
#endif

int         kpmGetRefDataSet( KpmHandle *kpmHandle, KpmRefDataSet **refDataSet );
int         kpmGetInDataSet( KpmHandle *kpmHandle, KpmInputDataSet **inDataSet );
#if !BINARY_FEATURE
int         kpmGetMatchingResult( KpmHandle *kpmHandle, KpmMatchResult **preRANSAC, KpmMatchResult **aftRANSAC );
#endif
int         kpmGetPose( KpmHandle *kpmHandle, float  pose[3][4], int *pageNo, float  *error );
int         kpmGetResult( KpmHandle *kpmHandle, KpmResult **result, int *resultNum );


int         kpmGenRefDataSet ( ARUint8 *refImage, AR_PIXEL_FORMAT pixFormat, int xsize, int ysize, float  dpi, int procMode, int compMode, int maxFeatureNum,
                               int pageNo, int imageNo, KpmRefDataSet **refDataSet );
int         kpmAddRefDataSet ( ARUint8 *refImage, AR_PIXEL_FORMAT pixFormat, int xsize, int ysize, float  dpi, int procMode, int compMode, int maxFeatureNum,
                               int pageNo, int imageNo, KpmRefDataSet **refDataSet );

/*!
    @function
    @abstract Merge a second KPM dataset into the first, and dispose of second.
    @discussion
        This function merges two KPM datasets by adding the reference points in
        the second into the first (allocating a new set if the location pointed to
        by refDataSetPtr1 is NULL) and then deleting the second set.
    @param refDataSetPtr1 Pointer to a location which points to the first data set, or pointer
        to NULL if a new dataset is to be created. This will hold the results of the merge.
    @param refDataSetPtr2 Pointer to a location which points to the second data set. After the
        merge, the dataset pointed to will be deleted and the location pointed to set to NULL.
    @result 0 if the merge succeeded, or a value &lt; 0 in case of error.
 */
int         kpmMergeRefDataSet  ( KpmRefDataSet **refDataSetPtr1, KpmRefDataSet **refDataSetPtr2 );
#define     kpmMargeRefDataSet kpmMergeRefDataSet
    
/*!
    @function
    @abstract Dispose of a reference data set and its allocated memory.
    @discussion
        Once a data set has been loaded into a KPM handle, or is otherwise no longer required
        to be held in memory, it should be deleted (i.e. disposed) from memory by calling
        this function.
    @param refDataSetPtr Pointer to memory location which points to the dataset. On success,
        this location will be set to NULL.
    @result 0 if the delete succeeded, or a value &lt; 0 in case of error.
    @seealso kpmLoadRefDataSet kpmLoadRefDataSet
 */
int         kpmDeleteRefDataSet ( KpmRefDataSet **refDataSetPtr );

/*!
    @function
    @abstract 
    @discussion 
    @param filename
    @param ext
    @param refDataSet
    @result 
 */
int         kpmSaveRefDataSet   ( const char *filename, const char *ext, KpmRefDataSet  *refDataSet );

/*!
    @function
    @abstract Load a reference data set from the filesystem into memory.
    @discussion
        This does not set the reference data as the current tracking set. To do that, call
        kpmSetRefDataSet after this load completes. Alternately, the loaded set can be
        merged with another loaded set by calling kpmMergeRefDataSet. To dispose of the
        loaded dataset, call kpmDeleteRefDataSet.
    @param filename Path to the dataset. Either full path, or a relative path if supported by
        the operating system.
    @param ext If non-NULL, a '.' charater and this string will be appended to 'filename'.
        Often, this parameter is a pointer to the string "fset3".
    @result Returns 0 if successful, or value &lt;0 in case of error.
    @param refDataSetPtr Pointer to a location which after loading will point to the loaded
        reference data set.
    @result 0 if the load succeeded, or a value &lt; 0 in case of error.
    @seealso kpmSetRefDataSet kpmSetRefDataSet
    @seealso kpmMergeRefDataSet kpmMergeRefDataSet
    @seealso kpmDeleteRefDataSet kpmDeleteRefDataSet
 */
int         kpmLoadRefDataSet   ( const char *filename, const char *ext, KpmRefDataSet **refDataSetPtr );

int         kpmLoadRefDataSetOld( const char *filename, const char *ext, KpmRefDataSet **refDataSetPtr );

/*!
    @function
    @abstract 
    @discussion 
    @param refDataSet
    @param oldPageNo
    @param newPageNo
    @result 
 */
int         kpmChangePageNoOfRefDataSet ( KpmRefDataSet *refDataSet, int oldPageNo, int newPageNo );


/*!
    @function
    @abstract 
    @discussion 
    @param image Source image, as an unpadded pixel buffer beginning with the leftmost pixel of the top row.
    @param pixFormat Layout of pixel data in 'image'.
    @param xsize Layout of pixel data in 'image'.
    @param ysize Layout of pixel data in 'image'.
    @param procMode
    @result 
 */
ARUint8    *kpmUtilGenBWImage( ARUint8 *image, AR_PIXEL_FORMAT pixFormat, int xsize, int ysize, int procMode, int *newXsize, int *newYsize );

#if !BINARY_FEATURE
int         kpmUtilGetPose ( ARParamLT *cparamLT, KpmMatchResult *matchData, KpmRefDataSet *refDataSet, KpmInputDataSet *inputDataSet, float  camPose[3][4], float  *err );
    
int         kpmUtilGetPose2( ARParamLT *cparamLT, KpmMatchResult *matchData, KpmRefDataSet *refDataSet, int *redDataIndex, KpmInputDataSet *inputDataSet, float  camPose[3][4], float  *error );
int         kpmUtilGetPoseHomography( KpmMatchResult *matchData, KpmRefDataSet *refDataSet, KpmInputDataSet *inputDataSet, float  camPose[3][4], float  *err );
#endif
int         kpmUtilGetCorner( ARUint8 *inImagePtr, AR_PIXEL_FORMAT pixFormat, int xsize, int ysize, int procMode, int maxPointNum, CornerPoints *cornerPoints );


double wallclock();
    
int kpmLoadImageDb(const char *filename);
    
#ifdef __cplusplus
}
#endif
#endif
