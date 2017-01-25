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

    public bool enable = true;

    // hard-coded HoloLens tracking property
    private int frameWidth = 896, frameHeight = 504;
    private int pixelFormat = ARUWP.AR_PIXEL_FORMAT_RGBA;

    public bool useCameraParamFile = true;
    public string cameraParam = "hololens896x504.dat";
    private byte[] cameraParamBuffer = null;

    private static string TAG = "ARUWPController";

    public static Dictionary<int, ARUWPMarker> markers;
    private ARUWPMarker[] unaddedMarkers = null;

    [Range(0, 255)]
    public int threshold = 100;
    [Range(0f, 0.5f)]
    public float borderSize = 0.25f;

    #region Public Enums
    public enum ThresholdMode {
        AR_LABELING_THRESH_MODE_MANUAL = ARUWP.AR_LABELING_THRESH_MODE_MANUAL,
        AR_LABELING_THRESH_MODE_AUTO_MEDIAN = ARUWP.AR_LABELING_THRESH_MODE_AUTO_MEDIAN,
        AR_LABELING_THRESH_MODE_AUTO_OTSU = ARUWP.AR_LABELING_THRESH_MODE_AUTO_OTSU,
        AR_LABELING_THRESH_MODE_AUTO_ADAPTIVE = ARUWP.AR_LABELING_THRESH_MODE_AUTO_ADAPTIVE,
        AR_LABELING_THRESH_MODE_AUTO_BRACKETING = ARUWP.AR_LABELING_THRESH_MODE_AUTO_BRACKETING
    }
    public enum LabelingMode {
        AR_LABELING_WHITE_REGION = ARUWP.AR_LABELING_WHITE_REGION,
        AR_LABELING_BLACK_REGION = ARUWP.AR_LABELING_BLACK_REGION
    }
    public enum PatternDetectionMode {
        AR_TEMPLATE_MATCHING_COLOR = ARUWP.AR_TEMPLATE_MATCHING_COLOR,
        AR_TEMPLATE_MATCHING_MONO = ARUWP.AR_TEMPLATE_MATCHING_MONO,
        AR_MATRIX_CODE_DETECTION = ARUWP.AR_MATRIX_CODE_DETECTION,
        AR_TEMPLATE_MATCHING_COLOR_AND_MATRIX = ARUWP.AR_TEMPLATE_MATCHING_COLOR_AND_MATRIX,
        AR_TEMPLATE_MATCHING_MONO_AND_MATRIX = ARUWP.AR_TEMPLATE_MATCHING_MONO_AND_MATRIX
    }
    public enum MatrixCodeType {
        AR_MATRIX_CODE_3x3 = ARUWP.AR_MATRIX_CODE_3x3,
        AR_MATRIX_CODE_3x3_PARITY65 = ARUWP.AR_MATRIX_CODE_3x3_PARITY65,
        AR_MATRIX_CODE_3x3_HAMMING63 = ARUWP.AR_MATRIX_CODE_3x3_HAMMING63,
        AR_MATRIX_CODE_4x4 = ARUWP.AR_MATRIX_CODE_4x4,
        AR_MATRIX_CODE_4x4_BCH_13_9_3 = ARUWP.AR_MATRIX_CODE_4x4_BCH_13_9_3,
        AR_MATRIX_CODE_4x4_BCH_13_5_5 = ARUWP.AR_MATRIX_CODE_4x4_BCH_13_5_5
    }
    public enum ImageProcMode {
        AR_IMAGE_PROC_FIELD_IMAGE = ARUWP.AR_IMAGE_PROC_FIELD_IMAGE,
        AR_IMAGE_PROC_FRAME_IMAGE = ARUWP.AR_IMAGE_PROC_FRAME_IMAGE
    }
    public enum AR_LOG_LEVEL {
        AR_LOG_LEVEL_DEBUG = 0,
        AR_LOG_LEVEL_INFO,
        AR_LOG_LEVEL_WARN,
        AR_LOG_LEVEL_ERROR,
        AR_LOG_LEVEL_REL_INFO
    }
    #endregion

    public ThresholdMode thresholdMode = ThresholdMode.AR_LABELING_THRESH_MODE_MANUAL;
    public LabelingMode labelingMode = LabelingMode.AR_LABELING_BLACK_REGION;
    public PatternDetectionMode patternDetectionMode = PatternDetectionMode.AR_TEMPLATE_MATCHING_COLOR;
    public MatrixCodeType matrixCodeType = MatrixCodeType.AR_MATRIX_CODE_3x3;
    public ImageProcMode imageProcMode = ImageProcMode.AR_IMAGE_PROC_FRAME_IMAGE;

    private AR_LOG_LEVEL currentLogLevel = AR_LOG_LEVEL.AR_LOG_LEVEL_DEBUG;


    private enum State {
        NOTHING_INITIALISED,        /// No initialisation yet and no resources allocated.
        BASE_INITIALISED,           /// Marker management initialised, markers can be added.
        WAITING_FOR_FRAME,          /// Marker added done, waiting for frame source to become ready.
        DETECTION_RUNNING           /// Frame running, additional initialisation occurred, marker detection running.
    }
    private State state = State.NOTHING_INITIALISED;



    public Material webcamMaterial;
    private WebCamTexture webcamTexture;
    public GameObject webcamPlane;
    private bool displayWebcam = true;
    private Color32[] imageData;
    private bool detecting = false;

    private readonly static Queue<Action> ExecuteOnMainThread = new Queue<Action>();


    // ARToolKit running rate
    private float deltaTime;
    private float lastTick;
    private float msec, fps;
    public Text ARUWPFrameRateTextbox;



    private bool firstFrameSent = false;


    public void TuneWebcamDisplay() {
        displayWebcam = !displayWebcam;
        if (displayWebcam) {
            webcamMaterial.mainTexture = webcamTexture;
            webcamPlane.SetActive(true);
        }
        else {
            webcamMaterial.mainTexture = null;
            webcamPlane.SetActive(false);
        }
    }

    // Use this for initialization
    void Start () {
        ARUWPUtils.aruwpRegisterLogCallback(Log);
        ARUWPUtils.aruwpSetLogLevel((int)currentLogLevel);
        lastTick = Time.time;
        deltaTime = 0;
        startWebcam();
        startARUWP();
    }


    private void startWebcam() {
        // init Webcam
        WebCamDevice[] cam_devices = WebCamTexture.devices;
        webcamTexture = new WebCamTexture(cam_devices[0].name, frameWidth, frameHeight, 15);
        if (displayWebcam) {
            webcamMaterial.mainTexture = webcamTexture;
            webcamPlane.SetActive(true);
        }
        else {
            webcamPlane.SetActive(false);
        }
        webcamTexture.Play();
    }
    
    private void startARUWP() {
        if (state != State.NOTHING_INITIALISED) {
            Debug.Log(TAG + ": startARUWP() fails unless in state NOTHING_INITIALISED");
            return;
        }
        // get potential markers
        markers = new Dictionary<int, ARUWPMarker>();
        unaddedMarkers = GetComponents<ARUWPMarker>();
        initAR();
        foreach (var m in unaddedMarkers) {
            m.addMarker();
            m.LogMarkerInformation();
        }
        startRunning();
        logVersionString();
        logFrameInforamtion();
    }


    void OnDestroy() {
        removeAllMarkers();
        shutDownAR();
        ARUWP.aruwpRegisterLogCallback(null);
    }


#if !UNITY_EDITOR && UNITY_METRO
    // Update is called once per frame
    void Update() {
        while (ExecuteOnMainThread.Count > 0) {
            ExecuteOnMainThread.Dequeue().Invoke();
        }
        if (webcamTexture.isPlaying) {
            if (enable && !detecting ) {
                detecting = true;
                imageData = webcamTexture.GetPixels32();
                Task.Run(() => Detect(imageData));
            }
        }
        if (ARUWPFrameRateTextbox != null){
            ARUWPFrameRateTextbox.text = string.Format("AR: {0:0.0} ms ({1:0.} fps)", msec, fps);
        }
    }



    private async Task Detect(Color32[] color) {
        IntPtr imageHandle = await getImageHandleAsync(color);
        ARUWP.aruwpUpdate(imageHandle);
        if (ExecuteOnMainThread.Count == 0) {
            ExecuteOnMainThread.Enqueue(() => {
                DetectDone();
            });
        }
    }


    private async Task<IntPtr> getImageHandleAsync(Color32[] color) {
        IntPtr ptr;
        GCHandle handle = default(GCHandle);
        try {
            handle = GCHandle.Alloc(color, GCHandleType.Pinned);
            ptr = handle.AddrOfPinnedObject();
        }
        finally {
            if (handle != default(GCHandle))
                handle.Free();
        }
        return ptr;
    }


    private IntPtr getImageHandle(Color32[] color) {
        IntPtr ptr;
        GCHandle handle = default(GCHandle);
        try {
            handle = GCHandle.Alloc(color, GCHandleType.Pinned);
            ptr = handle.AddrOfPinnedObject();
        }
        finally {
            if (handle != default(GCHandle))
                handle.Free();
        }
        return ptr;
    }
    
#endif


    private void DetectDone() {
        foreach(var key in markers.Keys) {
            markers[key].updateMarkerTracking();
        }
        if (!firstFrameSent) {
            firstFrameSent = true;
            // update state
            isRunning();
            if ( state != State.DETECTION_RUNNING) {
                Debug.Log(TAG + ": Cannot enter DETECTION_RUNNING state");
                firstFrameSent = false;
            }
            else {
                setAllTrackingOptions();
            }
        }
        Tick();
        detecting = false;
    }



    private void Tick() {
        float currentTick = Time.time;
        deltaTime += (currentTick - lastTick - deltaTime) * 0.1f;
        msec = deltaTime * 1000f;
        fps = 1 / deltaTime;
        lastTick = currentTick;
    }



    void removeAllMarkers() {
        ARUWP.aruwpRemoveAllMarkers();
        markers.Clear();
    }

    

    public void initAR() {
        if ( state == State.NOTHING_INITIALISED) {
            var ret = ARUWP.aruwpInitialiseAR(frameWidth, frameHeight, pixelFormat);
            if (ret) {
                state = State.BASE_INITIALISED;
            }
            else {
                Debug.Log(TAG + ": initAR() failed");
            }
        }
        else {
            Debug.Log(TAG + ": initAR() fails unless current status is NOTHING_INITIALISED");
        }
    }
    
    public void startRunning() {
        if (state == State.BASE_INITIALISED) {
            bool ret = false;
            if (useCameraParamFile) {
                ret = ARUWP.aruwpStartRunning("Data/StreamingAssets/" + cameraParam);
            }
            else {
                if (cameraParamBuffer != null) {
                    ret = ARUWP.aruwpStartRunningBuffer(cameraParamBuffer, cameraParamBuffer.Length);
                }
                else {
                    Debug.Log(TAG + ": cameraParamBuffer is null");
                }
            }
            if (ret) {
                state = State.WAITING_FOR_FRAME;
            }
            else {
                Debug.Log(TAG + ": startRunning() fails");
            }
        }
        else {
            Debug.Log(TAG + ": startRunning() fails unless current status is BASE_INITIALISED");
        }
    }
    
    public bool isRunning() {
        if (!enable) {
            return false;
        }
        else {
            bool ret = ARUWP.aruwpIsRunning();
            if (ret) {
                state = State.DETECTION_RUNNING;
            }
            return ret;
        }
    }
    
    public bool stopRunning() {
        if (state == State.WAITING_FOR_FRAME || state == State.DETECTION_RUNNING) {
            bool ret = ARUWP.aruwpStopRunning();
            if (ret) {
                state = State.BASE_INITIALISED;
                return true;
            }
            else {
                Debug.Log(TAG + ": stopRunning() fails");
                return false;
            }
        }
        else {
            Debug.Log(TAG + ": stopRunning() fails unless current status is WAITING_FOR_FRAME or DETECTION_RUNNING");
            return false;
        }
    }

    public bool shutDownAR() {
        bool ret = ARUWP.aruwpShutdownAR();
        if (ret) {
            state = State.NOTHING_INITIALISED;
            return true;
        }
        else {
            Debug.Log(TAG + ": shutDownAR() fails");
            return false;
        }
    }



    public static string getVersionString() {
        StringBuilder buffer = new StringBuilder(20);
        ARUWP.aruwpGetARToolKitVersion(buffer, 20);
        return buffer.ToString();
    }

    public static void logVersionString() {
        Debug.Log(TAG + ": Version String: " + getVersionString());
    }


    public static void logFrameInforamtion() {
        int width, height, pixelSize;
        StringBuilder buffer = new StringBuilder(30);
        ARUWP.aruwpGetFrameParams(out width, out height, out pixelSize, buffer, 30);
        Debug.Log("ARUWP: Frame width and height: " + width + "x" + height);
        Debug.Log("ARUWP: Frame pixel size: " + pixelSize);
        Debug.Log("ARUWP: Frame pixel format string: " + buffer.ToString());
    }


    public void setCameraParamBuffer(byte[] buffer) {
        cameraParamBuffer = new byte[buffer.Length];
        buffer.CopyTo(cameraParamBuffer, 0);
    }

    public byte[] getCameraParamBuffer() {
        return cameraParamBuffer;
    }




    private void setAllTrackingOptions() {
        setVideoThreshold(threshold);
        setVideoThresholdMode(thresholdMode);
        setLabelingMode(labelingMode);
        setPatternDetectionMode(patternDetectionMode);
        setBorderSize(borderSize);
        setMatrixCodeType(matrixCodeType);
        setImageProcMode(imageProcMode);
    }

    #region setter at runtime
    public void setVideoThreshold(int o) {
        if (state == State.DETECTION_RUNNING) {
            ARUWP.aruwpSetVideoThreshold(o);
            threshold = ARUWP.aruwpGetVideoThreshold();
            if (threshold != o) {
                Debug.Log(TAG + ": Unable to set video threshold to " + o);
            }
        }
        else {
            threshold = o;
        }
    }

    public void setVideoThresholdMode(ThresholdMode o) {
        if (state == State.DETECTION_RUNNING) {
            ARUWP.aruwpSetVideoThresholdMode((int)o);
            thresholdMode = (ThresholdMode)ARUWP.aruwpGetVideoThresholdMode();
            if (thresholdMode != o) {
                Debug.Log(TAG + ": Unable to set video threshold mode to " + o);
            }
        }
        else {
            thresholdMode = o;
        }
    }

    public void setLabelingMode(LabelingMode o) {
        if (state == State.DETECTION_RUNNING) {
            ARUWP.aruwpSetLabelingMode((int)o);
            labelingMode = (LabelingMode)ARUWP.aruwpGetLabelingMode();
            if (labelingMode != o) {
                Debug.Log(TAG + ": Unable to set labeling mode to " + o);
            }
        }
        else {
            labelingMode = o;
        }
    }


    public void setPatternDetectionMode(PatternDetectionMode o) {
        if (state == State.DETECTION_RUNNING) {
            ARUWP.aruwpSetPatternDetectionMode((int)o);
            patternDetectionMode = (PatternDetectionMode)ARUWP.aruwpGetPatternDetectionMode();
            if (patternDetectionMode != o) {
                Debug.Log(TAG + ": Unable to set pattern detection mode to " + o);
            }
        }
        else {
            patternDetectionMode = o;
        }
    }

    public void setBorderSize(float o) {
        if (state == State.DETECTION_RUNNING) {
            ARUWP.aruwpSetBorderSize(o);
            borderSize = ARUWP.aruwpGetBorderSize();
            if (borderSize != o) {
                Debug.Log(TAG + ": Unable to set border size to " + o);
            }
        }
        else {
            borderSize = o;
        }
    }

    public void setMatrixCodeType(MatrixCodeType o) {
        if (state == State.DETECTION_RUNNING) {
            ARUWP.aruwpSetMatrixCodeType((int)o);
            matrixCodeType = (MatrixCodeType)ARUWP.aruwpGetMatrixCodeType();
            if (matrixCodeType != o) {
                Debug.Log(TAG + ": Unable to set matrix code type to " + o);
            }
        }
        else {
            matrixCodeType = o;
        }
    }

    public void setImageProcMode(ImageProcMode o) {
        if (state == State.DETECTION_RUNNING) {
            ARUWP.aruwpSetImageProcMode((int)o);
            imageProcMode = (ImageProcMode)ARUWP.aruwpGetImageProcMode();
            if (imageProcMode != o) {
                Debug.Log(TAG + ": Unable to set image process mode to " + o);
            }
        }
        else {
            imageProcMode = o;
        }
    }
    #endregion



    private static List<String> logMessages = new List<String>();
    private const int MaximumLogMessages = 1000;

    public static Action<String> logCallback { get; set; }

    public static void Log(String msg) {
        // Add the new log message to the collection. If the collection has grown too large
        // then remove the oldest messages.
        logMessages.Add(msg);
        while (logMessages.Count > MaximumLogMessages) logMessages.RemoveAt(0);

        // If there is a logCallback then use that to handle the log message. Otherwise simply
        // print out on the debug console.
        if (logCallback != null) logCallback(msg);
        else Debug.Log(msg);
    }
}
