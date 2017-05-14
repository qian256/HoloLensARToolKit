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
/// The ARUWPControllerEditor class controles the Unity inspector options of
/// each ARUWPController object.
///
/// Author:     Long Qian
/// Email:      lqian8@jhu.edu
/// </summary>
[CustomEditor(typeof(ARUWPController))]
[CanEditMultipleObjects]
public class ARUWPControllerEditor : Editor {
    public SerializedProperty useCameraParamFile_Prop,
        cameraParam_Prop,
        threshold_Prop,
        borderSize_Prop,
        thresholdMode_Prop,
        labelingMode_Prop,
        patternDetectionMode_Prop,
        matrixCodeType_Prop,
        imageProcMode_Prop,
        trackFPS_Prop,
        renderFPS_Prop, 
        showOptions_Prop;

    public void OnEnable() {
        useCameraParamFile_Prop = serializedObject.FindProperty("useCameraParamFile");
        cameraParam_Prop = serializedObject.FindProperty("cameraParam");
        threshold_Prop = serializedObject.FindProperty("threshold");
        borderSize_Prop = serializedObject.FindProperty("borderSize");
        thresholdMode_Prop = serializedObject.FindProperty("thresholdMode");
        labelingMode_Prop = serializedObject.FindProperty("labelingMode");
        patternDetectionMode_Prop = serializedObject.FindProperty("patternDetectionMode");
        matrixCodeType_Prop = serializedObject.FindProperty("matrixCodeType");
        imageProcMode_Prop = serializedObject.FindProperty("imageProcMode");
        trackFPS_Prop = serializedObject.FindProperty("trackFPS");
        renderFPS_Prop = serializedObject.FindProperty("renderFPS");
        showOptions_Prop = serializedObject.FindProperty("showOptions");
    }

    public override void OnInspectorGUI() {
        serializedObject.Update();

        EditorGUILayout.PropertyField(useCameraParamFile_Prop, new GUIContent("Use Camera Param File"));
        bool useCameraParamFile = useCameraParamFile_Prop.boolValue;
        if (useCameraParamFile) {
            EditorGUILayout.PropertyField(cameraParam_Prop, new GUIContent("Camera Param Filename"));
        }
        else {
            EditorGUILayout.HelpBox("You need to manually create a buffer to store camera parameters, and call SetCameraParamBuffer() before the initialization of tracking.", MessageType.Info);
        }

        EditorGUILayout.PropertyField(patternDetectionMode_Prop, new GUIContent("Pattern Detection Mode"));
        int patternDetectionMode = patternDetectionMode_Prop.enumValueIndex;
        if (patternDetectionMode == ARUWP.AR_TEMPLATE_MATCHING_COLOR_AND_MATRIX ||
            patternDetectionMode == ARUWP.AR_TEMPLATE_MATCHING_MONO_AND_MATRIX ||
            patternDetectionMode == ARUWP.AR_MATRIX_CODE_DETECTION) {
            EditorGUILayout.PropertyField(matrixCodeType_Prop, new GUIContent("Matrix Code Type"));
        }

        EditorGUILayout.PropertyField(trackFPS_Prop, new GUIContent("Track FPS Holder (optional)"));
        EditorGUILayout.PropertyField(renderFPS_Prop, new GUIContent("Render FPS Holder (optional)"));

        EditorGUILayout.PropertyField(showOptions_Prop, new GUIContent("Advanced Options"));
        bool showOptions = showOptions_Prop.boolValue;
        if (showOptions) {
            EditorGUILayout.PropertyField(borderSize_Prop, new GUIContent("Border Size"));
            EditorGUILayout.PropertyField(labelingMode_Prop, new GUIContent("Labeling Mode"));
            EditorGUILayout.PropertyField(imageProcMode_Prop, new GUIContent("Image Processing Mode"));
            EditorGUILayout.PropertyField(thresholdMode_Prop, new GUIContent("Thresholding Mode"));
            int thresholdingMode = thresholdMode_Prop.enumValueIndex;
            if (thresholdingMode == ARUWP.AR_LABELING_THRESH_MODE_MANUAL) {
                EditorGUILayout.PropertyField(threshold_Prop, new GUIContent("Threshold Value"));
            }
        }
        
        serializedObject.ApplyModifiedProperties();
    }
}
