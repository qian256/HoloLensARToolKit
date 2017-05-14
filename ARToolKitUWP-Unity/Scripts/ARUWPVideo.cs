/*
*  ARUWPUtils.cs
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
using System.IO;
using System.Text;
using UnityEngine.UI;
#if !UNITY_EDITOR && UNITY_METRO
using Windows.Media;
using Windows.Media.Capture;
using Windows.Media.Capture.Frames;
using System.Threading.Tasks;
using System;
using Windows.Graphics.Imaging;
using System.Threading;
using System.Linq;
#endif

/// <summary>
/// The ARUWPVideo class handles video access, using Windows MediaCapture APIs.
/// Through this class, ARUWPController.cs is able to control video initialization,
/// start, stop, enable or disable preview. ARUWPVideo uses .NET asynchronous feature
/// to optimize the video pipeline, including maintaining a high video frame rate
/// even if the tracking or rendering is slow.
///
/// Author:     Long Qian
/// Email:      lqian8@jhu.edu
/// </summary>
public class ARUWPVideo : MonoBehaviour {

    /// <summary>
    /// Class and object identifier for logging. [internal use]
    /// </summary>
    private static string TAG = "ARUWPVideo";
    
    /// <summary>
    /// ARUWPController instance. [internal use]
    /// </summary>
    private ARUWPController controller = null;

    /// <summary>
    /// Unity Material to hold the Unity Texture of camera preview image. [internal use]
    /// </summary>
    private Material mediaMaterial = null;

    /// <summary>
    /// Unity Texture to hold the camera preview image. [internal use]
    /// </summary>
    private Texture2D mediaTexture = null;

    /// <summary>
    /// Initial value for video preview visualization. Video preview visualization might pose
    /// additional burden to rendering, but is useful for debugging tracking behavior.
    /// At runtime, please use EnablePreview() and DisablePreview() to control it. [public use]
    /// [initialization only]
    /// </summary>
    public bool videoPreview = true;

    /// <summary>
    /// Initial value for the GameObject to hold the video preview. It is a Quad object in the
    /// samples, but can potentially be any GameObject that has a MeshRenderer. [public use]
    /// </summary>
    public GameObject previewPlane = null;

    /// <summary>
    /// Initial value for holding the text containing video frame rate information. It is useful
    /// to indicate the runtime performance of the application. [public use]
    /// </summary>
    public Text videoFPS = null;


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
    /// Temporary SoftwareBitmap used for data exchange between Unity UI thread (Update 
    /// function) and the video thread (OnFrameArrived function). [internal use]
    /// </summary>
    private SoftwareBitmap _bitmap = null;

    /// <summary>
    /// Signal indicating that the video has updated between the last render loop, the previous
    /// Update() and the current Update(). [internal use]
    /// </summary>
    private bool signalTrackingUpdated = false;

    /// <summary>
    /// Signal indicating that the initialization of video loop is done. [internal use]
    /// </summary>
    private bool signalInitDone = false;

    /// <summary>
    /// The SoftwareBitmap used for Unity UI thread for video previewing. [internal use]
    /// </summary>
    private SoftwareBitmap updateBitmap = null;

    /// <summary>
    /// Windows MediaCapture object used for UWP video loop. [internal use]
    /// </summary>
    private MediaCapture mediaCapture = null;

    /// <summary>
    /// Windows MediaFrameReader object used for UWP video loop. [internal use]
    /// </summary>
    private MediaFrameReader frameReader = null;

    /// <summary>
    /// Size of the video buffer used to create Unity texture object. [internal use]
    /// </summary>
    private int videoBufferSize = 0;

    /// <summary>
    /// Average video frame period in millisecond for previous 50 frames, calculated when 
    /// necessary. [internal use]
    /// </summary>
    private float videoDeltaTime = 0.0f;

    /// <summary>
    /// Retrieve the FPS of video. [public use]
    /// </summary>
    /// <returns>Number of frames per second</returns>
    public float GetVideoFPS() {
        return 1000.0f / videoDeltaTime;
    }

    /// <summary>
    /// The Task to asynchronously initialize MediaCapture in UWP. The camera of HoloLens will
    /// be configured to preview video of 896x504 at 30 fps, pixel format is NV12. MediaFrameReader
    /// will be initialized and register the callback function OnFrameArrived to each video
    /// frame. Note that this task does not start running the video preview, but configures the
    /// running behavior. This task should be executed when ARUWPController status is 
    /// ARUWP_STATUS_CLEAN, and will change it to ARUWP_STATUS_VIDEO_INITIALIZED if no error 
    /// occurred. [internal use]
    /// </summary>
    /// <returns>Whether video pipeline is successfully initialized</returns>
    public async Task<bool> InitializeMediaCaptureAsyncTask() {
        if (controller.status != ARUWP.ARUWP_STATUS_CLEAN) {
            Debug.Log(TAG + ": InitializeMediaCaptureAsyncTask() unsupported status");
            return false;
        }

        if (mediaCapture != null) {
            Debug.Log(TAG + ": InitializeMediaCaptureAsyncTask() fails because mediaCapture is not null");
            return false;
        }

        var allGroups = await MediaFrameSourceGroup.FindAllAsync();
        foreach (var group in allGroups) {
            Debug.Log(group.DisplayName + ", " + group.Id);
        }

        if (allGroups.Count <= 0) {
            Debug.Log(TAG + ": InitializeMediaCaptureAsyncTask() fails because there is no MediaFrameSourceGroup");
            return false;
        }

        // Initialize mediacapture with the source group.
        mediaCapture = new MediaCapture();
        var settings = new MediaCaptureInitializationSettings {
            SourceGroup = allGroups[0],
            // This media capture can share streaming with other apps.
            SharingMode = MediaCaptureSharingMode.SharedReadOnly,
            // Only stream video and don't initialize audio capture devices.
            StreamingCaptureMode = StreamingCaptureMode.Video,
            // Set to CPU to ensure frames always contain CPU SoftwareBitmap images
            // instead of preferring GPU D3DSurface images.
            MemoryPreference = MediaCaptureMemoryPreference.Cpu
        };

        await mediaCapture.InitializeAsync(settings);
        Debug.Log(TAG + ": MediaCapture is successfully initialized in shared mode.");

        try {
            var mediaFrameSourceVideoPreview = mediaCapture.FrameSources.Values.Single(x => x.Info.MediaStreamType == MediaStreamType.VideoPreview);
            var minResFormat = mediaFrameSourceVideoPreview.SupportedFormats.OrderBy(x => x.VideoFormat.Width * x.VideoFormat.Height).FirstOrDefault();
            await mediaFrameSourceVideoPreview.SetFormatAsync(minResFormat);
            Debug.Log(TAG + ": minResFormat.Subtype is " + minResFormat.Subtype);
            frameReader = await mediaCapture.CreateFrameReaderAsync(mediaFrameSourceVideoPreview, minResFormat.Subtype);
            frameReader.FrameArrived += OnFrameArrived;
            controller.frameWidth = Convert.ToInt32(minResFormat.VideoFormat.Width);
            controller.frameHeight = Convert.ToInt32(minResFormat.VideoFormat.Height);
            videoBufferSize = controller.frameWidth * controller.frameHeight * 4;
            Debug.Log(TAG + ": FrameReader is successfully initialized");
        }
        catch (Exception e) {
            Debug.Log(TAG + ": FrameReader is not initialized");
            Debug.Log(TAG + ": Exception: " + e);
            return false;
        }

        controller.status = ARUWP.ARUWP_STATUS_VIDEO_INITIALIZED;
        signalInitDone = true;
        Debug.Log(TAG + ": InitializeMediaCaptureAsyncTask() is successful");
        return true;
    }

    /// <summary>
    /// The task to asynchronously starts the video pipeline and frame reading. The task should
    /// be executed when the ARUWPController status is ARUWP_STATUS_CTRL_INITIALIZED, and will
    /// change the status to ARUWP_STATUS_RUNNING if the task is successful. The task is wrapped
    /// up in ARUWPController.Resume() function. [internal use]
    /// </summary>
    /// <returns>Whether the frame reader is successfully started</returns>
    public async Task<bool> StartFrameReaderAsyncTask() {
        if (controller.status != ARUWP.ARUWP_STATUS_CTRL_INITIALIZED) {
            Debug.Log(TAG + ": StartFrameReaderAsyncTask() fails because of incorrect status");
            return false;
        }

        MediaFrameReaderStartStatus mediaFrameReaderStartStatus = await frameReader.StartAsync();
        if (mediaFrameReaderStartStatus == MediaFrameReaderStartStatus.Success) {
            Debug.Log(TAG + ": StartFrameReaderAsyncTask() is successful");
            controller.status = ARUWP.ARUWP_STATUS_RUNNING;
            return true;
        }
        else {
            Debug.Log(TAG + ": StartFrameReaderAsyncTask() is not successful, status = " + mediaFrameReaderStartStatus);
            return false;
        }
    }

    /// <summary>
    /// The task to asynchronously stops the video pipeline and frame reading. The task should
    /// be executed when the ARUWPController status is ARUWP_STATUS_RUNNING, and will change it
    /// to ARUWP_STATUS_CTRL_INITIALIZED when the task is successful. Note that the frame reader
    /// can be restarted after stop. It is more pause-resume instead of stop-start, because the 
    /// video pipeline does not need to be configured again. The task is wrapped up in the 
    /// ARUWPController.Pause() function. [internal use]
    /// </summary>
    /// <returns>Whether the video frame reader is successfully stopped</returns>
    public async Task<bool> StopFrameReaderAsyncTask() {
        if (controller.status != ARUWP.ARUWP_STATUS_RUNNING) {
            Debug.Log(TAG + ": StopFrameReaderAsyncTask() fails because of incorrect status");
            return false;
        }
        await frameReader.StopAsync();
        controller.status = ARUWP.ARUWP_STATUS_CTRL_INITIALIZED;
        Debug.Log(TAG + ": StopFrameReaderAsyncTask() is successful");
        return true;
    }




    /// <summary>
    /// The callback that is triggered when new video preview frame arrives. In this function,
    /// video frame is saved for Unity UI if videoPreview is enabled, tracking task is triggered
    /// in this function call, and video FPS is recorded. [internal use]
    /// </summary>
    /// <param name="sender">MediaFrameReader object</param>
    /// <param name="args">arguments not used here</param>
    private void OnFrameArrived(MediaFrameReader sender, MediaFrameArrivedEventArgs args) {
        ARUWPUtils.VideoTick();
        using (var frame = sender.TryAcquireLatestFrame()) {
            if (frame != null) {
                var softwareBitmap = SoftwareBitmap.Convert(frame.VideoMediaFrame.SoftwareBitmap, BitmapPixelFormat.Rgba8, BitmapAlphaMode.Ignore);
                if (videoPreview) {
                    Interlocked.Exchange(ref _bitmap, softwareBitmap);
                    controller.ProcessFrameAsync(SoftwareBitmap.Copy(softwareBitmap));
                }
                else {
                    controller.ProcessFrameAsync(softwareBitmap);
                }
                signalTrackingUpdated = true;
            }
        }
    }

    

    
    /// <summary>
    /// Unity Monobehavior function. ARUWPController is set here. Video preview is
    /// initialized depending on the initial value. [internal use]
    /// </summary>
    private void Start() {
        controller = GetComponent<ARUWPController>();
        if (controller == null) {
            Debug.Log(TAG + ": not able to find ARUWPController");
            Application.Quit();
        }
        mediaMaterial = previewPlane.GetComponent<MeshRenderer>().material;
        if (videoPreview) {
            EnablePreview();
        }
    }

    /// <summary>
    /// Update the video preview texture. It is an unsafe function since unsafe interface is
    /// used. Here the memory exchange between video thread and Unity UI thread happens, and
    /// refresh Unity Texture object based on video SoftwareBitmap. [internal use]
    /// </summary>
    private unsafe void UpdateVideoPreview() {
        Interlocked.Exchange(ref updateBitmap, _bitmap);
        using (var input = updateBitmap.LockBuffer(BitmapBufferAccessMode.Read))
        using (var inputReference = input.CreateReference()) {
            byte* inputBytes;
            uint inputCapacity;
            ((IMemoryBufferByteAccess)inputReference).GetBuffer(out inputBytes, out inputCapacity);
            mediaTexture.LoadRawTextureData((IntPtr)inputBytes, videoBufferSize);
            mediaTexture.Apply();
        }
    }

    /// <summary>
    /// Unity Monobehavior function: update texture depending on whether new frame arrives,
    /// trigger visualization of video frame rate information, initialize media texture when
    /// initialization of video pipeline is done. [internal use]
    /// </summary>
    private void Update() {

        if (signalInitDone) {
            mediaTexture = new Texture2D(controller.frameWidth, controller.frameHeight, TextureFormat.RGBA32, false);
            mediaMaterial.mainTexture = mediaTexture;
            signalInitDone = false;
        }


        if (videoPreview && previewPlane.activeSelf && mediaTexture != null && signalTrackingUpdated) {
            UpdateVideoPreview();
        }

        videoDeltaTime = ARUWPUtils.GetVideoDeltaTime();
        if (videoFPS != null) {
            videoFPS.text = string.Format("Video:   {0:0.0} ms ({1:0.} fps)", videoDeltaTime, 1000.0f / videoDeltaTime);
        }

        signalTrackingUpdated = false;
    }




    /// <summary>
    /// Determine whether the access to the native code, ARToolKitUWP.dll is given to video thread.
    /// Only when the ARUWPController status is ARUWP_STATUS_CLEAN or ARUWP_STATUS_RUNNING, the
    /// video thread is able to call native functions. When a call to native function is used, it 
    /// is recommended to check the access handle first, in order to avoid shared memory issue.
    /// [internal use]
    /// </summary>
    /// <returns></returns>
    private bool HasNativeHandlePreviewThread() {
        var t_status = controller.status;
        return (t_status == ARUWP.ARUWP_STATUS_CLEAN || t_status == ARUWP.ARUWP_STATUS_RUNNING);
    }



    /// <summary>
    /// TODO: Clean the video thread when the application quits.
    /// </summary>
    //private void OnApplicationQuit() {
    //    if (Interlocked.Equals(videoStatus, ARUWP_VIDEO_RUNNING)) {
    //        StopFrameReaderAsyncWrapper();
    //    }
    //}



#else
    
#endif

    /// <summary>
    /// Enable video preview. Video preview allows the user to see the current video frame.
    /// [public use]
    /// </summary>
    public void EnablePreview() {
        Debug.Log(TAG + ": EnablePreview() called");
        previewPlane.SetActive(true);
        videoPreview = true;
    }

    /// <summary>
    /// Disable video preview. Rendering frame rate can be higher because per-frame bitmap 
    /// manipulation is expensive. [public use]
    /// </summary>
    public void DisablePreview() {
        Debug.Log(TAG + ": DisablePreview() called");
        previewPlane.SetActive(false);
        videoPreview = false;
    }

    /// <summary>
    /// Toggle the video previewing mode.
    /// </summary>
    public void TogglePreview() {
        Debug.Log(TAG + ": TogglePreview() called");
        if (videoPreview) {
            DisablePreview();
        }
        else {
            EnablePreview();
        }
    }

}
