/*
*  ARUWPMarkerEditor.cs
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
        anchoredToWorld_Prop,
        applyRotation_Prop,
        applyTranslation_Prop,
        magicFunction_Prop;

    
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
        anchoredToWorld_Prop = serializedObject.FindProperty("anchoredToWorld");
        applyRotation_Prop = serializedObject.FindProperty("applyRotation");
        applyTranslation_Prop = serializedObject.FindProperty("applyTranslation");
        magicFunction_Prop = serializedObject.FindProperty("performMagicFunction");
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

        EditorGUILayout.PropertyField(target_Prop, new GUIContent("Visualization Target"));
        EditorGUILayout.PropertyField(showOptions_Prop, new GUIContent("Advanced Options"));
        bool showOptions = showOptions_Prop.boolValue;
        if (showOptions) {
            EditorGUILayout.PropertyField(oFiltered_Prop, new GUIContent("Filtered"));
            bool oFiltered = oFiltered_Prop.boolValue;
            if (oFiltered) {
                EditorGUILayout.PropertyField(oSampleRate_Prop, new GUIContent("Sample Rate"));
                EditorGUILayout.PropertyField(oCutOffFreq_Prop, new GUIContent("Cutoff Frequency"));
            }
            // single markers
            if (type != ARUWPMarker.MarkerType.multi) {
                EditorGUILayout.PropertyField(oUseContPoseEst_Prop, new GUIContent("Continuous Pose Estimation"));
                EditorGUILayout.PropertyField(oConfCutOff_Prop, new GUIContent("Confidence Cutoff"));
            }
            else {
                EditorGUILayout.PropertyField(oMinSubMarkers_Prop, new GUIContent("Minimum Sub Markers"));
                EditorGUILayout.PropertyField(oMinConfSubMatrix_Prop, new GUIContent("Minumum Sub Matrix Confidence"));
                EditorGUILayout.PropertyField(oMinConfSubPattern_Prop, new GUIContent("Minumum Sub Pattern Confidence"));
            }

            EditorGUILayout.PropertyField(anchoredToWorld_Prop, new GUIContent("Anchored to World"));
            EditorGUILayout.PropertyField(applyRotation_Prop, new GUIContent("Apply Rotation to Target"));
            EditorGUILayout.PropertyField(applyTranslation_Prop, new GUIContent("Apply Translation to Target"));
            EditorGUILayout.PropertyField(magicFunction_Prop, new GUIContent("Apply Undistortion Function"));
        }
        
        serializedObject.ApplyModifiedProperties();
    }


}
