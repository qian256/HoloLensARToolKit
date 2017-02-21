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
    private static string TAG = "ARUWPMarker";

    public enum MarkerType {
        single,
        single_barcode,
        single_buffer,
        multi
    }
    public MarkerType type;

    // marker options
    public bool showOptions = false;
    public bool oFiltered = false;
    public float oSampleRate = 30.0f;
    public float oCutOffFreq = 15.0f;


    // single marker
    public string singleFileName = "hiro.patt";
    // single barcode
    public int singleBarcodeID = 0;
    // single buffer
    private byte[] singleBufferBuffer = null;


    // shared by single markers
    public float singleWidth = 80.0f;
    public bool oUseContPoseEst = true;
    public float oConfCutOff = 0.5f;




    // multi
    public string multiFileName = "multi-barcode-4x3.dat";
    private int multiPatternCount = 0;
    private ARUWPPattern[] multiPatterns;
    public struct ARUWPPattern {
        public float width;
        public float height;
        public int imageSizeX;
        public int imageSizeY;
        public float[] matrix;
        public int patternID;
    }
    public int oMinSubMarkers = 0;
    public float oMinConfSubMatrix = 0.5f;
    public float oMinConfSubPattern = 0.5f;




    // visualization
    public GameObject target = null;
    public bool anchoredToWorld = true;
    public bool applyRotation = true;
    public bool applyTranslation = true;
    // visualization for single marker
    public Text confidenceTextBox = null;
    public Image confidenceColoBox = null;

    private int id = -1;

    // runtime property
    private GameObject HoloLensCamera;
    private GameObject dummyGameObject;
    private bool visible = false;
    private float confidence;
    private float[] trans = new float[12];
    private Matrix4x4 transMatrix = new Matrix4x4();
    

       

    void Start() {
        HoloLensCamera = GameObject.Find("Main Camera");
        if (HoloLensCamera == null) {
            Debug.Log(TAG + ": Main Camera does not exist in the scene");
        }
        else {
            if (!anchoredToWorld) {
                target.transform.SetParent(HoloLensCamera.transform);
            }
            else {
                dummyGameObject = new GameObject("Dummy");
                dummyGameObject.transform.SetParent(HoloLensCamera.transform);
            }
        }
        // magic function initialization
        initMagicFunction();
    }


    void Update() {
        // visualization for single markers
        if ( type != MarkerType.multi) {
            if (confidenceColoBox != null) {
                confidenceColoBox.color = new Color(0, confidence, 0, 1);
            }
            if (confidenceTextBox != null) {
                confidenceTextBox.text = string.Format("{0:0.000}", confidence);
            }
        }
        // visulization for all
        if (target != null && visible) {
            if (!anchoredToWorld) {
                if (applyRotation) {
                    target.transform.localRotation = ARUWPUtils.QuaternionFromMatrix(transMatrix);
                }
                if (applyTranslation) {
                    target.transform.localPosition = ARUWPUtils.PositionFromMatrix(transMatrix);
                }
            }
            else {
                if (applyRotation) {
                    dummyGameObject.transform.localRotation = ARUWPUtils.QuaternionFromMatrix(transMatrix);
                }
                if (applyTranslation) {
                    dummyGameObject.transform.localPosition = ARUWPUtils.PositionFromMatrix(transMatrix);
                }
                ARUWPUtils.SetMatrix4x4ToGameObject(ref target, dummyGameObject.transform.localToWorldMatrix);
            }
        }
    }



    // return true if visible
    // return false if error or invisible
    public bool updateMarkerTracking() {
        if (id != -1) {
            if (ARUWP.aruwpQueryMarkerTransformation(id, trans)) {
                transMatrix = ARUWPUtils.ConvertARUWPFloatArrayToMatrix4x4(trans);
                if (performMagicFunction) { MagicFunction(); }
                if (type != MarkerType.multi) {
                    confidence = ARUWP.aruwpGetMarkerOptionFloat(id, ARUWP.ARUWP_MARKER_OPTION_SQUARE_CONFIDENCE);
                }
                visible = true;
                return true;
            }
            else {
                visible = false;
                confidence = 0.0f;
                return false;
            }
        }
        else {
            Debug.Log(TAG + ": Transformation not valid for id -1");
            visible = false;
            return false;
        }
    }


    public bool getMarkerVisibility() {
        return visible;
    }

    public float[] getMarkerTransformationFloat() {
        return trans;
    }

    public Matrix4x4 getMarkerTransformationMatrix() {
        return transMatrix;
    }


    public void addMarker() {
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
            setAllMarkerOptions();
            retrieveMarkerInformation();
        }
        else {
            Debug.Log(TAG + ": Marker not able to add with configuration:\n" + str);
        }
    }


    public void removeMarker() {
        if (id != -1) {
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
            Debug.Log(TAG + ": Cannot remove marker with id -1");
        }
    }

    private void setAllMarkerOptions() {
        setOptionFiltered(oFiltered);
        setOptionFilterSampleRate(oSampleRate);
        setOptionFilterCutoffFreq(oCutOffFreq);
        setOptionUseContPoseEst(oUseContPoseEst);
        setOptionConfCutOff(oConfCutOff);
        setOptionMinSubMarkers(oMinSubMarkers);
        setOptionMinConfMatrix(oMinConfSubMatrix);
        setOptionMinConfPattern(oMinConfSubPattern);
    }



    private void retrieveMarkerInformation() {
        if (id != -1) {
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
            Debug.Log(TAG + ": Marker not added");
        }
    }



    #region setters at runtime
    public void setOptionFiltered(bool o) {
        if (id != -1) {
            ARUWP.aruwpSetMarkerOptionBool(id, ARUWP.ARUWP_MARKER_OPTION_FILTERED, o ? 1 : 0);
            oFiltered = ARUWP.aruwpGetMarkerOptionBool(id, ARUWP.ARUWP_MARKER_OPTION_FILTERED);
            if (oFiltered != o) {
                Debug.Log(TAG + ": Unable to set ARUWP_MARKER_OPTION_FILTERED to " + o);
            }
        }
        else {
            oFiltered = o;
        }
    }
    public void setOptionFilterSampleRate(float o) {
        if (id != -1 && oFiltered == true) {
            ARUWP.aruwpSetMarkerOptionFloat(id, ARUWP.ARUWP_MARKER_OPTION_FILTER_SAMPLE_RATE, o);
            oSampleRate = ARUWP.aruwpGetMarkerOptionFloat(id, ARUWP.ARUWP_MARKER_OPTION_FILTER_SAMPLE_RATE);
            if (oSampleRate != o) {
                Debug.Log(TAG + ": Unable to set ARUWP_MARKER_OPTION_FILTER_SAMPLE_RATE to " + o);
            }
        }
        else {
            oSampleRate = o;
        }
    }
    public void setOptionFilterCutoffFreq(float o) {
        if (id != -1 && oFiltered == true) {
            ARUWP.aruwpSetMarkerOptionFloat(id, ARUWP.ARUWP_MARKER_OPTION_FILTER_CUTOFF_FREQ, o);
            oCutOffFreq = ARUWP.aruwpGetMarkerOptionFloat(id, ARUWP.ARUWP_MARKER_OPTION_FILTER_CUTOFF_FREQ);
            if (oCutOffFreq != o) {
                Debug.Log(TAG + ": Unable to set ARUWP_MARKER_OPTION_FILTER_CUTOFF_FREQ to " + o);
            }
        }
        else {
            oCutOffFreq = o;
        }
    }
    public void setOptionUseContPoseEst(bool o) {
        if (id != -1 && type != MarkerType.multi) {
            ARUWP.aruwpSetMarkerOptionBool(id, ARUWP.ARUWP_MARKER_OPTION_SQUARE_USE_CONT_POSE_ESTIMATION, o? 1:0);
            oUseContPoseEst = ARUWP.aruwpGetMarkerOptionBool(id, ARUWP.ARUWP_MARKER_OPTION_SQUARE_USE_CONT_POSE_ESTIMATION);
            if (oUseContPoseEst != o) {
                Debug.Log(TAG + ": Unable to set ARUWP_MARKER_OPTION_SQUARE_USE_CONT_POSE_ESTIMATION to " + o);
            }
        }
        else {
            oUseContPoseEst = o;
        }
    }
    public void setOptionConfCutOff(float o) {
        if (id != -1 && type != MarkerType.multi) {
            ARUWP.aruwpSetMarkerOptionFloat(id, ARUWP.ARUWP_MARKER_OPTION_SQUARE_CONFIDENCE_CUTOFF, o);
            oConfCutOff = ARUWP.aruwpGetMarkerOptionFloat(id, ARUWP.ARUWP_MARKER_OPTION_SQUARE_CONFIDENCE_CUTOFF);
            if (oConfCutOff != o) {
                Debug.Log(TAG + ": Unable to set ARUWP_MARKER_OPTION_SQUARE_CONFIDENCE_CUTOFF to " + o);
            }
        }
        else {
            oConfCutOff = o;
        }
    }
    public void setOptionMinSubMarkers(int o) {
        if (id != -1 && type == MarkerType.multi) {
            ARUWP.aruwpSetMarkerOptionInt(id, ARUWP.ARUWP_MARKER_OPTION_MULTI_MIN_SUBMARKERS, o);
            oMinSubMarkers = ARUWP.aruwpGetMarkerOptionInt(id, ARUWP.ARUWP_MARKER_OPTION_MULTI_MIN_SUBMARKERS);
            if (oMinSubMarkers != o) {
                Debug.Log(TAG + ": Unable to set ARUWP_MARKER_OPTION_MULTI_MIN_SUBMARKERS to " + o);
            }
        }
        else {
            oMinSubMarkers = o;
        }
    }
    public void setOptionMinConfMatrix(float o) {
        if (id != -1 && type == MarkerType.multi) {
            ARUWP.aruwpSetMarkerOptionFloat(id, ARUWP.ARUWP_MARKER_OPTION_MULTI_MIN_CONF_MATRIX, o);
            oMinConfSubMatrix = ARUWP.aruwpGetMarkerOptionFloat(id, ARUWP.ARUWP_MARKER_OPTION_MULTI_MIN_CONF_MATRIX);
            if (oMinConfSubMatrix != o) {
                Debug.Log(TAG + ": Unable to set ARUWP_MARKER_OPTION_MULTI_MIN_CONF_MATRIX to " + o);
            }
        }
        else {
            oMinConfSubMatrix = o;
        }
    }
    public void setOptionMinConfPattern(float o) {
        if (id != -1 && type == MarkerType.multi) {
            ARUWP.aruwpSetMarkerOptionFloat(id, ARUWP.ARUWP_MARKER_OPTION_MULTI_MIN_CONF_PATTERN, o);
            oMinConfSubPattern = ARUWP.aruwpGetMarkerOptionFloat(id, ARUWP.ARUWP_MARKER_OPTION_MULTI_MIN_CONF_PATTERN);
            if (oMinConfSubPattern != o) {
                Debug.Log(TAG + ": Unable to set ARUWP_MARKER_OPTION_MULTI_MIN_CONF_PATTERN to " + o);
            }
        }
        else {
            oMinConfSubPattern = o;
        }
    }
    #endregion


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




    // setter and getter of the buffer of singleBuffer marker
    public void setSingleBufferBuffer(byte[] buffer) {
        singleBufferBuffer = new byte[buffer.Length];
        buffer.CopyTo(singleBufferBuffer, 0);
    }

    public byte[] getSingleBufferBuffer() {
        return singleBufferBuffer;
    }



    #region magic functions
    private Matrix4x4 magicMatrix1 = Matrix4x4.identity;
    private Matrix4x4 magicMatrix2 = Matrix4x4.identity;
    private bool performMagicFunction = true;
    public void setMagicFunction(Matrix4x4 m1, Matrix4x4 m2) {
        magicMatrix1 = m1;
        magicMatrix2 = m2;
    }

    public void enableMagicFunction(bool perf) {
        performMagicFunction = perf;
    }

    private void initMagicFunction() {
        magicMatrix1.SetRow(0, new Vector4(0.912590966818f, 0.00264415233376f, -0.0128817795355f, 0.000747730441087f));
        magicMatrix1.SetRow(1, new Vector4(-0.00522566480145f, 0.906864775491f, -0.0896724176439f, 0.0177872745822f));
        magicMatrix1.SetRow(2, new Vector4(-0.0232595319748f, 0.00845030987907f, 0.887221375125f, 0.0730807968318f));
        magicMatrix2.SetRow(0, new Vector4(0.999970522935f, 0.00515075228031f, 0.00569412074919f, 0.0f));
        magicMatrix2.SetRow(1, new Vector4(-0.00483183598788f, 0.998492806032f, -0.0546696411472f, 0.0f));
        magicMatrix2.SetRow(2, new Vector4(-0.00596712838354f, 0.0546405165891f, 0.998488260985f, 0.0f));
    }

    private void MagicFunction() {
        Vector4 vp = magicMatrix1 * transMatrix.GetColumn(3);
        transMatrix = magicMatrix2 * transMatrix;
        transMatrix.SetColumn(3, vp);
    }

    #endregion

}
