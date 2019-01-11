/*
*  ARUWPMarker.cs
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
using UnityEngine.UI;
#if !UNITY_EDITOR && UNITY_METRO
using System.Threading;
#endif

/// <summary>
/// The ARUWPMarker class represents an ARToolKit marker: single marker or 
/// multiple-pattern markers. NFT markers are not supported by this project.
/// This script should be attached to any GameObject of Unity, to be tracked.
/// Marker configuration can be set in Unity inspector, or at runtime via
/// the runtime setter functions.
///
/// Author:     Long Qian
/// Email:      lqian8@jhu.edu
/// </summary>
public class ARUWPMarker : MonoBehaviour{

    /// <summary>
    /// Class and object identifier for logging. [internal use]
    /// </summary>
    private static string TAG = "ARUWPMarker";

    /// <summary>
    /// ARUWPController instance. [internal use]
    /// </summary>
    private ARUWPController controller = null;

    /// <summary>
    /// The types of ARUWP marker. [public use]
    /// </summary>
    public enum MarkerType {
        /// <summary>
        /// Single pattern marker like Hiro and Kanji marker, described by a file
        /// </summary>
        single,
        /// <summary>
        /// Single barcode/matrix code marker, described by an ID
        /// </summary>
        single_barcode,
        /// <summary>
        /// Single pattern marker like Hiro and Kanji marker, but described by a byte buffer 
        /// instead of a file.
        /// </summary>
        single_buffer,
        /// <summary>
        /// Multi-marker consisting several pattern markers or several matrix code markers,
        /// described by a configuration file.
        /// </summary>
        multi
    }

    /// <summary>
    /// Type of the current marker. [public use]
    /// </summary>
    public MarkerType type;

    /// <summary>
    /// Toggle Unity Inspector to show or hide more options of the marker. [editor use]
    /// </summary>
    public bool showOptions = false;

    /// <summary>
    /// Initial value representing the marker pose result is filter. Toggle Unity inspector
    /// to show options about filtering.
    /// At runtime, please use SetOptionFiltered() to modify the value. [public use] 
    /// [initialization only] [editor use]
    /// </summary>
    public bool oFiltered = false;

    /// <summary>
    /// Initial value representing the sample rate of the marker.
    /// At runtime, please use SetOptionFilterSampleRate() to modify the value. [public use] 
    /// [initialization only]
    /// </summary>
    public float oSampleRate = 30.0f;

    /// <summary>
    /// Initial value representing the cutoff frequency of the marker.
    /// At runtime, please use SetOptionFilterCutoffFreq() to modify the value [public use] 
    /// [initialization only]
    /// </summary>
    public float oCutOffFreq = 15.0f;


    /// <summary>
    /// Indicate the filename of the pattern marker description. Useful when marker type is single.
    /// [public use] [initialization only]
    /// </summary>
    public string singleFileName = "hiro.patt";

    /// <summary>
    /// Indicate the barcode ID of the barcode marker. Useful when the marker type is single_barcode.
    /// [public use] [initialization only]
    /// </summary>
    public int singleBarcodeID = 0;

    /// <summary>
    /// Indicate the buffer of pattern marker description. Useful when the marker type is single_buffer.
    /// The buffer should be set using SetSingleBufferBuffer() and get using GetSingleBufferBuffer().
    /// [internal use]
    /// </summary>
    private byte[] singleBufferBuffer = null;


    /// <summary>
    /// Initial value of the width of the marker. Useful when the marker type is single, single_barcode 
    /// and single_buffer. [public use] [initialization only]
    /// </summary>
    public float singleWidth = 80.0f;

    /// <summary>
    /// Initial value of whether use continous pose estimation. At runtime, please use SetOptionUseContPoseEst() 
    /// to modify the value. [public use] [initialization only]
    /// </summary>
    public bool oUseContPoseEst = true;

    /// <summary>
    /// Initial value of the cutoff confidence of tracking single markers. At runtime, please use 
    /// SetOptionConfCutOff() to modify the value. [public use] [initialization only]
    /// </summary>
    public float oConfCutOff = 0.5f;


    /// <summary>
    /// Indicate the filename of multi-marker configuration. Useful when marker type is multi.
    /// [public use] [initialization only]
    /// </summary>
    public string multiFileName = "multi-barcode-4x3.dat";

    /// <summary>
    /// The number of single markers in the multi-marker. [internal use]
    /// </summary>
    private int multiPatternCount = 0;

    /// <summary>
    /// The array of ARUWPPatterns contained in the multi-marker. [internal use]
    /// </summary>
    private ARUWPPattern[] multiPatterns;

    /// <summary>
    /// The definition of ARUWPPattern, representing single markers in a multi-marker.
    /// TODO: elaborate this structure.
    /// </summary>
    public struct ARUWPPattern {
        public float width;
        public float height;
        public int imageSizeX;
        public int imageSizeY;
        public float[] matrix;
        public int patternID;
    }

    /// <summary>
    /// Initial value of the number of sub-markers that needs to be tracked in order to determine
    /// the visibility of the multi-marker. At runtime, please use SetOptionMinSubMarkers()
    /// to modify the value. [public use] [initialization only]
    /// </summary>
    public int oMinSubMarkers = 0;

    /// <summary>
    /// Initial value of the minimum confidence value of sub matrix marker.
    /// At runtime, please use SetOptionMinConfMatrix() to modify the value. [public use] 
    /// [initialization only]
    /// </summary>
    public float oMinConfSubMatrix = 0.5f;

    /// <summary>
    /// Initial value of the minimum confidence value of sub pattern marker.
    /// At runtime, please use SetOptionMinConfPattern() to modify the value. [public use] 
    /// [initialization only]
    /// </summary>
    public float oMinConfSubPattern = 0.5f;



    /// <summary>
    /// The target Unity GameObject to visualize the tracking result of the current marker.
    /// Usually, it is a "Scene root" that has one or multiple GameObject in the scene.
    /// [public use]
    /// </summary>
    public GameObject target = null;

    /// <summary>
    /// Initial value of anchoring behavior of the target scene root. If this parameter is set
    /// to true, then when the marker is not visible, the scene root associated with the 
    /// tracking result will be fixed w.r.t the world coordinates. It takes advantage of
    /// the HoloLens self-localization to achieve the result. If it is set to false, then 
    /// the scene root will be fixed w.r.t. user's view (fixed on screen). [public use]
    /// </summary>
    public bool anchoredToWorld = true;

    /// <summary>
    /// Initial value to indicate whether the scene root takes the rotational part of the 
    /// tracking result. [public use]
    /// </summary>
    public bool applyRotation = true;

    /// <summary>
    /// Initial value to indicate whether the scene root takes the translational part of the 
    /// tracking result. [public use]
    /// </summary>
    public bool applyTranslation = true;

    /// <summary>
    /// The ID of the marker assigned by the controller, acting as a unique identifier of the
    /// current marker. If it is -1, then this marker is not added to the tracking controller.
    /// [internal use]
    /// </summary>
    private int id = -1;

    /// <summary>
    /// GameObject representing the "Main Camera" of the scene. [internal use]
    /// </summary>
    private GameObject holoLensCamera;

    /// <summary>
    /// GameObject that is temporarily used to hold transformation. [internal use]
    /// </summary>
    private GameObject dummyGameObject;

    /// <summary>
    /// Class representing the tracking information. [public use]
    /// </summary>
    class ARUWPMarkerTrackingInfo {
        /// <summary>
        /// Visibility of the marker.
        /// </summary>
        public bool visible = false;
        /// <summary>
        /// Confidence of tracking if the marker type is single, or single_buffer
        /// </summary>
        public float confidence = 0.0f;
        /// <summary>
        /// Tracking transformation returned by ARToolKitUWP.dll. Note that this field is
        /// temporary, and not useable in Unity. To acquire the transformation, please
        /// use latestTransMatrix and GetMarkerTransformationMatrix() instead.
        /// </summary>
        public float[] trans = new float[12];
    }

    /// <summary>
    /// The most recent tracking information of the current marker. [internal use]
    /// </summary>
    private ARUWPMarkerTrackingInfo latestTrackingInfo;

    /// <summary>
    /// Temporary tracking information holder, in order to share memory between video thread
    /// and Unity UI thread. [internal use]
    /// </summary>
    private ARUWPMarkerTrackingInfo _info = new ARUWPMarkerTrackingInfo();

    /// <summary>
    /// Temporary tracking information holder that get tracking information in video thread.
    /// [internal use]
    /// </summary>
    private ARUWPMarkerTrackingInfo __info = new ARUWPMarkerTrackingInfo();

    /// <summary>
    /// The most recent transformation of the current marker. Please use 
    /// GetMarkerTransformationMatrix() to query it. [internal use]
    /// </summary>
    private Matrix4x4 latestTransMatrix = new Matrix4x4();

    /// <summary>
    /// Signal indicating the tracking information has been changed in the previous render
    /// loop, between last Update() and this Update(). [internal use]
    /// </summary>
    private bool signalTrackingUpdated = false;


    /// <summary>
    /// Set the buffer of pattern marker description for initialization of the marker.
    /// This function should be called before marker is added to the contoller. Useful
    /// only when the type of the marker is single_buffer. [public use]
    /// </summary>
    /// <param name="buffer">Input buffer holding pattern marker description.</param>
    public void SetSingleBufferBuffer(byte[] buffer) {
        singleBufferBuffer = new byte[buffer.Length];
        buffer.CopyTo(singleBufferBuffer, 0);
    }

    /// <summary>
    /// Retrieve the pattern marker description. It is useful when the type of the marker
    /// is single_buffer and the singleBufferBuffer is set. [public use]
    /// </summary>
    /// <returns>singleBufferBuffer</returns>
    public byte[] GetSingleBufferBuffer() {
        return singleBufferBuffer;
    }

    /// <summary>
    /// Retrieve the visibility of the marker depending on the latest tracking information. 
    /// [public use]
    /// </summary>
    /// <returns>The latest visibility</returns>
    public bool GetMarkerVisibility() {
        return latestTrackingInfo.visible;
    }

    /// <summary>
    /// Retrieve the confidence of the marker tracking depending on the latest tracking information. 
    /// It is useful when the marker type is single or single_buffer. [public use]
    /// </summary>
    /// <returns>Confidence value in the scale of 0-1</returns>
    public float GetMarkerConfidence() {
        return latestTrackingInfo.confidence;
    }

    /// <summary>
    /// Retrieve the transformation matrix of the marker depending on the latest tracking
    /// information. It is correctly adapted to Unity left-hand coordinate system already.
    /// </summary>
    /// <returns>Matrix4x4 object representing the transformation</returns>
    public Matrix4x4 GetMarkerTransformationMatrix() {
        return latestTransMatrix;
    }

#if !UNITY_EDITOR && UNITY_METRO


    /// <summary>
    /// Unity Monobehavior function. ARUWPController, holoLensCamera are set here. Magic function
    /// is initialized. [internal use]
    /// </summary>
    private void Start() {
        controller = GetComponent<ARUWPController>();
        if (controller == null) {
            Debug.Log(TAG + ": not able to find ARUWPController");
            Application.Quit();
        }
        holoLensCamera = GameObject.Find("Main Camera");
        if (holoLensCamera == null) {
            Debug.Log(TAG + ": Main Camera does not exist in the scene");
            Application.Quit();
        }

        else {
            if (!anchoredToWorld) {
                target.transform.SetParent(holoLensCamera.transform);
            }
            else {
                // Create dummy object with unique object ID for different markers
                dummyGameObject = new GameObject("Dummy" + GetInstanceID());
                dummyGameObject.transform.SetParent(controller.LocatableCameraRoot.transform);
            }
        }
        // magic function initialization
        InitMagicFunction();
    }


    /// <summary>
    /// Unity Monobehavior function: update tracking information of the current marker.
    /// If magic function is enabled, then a distortion transformation is applied on top
    /// of the tracking result. [internal use]
    /// </summary>
    private void Update() {
        if (signalTrackingUpdated && id != -1) {
            Interlocked.Exchange(ref latestTrackingInfo, _info);
            if ( latestTrackingInfo.visible) {
                latestTransMatrix = ARUWPUtils.ConvertARUWPFloatArrayToMatrix4x4(latestTrackingInfo.trans);

                if (performMagicFunction) { MagicFunction(); }

                if (target != null)
                {
                    if (!anchoredToWorld)
                    {
                        if (applyRotation)
                        {
                            target.transform.localRotation = ARUWPUtils.QuaternionFromMatrix(latestTransMatrix);
                        }
                        if (applyTranslation)
                        {
                            target.transform.localPosition = ARUWPUtils.PositionFromMatrix(latestTransMatrix);
                        }
                    }
                    else
                    {
                        if (applyRotation)
                        {
                            dummyGameObject.transform.localRotation = ARUWPUtils.QuaternionFromMatrix(latestTransMatrix);
                        }
                        if (applyTranslation)
                        {
                            dummyGameObject.transform.localPosition = ARUWPUtils.PositionFromMatrix(latestTransMatrix);
                        }
                        ARUWPUtils.SetMatrix4x4ToGameObject(ref target, dummyGameObject.transform.localToWorldMatrix);
                    }
                }
            }
        }

        signalTrackingUpdated = false;
    }

    


    /// <summary>
    /// Update tracking information, called by ARUWPController.cs. [internal use]
    /// </summary>
    public void UpdateTrackingInfo() {
        if (id != -1) {
            if (ARUWP.aruwpQueryMarkerTransformation(id, __info.trans)) {
                if (type != MarkerType.multi) {
                    __info.confidence = ARUWP.aruwpGetMarkerOptionFloat(id, ARUWP.ARUWP_MARKER_OPTION_SQUARE_CONFIDENCE);
                }
                __info.visible = true;
            }
            else {
                __info.visible = false;
                __info.confidence = 0.0f;
            }
            Interlocked.Exchange(ref _info, __info);
            signalTrackingUpdated = true;
        }
        else {
            Debug.Log(TAG + ": Transformation not valid for id -1");
        }
    }


    /// <summary>
    /// Add this marker to the controller, so that it will be tracked. This function is
    /// called by ARUWPController.cs. [internal use]
    /// </summary>
    public void AddMarker() {
        if (HasNativeHandle()) {
            string str = "";
            switch (type) {
                case MarkerType.single:
                    str = "single;Data/StreamingAssets/" + singleFileName + ";" + singleWidth;
                    break;
                case MarkerType.single_barcode:
                    str = "single_barcode;" + singleBarcodeID + ";" + singleWidth;
                    break;
                case MarkerType.single_buffer:
                    if (singleBufferBuffer == null) {
                        Debug.Log(TAG + ": Marker buffer is not set\n");
                        return;
                    }
                    str = "single_buffer;" + singleWidth + ";buffer=" + singleBufferBuffer;
                    break;
                case MarkerType.multi:
                    str = "multi;Data/StreamingAssets/" + multiFileName;
                    break;
            }
            id = ARUWP.aruwpAddMarker(str);
            if (id != -1) {
                ARUWPController.markers.Add(id, this);
                Debug.Log(TAG + ": Marker added with UID: " + id);
                SetOptionFiltered(oFiltered);
                SetOptionFilterSampleRate(oSampleRate);
                SetOptionFilterCutoffFreq(oCutOffFreq);
                SetOptionUseContPoseEst(oUseContPoseEst);
                SetOptionConfCutOff(oConfCutOff);
                SetOptionMinSubMarkers(oMinSubMarkers);
                SetOptionMinConfMatrix(oMinConfSubMatrix);
                SetOptionMinConfPattern(oMinConfSubPattern);
                RetrieveMarkerInformation();
            }
            else {
                Debug.Log(TAG + ": Marker not able to add with configuration:\n" + str);
            }
        }
        else {
            Debug.Log(TAG + ": AddMarker() unsupported status");
        }
    }


    /// <summary>
    /// Remove the current marker from the controller, called by ARUWPController.cs. 
    /// [internal use]
    /// </summary>
    public void RemoveMarker() {
        if (id != -1) {
            if (HasNativeHandle()) {
                if (ARUWP.aruwpRemoveMarker(id)) {
                    Debug.Log(TAG + ": Marker removed with UID " + id);
                    ARUWPController.markers.Remove(id);
                    id = -1;
                }
                else {
                    Debug.Log(TAG + ": Cannot remove marker with UID " + id);
                }
            }
            else {
                Debug.Log(TAG + ": RemoveMarker() unsupported status");
            }
        }
        else {
            Debug.Log(TAG + ": Cannot remove marker with id -1");
        }
    }

    /// <summary>
    /// Retrieve the marker information from the controller. The result represents the actual
    /// configuration of the marker being tracked, because some of the setting might not be 
    /// taken for various reasons. [internal use]
    /// </summary>
    private void RetrieveMarkerInformation() {
        if (id != -1) {
            if (HasNativeHandle()) {
                oFiltered = ARUWP.aruwpGetMarkerOptionBool(id, ARUWP.ARUWP_MARKER_OPTION_FILTERED);
                if (oFiltered) {
                    oSampleRate = ARUWP.aruwpGetMarkerOptionFloat(id, ARUWP.ARUWP_MARKER_OPTION_FILTER_SAMPLE_RATE);
                    oCutOffFreq = ARUWP.aruwpGetMarkerOptionFloat(id, ARUWP.ARUWP_MARKER_OPTION_FILTER_CUTOFF_FREQ);
                }
                if (type != MarkerType.multi) {
                    oUseContPoseEst = ARUWP.aruwpGetMarkerOptionBool(id, ARUWP.ARUWP_MARKER_OPTION_SQUARE_USE_CONT_POSE_ESTIMATION);
                    oConfCutOff = ARUWP.aruwpGetMarkerOptionFloat(id, ARUWP.ARUWP_MARKER_OPTION_SQUARE_CONFIDENCE_CUTOFF);
                }
                else {
                    multiPatternCount = ARUWP.aruwpGetMarkerPatternCount(id);
                    oMinSubMarkers = ARUWP.aruwpGetMarkerOptionInt(id, ARUWP.ARUWP_MARKER_OPTION_MULTI_MIN_SUBMARKERS);
                    oMinConfSubMatrix = ARUWP.aruwpGetMarkerOptionFloat(id, ARUWP.ARUWP_MARKER_OPTION_MULTI_MIN_CONF_MATRIX);
                    oMinConfSubPattern = ARUWP.aruwpGetMarkerOptionFloat(id, ARUWP.ARUWP_MARKER_OPTION_MULTI_MIN_CONF_PATTERN);
                    multiPatterns = new ARUWPPattern[multiPatternCount];
                    for (int i = 0; i < multiPatternCount; i++) {
                        multiPatterns[i].patternID = i;
                        multiPatterns[i].matrix = new float[16];
                        ARUWP.aruwpGetMarkerPatternConfig(id, i, multiPatterns[i].matrix,
                            out multiPatterns[i].width, out multiPatterns[i].height, out multiPatterns[i].imageSizeX, out multiPatterns[i].imageSizeY);
                    }
                }
            }
            else {
                Debug.Log(TAG + ": RetrieveMarkerInformation() unsupported status");
            }
        }
        else {
            Debug.Log(TAG + ": Marker not added");
        }
    }



    #region setters at runtime


    /// <summary>
    /// Set oFilted parameter at runtime. [public use]
    /// </summary>
    /// <param name="o">New parameter</param>
    public void SetOptionFiltered(bool o) {
        if (id != -1) {
            if (HasNativeHandle()) {
                ARUWP.aruwpSetMarkerOptionBool(id, ARUWP.ARUWP_MARKER_OPTION_FILTERED, o ? 1 : 0);
                oFiltered = ARUWP.aruwpGetMarkerOptionBool(id, ARUWP.ARUWP_MARKER_OPTION_FILTERED);
                if (oFiltered != o) {
                    Debug.Log(TAG + ": Unable to set ARUWP_MARKER_OPTION_FILTERED to " + o);
                }
            }
            else {
                Debug.Log(TAG + ": SetOptionFiltered() unsupported status");
            }
        }
        else {
            oFiltered = o;
        }
    }

    /// <summary>
    /// Set oSampleRate parameter at runtime. [public use]
    /// </summary>
    /// <param name="o">New parameter</param>
    public void SetOptionFilterSampleRate(float o) {
        if (id != -1 && oFiltered == true) {
            if (HasNativeHandle()) {
                ARUWP.aruwpSetMarkerOptionFloat(id, ARUWP.ARUWP_MARKER_OPTION_FILTER_SAMPLE_RATE, o);
                oSampleRate = ARUWP.aruwpGetMarkerOptionFloat(id, ARUWP.ARUWP_MARKER_OPTION_FILTER_SAMPLE_RATE);
                if (oSampleRate != o) {
                    Debug.Log(TAG + ": Unable to set ARUWP_MARKER_OPTION_FILTER_SAMPLE_RATE to " + o);
                }
            }
            else {
                Debug.Log(TAG + ": SetOptionFilterSampleRate() unsupported status");
            }
        }
        else {
            oSampleRate = o;
        }
    }

    /// <summary>
    /// Set oCutOffFreq parameter at runtime. [public use]
    /// </summary>
    /// <param name="o">New parameter</param>
    public void SetOptionFilterCutoffFreq(float o) {
        if (id != -1 && oFiltered == true) {
            if (HasNativeHandle()) {
                ARUWP.aruwpSetMarkerOptionFloat(id, ARUWP.ARUWP_MARKER_OPTION_FILTER_CUTOFF_FREQ, o);
                oCutOffFreq = ARUWP.aruwpGetMarkerOptionFloat(id, ARUWP.ARUWP_MARKER_OPTION_FILTER_CUTOFF_FREQ);
                if (oCutOffFreq != o) {
                    Debug.Log(TAG + ": Unable to set ARUWP_MARKER_OPTION_FILTER_CUTOFF_FREQ to " + o);
                }
            }
            else {
                Debug.Log(TAG + ": SetOptionFilterCutoffFreq() unsupported status");
            }
        }
        else {
            oCutOffFreq = o;
        }
    }

    /// <summary>
    /// Set oUseContPoseEst parameter at runtime. [public use]
    /// </summary>
    /// <param name="o">New parameter</param>
    public void SetOptionUseContPoseEst(bool o) {
        if (id != -1 && type != MarkerType.multi) {
            if (HasNativeHandle()) {
                ARUWP.aruwpSetMarkerOptionBool(id, ARUWP.ARUWP_MARKER_OPTION_SQUARE_USE_CONT_POSE_ESTIMATION, o? 1:0);
                oUseContPoseEst = ARUWP.aruwpGetMarkerOptionBool(id, ARUWP.ARUWP_MARKER_OPTION_SQUARE_USE_CONT_POSE_ESTIMATION);
                if (oUseContPoseEst != o) {
                    Debug.Log(TAG + ": Unable to set ARUWP_MARKER_OPTION_SQUARE_USE_CONT_POSE_ESTIMATION to " + o);
                }
            }
            else {
                Debug.Log(TAG + ": SetOptionUseContPoseEst() unsupported status");
            }
        }
        else {
            oUseContPoseEst = o;
        }
    }

    /// <summary>
    /// Set oConfCutOff parameter at runtime. [public use]
    /// </summary>
    /// <param name="o">New parameter</param>
    public void SetOptionConfCutOff(float o) {
        if (id != -1 && type != MarkerType.multi) {
            if (HasNativeHandle()) {
                ARUWP.aruwpSetMarkerOptionFloat(id, ARUWP.ARUWP_MARKER_OPTION_SQUARE_CONFIDENCE_CUTOFF, o);
                oConfCutOff = ARUWP.aruwpGetMarkerOptionFloat(id, ARUWP.ARUWP_MARKER_OPTION_SQUARE_CONFIDENCE_CUTOFF);
                if (oConfCutOff != o) {
                    Debug.Log(TAG + ": Unable to set ARUWP_MARKER_OPTION_SQUARE_CONFIDENCE_CUTOFF to " + o);
                }
            }
            else {
                Debug.Log(TAG + ": SetOptionConfCutOff() unsupported status");
            }
        }
        else {
            oConfCutOff = o;
        }
    }

    /// <summary>
    /// Set oMinSubMarkers parameter at runtime. [public use]
    /// </summary>
    /// <param name="o">New parameter</param>
    public void SetOptionMinSubMarkers(int o) {
        if (id != -1 && type == MarkerType.multi) {
            if (HasNativeHandle()) {
                ARUWP.aruwpSetMarkerOptionInt(id, ARUWP.ARUWP_MARKER_OPTION_MULTI_MIN_SUBMARKERS, o);
                oMinSubMarkers = ARUWP.aruwpGetMarkerOptionInt(id, ARUWP.ARUWP_MARKER_OPTION_MULTI_MIN_SUBMARKERS);
                if (oMinSubMarkers != o) {
                    Debug.Log(TAG + ": Unable to set ARUWP_MARKER_OPTION_MULTI_MIN_SUBMARKERS to " + o);
                }
            }
            else {
                Debug.Log(TAG + ": SetOptionMinSubMarkers() unsupported status");
            }
        }
        else {
            oMinSubMarkers = o;
        }
    }

    /// <summary>
    /// Set oMinConfSubMatrix parameter at runtime. [public use]
    /// </summary>
    /// <param name="o">New parameter</param>
    public void SetOptionMinConfMatrix(float o) {
        if (id != -1 && type == MarkerType.multi) {
            if (HasNativeHandle()) {
                ARUWP.aruwpSetMarkerOptionFloat(id, ARUWP.ARUWP_MARKER_OPTION_MULTI_MIN_CONF_MATRIX, o);
                oMinConfSubMatrix = ARUWP.aruwpGetMarkerOptionFloat(id, ARUWP.ARUWP_MARKER_OPTION_MULTI_MIN_CONF_MATRIX);
                if (oMinConfSubMatrix != o) {
                    Debug.Log(TAG + ": Unable to set ARUWP_MARKER_OPTION_MULTI_MIN_CONF_MATRIX to " + o);
                }
            }
            else {
                Debug.Log(TAG + ": SetOptionMinConfMatrix() unsupported status");
            }
        }
        else {
            oMinConfSubMatrix = o;
        }
    }

    /// <summary>
    /// Set oMinConfSubPattern parameter at runtime. [public use]
    /// </summary>
    /// <param name="o">New parameter</param>
    public void SetOptionMinConfPattern(float o) {
        if (id != -1 && type == MarkerType.multi) {
            if (HasNativeHandle()) {
                ARUWP.aruwpSetMarkerOptionFloat(id, ARUWP.ARUWP_MARKER_OPTION_MULTI_MIN_CONF_PATTERN, o);
                oMinConfSubPattern = ARUWP.aruwpGetMarkerOptionFloat(id, ARUWP.ARUWP_MARKER_OPTION_MULTI_MIN_CONF_PATTERN);
                if (oMinConfSubPattern != o) {
                    Debug.Log(TAG + ": Unable to set ARUWP_MARKER_OPTION_MULTI_MIN_CONF_PATTERN to " + o);
                }
            }
            else {
                Debug.Log(TAG + ": SetOptionMinConfPattern() unsupported status");
            }
        }
        else {
            oMinConfSubPattern = o;
        }
    }
    #endregion

    /// <summary>
    /// Logging current marker information using Debug.Log. [public use]
    /// </summary>
    public void LogMarkerInformation() {
        bool verbose = true;
        string str = TAG + ": LogMarkerInformation\n";
        str += "Marker UID: " + id + "\n";
        str += "Marker type: " + type.ToString() + "\n";
        switch (type) {
            case MarkerType.single:
                str += "Marker filename: " + singleFileName + "\n";
                str += "Marker size: " + singleWidth + "\n";
                break;
            case MarkerType.single_barcode:
                str += "Marker barcode ID: " + singleBarcodeID + "\n";
                str += "Marker size: " + singleWidth + "\n";
                break;
            case MarkerType.single_buffer:
                str += "Marker size: " + singleWidth + "\n";
                break;
            case MarkerType.multi:
                str += "Marker filename: " + multiFileName + "\n";
                break;
        }
        str += "Marker filtered: " + oFiltered + "\n";
        if (oFiltered) {
            str += "Marker filtered sample rate: " + oSampleRate + "\n";
            str += "Marker filtered cutoff frequency: " + oCutOffFreq + "\n";
        }
        // single marker
        if (type != MarkerType.multi) {
            str += "Marker continuous pose estimation: " + oUseContPoseEst + "\n";
            str += "Marker confidence cutoff: " + oConfCutOff + "\n";
        }
        else {
            str += "Marker patterns: " + multiPatternCount + "\n";
            str += "Marker minimum submarkers: " + oMinSubMarkers + "\n";
            str += "Marker confidence value for submarker valid matrix tracking: " + oMinConfSubMatrix + "\n";
            str += "Marker confidence value for submarker valid pattern tracking: " + oMinConfSubPattern + "\n";
            if (verbose) {
                for (int i = 0; i < multiPatternCount; i++) {
                    str += "Marker subpattern[" + i + "]: size: " + multiPatterns[i].width + "x" + multiPatterns[i].height + "\n";
                    str += "Marker subpattern[" + i + "]: image size: " + multiPatterns[i].imageSizeX + "x" + multiPatterns[i].imageSizeY + "\n";
                    str += string.Format("{0:0.0000}, {1:0.0000}, {2:0.0000}, {3:0.0000}\n", 
                        multiPatterns[i].matrix[0], multiPatterns[i].matrix[4], multiPatterns[i].matrix[8], multiPatterns[i].matrix[12]);
                    str += string.Format("{0:0.0000}, {1:0.0000}, {2:0.0000}, {3:0.0000}\n", 
                        multiPatterns[i].matrix[1], multiPatterns[i].matrix[5], multiPatterns[i].matrix[9], multiPatterns[i].matrix[13]);
                    str += string.Format("{0:0.0000}, {1:0.0000}, {2:0.0000}, {3:0.0000}\n", 
                        multiPatterns[i].matrix[2], multiPatterns[i].matrix[6], multiPatterns[i].matrix[10], multiPatterns[i].matrix[14]);
                    str += string.Format("{0:0.0000}, {1:0.0000}, {2:0.0000}, {3:0.0000}\n", 
                        multiPatterns[i].matrix[3], multiPatterns[i].matrix[7], multiPatterns[i].matrix[11], multiPatterns[i].matrix[15]);
                }
            }
        }
        Debug.Log(str);
    }



    /// <summary>
    /// Determine whether the access to the native code, ARToolKitUWP.dll is given to the marker.
    /// Only when the ARUWPController status is ARUWP_STATUS_VIDEO_INITIALIZED or 
    /// ARUWP_STATUS_CTRL_INITIALIZED, the unity thread is able to call native functions. 
    /// When a call to native function is used, it is recommended to check the access handle first, 
    /// in order to avoid shared memory issue. [internal use]
    /// </summary>
    /// <returns>Whether the script can call native functions</returns>
    private bool HasNativeHandle() {
        var t_status = controller.status;
        return (t_status == ARUWP.ARUWP_STATUS_VIDEO_INITIALIZED ||
            t_status == ARUWP.ARUWP_STATUS_CTRL_INITIALIZED);
    }



#endif

    #region magic functions

    /// <summary>
    /// magicMatrix1 represents the distortion applied to the translational part of the
    /// tracking result. [public use]
    /// </summary>
    public Matrix4x4 magicMatrix1 = Matrix4x4.identity;

    /// <summary>
    /// magicMatrix1 represents the distortion applied to the rotational part of the
    /// tracking result. [public use]
    /// </summary>
    public Matrix4x4 magicMatrix2 = Matrix4x4.identity;

    /// <summary>
    /// Indicate whether the distortion of tracking result should be applied. [public use]
    /// </summary>
    public bool performMagicFunction = false;
    

    /// <summary>
    /// Initial value of magicMatrix1 and magicMatrix2. This result is obtained from a calibration
    /// procedure. Users can modify these values to better align the visualization target and the
    /// actual marker. [internal use]
    /// </summary>
    private void InitMagicFunction() {
        magicMatrix1.SetRow(0, new Vector4(0.912590966818f, 0.00264415233376f, -0.0128817795355f, 0.000747730441087f));
        magicMatrix1.SetRow(1, new Vector4(-0.00522566480145f, 0.906864775491f, -0.0896724176439f, 0.0177872745822f));
        magicMatrix1.SetRow(2, new Vector4(-0.0232595319748f, 0.00845030987907f, 0.887221375125f, 0.0730807968318f));
        magicMatrix2.SetRow(0, new Vector4(0.999970522935f, 0.00515075228031f, 0.00569412074919f, 0.0f));
        magicMatrix2.SetRow(1, new Vector4(-0.00483183598788f, 0.998492806032f, -0.0546696411472f, 0.0f));
        magicMatrix2.SetRow(2, new Vector4(-0.00596712838354f, 0.0546405165891f, 0.998488260985f, 0.0f));
    }

    /// <summary>
    /// Perform the magic function, add distortion to the tracking result matrix. [internal use]
    /// </summary>
    private void MagicFunction() {
        if (performMagicFunction) {
            Vector4 vp = magicMatrix1 * latestTransMatrix.GetColumn(3);
            latestTransMatrix = magicMatrix2 * latestTransMatrix;
            latestTransMatrix.SetColumn(3, vp);
        }
    }

    #endregion

}
