/*
 *	param.h
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
 *  Author(s): Takeshi Mita, Shinsaku Hiura, Hirokazu Kato, Philip Lamb
 *
 */
/*******************************************************
 *
 * Author: Takeshi Mita, Shinsaku Hiura, Hirokazu Kato
 *
 *         tmita@inolab.sys.es.osaka-u.ac.jp
 *         shinsaku@sys.es.osaka-u.ac.jp
 *         kato@sys.im.hiroshima-cu.ac.jp
 *
 * Revision: 5.0
 * Date: 03/08/13
 *
 *******************************************************/

#ifndef AR_PARAM_H
#define AR_PARAM_H

#include <AR/ar.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
    @header param
    @abstract   ARToolKit functions for handling calibrated camera parameters.
    @discussion
*/
/*!
    @defined 
    @abstract   Default version for functions accepting a "distortion function version" parameter.
    @discussion See function arParamObserv2Ideal() for discussion.
*/
#define AR_DIST_FUNCTION_VERSION_DEFAULT 4
/*!
    @defined 
    @abstract   Maximum version allowable for functions accepting a "distortion function version" parameter.
    @discussion See function arParamObserv2Ideal() for discussion.
*/
#define AR_DIST_FUNCTION_VERSION_MAX 4
/*!
    @defined 
    @abstract   Maximum number of values in a distortion factor array.
    @discussion See function arParamObserv2Ideal() for discussion.
*/
#define AR_DIST_FACTOR_NUM_MAX 9
/*!
    @defined 
    @abstract   Default padding added around a lookup-table based camera parameter.
    @discussion See function arParamLTCreate() for discussion.
*/
#define   AR_PARAM_LT_DEFAULT_OFFSET  15

/*!
    @typedef 
    @abstract   Structure holding camera parameters, including image size, projection matrix and lens distortion parameters.
    @discussion
        ARToolKit's tracking depends on accurate knowledge of the properties of the
        imaging system used to acquire input images. This structure holds the properties
        of an imaging system for internal use in ARToolKit. This information is used
        throughout the entire ARToolKit pipeline, including knowing the size of images
        being returned by the video library, marker detection and pose estimation,
        and warping of camera images for video-see-through registration.
    @field      xsize The width in pixels of images returned by arVideoGetImage() for the camera.
    @field      ysize The height in pixels of images returned by arVideoGetImage() for the camera.
    @field      mat The projection matrix for the calibrated camera parameters. This can be
        converted to an OpenGL projection matrix by the function arglCameraFrustumRHf().
    @field      dist_factor See function arParamObserv2Ideal() for discussion.
    @field      dist_function_version See function arParamObserv2Ideal() for discussion.
    @seealso    arParamClear arParamClear
    @seealso    arParamLoad arParamLoad
    @seealso    arParamSave arParamSave
 */
typedef struct {
    int      xsize;
    int      ysize;
    ARdouble mat[3][4];
    ARdouble dist_factor[AR_DIST_FACTOR_NUM_MAX];
    int      dist_function_version; // Must be last field in structure (as will not be written to disk).
} ARParam;

// Constant array with parameters applicable to each version
// of the camera parameter distortion function.
typedef struct {
	int dist_factor_num;
	int ARParam_size;
} arParamVersionInfo_t;
extern const arParamVersionInfo_t arParamVersionInfo[AR_DIST_FUNCTION_VERSION_MAX];

typedef struct {
    float   *i2o;
    float   *o2i;
    int      xsize;
    int      ysize;
    int      xOff;
    int      yOff;
} ARParamLTf;
    
//typedef struct {
//    short   *i2o;
//    short   *o2i;
//    int      xsize;
//    int      ysize;
//    int      xOff;
//    int      yOff;
//} ARParamLTi;

/*!
    @typedef 
    @abstract   Structure holding camera parameters, in lookup table form.
    @discussion
        ARToolKit's tracking depends on accurate knowledge of the properties of the
        imaging system used to acquire input images. This structure holds the properties
        of an imaging system for internal use in ARToolKit. This information is used
        throughout the entire ARToolKit pipeline, including knowing the size of images
        being returned by the video library, marker detection and pose estimation,
        and warping of camera images for video-see-through registration.

        This version of the structure contains a pre-calculated lookup table of
        values covering the camera image width and height, plus a padded border.
    @field      param A copy of original ARParam from which the lookup table was calculated.
    @field      paramLTf The lookup table.
*/
typedef struct {
    ARParam      param;
    ARParamLTf   paramLTf;
    //ARParamLTi   paramLTi;
} ARParamLT;

int    arParamDisp( const ARParam *param );

/*!
    @function
    @abstract Create a camera parameter structure representing an idealised lens.
    @discussion
        This function creates a camera parameter structure representing an idealised lens,
        that is, a lens with a symmetrical perspective projection and no distortion.
        This idealised lens is useful in cases where you wish to match the lens model
        of an OpenGL camera with the same window dimensions.
    @param param Pointer to an ARParam structure which will be filled out with
        the resulting camera parameters.
    @param xsize The horizontal dimension of the image.
    @param ysize The vertical dimension of the image.
    @param dist_function_version Allows creation of parameters with an older version
        of the lens distortion model. Pass AR_DIST_FUNCTION_VERSION_DEFAULT to create an ARParam
        with the current lens distortion model, or a lesser integer to use an earlier version.
    @result 0 if the function completed successfully, or -1 in case of error.
 */
int    arParamClear( ARParam *param, int xsize, int ysize, int dist_function_version );

int    arParamDistFactorClear( ARdouble dist_factor[AR_DIST_FACTOR_NUM_MAX], int xsize, int ysize, int dist_function_version );

int    arParamChangeSize( ARParam *source, int xsize, int ysize, ARParam *newparam );

int    arParamDecomp( const ARParam *source, ARParam *icpara, ARdouble trans[3][4] );

int    arParamDecompMat( const ARdouble source[3][4], ARdouble cpara[3][4], ARdouble trans[3][4] );

#ifdef ARDOUBLE_IS_FLOAT
#define arParamDecompMatf arParamDecompMat
#else
int    arParamDecompMatf( const ARdouble source[3][4], float cpara[3][4], float trans[3][4] );
#endif

/*!
    @function
    @abstract   Use lens distortion parameters to convert idealised (zero-distortion) window coordinates to observed (distorted) coordinates.
    @discussion
        See function arParamObserv2Ideal() for full discussion.
 
        This function is the output function of the pair. It's inputs are
        idealised coordinates, e.g. taken from OpenGL. The outputs are the
        location where in a distorted image where the same point would lie.
    @param      dist_factor An array of ARdouble values holding the lens distortion
        parameters. These values are generated as part of the camera calibration
        process in ARToolKit. The exact number of values from the array used by the
        function is determined by the distortion function version, but does not
        exceed AR_DIST_FACTOR_NUM_MAX.
    @param      ix Input idealised normalised window coordinate x axis value.
    @param      iy Input idealised normalised window coordinate y axis value.
    @param      ox Pointer to ARdouble, which on return will hold the observed normalised window coordinate x axis value.
    @param      oy Pointer to ARdouble, which on return will hold the observed normalised window coordinate y axis value.
    @param      dist_function_version An integer, in the range [1, AR_DIST_FUNCTION_VERSION_MAX] which determines the
        algorithm used to interpret the dist_factor values.
 
        See function arParamObserv2Ideal() for full discussion.
    @result     0 in case of function success, or -1 if an error occured. At
        present the only error possible is an invalid value of dist_function_version.
    @seealso arParamObserv2Ideal arParamObserv2Ideal
    @seealso arParamObserv2IdealLTf arParamObserv2IdealLTf
*/
int    arParamIdeal2Observ( const ARdouble dist_factor[AR_DIST_FACTOR_NUM_MAX], const ARdouble ix, const ARdouble iy,
                            ARdouble *ox, ARdouble *oy, const int dist_function_version );

/*!
    @function
    @abstract   Use lens distortion parameters to convert observed (distorted) window coordinates to idealised (zero-distortion) coordinates.
    @discussion
        This function is used by ARToolKit to convert for the
        the effects of lens distortion in images which have been acquired
        from lens-based optical systems. All lenses introduce some amount
        of lens distortion. ARToolKit includes calibration utilities to
        measure the centre of distortion and the radial distortion factors
        in the x and y dimensions. This calibration information is saved
        as part of the camera parameters.

        This function is one of a pair of functions which convert between
        OBSERVED window coordinates (i.e. the location of a known reference
        point, as measured in the x-y viewing plane of an image containing
        lens distortion) and IDEALISED coordinates (i.e. the location of the
        same reference point as measured in an image containing no radial
        distortion, e.g. an image rendered using OpenGL's viewing model.)
 
        This function is the input function of the pair. It's inputs are
        distorted coordinates, e.g. taken from an image acquired from a camera
        The outputs are the location where in an idealised image (e.g. generated
        with OpenGL) where the same point would lie.
 
    @param      dist_factor An array of ARdouble values holding the lens distortion
        parameters. These values are generated as part of the camera calibration
        process in ARToolKit. The exact number of values from the array used by the
        function is determined by the distortion function version, but does not
        exceed AR_DIST_FACTOR_NUM_MAX.
    @param      ix Input observed normalised window coordinate x axis value.
    @param      iy Input observed normalised window coordinate y axis value.
    @param      ox Pointer to ARdouble, which on return will hold the idealised normalised window coordinate x axis value.
    @param      oy Pointer to ARdouble, which on return will hold the idealised normalised window coordinate y axis value.
    @param      dist_function_version An integer, in the range [1, AR_DIST_FUNCTION_VERSION_MAX] which determines the
        algorithm used to interpret the dist_factor values.
 
        The values correspond to the following algorithms:
 
        version 1: The original ARToolKit lens model, with a single radial distortion factor, plus center of distortion.<br>
        version 2: Improved distortion model, introduced in ARToolKit v4.0. This algorithm adds a quadratic term to the radial distortion factor of the version 1 algorithm.<br>
        version 3: Improved distortion model with aspect ratio, introduced in ARToolKit v4.0. The addition of an aspect ratio to the version 2 algorithm allows for non-square pixels, as found e.g. in DV image streams.<br>
        version 4: OpenCV-based distortion model, introduced in ARToolKit v4.3. This differs from the standard OpenCV model by the addition of a scale factor, so that input values do not exceed the range [-1.0, 1.0] in either forward or inverse conversions.
    @result     0 in case of function success, or -1 if an error occured. At
        present the only error possible is an invalid value of dist_function_version.
    @seealso arParamIdeal2Observ arParamIdeal2Observ
    @seealso arParamIdeal2ObservLTf arParamIdeal2ObservLTf
*/
int    arParamObserv2Ideal( const ARdouble dist_factor[AR_DIST_FACTOR_NUM_MAX], const ARdouble ox, const ARdouble oy,
                            ARdouble *ix, ARdouble *iy, const int dist_function_version );
/*!
    @function
    @abstract Save lens parameters to a file.
    @discussion
        See the discussion under ARParam for more info.
    @param filename Path to file to which to save the parameters.
        The file pointed to may later be reloaded with arParamLoad.
    @param num Number of ARParams to be saved, normally 1.
    @param param Pointer to the ARParam structure to save.
    @result 0 if successful, or -1 if an error occured.
    @seealso ARParam ARParam
    @seealso arParamLoad arParamLoad
    @seealso arParamLoadFromBuffer arParamLoadFromBuffer
 */
int    arParamSave( const char *filename, const int num, const ARParam *param, ...);

/*!
    @function
    @abstract Load lens parameters from a file.
    @discussion
        See the discussion under ARParam for more info.
    @param filename Path to file from which to load the parameters.
        The file pointed to should be a file saved with arParamSave.
    @param num Number of ARParams to be loaded, normally 1.
    @param param Pointer to the ARParam structure into which the parameters will be read.
    @result 0 if successful, or -1 if an error occured.
    @seealso ARParam ARParam
    @seealso arParamLoadFromBuffer arParamLoadFromBuffer
    @seealso arParamSave arParamSave
 */
int    arParamLoad( const char *filename, int num, ARParam *param, ...);

    /*!
     @function
     @abstract Load lens parameters from a buffer.
     @discussion
        See the discussion under ARParam for more info.
     @param buffer Buffer from which the parameter(s) will be loaded.
        The buffer could be (for example) the contents of a parameter file read with fread().
     @param bufsize Size of the contents of buffer.
     @param param Pointer to the ARParam structure into which the parameters will be read.
     @result 0 if successful, or -1 if an error occured.
     @seealso ARParam ARParam
     @seealso arParamLoad arParamLoad
     @seealso arParamSave arParamSave
     */
int    arParamLoadFromBuffer( const void *buffer, size_t bufsize, ARParam *param);
    
int    arParamGetPerspectiveMat( ARdouble global[][3], ARdouble idealScreen[][2], int data_num, ARdouble mat[3][4] );

int    arParamSaveExt( const char *filename, ARdouble para[3][4] );
int    arParamLoadExt( const char *filename, ARdouble para[3][4] );
int    arParamLoadExtFromBuffer(const void *buffer, size_t bufsize, ARdouble para[3][4] );
int    arParamDispExt( ARdouble para[3][4]);

int arParamSaveOptical(const char *filename, const ARdouble fovy, const ARdouble aspect, const ARdouble m[16]);
int arParamLoadOptical(const char *filename, ARdouble *fovy_p, ARdouble *aspect_p, ARdouble m[16]);
int arParamLoadOpticalFromBuffer(const void *buffer, size_t bufsize, ARdouble *fovy_p, ARdouble *aspect_p, ARdouble m[16]);
int arParamDispOptical(const ARdouble fovy, const ARdouble aspect, const ARdouble m[16]);

int         arParamLTSave( char *filename, char *ext, ARParamLT *paramLT );
ARParamLT  *arParamLTLoad( char *filename, char *ext );

/*!
    @function
    @abstract Allocate and calculate a lookup-table camera parameter from a standard camera parameter.
    @discussion A lookup-table based camera parameter offers significant performance
        savings in certain ARToolKit operations (including unwarping of pattern spaces)
        compared to use of the standard camera parameter.
    
        The original ARParam camera parameters structure is copied into the ARParamLT
        structure, and is available as paramLT->param.
    @param param A pointer to an ARParam structure from which the lookup table will be generaeted.
        This ARParam structure will be copied, and the original may be disposed of.
    @param offset An integer value which specifies how much the lookup table values will be
        padded around the original camera parameters size. Normally, the default value
        AR_PARAM_LT_DEFAULT_OFFSET (defined elsewhere in this header) should be used. However,
        when using a camera with a large amount of lens distortion, a higher value may be
        required to cope with the corners or sides of the camera image frame.
    @result A pointer to a newly-allocated ARParamLT structure, or NULL if an error
        occurred. Once the ARParamLT is no longer needed, it should be disposed
        of by calling arParamLTFree() on it.
    @seealso arParamLTFree arParamLTFree
 */
ARParamLT  *arParamLTCreate( ARParam *param, int offset );

/*!
    @function
    @abstract Dispose of a memory allocated to a lookup-table camera parameter.
    @discussion
    @param paramLT_p Pointer to a pointer to the paramLT structure to be disposed of.
        On return, the location pointed to will be set to NULL.
    @result -1 if an error occurred, or 0 in the case of no error.
    @seealso arParamLTCreate arParamLTCreate
 */
int         arParamLTFree( ARParamLT **paramLT_p );

/*!
    @function
    @abstract   Use a lookup-table camera parameter to convert idealised (zero-distortion) window coordinates to observed (distorted) coordinates.
    @discussion
        See function arParamObserv2IdealLTf() for full discussion.
 
        This function is the output function of the pair. It's inputs are
        idealised coordinates, e.g. taken from OpenGL. The outputs are the
        location where in a distorted image where the same point would lie.
    @param      paramLTf A lookup-table based version of the lens distortion
        parameters. These values are generated as part of the camera calibration
        process in ARToolKit, and converted to a lookup table by arParamLTCreate().
    @param      ix Input idealised normalised window coordinate x axis value.
    @param      iy Input idealised normalised window coordinate y axis value.
    @param      ox Pointer to ARdouble, which on return will hold the observed normalised window coordinate x axis value.
    @param      oy Pointer to ARdouble, which on return will hold the observed normalised window coordinate y axis value.
    @result     0 in case of function success, or -1 if an error occured. One possible
        error condition is the case where the input portion of the pair is outside the
        range of coordinates covered by the lookup table.
    @seealso arParamLTCreate arParamLTCreate
    @seealso arParamIdeal2Observ arParamIdeal2Observ
    @seealso arParamObserv2IdealLTf arParamObserv2IdealLTf
*/

int         arParamIdeal2ObservLTf( const ARParamLTf *paramLTf, const float  ix, const float  iy, float  *ox, float  *oy);

    
/*!
    @function
    @abstract   Use a lookup-table camera parameter to convert observed (distorted) window coordinates to idealised (zero-distortion) coordinates.
    @discussion
        This function is used by ARToolKit to convert for the
        the effects of lens distortion in images which have been acquired
        from lens-based optical systems. All lenses introduce some amount
        of lens distortion. ARToolKit includes calibration utilities to
        measure the centre of distortion and the radial distortion factors
        in the x and y dimensions. This calibration information is saved
        as part of the camera parameters.

        This function is one of a pair of functions which convert between
        OBSERVED window coordinates (i.e. the location of a known reference
        point, as measured in the x-y viewing plane of an image containing
        lens distortion) and IDEALISED coordinates (i.e. the location of the
        same reference point as measured in an image containing no radial
        distortion, e.g. an image rendered using OpenGL's viewing model.)
 
        This function is the input function of the pair. It's inputs are
        distorted coordinates, e.g. taken from an image acquired from a camera
        The outputs are the location where in an idealised image (e.g. generated
        with OpenGL) where the same point would lie.
 
    @param      paramLTf A lookup-table based version of the lens distortion
        parameters. These values are generated as part of the camera calibration
        process in ARToolKit, and converted to a lookup table by arParamLTCreate().
    @param      ix Input observed normalised window coordinate x axis value.
    @param      iy Input observed normalised window coordinate y axis value.
    @param      ox Pointer to ARdouble, which on return will hold the idealised normalised window coordinate x axis value.
    @param      oy Pointer to ARdouble, which on return will hold the idealised normalised window coordinate y axis value.
    @result     0 in case of function success, or -1 if an error occured. One possible
        error condition is the case where the input portion of the pair is outside the
        range of coordinates covered by the lookup table.
    @seealso arParamObserv2Ideal arParamObserv2Ideal
    @seealso arParamIdeal2ObservLTf arParamIdeal2ObservLTf
*/
int         arParamObserv2IdealLTf( const ARParamLTf *paramLTf, const float  ox, const float  oy, float  *ix, float  *iy);

//int         arParamIdeal2ObservLTi( const ARParamLTi *paramLTi, const int    ix, const int    iy, int    *ox, int    *oy);

//int         arParamObserv2IdealLTi( const ARParamLTi *paramLTi, const int    ox, const int    oy, int    *ix, int    *iy);

#ifdef __cplusplus
}
#endif
#endif
