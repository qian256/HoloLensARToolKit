/*
*  ARUWPController.cs
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


using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;
using UnityEngine;
using UnityEngine.UI;

#if !UNITY_EDITOR && UNITY_METRO
using System.Threading.Tasks;
using System.Threading;
using Windows.Media.Capture.Frames;
using Windows.Graphics.Imaging;
#endif

/// <summary>
/// The ARUWPController class represents an ARToolKit controller, much similar to 
/// ARController.h and ARController.cpp in the wrapper project. It manages markers,
/// initiates camera capture, starts native tracking functions and retrieve 
/// tracking results.
/// Parameters for ARUWPController can be set in Unity inspector, and can be changed
/// via the runtime setter functions during application.
///
/// Author:     Long Qian
/// Email:      lqian8@jhu.edu
/// </summary>
public class ARUWPController : MonoBehaviour {

    /// <summary>
    /// Class and object identifier for logging. [internal use]
    /// </summary>
    private static string TAG = "ARUWPController";

    /// <summary>
    /// The status of ARUWPController is the most important parameter showing the property of the
    /// application. It is shared by all scirpts. It is safe to read it from anywhere anytime, but
    /// external changes to the status is prohibited. The value of the status can be:
    /// <para/>ARUWP_STATUS_CLEAN: Nothing has been initialized. Video capture should be initialized
    /// at this status.
    /// <para/>ARUWP_STATUS_VIDEO_INITIALIZED: The video capture has been initialized, but the 
    /// tracking has not been initialized. At this stage, markers can be added, tracking options 
    /// can be changed. The access of native library is given to ARUWPController.cs and ARUWPMarker.cs
    /// <para/>ARUWP_STATUS_CTRL_INITIALIZED: The video and tracking are both initialized, but the 
    /// video pipeline is not started yet. At this stage, the access of native library is given to
    /// ARUWPController.cs and ARUWPMarker.cs. StartFrameReaderAsync() should be called at this time.
    /// <para/>ARUWP_STATUS_RUNNING: Video pipeline is running, new frame arrives, and tracking is
    /// running. The access to the native library is given to the video thread (capture and tracking).
    /// If StopFrameReaderAsync() is called, the status can be move back to ARUWP_STATUS_CTRL_INITIALIZED.
    /// <para/>[read only]
    /// </summary>
    public int status = ARUWP.ARUWP_STATUS_CLEAN;

    /// <summary>
    /// Frame width in pixel, shared by all scripts. It is set by ARUWPVideo.cs. [read only]
    /// </summary>
    public int frameWidth = 0;

    /// <summary>
    /// Frame height in pixel, shared by all scripts. It is set by ARUWPVideo.cs. [read only]
    /// </summary>
    public int frameHeight = 0;
    

    
    /// <summary>
    /// Initail value of whether use camera parameter file or byte buffer to get camera calibration
    /// information. [public use] [initialization only]
    /// </summary>
    public bool useCameraParamFile = true;

    /// <summary>
    /// Initial value of the camera calibration filename. The file must be put under
    /// Assets/StreamingAssets/ folder. It is useful when useCameraParamFile == true. [public use] 
    /// [initialization only]
    /// </summary>
    public string cameraParam = "hololens896x504.dat";

    /// <summary>
    /// The byte buffer to hold camera calibration content. It is useful when useCameraParamFile == 
    /// false. The buffer should be set using SetCameraParamBuffer(). [internal use]
    /// </summary>
    private byte[] cameraParamBuffer = null;

    /// <summary>
    /// A Dictionary holding all the markers that are successfully added to the controller. The
    /// key is the ID of the marker, and the value is ARUWPMarker object. [internal use]
    /// </summary>
    public static Dictionary<int, ARUWPMarker> markers = new Dictionary<int, ARUWPMarker>();

    /// <summary>
    /// The array of ARUWPMarker objects in the scene before add them to the controller. It is
    /// used for initialization. [internal use]
    /// </summary>
    private ARUWPMarker[] unaddedMarkers = null;

    /// <summary>
    /// ARUWPVideo instance. [internal use]
    /// </summary>
    private ARUWPVideo videoManager = null;

    /// <summary>
    /// The threshold to make the source image black/white before detecting markers in the image.
    /// At runtime, the threshold shoule be modified using SetVideoThreshold() function. 
    /// [public use] [initialization only]
    /// </summary>
    [Range(0, 255)]
    public int threshold = 100;

    /// <summary>
    /// The bordersize of the markers in this scene, expressed in percentage. Normally it is 0.25.
    /// At runtime, it can be modified using SetBorderSize() function. [public use] 
    /// [initialization only]
    /// </summary>
    [Range(0f, 0.5f)]
    public float borderSize = 0.25f;

    /// <summary>
    /// GameObject that represents the location of the locatable camera. Whenever we get a new
    /// frame, we update the pose of this GameObject so that it is at the position of where the
    /// locatable camera was during that frame. Note that this is not the same as the "holoLensCamera"
    /// object -- the "Main Camera" of the scene is a virtual location defined by the HoloLens IMU,
    /// not by the HoloLens locatable camera. [internal use]
    /// </summary>
    public GameObject LocatableCameraRoot { get; private set; }


    #region Public Enums

    /// <summary>
    /// ThresholdMode types, same definition as ARToolKit. [public use]
    /// </summary>
    public enum ThresholdMode {
        AR_LABELING_THRESH_MODE_MANUAL = ARUWP.AR_LABELING_THRESH_MODE_MANUAL,
        AR_LABELING_THRESH_MODE_AUTO_MEDIAN = ARUWP.AR_LABELING_THRESH_MODE_AUTO_MEDIAN,
        AR_LABELING_THRESH_MODE_AUTO_OTSU = ARUWP.AR_LABELING_THRESH_MODE_AUTO_OTSU,
        AR_LABELING_THRESH_MODE_AUTO_ADAPTIVE = ARUWP.AR_LABELING_THRESH_MODE_AUTO_ADAPTIVE,
        AR_LABELING_THRESH_MODE_AUTO_BRACKETING = ARUWP.AR_LABELING_THRESH_MODE_AUTO_BRACKETING
    }

    /// <summary>
    /// LabelingMode types, same definiton as ARToolKit. [public use]
    /// </summary>
    public enum LabelingMode {
        AR_LABELING_WHITE_REGION = ARUWP.AR_LABELING_WHITE_REGION,
        AR_LABELING_BLACK_REGION = ARUWP.AR_LABELING_BLACK_REGION
    }

    /// <summary>
    /// PatternDetectionMode types, same definition as ARToolKit. [public use]
    /// </summary>
    public enum PatternDetectionMode {
        AR_TEMPLATE_MATCHING_COLOR = ARUWP.AR_TEMPLATE_MATCHING_COLOR,
        AR_TEMPLATE_MATCHING_MONO = ARUWP.AR_TEMPLATE_MATCHING_MONO,
        AR_MATRIX_CODE_DETECTION = ARUWP.AR_MATRIX_CODE_DETECTION,
        AR_TEMPLATE_MATCHING_COLOR_AND_MATRIX = ARUWP.AR_TEMPLATE_MATCHING_COLOR_AND_MATRIX,
        AR_TEMPLATE_MATCHING_MONO_AND_MATRIX = ARUWP.AR_TEMPLATE_MATCHING_MONO_AND_MATRIX
    }

    /// <summary>
    /// MatrixCodeType types, same definition as ARToolKit. [public use]
    /// </summary>
    public enum MatrixCodeType {
        AR_MATRIX_CODE_3x3 = ARUWP.AR_MATRIX_CODE_3x3,
        AR_MATRIX_CODE_3x3_PARITY65 = ARUWP.AR_MATRIX_CODE_3x3_PARITY65,
        AR_MATRIX_CODE_3x3_HAMMING63 = ARUWP.AR_MATRIX_CODE_3x3_HAMMING63,
        AR_MATRIX_CODE_4x4 = ARUWP.AR_MATRIX_CODE_4x4,
        AR_MATRIX_CODE_4x4_BCH_13_9_3 = ARUWP.AR_MATRIX_CODE_4x4_BCH_13_9_3,
        AR_MATRIX_CODE_4x4_BCH_13_5_5 = ARUWP.AR_MATRIX_CODE_4x4_BCH_13_5_5
    }

    /// <summary>
    /// Image processing mode, same definition as ARToolKit. [public use]
    /// </summary>
    public enum ImageProcMode {
        AR_IMAGE_PROC_FIELD_IMAGE = ARUWP.AR_IMAGE_PROC_FIELD_IMAGE,
        AR_IMAGE_PROC_FRAME_IMAGE = ARUWP.AR_IMAGE_PROC_FRAME_IMAGE
    }

    /// <summary>
    /// Log level of native library, same definition as ARToolKit. [public use]
    /// </summary>
    public enum AR_LOG_LEVEL {
        AR_LOG_LEVEL_DEBUG = 0,
        AR_LOG_LEVEL_INFO,
        AR_LOG_LEVEL_WARN,
        AR_LOG_LEVEL_ERROR,
        AR_LOG_LEVEL_REL_INFO
    }

    #endregion

    /// <summary>
    /// Initial value of ThresholdMode. At runtime, please use SetVideoThresholdMode() to modify
    /// the value. [public use] [initialization only]
    /// </summary>
    public ThresholdMode thresholdMode = ThresholdMode.AR_LABELING_THRESH_MODE_MANUAL;

    /// <summary>
    /// Initial value of LabelingMode. At runtime please use SetLabelingMode() to modify the
    /// value. [public use] [initialization only]
    /// </summary>
    public LabelingMode labelingMode = LabelingMode.AR_LABELING_BLACK_REGION;

    /// <summary>
    /// Initial value of PatternDetectionMode. At runtime, please use SetPatternDetectionMode() to
    /// modify the value. [public use] [initialization only]
    /// </summary>
    public PatternDetectionMode patternDetectionMode = PatternDetectionMode.AR_TEMPLATE_MATCHING_COLOR;

    /// <summary>
    /// Initial value of MatrixCodeType. At runtime, please use SetMatrixCodeType() to modify the
    /// value. [public use] [initialization only]
    /// </summary>
    public MatrixCodeType matrixCodeType = MatrixCodeType.AR_MATRIX_CODE_3x3;

    /// <summary>
    /// Initial value of ImageProcMode. At runtime, please use SetImageProcMode() to modify the
    /// value. [public use] [initialization only]
    /// </summary>
    public ImageProcMode imageProcMode = ImageProcMode.AR_IMAGE_PROC_FRAME_IMAGE;
    
    /// <summary>
    /// Set the camera parameter content buffer. This should be called before the camera parameters
    /// are set to the native library. [public use]
    /// </summary>
    /// <param name="buffer"></param>
    public void SetCameraParamBuffer(byte[] buffer) {
        cameraParamBuffer = new byte[buffer.Length];
        buffer.CopyTo(cameraParamBuffer, 0);
    }

    /// <summary>
    /// Retrieve the camera parameter content buffer. [public use]
    /// </summary>
    /// <returns></returns>
    public byte[] GetCameraParamBuffer() {
        return cameraParamBuffer;
    }

    /// <summary>
    /// Toggle the Unity inspector to show more initialization options. [editor use]
    /// </summary>
    public bool showOptions = false;

    /// <summary>
    /// Initial value for holding the text containing tracking frame rate information. It is useful
    /// to indicate the runtime performance of the application. [public use]
    /// </summary>
    public Text trackFPS = null;

    /// <summary>
    /// Initial value for holding the text containing rendering frame rate information. It is useful
    /// to indicate the runtime performance of the application. [public use]
    /// </summary>
    public Text renderFPS = null;

    /// <summary>
    /// The byte buffer of current frame image. [internal use]
    /// </summary>
    private byte[] frameData = null;

    /// <summary>
    /// Signal to indicate that the tracking information was updated within the previous render frame.
    /// [internal use]
    /// </summary>
    private bool signalTrackingUpdated = false;

    /// <summary>
    /// Signal to indicate that the initialization of video and tracking is triggered. It helps the 
    /// auto-start of initialization after the application starts. [internal use]
    /// </summary>
    private bool signalInitTriggered = false;

    /// <summary>
    /// Average render frame period in millisecond for previous 50 frames, calculated when 
    /// necessary. [internal use]
    /// </summary>
    private float renderDeltaTime = 0.0f;

    /// <summary>
    /// Average tracking frame period in millisecond for previous 50 frames, calculated when 
    /// necessary. [internal use]
    /// </summary>
    private float trackDeltaTime = 0.0f;


    /// <summary>
    /// Retrieve the FPS of rendering. [public use]
    /// </summary>
    /// <returns>Number of frames per second</returns>
    public float GetRenderFPS() {
        return 1000.0f / renderDeltaTime;
    }

    /// <summary>
    /// Retrieve the FPS of tracking. [public use]
    /// </summary>
    /// <returns>Number of frames per second</returns>
    public float GetTrackingFPS() {
        return 1000.0f / trackDeltaTime;
    }



#if !UNITY_EDITOR && UNITY_METRO
    [ComImport]
    [Guid("5B0D3235-4DBA-4D44-865E-8F1D0E4FD04D")]
    [InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    unsafe interface IMemoryBufferByteAccess {
        /// <summary>
        /// Unsafe function to retrieve the pointer and size information of the underlying
        /// buffer object. Must be used within unsafe functions. In addition, the project needs
        /// to be configured as "Allow unsafe code". [internal use]
        /// </summary>
        /// <param name="buffer">byte pointer to the start of the buffer</param>
        /// <param name="capacity">the size of the buffer</param>
        void GetBuffer(out byte* buffer, out uint capacity);
    }


    /// <summary>
    /// The heavy function of initialize the controller, that will be wrapped into a task that
    /// can be executed asynchronously. In this function, log callback will be set; native 
    /// controller will be initialized, markers will be added, tracking options will be set, 
    /// and ARUWPController status will be changed from ARUWP_STATUS_VIDEO_INITIALIZED to 
    /// ARUWP_STATUS_CTRL_INITIALIZED if successful. [internal use]
    /// </summary>
    /// <returns>Whether initialization is successful</returns>
    private bool InitializeControllerAsyncTaskFunc() {
        if (status != ARUWP.ARUWP_STATUS_VIDEO_INITIALIZED) {
            Debug.Log(TAG + ": InitializeControllerAsyncTaskFunc() unsupported status");
            return false;
        }

        ARUWP.aruwpRegisterLogCallbackWrapper(ARUWP.Log);
        ARUWP.aruwpSetLogLevel((int)(AR_LOG_LEVEL.AR_LOG_LEVEL_INFO));
        
        var ret = ARUWP.aruwpInitialiseAR(frameWidth, frameHeight, ARUWP.AR_PIXEL_FORMAT_RGBA);
        if (!ret) {
            Debug.Log(TAG + ": aruwpInitialiseAR() failed");
            return false;
        }

        foreach (var m in unaddedMarkers) {
            m.AddMarker();
            m.LogMarkerInformation();
        }

        if (useCameraParamFile) {
            ret = ARUWP.aruwpStartRunning("Data/StreamingAssets/" + cameraParam);
        }
        else {
            if (cameraParamBuffer != null) {
                ret = ARUWP.aruwpStartRunningBuffer(cameraParamBuffer, cameraParamBuffer.Length);
            }
            else {
                Debug.Log(TAG + ": cameraParamBuffer is null");
                return false;
            }
        }
        if (!ret) {
            Debug.Log(TAG + ": startRunning() fails");
            return false;
        }

        // Set default tracking options
        SetVideoThreshold(threshold);
        SetVideoThresholdMode(thresholdMode);
        SetLabelingMode(labelingMode);
        SetPatternDetectionMode(patternDetectionMode);
        SetBorderSize(borderSize);
        SetMatrixCodeType(matrixCodeType);
        SetImageProcMode(imageProcMode);

        LogVersionString();
        LogFrameInforamtion();

        status = ARUWP.ARUWP_STATUS_CTRL_INITIALIZED;
        Debug.Log(TAG + ": InitializeControllerAsyncTaskFunc() is successful");

        return true;
    }
    
    /// <summary>
    /// The task wrapper of initialization heavy funtion. [internal use]
    /// </summary>
    /// <returns></returns>
    private Task<bool> InitializeControllerAsyncTask() {
        return Task.Run(() => InitializeControllerAsyncTaskFunc());
    }
    
    /// <summary>
    /// The task wrapper of starting frame reader of ARUWPVideo. [internal use]
    /// </summary>
    private async void StartFrameReaderAsync() {
        await videoManager.StartFrameReaderAsyncTask();
    }

    /// <summary>
    /// The task wrapper of stopping frame reader of ARUWPVideo. [internal use]
    /// </summary>
    private async void StopFrameReaderAsync() {
        await videoManager.StopFrameReaderAsyncTask();
    }

    /// <summary>
    /// Initilization of video capture and tracking sequentially. This is an asynchronous
    /// function, in order not to block the Unity UI thread. [internal use]
    /// </summary>
    private async void InitAsync() {
        Debug.Log(TAG + ": Init() starts");
        bool ret = await videoManager.InitializeMediaCaptureAsyncTask();
        if (!ret) {
            Debug.Log(TAG + ": Init() fails");
            return;
        }
        frameData = new byte[frameHeight * frameWidth * 4];
        ret = await InitializeControllerAsyncTask();
        if (!ret) {
            Debug.Log(TAG + ": Init() fails");
            return;
        }
        StartFrameReaderAsync();
    }


    /// <summary>
    /// Unity Monobehavior function. Initialize the locatable camera root before
    /// ARUWPMarker.Start() reference this object. [internal use]
    /// </summary>
    private void OnEnable() {
        LocatableCameraRoot = new GameObject("Locatable Camera Root");
    }

    /// <summary>
    /// Unity Monobehavior function. ARUWPVideo is set here. Target the render frame rate to 60.
    /// Create unaddedMarkers list, preparing the initialization. [internal use]
    /// </summary>
    private void Start() {

        videoManager = GetComponent<ARUWPVideo>();
        if (videoManager == null) {
            Debug.Log(TAG + ": not able to find ARUWPVideo");
            Application.Quit();
        }
        Application.targetFrameRate = 60;
        unaddedMarkers = GetComponents<ARUWPMarker>();
    }
    
    /// <summary>
    /// Pause the frame reader, thus pause the video capture and tracking. It can be called when
    /// the status is ARUWP_STATUS_RUNNING. [public use]
    /// </summary>
    public void Pause() {
        StopFrameReaderAsync();
    }


    /// <summary>
    /// Resume the frame reader, thus resume the video capture and tracking. It can be called when
    /// the status is ARUWP_STATUS_CTRL_INITIALIZED. [public use]
    /// </summary>
    public void Resume() {
        StartFrameReaderAsync();
    }


    /// <summary>
    /// Unity Monobehavior function. Initialization of video and tracking will be auto-started in the
    /// Update() loop. User customized functionalities can be place here. [internal use]
    /// </summary>
    private void Update() {
        if (!signalInitTriggered) {
            InitAsync();
            signalInitTriggered = true;
        }


        // Do stuff, for example, control the behavior of tracking and preview by bluetooth keyboard
        if (Input.GetKeyDown(KeyCode.P)) {
            if (videoManager.videoPreview) {
                videoManager.DisablePreview();
            }
            else {
                videoManager.EnablePreview();
            }
        }
        if (Input.GetKeyDown(KeyCode.V)) {
            if (status == ARUWP.ARUWP_STATUS_RUNNING) {
                Pause();
            }
            else if (status == ARUWP.ARUWP_STATUS_CTRL_INITIALIZED) {
                Resume();
            }
        }


        ARUWPUtils.RenderTick();
        renderDeltaTime = ARUWPUtils.GetRenderDeltaTime();
        trackDeltaTime = ARUWPUtils.GetTrackDeltaTime();

        if (renderFPS != null) {
            renderFPS.text = string.Format("Render: {0:0.0} ms ({1:0.} fps)", renderDeltaTime, 1000.0f / renderDeltaTime);
        }
        if (trackFPS != null) {
            trackFPS.text = string.Format("Track:   {0:0.0} ms ({1:0.} fps)", trackDeltaTime, 1000.0f / trackDeltaTime);
        }
        signalTrackingUpdated = false;
    }




    /// <summary>
    /// Create IntPtr object pinned at the frameData buffer. This pointer will be passed to
    /// the native library as a uint8 pointer. [internal use]
    /// </summary>
    /// <param name="frame">Image data saved in byte array</param>
    /// <returns>IntPtr object pinned at the image data</returns>
    private IntPtr GetImageHandle(byte[] frame) {
        IntPtr ptr;
        GCHandle handle = default(GCHandle);
        try {
            handle = GCHandle.Alloc(frame, GCHandleType.Pinned);
            ptr = handle.AddrOfPinnedObject();
        }
        finally {
            if (handle != default(GCHandle))
                handle.Free();
        }
        return ptr;
    }


    /// <summary>
    /// Actual calls to tracking and update tracking result are here. This function is heavy, and
    /// will be wrapped up in a task so that it can be executed asynchronously with video pipeline
    /// and Unity UI thread. [internal use]
    /// </summary>
    /// <param name="softwareBitmap">The input from video pipeline, the current image frame</param>
    private unsafe void ProcessFrameAsyncTaskFunc(SoftwareBitmap softwareBitmap) {
        if (status == ARUWP.ARUWP_STATUS_RUNNING) {
            using (var input = softwareBitmap.LockBuffer(BitmapBufferAccessMode.ReadWrite))
            using (var inputReference = input.CreateReference()) {
                byte* inputBytes;
                uint inputCapacity;
                ((IMemoryBufferByteAccess)inputReference).GetBuffer(out inputBytes, out inputCapacity);

                Marshal.Copy((IntPtr)inputBytes, frameData, 0, frameData.Length);

                IntPtr p = GetImageHandle(frameData);
                Detect(p);
                DetectDone();
            }
        }
    }

    /// <summary>
    /// The handle to the asynchronous task of doing marker tracking in the current frame. The idea
    /// is to check if there is already a processing task running, and prevent several processing
    /// tasks to run at the sametime. [internal use]
    /// </summary>
    private Task processFrameTask = null;

    /// <summary>
    /// The task wrapper of heavy function ProcessFrameAsyncTaskFunc(). It returns and reset the task
    /// handle of processFrameTask. [internal use]
    /// </summary>
    /// <param name="softwareBitmap">The input from video pipeline, the current image frame</param>
    /// <returns></returns>
    private Task ProcessFrameAsyncTask(SoftwareBitmap softwareBitmap) {
        processFrameTask = Task.Run(() => ProcessFrameAsyncTaskFunc(softwareBitmap));
        return processFrameTask;
    }

    /// <summary>
    /// The asynchronous function to execute the wrapped task of tracking. It will omit the current 
    /// frame if the processing of previous frame is not yet finished. [internal use]
    /// </summary>
    /// <param name="softwareBitmap">The input from video pipeline, the current image frame</param>
    public async void ProcessFrameAsync(SoftwareBitmap softwareBitmap) {
        if (processFrameTask != null && !processFrameTask.IsCompleted) {
            // Debug.Log(TAG + ": processFrameTask not completed yet");
            return;
        }
        await ProcessFrameAsyncTask(softwareBitmap);
    }

    /// <summary>
    /// The actual detection function, wrapping up a call to native ARToolKitUWP.dll library.
    /// [internal use]
    /// </summary>
    /// <param name="imageHandle">IntPtr object that points at the image frameData</param>
    private void Detect(IntPtr imageHandle) {
        ARUWP.aruwpUpdate(imageHandle);
    }

    /// <summary>
    /// DetectDone function executes when the detection finishes, to update the information of all 
    /// the markers in the scene. [internal use]
    /// </summary>
    private void DetectDone() {
        foreach (var key in markers.Keys) {
            markers[key].UpdateTrackingInfo();
        }
        signalTrackingUpdated = true;
        ARUWPUtils.TrackTick();
    }



    /// <summary>
    /// Determine whether the access to the native code, ARToolKitUWP.dll is given to the controller.
    /// Only when the ARUWPController status is ARUWP_STATUS_VIDEO_INITIALIZED or 
    /// ARUWP_STATUS_CTRL_INITIALIZED, the unity thread is able to call native functions. 
    /// When a call to native function is used, it is recommended to check the access handle first, 
    /// in order to avoid shared memory issue. [internal use]
    /// </summary>
    /// <returns>Whether the script can call native functions</returns>
    private bool HasNativeHandle() {
        var t_status = status;
        return (t_status == ARUWP.ARUWP_STATUS_VIDEO_INITIALIZED ||
            t_status == ARUWP.ARUWP_STATUS_CTRL_INITIALIZED);
    }



    /// <summary>
    /// TODO: correctly shutdown the controller and free resources
    /// </summary>
    //public void RemoveAllMarkers() {
    //    ARUWP.aruwpRemoveAllMarkers();
    //    markers.Clear();
    //}    
    //public bool stopRunning() {
    //    if (state == State.WAITING_FOR_FRAME || state == State.DETECTION_RUNNING) {
    //        bool ret = ARUWP.aruwpStopRunning();
    //        if (ret) {
    //            state = State.BASE_INITIALISED;
    //            return true;
    //        }
    //        else {
    //            Debug.Log(TAG + ": stopRunning() fails");
    //            return false;
    //        }
    //    }
    //    else {
    //        Debug.Log(TAG + ": stopRunning() fails unless current status is WAITING_FOR_FRAME or DETECTION_RUNNING");
    //        return false;
    //    }
    //}    //public bool shutDownAR() {
    //    bool ret = ARUWP.aruwpShutdownAR();
    //    if (ret) {
    //        state = State.NOTHING_INITIALISED;
    //        return true;
    //    }
    //    else {
    //        Debug.Log(TAG + ": shutDownAR() fails");
    //        return false;
    //    }
    //}
    //private void OnDestroy() {
    //    removeAllMarkers();
    //    shutDownAR();
    //    ARUWP.aruwpRegisterLogCallback(null);
    //}


    #region setters when access is granted


    /// <summary>
    /// Log the version of ARToolKit using Debug.Log(). [internal use]
    /// </summary>
    private void LogVersionString() {
        if (HasNativeHandle()) {
            StringBuilder buffer = new StringBuilder(20);
            ARUWP.aruwpGetARToolKitVersion(buffer, 20);
            Debug.Log(TAG + ": Version String: " + buffer.ToString());
        }
        else {
            Debug.Log(TAG + ": LogVersionString() unsupported status");
        }
    }

    /// <summary>
    /// Log the current frame information using Debug.Log(). [internal use]
    /// </summary>
    private void LogFrameInforamtion() {
        if (HasNativeHandle()) {
            int width, height, pixelSize;
            StringBuilder buffer = new StringBuilder(30);
            ARUWP.aruwpGetFrameParams(out width, out height, out pixelSize, buffer, 30);
            Debug.Log("ARUWP: Frame width and height: " + width + "x" + height);
            Debug.Log("ARUWP: Frame pixel size: " + pixelSize);
            Debug.Log("ARUWP: Frame pixel format string: " + buffer.ToString());
        }
        else {
            Debug.Log(TAG + ": LogFrameInforamtion() unsupported status");
        }
    }


    /// <summary>
    /// Set threshold parameter at runtime. [public use]
    /// </summary>
    /// <param name="o">New parameter</param>
    public void SetVideoThreshold(int o) {
        if (HasNativeHandle()) {
            ARUWP.aruwpSetVideoThreshold(o);
            threshold = ARUWP.aruwpGetVideoThreshold();
            if (threshold != o) {
                Debug.Log(TAG + ": Unable to set video threshold to " + o);
            }
        }
        else {
            Debug.Log(TAG + ": SetVideoThreshold() unsupported status");
        }
    }

    /// <summary>
    /// Set thresholdMode parameter at runtime. [public use]
    /// </summary>
    /// <param name="o">New parameter</param>
    public void SetVideoThresholdMode(ThresholdMode o) {
        if (HasNativeHandle()) {
            ARUWP.aruwpSetVideoThresholdMode((int)o);
            thresholdMode = (ThresholdMode)ARUWP.aruwpGetVideoThresholdMode();
            if (thresholdMode != o) {
                Debug.Log(TAG + ": Unable to set video threshold mode to " + o);
            }
        }
        else {
            Debug.Log(TAG + ": SetVideoThresholdMode() unsupported status");
        }
    }

    /// <summary>
    /// Set labelingMode parameter at runtime. [public use]
    /// </summary>
    /// <param name="o">New parameter</param>
    public void SetLabelingMode(LabelingMode o) {
        if (HasNativeHandle()) {
            ARUWP.aruwpSetLabelingMode((int)o);
            labelingMode = (LabelingMode)ARUWP.aruwpGetLabelingMode();
            if (labelingMode != o) {
                Debug.Log(TAG + ": Unable to set labeling mode to " + o);
            }
        }
        else {
            Debug.Log(TAG + ": SetLabelingMode() unsupported status");
        }
    }


    /// <summary>
    /// Set patternDetectionMode parameter at runtime. [public use]
    /// </summary>
    /// <param name="o">New parameter</param>
    public void SetPatternDetectionMode(PatternDetectionMode o) {
        if (HasNativeHandle()) {
            ARUWP.aruwpSetPatternDetectionMode((int)o);
            patternDetectionMode = (PatternDetectionMode)ARUWP.aruwpGetPatternDetectionMode();
            if (patternDetectionMode != o) {
                Debug.Log(TAG + ": Unable to set pattern detection mode to " + o);
            }
        }
        else {
            Debug.Log(TAG + ": SetPatternDetectionMode() unsupported status");
        }
    }

    /// <summary>
    /// Set borderSize parameter at runtime. [public use]
    /// </summary>
    /// <param name="o">New parameter</param>
    public void SetBorderSize(float o) {
        if (HasNativeHandle()) {
            ARUWP.aruwpSetBorderSize(o);
            borderSize = ARUWP.aruwpGetBorderSize();
            if (borderSize != o) {
                Debug.Log(TAG + ": Unable to set border size to " + o);
            }
        }
        else {
            Debug.Log(TAG + ": SetBorderSize() unsupported status");
        }
    }

    /// <summary>
    /// Set matrixCodeType parameter at runtime. [public use]
    /// </summary>
    /// <param name="o">New parameter</param>
    public void SetMatrixCodeType(MatrixCodeType o) {
        if (HasNativeHandle()) {
            ARUWP.aruwpSetMatrixCodeType((int)o);
            matrixCodeType = (MatrixCodeType)ARUWP.aruwpGetMatrixCodeType();
            if (matrixCodeType != o) {
                Debug.Log(TAG + ": Unable to set matrix code type to " + o);
            }
        }
        else {
            Debug.Log(TAG + ": SetMatrixCodeType() unsupported status");
        }
    }

    /// <summary>
    /// Set imageProcMode parameter at runtime. [public use]
    /// </summary>
    /// <param name="o">New parameter</param>
    public void SetImageProcMode(ImageProcMode o) {
        if (HasNativeHandle()) {
            ARUWP.aruwpSetImageProcMode((int)o);
            imageProcMode = (ImageProcMode)ARUWP.aruwpGetImageProcMode();
            if (imageProcMode != o) {
                Debug.Log(TAG + ": Unable to set image process mode to " + o);
            }
        }
        else {
            Debug.Log(TAG + ": SetImageProcMode() unsupported status");
        }
    }

    #endregion
    
#endif


}
