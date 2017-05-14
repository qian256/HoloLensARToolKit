/*
*  ARUWPNative.cs
*  ARToolKitUWP-Unity
*
*  This file is a part of ARToolKitUWP-Unity.
*
*  ARToolKitUWP-Unity is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  ARToolKitUWP-Unity is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public License
*  along with ARToolKitUWP-Unity.  If not, see <http://www.gnu.org/licenses/>.
*
*  Copyright 2017 Long Qian
*
*  Author: Long Qian
*  Contact: lqian8@jhu.edu
*
*/

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;
using System.Text;
using System;

/// <summary>
/// The ARUWP class interfaces native plugin of ARToolKitUWP.dll (unmanaged),
/// including function access point, parameter marshaling, and ARToolKit 
/// constant definitions.
/// The detailed implementations are in the Visual Studio project of ARToolKitUWP
/// library.
/// These functions should be considered internal use only. There is generally no
/// need for users to call these functions directly, since these functionalities
/// are wrapped up in ARUWPController.cs or ARUWPMarker.cs.
///
/// Author:     Long Qian
/// Email:      lqian8@jhu.edu
/// </summary>
public static class ARUWP {

    #region Log support for messages from ARToolKitUWP.dll

    /// <summary>
    /// Declare the LogCallback delegate. [internal use]
    /// </summary>
    /// <param name="msg">The input string object</param>
    public delegate void LogCallback([MarshalAs(UnmanagedType.LPStr)] string msg);

    /// <summary>
    /// Pre-defined LogCallback instance. [internal use]
    /// </summary>
    private static LogCallback logCallback = null;

    /// <summary>
    /// The wrapper for ARUWP.aruwpRegisterLogCallback() function, register the callback function. [internal use]
    /// </summary>
    /// <param name="lcb">LogCallback instance</param>
    public static void aruwpRegisterLogCallbackWrapper(LogCallback lcb) {
        GCHandle logCallbackGCH = new GCHandle();
        if (lcb != null) {
            logCallback = lcb;
            logCallbackGCH = GCHandle.Alloc(logCallback); // Does not need to be pinned, see http://stackoverflow.com/a/19866119/316487 
        }
        ARUWP.aruwpRegisterLogCallback(logCallback);
        if (lcb == null) {
            logCallback = null;
            logCallbackGCH.Free();
        }
    }

    /// <summary>
    /// User-defined behavior that executes with new log messages from the native library. [public use]
    /// </summary>
    public static Action<String> logCallbackAct { get; set; }

    /// <summary>
    /// Pre-defined LogCallback instance: if logCallbackAct is set, use it for customized behavior,
    /// otherwise use Debug.Log to output it to the VS console. [internal use]
    /// </summary>
    /// <param name="msg">Input string object</param>
    public static void Log(String msg) {
        // If there is a logCallback then use that to handle the log message. Otherwise simply
        // print out on the debug console.
        if (logCallbackAct != null) logCallbackAct(msg);
        else Debug.Log(msg);
    }
    #endregion


    #region Marshalled functions from ARToolKitUWP.dll

    [DllImport("ARToolKitUWP.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void aruwpRegisterLogCallback(LogCallback callback);

    [DllImport("ARToolKitUWP.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void aruwpSetLogLevel(int logLevel);

    [DllImport("ARToolKitUWP.dll")]
    [return: MarshalAs(UnmanagedType.I1)]
    public static extern bool aruwpInitialiseAR(int width, int height, int pixelFormat);

    [DllImport("ARToolKitUWP")]
    [return: MarshalAsAttribute(UnmanagedType.I1)]
    public static extern bool aruwpInitialiseARWithOptions(int width, int height, int pixelFormat, int pattSize, int pattCountMax);

    [DllImport("ARToolKitUWP")]
    [return: MarshalAsAttribute(UnmanagedType.I1)]
    public static extern bool aruwpGetARToolKitVersion([MarshalAs(UnmanagedType.LPStr)]StringBuilder buffer, int length);

    [DllImport("ARToolKitUWP", CharSet = CharSet.Ansi)]
    [return: MarshalAsAttribute(UnmanagedType.I1)]
    public static extern bool aruwpStartRunningBuffer(byte[] cparaBuff, int cparaBuffLen);

    [DllImport("ARToolKitUWP", CharSet = CharSet.Ansi)]
    [return: MarshalAsAttribute(UnmanagedType.I1)]
    public static extern bool aruwpStartRunning([MarshalAs(UnmanagedType.LPStr)] string lpString);

    [DllImport("ARToolKitUWP")]
    [return: MarshalAsAttribute(UnmanagedType.I1)]
    public static extern bool aruwpIsRunning();

    [DllImport("ARToolKitUWP")]
    [return: MarshalAsAttribute(UnmanagedType.I1)]
    public static extern bool aruwpStopRunning();

    [DllImport("ARToolKitUWP")]
    [return: MarshalAsAttribute(UnmanagedType.I1)]
    public static extern bool aruwpShutdownAR();

    [DllImport("ARToolKitUWP")]
    [return: MarshalAsAttribute(UnmanagedType.I1)]
    public static extern bool aruwpGetFrameParams(out int width, out int height, out int pixelSize, [MarshalAs(UnmanagedType.LPStr)]StringBuilder pixelFormatBuffer, int pixelFormatBufferLen); //todo
    
    [DllImport("ARToolKitUWP")]
    [return: MarshalAsAttribute(UnmanagedType.I1)]
    public static extern bool aruwpUpdate(IntPtr frame);

    [DllImport("ARToolKitUWP")]
    public static extern void aruwpSetVideoThreshold(int threshold);

    [DllImport("ARToolKitUWP")]
    public static extern int aruwpGetVideoThreshold();

    [DllImport("ARToolKitUWP")]
    public static extern void aruwpSetVideoThresholdMode(int mode);

    [DllImport("ARToolKitUWP")]
    public static extern int aruwpGetVideoThresholdMode();

    [DllImport("ARToolKitUWP")]
    public static extern void aruwpSetLabelingMode(int mode);

    [DllImport("ARToolKitUWP")]
    public static extern int aruwpGetLabelingMode();

    [DllImport("ARToolKitUWP")]
    public static extern void aruwpSetPatternDetectionMode(int mode);

    [DllImport("ARToolKitUWP")]
    public static extern int aruwpGetPatternDetectionMode();

    [DllImport("ARToolKitUWP")]
    public static extern void aruwpSetBorderSize(float mode);

    [DllImport("ARToolKitUWP")]
    public static extern float aruwpGetBorderSize();

    [DllImport("ARToolKitUWP")]
    public static extern void aruwpSetMatrixCodeType(int type);

    [DllImport("ARToolKitUWP")]
    public static extern int aruwpGetMatrixCodeType();

    [DllImport("ARToolKitUWP")]
    public static extern void aruwpSetImageProcMode(int mode);

    [DllImport("ARToolKitUWP")]
    public static extern int aruwpGetImageProcMode();

    [DllImport("ARToolKitUWP.dll", CharSet = CharSet.Ansi)]
    public static extern int aruwpAddMarker([MarshalAs(UnmanagedType.LPStr)] string lpString);

    [DllImport("ARToolKitUWP")]
    [return: MarshalAsAttribute(UnmanagedType.I1)]
    public static extern bool aruwpRemoveMarker(int markerID);

    [DllImport("ARToolKitUWP")]
    public static extern int aruwpRemoveAllMarkers();

    [DllImport("ARToolKitUWP")]
    [return: MarshalAsAttribute(UnmanagedType.I1)]
    public static extern bool aruwpQueryMarkerVisibility(int markerID);

    [DllImport("ARToolKitUWP")]
    [return: MarshalAsAttribute(UnmanagedType.I1)]
    public static extern bool aruwpQueryMarkerTransformation(int markerID, [MarshalAs(UnmanagedType.LPArray, SizeConst = 16)] float[] matrix);
    
    [DllImport("ARToolKitUWP")]
    public static extern int aruwpGetMarkerPatternCount(int markerID);

    [DllImport("ARToolKitUWP")]
    [return: MarshalAsAttribute(UnmanagedType.I1)]
    public static extern bool aruwpGetMarkerPatternConfig(int markerID, int patternID, [MarshalAs(UnmanagedType.LPArray, SizeConst = 16)] float[] matrix, out float width, out float height, out int imageSizeX, out int imageSizeY);

    [DllImport("ARToolKitUWP")]
    [return: MarshalAsAttribute(UnmanagedType.I1)]
    public static extern bool aruwpGetMarkerOptionBool(int markerID, int option);

    // bool parsed as integer
    [DllImport("ARToolKitUWP")]
    public static extern void aruwpSetMarkerOptionBool(int markerID, int option, int value);

    [DllImport("ARToolKitUWP")]
    public static extern int aruwpGetMarkerOptionInt(int markerID, int option);

    [DllImport("ARToolKitUWP")]
    public static extern void aruwpSetMarkerOptionInt(int markerID, int option, int value);

    [DllImport("ARToolKitUWP")]
    public static extern float aruwpGetMarkerOptionFloat(int markerID, int option);

    [DllImport("ARToolKitUWP")]
    public static extern void aruwpSetMarkerOptionFloat(int markerID, int option, float value);

    #endregion


    #region Constants declaration that used across the project
    public const int ARUWP_MARKER_OPTION_FILTERED = 1;                         // bool, true for filtering enabled.
	public const int ARUWP_MARKER_OPTION_FILTER_SAMPLE_RATE = 2;               // float, sample rate for filter calculations.
	public const int ARUWP_MARKER_OPTION_FILTER_CUTOFF_FREQ = 3;               // float, cutoff frequency of filter.
	public const int ARUWP_MARKER_OPTION_SQUARE_USE_CONT_POSE_ESTIMATION = 4;  // bool, true to use continuous pose estimate.
	public const int ARUWP_MARKER_OPTION_SQUARE_CONFIDENCE = 5;                // float, confidence value of most recent marker match
	public const int ARUWP_MARKER_OPTION_SQUARE_CONFIDENCE_CUTOFF = 6;         // float, minimum allowable confidence value used in marker matching.
	public const int ARUWP_MARKER_OPTION_MULTI_MIN_SUBMARKERS = 8;             // int, minimum number of submarkers for tracking to be valid.
	public const int ARUWP_MARKER_OPTION_MULTI_MIN_CONF_MATRIX = 9;            // float, minimum confidence value for submarker matrix tracking to be valid.
	public const int ARUWP_MARKER_OPTION_MULTI_MIN_CONF_PATTERN = 10;          // float, minimum confidence value for submarker pattern tracking to be valid.

    

    public const int AR_LABELING_THRESH_MODE_MANUAL = 0;           // Uses a fixed threshold value
    public const int AR_LABELING_THRESH_MODE_AUTO_MEDIAN = 1;      // Automatically adjusts threshold to whole-image median
    public const int AR_LABELING_THRESH_MODE_AUTO_OTSU = 2;        // Automatically adjusts threshold using Otsu's method for foreground/background determination
    public const int AR_LABELING_THRESH_MODE_AUTO_ADAPTIVE = 3;    // Uses adaptive dynamic thresholding (warning: computationally expensive)
    public const int AR_LABELING_THRESH_MODE_AUTO_BRACKETING = 4;  // Automatically adjusts threshold using bracketed threshold values

    public const int AR_LABELING_WHITE_REGION = 0;
    public const int AR_LABELING_BLACK_REGION = 1;




    public const int AR_TEMPLATE_MATCHING_COLOR = 0;
    public const int AR_TEMPLATE_MATCHING_MONO = 1;
    public const int AR_MATRIX_CODE_DETECTION = 2;
    public const int AR_TEMPLATE_MATCHING_COLOR_AND_MATRIX = 3;
    public const int AR_TEMPLATE_MATCHING_MONO_AND_MATRIX = 4;


    public const int AR_MATRIX_CODE_3x3 = 0x03;
    public const int AR_MATRIX_CODE_3x3_PARITY65 = 0x03 | 0x100;
    public const int AR_MATRIX_CODE_3x3_HAMMING63 = 0x03 | 0x200;
    public const int AR_MATRIX_CODE_4x4 = 0x04;
    public const int AR_MATRIX_CODE_4x4_BCH_13_9_3 = 0x04 | 0x300;
    public const int AR_MATRIX_CODE_4x4_BCH_13_5_5 = 0x04 | 0x400;



    public const int AR_IMAGE_PROC_FRAME_IMAGE = 0;
    public const int AR_IMAGE_PROC_FIELD_IMAGE = 1;

    

    public const int AR_PIXEL_FORMAT_INVALID = -1;
    public const int AR_PIXEL_FORMAT_RGB = 0;
    public const int AR_PIXEL_FORMAT_BGR = 1;
    public const int AR_PIXEL_FORMAT_RGBA = 2;
    public const int AR_PIXEL_FORMAT_BGRA = 3;
    public const int AR_PIXEL_FORMAT_ABGR = 4;
    public const int AR_PIXEL_FORMAT_MONO = 5;
    public const int AR_PIXEL_FORMAT_ARGB = 6;
    public const int AR_PIXEL_FORMAT_2vuy = 7;
    public const int AR_PIXEL_FORMAT_yuvs = 8;
    public const int AR_PIXEL_FORMAT_RGB_565 = 10;
    public const int AR_PIXEL_FORMAT_RGBA_5551 = 11;
    public const int AR_PIXEL_FORMAT_RGBA_4444 = 12;
    public const int AR_PIXEL_FORMAT_420v = 13;
    public const int AR_PIXEL_FORMAT_420f = 14;
    public const int AR_PIXEL_FORMAT_NV2 = 15;


    public const int ARUWP_STATUS_CLEAN = 0;                    // No initialisation yet and no resources allocated.
    public const int ARUWP_STATUS_VIDEO_INITIALIZED = 1;        // Video preview is initialized
    public const int ARUWP_STATUS_CTRL_INITIALIZED = 2;         // Controller context is initialized, markers can be added
    public const int ARUWP_STATUS_RUNNING = 3;                  // Running

    #endregion

}
