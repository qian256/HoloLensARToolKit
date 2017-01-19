using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

/// <summary>
/// The ARUWPMarkerEditor class controles the Unity inspector options of
/// each ARUWPMarker object, depending on the marker type and user options.
///
/// Author:     Long Qian
/// Email:      lqian8@jhu.edu
/// </summary>
[CustomEditor(typeof(ARUWPMarker))]
[CanEditMultipleObjects]
public class ARUWPMarkerEditor : Editor {
    public SerializedProperty type_Prop,
        singleFileName_Prop,
        singleWidth_Prop,
        singleBarcodeID_Prop,
        multiFileName_Prop,
        showOptions_Prop,
        oFiltered_Prop,
        oSampleRate_Prop,
        oCutOffFreq_Prop,
        oUseContPoseEst_Prop,
        oConfCutOff_Prop,
        oMinSubMarkers_Prop,
        oMinConfSubMatrix_Prop,
        oMinConfSubPattern_Prop,
        target_Prop,
        applyRotation_Prop,
        applyTranslation_Prop,
        confidenceTextBox_Prop,
        confidenceColoBox_Prop;

    
    void OnEnable() {
        type_Prop = serializedObject.FindProperty("type");
        singleFileName_Prop = serializedObject.FindProperty("singleFileName");
        singleWidth_Prop = serializedObject.FindProperty("singleWidth");
        singleBarcodeID_Prop = serializedObject.FindProperty("singleBarcodeID");
        multiFileName_Prop = serializedObject.FindProperty("multiFileName");
        showOptions_Prop = serializedObject.FindProperty("showOptions");
        oFiltered_Prop = serializedObject.FindProperty("oFiltered");
        oSampleRate_Prop = serializedObject.FindProperty("oSampleRate");
        oCutOffFreq_Prop = serializedObject.FindProperty("oCutOffFreq");
        oUseContPoseEst_Prop = serializedObject.FindProperty("oUseContPoseEst");
        oConfCutOff_Prop = serializedObject.FindProperty("oConfCutOff");
        oMinSubMarkers_Prop = serializedObject.FindProperty("oMinSubMarkers");
        oMinConfSubMatrix_Prop = serializedObject.FindProperty("oMinConfSubMatrix");
        oMinConfSubPattern_Prop = serializedObject.FindProperty("oMinConfSubPattern");
        target_Prop = serializedObject.FindProperty("target");
        applyRotation_Prop = serializedObject.FindProperty("applyRotation");
        applyTranslation_Prop = serializedObject.FindProperty("applyTranslation");
        confidenceTextBox_Prop = serializedObject.FindProperty("confidenceTextBox");
        confidenceColoBox_Prop = serializedObject.FindProperty("confidenceColoBox");
    }


    public override void OnInspectorGUI() {
        serializedObject.Update();
        EditorGUILayout.PropertyField(type_Prop);

        ARUWPMarker.MarkerType type = (ARUWPMarker.MarkerType)type_Prop.enumValueIndex;

        switch (type) {
            case ARUWPMarker.MarkerType.single:
                EditorGUILayout.PropertyField(singleFileName_Prop, new GUIContent("File Name"));
                EditorGUILayout.PropertyField(singleWidth_Prop, new GUIContent("Size in mm"));
                break;

            case ARUWPMarker.MarkerType.single_barcode:
                EditorGUILayout.PropertyField(singleBarcodeID_Prop, new GUIContent("Barcode ID"));
                EditorGUILayout.PropertyField(singleWidth_Prop, new GUIContent("Size in mm"));
                break;

            case ARUWPMarker.MarkerType.single_buffer:
                EditorGUILayout.PropertyField(singleWidth_Prop, new GUIContent("Size in mm"));
                break;

            case ARUWPMarker.MarkerType.multi:
                EditorGUILayout.PropertyField(multiFileName_Prop, new GUIContent("File Name"));
                break;
        }

        EditorGUILayout.PropertyField(showOptions_Prop, new GUIContent("Show options"));
        bool showOptions = showOptions_Prop.boolValue;
        if (showOptions) {
            EditorGUILayout.PropertyField(oFiltered_Prop, new GUIContent("Filtered"));
            bool oFiltered = oFiltered_Prop.boolValue;
            if (oFiltered) {
                EditorGUILayout.PropertyField(oSampleRate_Prop, new GUIContent("Sample rate"));
                EditorGUILayout.PropertyField(oCutOffFreq_Prop, new GUIContent("Cutoff frequency"));
            }
            // single markers
            if (type != ARUWPMarker.MarkerType.multi) {
                EditorGUILayout.PropertyField(oUseContPoseEst_Prop, new GUIContent("Continuous Pose Estimation"));
                EditorGUILayout.PropertyField(oConfCutOff_Prop, new GUIContent("Confidence cutoff"));
            }
            else {
                EditorGUILayout.PropertyField(oMinSubMarkers_Prop, new GUIContent("Minimum sub markers"));
                EditorGUILayout.PropertyField(oMinConfSubMatrix_Prop, new GUIContent("Minumum sub matrix confidence"));
                EditorGUILayout.PropertyField(oMinConfSubPattern_Prop, new GUIContent("Minumum sub pattern confidence"));
            }
        }
        EditorGUILayout.PropertyField(target_Prop, new GUIContent("Visualization target"));
        EditorGUILayout.PropertyField(applyRotation_Prop, new GUIContent("Apply rotation"));
        EditorGUILayout.PropertyField(applyTranslation_Prop, new GUIContent("Apply translation"));

        if (type != ARUWPMarker.MarkerType.multi) {
            EditorGUILayout.PropertyField(confidenceTextBox_Prop, new GUIContent("Confidence Textbox"));
            EditorGUILayout.PropertyField(confidenceColoBox_Prop, new GUIContent("Confidence Colorbox"));
        }

        serializedObject.ApplyModifiedProperties();
    }


}
