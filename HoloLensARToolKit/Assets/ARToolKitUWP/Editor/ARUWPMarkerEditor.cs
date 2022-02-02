﻿/*
*  ARUWPMarkerEditor.cs
*  HoloLensARToolKit
*
*  This file is a part of HoloLensARToolKit.
*
*  HoloLensARToolKit is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  HoloLensARToolKit is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public License
*  along with HoloLensARToolKit.  If not, see <http://www.gnu.org/licenses/>.
*
*  Copyright 2020 Long Qian
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
        nftFileName_Prop,
        showOptions_Prop,
        oFiltered_Prop,
        oSampleRate_Prop,
        oCutOffFreq_Prop,
        oUseContPoseEst_Prop,
        oConfCutOff_Prop,
        oMinSubMarkers_Prop,
        oMinConfSubMatrix_Prop,
        oMinConfSubPattern_Prop,
        oScaleFactor_Prop,
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
        nftFileName_Prop = serializedObject.FindProperty("nftFileName");
        showOptions_Prop = serializedObject.FindProperty("showOptions");
        oFiltered_Prop = serializedObject.FindProperty("oFiltered");
        oSampleRate_Prop = serializedObject.FindProperty("oSampleRate");
        oCutOffFreq_Prop = serializedObject.FindProperty("oCutOffFreq");
        oUseContPoseEst_Prop = serializedObject.FindProperty("oUseContPoseEst");
        oConfCutOff_Prop = serializedObject.FindProperty("oConfCutOff");
        oMinSubMarkers_Prop = serializedObject.FindProperty("oMinSubMarkers");
        oMinConfSubMatrix_Prop = serializedObject.FindProperty("oMinConfSubMatrix");
        oMinConfSubPattern_Prop = serializedObject.FindProperty("oMinConfSubPattern");
        oScaleFactor_Prop = serializedObject.FindProperty("oScaleFactor");
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

            case ARUWPMarker.MarkerType.nft:
                EditorGUILayout.PropertyField(nftFileName_Prop, new GUIContent("File Name"));
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
            if (type != ARUWPMarker.MarkerType.multi && type != ARUWPMarker.MarkerType.nft) {
                EditorGUILayout.PropertyField(oUseContPoseEst_Prop, new GUIContent("Continuous Pose Estimation"));
                EditorGUILayout.PropertyField(oConfCutOff_Prop, new GUIContent("Confidence Cutoff"));
            }
            // multiple markers
            else if (type == ARUWPMarker.MarkerType.multi) {
                EditorGUILayout.PropertyField(oMinSubMarkers_Prop, new GUIContent("Minimum Sub Markers"));
                EditorGUILayout.PropertyField(oMinConfSubMatrix_Prop, new GUIContent("Minumum Sub Matrix Confidence"));
                EditorGUILayout.PropertyField(oMinConfSubPattern_Prop, new GUIContent("Minumum Sub Pattern Confidence"));
            }
            // nft markers
            else if (type == ARUWPMarker.MarkerType.nft) {
                EditorGUILayout.PropertyField(oScaleFactor_Prop, new GUIContent("Scale Factor"));
            }

        }
        
        serializedObject.ApplyModifiedProperties();
    }


}
