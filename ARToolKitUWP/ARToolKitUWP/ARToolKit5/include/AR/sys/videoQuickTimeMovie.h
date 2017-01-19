/*
 *	videoQuickTimeMovie.h
 *  ARToolKit5
 *
 *  QuickTime movie playing routines.
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
 *  Copyright 2013-2015 ARToolworks, Inc.
 *  Copyright 2007-2013 Philip Lamb.
 *
 *  Author(s): Philip Lamb
 *
 *	Rev		Date		Who		Changes
 *	1.0.0	2007-01-12	PRL		Initial version.
 */
/*!
	@header videoQuickTimeMovie
	@abstract QuickTime movie playing routines.
	@version 5.2
	@updated 2015-05-05
	@discussion
		Mac OS, Mac OS X, and QuickTime for Windows are supported.
		On Windows, link against qtmlClient.lib
	@copyright 2015 Daqri, LLC
 */

#ifndef __videoQuickTimeMovie_h__
#define __videoQuickTimeMovie_h__

#if defined(__cplusplus)
extern "C" {
#endif

// ============================================================================
//	Includes
// ============================================================================
	
// ============================================================================
//	Public types.
// ============================================================================

typedef struct _AR_VIDEO_QUICKTIME_MOVIE_t *AR_VIDEO_QUICKTIME_MOVIE_t;

/*!
    @enum AR_E_*
	@abstract Symbolic codes for errors reported by routines in the AR library.
	@discussion
		As the actual numerical values for certain errors may change from time
		to time, these symbolic codes should be used instead.
	@constant AR_E_NONE No error.
	@constant AR_E_OUT_OF_MEMORY Out of memory.
	@constant AR_E_OVERFLOW Overflow.
	@constant AR_E_NODATA Data was requested but none was available.
	@constant AR_E_IOERROR Error during input / output operation.
	@constant AR_E_EOF End of file reached.
	@constant AR_E_TIMEOUT Timed out.
	@constant AR_E_INVALID_COMMAND Invalid command.
	@constant AR_E_INVALID_ENUM Invalid enumeration.
	@constant AR_E_THREADS An error occured during a thread-management operation.
	@constant AR_E_FILE_NOT_FOUND File not found.
	@constant AR_E_LENGTH_UNAVAILABLE Length not available.
	@constant AR_E_GENERIC Error.
	@constant AR_E_LIBRARY_NOT_AVAILABLE A required library is not installed.
	@constant AR_E_LIBRARY_TOO_OLD The minimum version requirement of a library was not met.
	@constant AR_E_LIBRARY_TOO_NEW The maximum version requirement of a library was not met.
	@constant AR_E_GENERIC_TOOLBOX Error returned from System tool.
	@constant AR_E_HARDWARE_NOT_AVAILABLE Required hardware is not available.
	@constant AR_E_BIRD_CONFIGURATION The bird hardware is incorrectly configured.
	@constant AR_E_BIRD_PHASEERROR Data from the bird was lost and arrived from the bird out-of-phase.
	@constant AR_E_HARDWARE_GENERIC Hardware error.
	@constant AR_E_NET_NOT_AVAILABLE Network not available.
	@constant AR_E_NET_NOT_CONNECTED Network not connected.
	@constant AR_E_NET_GENERIC Network error.
 */
enum {
	AR_E_NONE					=    0,
	AR_E_OUT_OF_MEMORY			=   -1,
	AR_E_OVERFLOW				=   -2,
	AR_E_NODATA					=   -3,
	AR_E_IOERROR				=   -4,
	AR_E_EOF					=	-6,
	AR_E_TIMEOUT				=   -7,
	AR_E_INVALID_COMMAND		=   -8,
	AR_E_INVALID_ENUM			=   -9,
	AR_E_THREADS				=   -10,
	AR_E_FILE_NOT_FOUND			=   -11,
	AR_E_LENGTH_UNAVAILABLE		=	-12,
	AR_E_GENERIC				=   -9999,
	AR_E_LIBRARY_NOT_AVAILABLE =	-10001,
	AR_E_LIBRARY_TOO_OLD		=	-10002,
	AR_E_LIBRARY_TOO_NEW		=	-10003,
	AR_E_GENERIC_TOOLBOX		=   -10999,
	AR_E_HARDWARE_NOT_AVAILABLE =	-11001,
	AR_E_HARDWARE_GENERIC		=   -11999,
	AR_E_NET_NOT_AVAILABLE		=	-12001,
	AR_E_NET_NOT_CONNECTED		=   -12002,
	AR_E_NET_GENERIC			=   -12999,
};
typedef int AR_E_t;

// ============================================================================
//	Public globals.
// ============================================================================

// ============================================================================
//	Public functions.
// ============================================================================

/*!
    @function 
    @abstract   Initialise QuickTime.
    @discussion This function load the QuickTime libraries.
		It must be called once per application, before any other
		arVideoQuickTimeMovie*() functions are called.
    @result		AR_E_NONE if initialisation occured without error,
		or an error code otherwise. Notable error codes include
		AR_E_LIBRARY_NOT_AVAILABLE if QuickTime is not installed,
		and AR_E_LIBRARY_TOO_OLD if the version of QuickTime
		installed is too old.
*/
	
AR_DLL_API AR_E_t arVideoQuickTimeMovieInit(void);

/*!
    @function 
    @abstract   Open and play any type of movie readable by QuickTime.
    @discussion (description)
    @param      URL
		This parameter is the full specification of the movie file
		or stream to be played. It must be specified in URL form,
        beginning with the characters "file://", "http://", "https://",
		"rtsp://" or "ftp://". QuickTime's URL handler will be used to
		attempt to open the movie file from the URL. Take care to
		observe correct URL encoding, e.g. encoding spaces in the name
		as "%20".
 
        If you wish to specify the path to a local file as a file://
        URL, you can use the convenience method arUtilGetFileURI.
	@param      conf
		This optional parameter points to a C-string specifying
		options describing the way in which to play the movie. Options
		should be separated by spaces. The following options are
		understood:<pre>
		-1to1
			Do not fit the movie to the window or buffer size, but
			instead display it at its original size (1 to 1 scaling).
		-fill
			Rather than fitting the movie into the window so that all
			the movie is visible, scale it so that it completely fills
			the window or buffer. The movie's aspect ratio will be
			maintained, and this may result in some of the movie
			being clipped at the top and bottom or left and right.
		-stretch
			Rather than fitting the movie into the window so that all
			the movie is visible, scale it so that it completely fills
			the window or buffer. The movie's aspect ratio will be
			stretched if necessary so that no pixels are clipped.
		-loop
			Request that the movie loop continuously. This
			parameter has no effect for streaming movies. Unless
			this parameter is specified, calls to arVideoQuickTimeMovieIdle()
			will return AR_E_EOF when the movie has finished playing.
		-showcontroller
			Show the QuickTime movie controller in the frame. Unless
			running in offscreen mode, the user will be able to
			interact with the controller to pause and jog the movie.
		-mute
			Set movie audio volume to 0.
		-pause
			Open movie in paused state. A call to videoQuickTimeMovieSetPlayRate()
			would be required to unpause the movie.
		-width=w
			Scale movie native frame to width w.
		-height=h
			Scale movie native frame to height h.
		-fliph
			Flip movie frame horizontally.
		-flipv
			Flip movie frame vertically.
		-offscreen
			Don't display the movie onscreen in a full-screen window,
			but instead render into an offscreen pixel buffer, which
			may then be retrieved by calling arVideoQuickTimeMovieGetFrame().
		-pixelformat=cccc
			Ignored unless -offscreen is also passed, requests return of
			movie frames with pixels in format cccc, where cccc is either a
			numeric pixel format number or a valid 4-character-code for a
			pixel format. The following values are supported:
			32, BGRA, RGBA, ABGR, 24, 24BG, 2vuy, yuvs.
			(See http://developer.apple.com/quicktime/icefloe/dispatch020.html.)
		-singlebuffer
			Ignored unless -offscreen is also passed, use single buffering of
			captured movie instead of triple-buffering.

			</pre>
		If you only wish to resume playback of a movie that has been paused,
		use arVideoQuickTimeMovieSetPlayRate(movie, 1.0f); instead.
	@param movie_p Pointer to a AR_VIDEO_QUICKTIME_MOVIE_t value, which will be
		filled in with the value on successful return from the function.
		This value should then be passed to other arVideoQuickTimeMovie*() functions
		to identify the movie.
	@result     AR_E_NONE in case of successful opening, or an error code
		less than 0 otherwise. Notable error codes to watch for include
		AR_E_FILE_NOT_FOUND if the pathname or URL could not be found.
*/
AR_DLL_API AR_E_t arVideoQuickTimeMoviePlay(char *URL, char *conf, AR_VIDEO_QUICKTIME_MOVIE_t *movie_p);

/*!
    @function 
    @abstract   Allocate processing time to a playing movie, and check for end of movie.
    @discussion
		In order to grant processor time to a playing movie, this function
		must be called at least as often as the frame rate of the movie
		while the movie is playing.
		This function also checks for the end of the movie (if the movie is
		not being looped.)
    @param      movie Movie to be granted processing time.
    @result     If the movie has finished playing, the result code AR_E_EOF
		will be returned, and the calling program can then perform any
		housekeeping needed before calling arVideoQuickTimeMovieStop() to
		stop and unload the movie. If no error, AR_E_NONE is returned.
*/
AR_DLL_API AR_E_t arVideoQuickTimeMovieIdle(AR_VIDEO_QUICKTIME_MOVIE_t movie);

/*!
    @function 
    @abstract   Set the playback rate of a movie, and pause/resume/fast forward/rewind.
    @discussion
		The playback rate of a movie may be continuously varied.
		This function sets the rate.
		Some useful rates include 0.0f (pause), 1.0f (playback at normal rate), -1.0f
		(play backwards), 2.0f (fast forward at 2x normal rate).
    @param      movie Movie to have its rate set.
	@param		rate Floating point value indicating the rate to set.
	@result     AR_E_NONE in case of no error, or an error code.
*/
AR_DLL_API AR_E_t arVideoQuickTimeMovieSetPlayRate(AR_VIDEO_QUICKTIME_MOVIE_t movie, float rate);

/*!
    @function 
    @abstract   Get the playback rate of a movie.
    @discussion
		The playback rate of a movie may be continuously varied.
		This function gets the rate.
		Some interesting rates include 0.0f (pause), 1.0f (playback at normal rate).
	@param      movie Movie to have its rate retrieved.
	@param      rate Pointer to a float, which will be filled out with the rate.
	@result     AR_E_NONE in case of no error, or an error code.
*/
AR_DLL_API AR_E_t arVideoQuickTimeMovieGetPlayRate(AR_VIDEO_QUICKTIME_MOVIE_t movie, float *rate);

/*!
    @function 
    @abstract   Set the playback volume of a movie.
    @discussion
		The volume of a movie sound tracks (if any) may be continuously varied.
		This function sets the volume.
		Some useful volumes include 0.0f (muted), 1.0f (full volume).
	@param      movie Movie to have its volume set.
	@param		rate Floating point value indicating the volume to set.
	@result     AR_E_NONE in case of no error, or an error code.
*/
AR_DLL_API AR_E_t arVideoQuickTimeMovieSetPlayVolume(AR_VIDEO_QUICKTIME_MOVIE_t movie, float volume);

/*!
    @function 
    @abstract   (description)
	@discussion
		The volume of a movie sound tracks (if any) may be continuously varied.
		This function gets the volume.
		Some interesting volumes include 0.0f (muted), 1.0f (full volume).
	@param      movie Movie to have its volume retrieved.
	@param      rate Pointer to a float, which will be filled out with the volume.
	@result     AR_E_NONE in case of no error, or an error code.
*/
AR_DLL_API AR_E_t arVideoQuickTimeMovieGetPlayVolume(AR_VIDEO_QUICKTIME_MOVIE_t movie, float *volume);

/*!
    @function 
    @abstract   Determine the length of a movie in seconds (if possible).
    @discussion
		This function determines the length in seconds of a playing movie.
		Some movies, e.g. streaming movies, QuickTime VR movies, and interactive
		movies, may not have a valid length, so be sure to check the function return value.
	@param      movie Movie to have its length retrieved.
	@param      rate Pointer to a double, which will be filled out with the length.
    @result     AR_E_NONE if the length was able to be retrieved, or AR_E_LENGTH_UNAVAILABLE
		if the movie does not have a valid length, or an error code < 0 otherwise.
*/
AR_DLL_API AR_E_t arVideoQuickTimeMovieGetPlayLength(AR_VIDEO_QUICKTIME_MOVIE_t movie, double *lengthInSeconds);
	
/*!
    @function 
    @abstract   Get the current movie playback position in seconds.
    @discussion
		While a movie is playing (or paused etc), you can use this function to
		determine how far through the movie has played.
	@param      movie Movie to have its position retrieved.
    @param      positionInSeconds Pointer to a double, which will be filled out with
		the current position of the playback head, in seconds since start.
	@result     AR_E_NONE if the position was able to be retrieved, or AR_E_LENGTH_UNAVAILABLE
		if the movie does not have a valid length, or an error code < 0 otherwise.
*/
AR_DLL_API AR_E_t arVideoQuickTimeMovieGetPlayPosition(AR_VIDEO_QUICKTIME_MOVIE_t movie, double *positionInSeconds);

/*!
    @function 
    @abstract   Set the current movie playback position in seconds.
    @discussion
		A playing movie can have its playback position set non-linearly.
		Use this function to position the playback head at the specified
		number of seconds after the movie start.
		Some movies, e.g. streaming movies, QuickTime VR movies, and interactive
		movies, may not have a valid length, in which case the movie
		playback position cannot be adjusted.
		Adjusting the movie playback position does not change the movie's
		rate (e.g. whether it is paused, running at normal speed etc.)
	@param      movie Movie to have its position set.
    @param      positionInSeconds Number of seconds since movie start to
		move the playback head to.
	@result     AR_E_NONE if the position was able to be set, or AR_E_LENGTH_UNAVAILABLE
		if the movie does not have a valid length, or AR_E_OVERFLOW if the position
		specified was outside the movie's valid position range, or an error code < 0 otherwise.
*/
AR_DLL_API AR_E_t arVideoQuickTimeMovieSetPlayPosition(AR_VIDEO_QUICKTIME_MOVIE_t movie, double positionInSeconds);

AR_DLL_API AR_E_t arVideoQuickTimeMovieSetPlayPositionNextFrame(AR_VIDEO_QUICKTIME_MOVIE_t movie);

AR_DLL_API int arVideoQuickTimeMovieIsPlayingEveryFrame(AR_VIDEO_QUICKTIME_MOVIE_t movie);

/*!
    @function 
	@abstract   Get pixel data for one frame of a movie playing offscreen.
	@discussion
		Offscreen movies play into a offscreen pixel buffer. This function
		retrieves the most recent frame (if available) in the form of a pointer
		to an unstructured block of pixels, beginning with the first colour
		component of the top-leftmost pixel, and continuing with the remaining
		pixels in the top line of the frame, then the remaining lines of the
		frame.
 
		If no new frame is currently available, NULL will be returned.
 
		Calling this function invalidates the data pointed to by any
		previous return value of this function, thus returned pointers
		should not be cached. The calling routine will have exclusive
		access to the returned buffer, however, at least until the next call
		to this function.
 
		The dimensions of the returned buffer (width, height and depth) are
		retrieved by calling arVideoQuickTimeMovieGetFrameSize().
	@param       movie A movie playing offscreen 
		(i.e. opened with the "-offscreen" config parameter).
    @param time If supplied, will be filled with the movie frame time (in decimal seconds).
	@result     Pointer to a pixel buffer, or NULL if no new pixels are available.
 */
AR_DLL_API unsigned char *arVideoQuickTimeMovieGetFrame(AR_VIDEO_QUICKTIME_MOVIE_t movie, double *time);

/*!
    @function 
	@abstract   Get size and format information for a movie playing offscreen.
	@discussion
		Offscreen movies play into a offscreen pixel buffer. This function
		retrieves the size and pixel format of the pixel buffers returned by
		calls to arVideoQuickTimeMovieGetFrame().
 
		The total size of the buffer in bytes = width * height * pixelSize.
	@param      width
		Pointer to an unsigned integer, which will be filled out with the width
		in pixels of the pixel buffer. You may pass NULL if you do not need the
		width.
	@param      height (description)
		Pointer to an unsigned integer, which will be filled out with the height
		in pixels of the pixel buffer. You may pass NULL if you do not need the
		height.
	@param      pixelSize (description)
		Pointer to an unsigned integer, which will be filled out with the size in
		bytes of individual pixels in the pixel buffer. E.g., ARGB pixels (the
		default on Mac OS) and BGRA pixels (the default on Windows) are 4 bytes
		in size. You may pass NULL if you do not need the
		pixelSize.
	@param      pixFormat (description)
		Pointer to an unsigned integer, which will be filled out with the a
		symbolic value corresponding to one of the AR_PIXEL_FORMAT_*
		constants defined in config.h.
		You may pass NULL if you do not need the
		pixel format.
	@result     AR_E_NONE in case of no error, or an error code.
 */
AR_DLL_API AR_E_t arVideoQuickTimeMovieGetFrameSize(AR_VIDEO_QUICKTIME_MOVIE_t movie, unsigned int *width, unsigned int *height, unsigned int *pixelSize, unsigned int *pixFormat);

/*!
    @function 
    @abstract   Stop and unload a movie.
    @discussion
		This function ceases playback of a movie. If the movie
		is playing in a fullscreen window, the window will be closed
		and removed. If the movie is playing offscreen, the offscreen
		buffers will be unloaded.
 
		If you only wish to pause a movie, use arVideoQuickTimeMovieSetPlayRate(movie, 0.0f);
		insteaad.
	@param movie_p Pointer to a AR_VIDEO_QUICKTIME_MOVIE_t value, which identifies
		the movie to be stopped and closed. On return, the contents of this
		location will be set to NULL.
	@result     AR_E_NONE in case of no error, or an error code.
*/
AR_DLL_API AR_E_t arVideoQuickTimeMovieStop(AR_VIDEO_QUICKTIME_MOVIE_t *movie_p);

/*!
    @function 
    @abstract   Finalise QuickTime.
	@discussion This function unloads the QuickTime libraries.
		It may be called once per application, after all use of
		arVideoQuickTimeMovie*() functions is complete.
 */
AR_DLL_API void arVideoQuickTimeMovieFinal(void);

#if defined(__cplusplus)
}
#endif

#endif // !__videoQuickTimeMovie_h__
