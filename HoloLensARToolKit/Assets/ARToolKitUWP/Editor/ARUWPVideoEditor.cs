/*
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
/// The ARUWPVideoEditor class controles the Unity inspector options of
/// each ARUWPVideo object.
///
/// Author:     Long Qian
/// Email:      lqian8@jhu.edu
/// </summary>
[CustomEditor(typeof(ARUWPVideo))]
[CanEditMultipleObjects]
public class ARUWPVideoEditor : Editor {
    public SerializedProperty videoParameter_Prop,
        videoPreview_Prop,
        previewPlane_Prop,
        videoFPS_Prop;

    void OnEnable() {
        videoParameter_Prop = serializedObject.FindProperty("videoParameter");
        videoPreview_Prop = serializedObject.FindProperty("videoPreview");
        previewPlane_Prop = serializedObject.FindProperty("previewPlane");
        videoFPS_Prop = serializedObject.FindProperty("videoFPS");
    }

    public override void OnInspectorGUI() {
        serializedObject.Update();
        EditorGUILayout.HelpBox("The video parameter needs to match the target device.\nFor HL1, it is recommended to use 1344x756x30\nFor HL2, it is recommended to use 1504x846x60", MessageType.Info);
        EditorGUILayout.PropertyField(videoParameter_Prop, new GUIContent("Video Parameter"));
        
        EditorGUILayout.PropertyField(videoPreview_Prop, new GUIContent("Enable Video Preview"));
        bool videoPreview = videoPreview_Prop.boolValue;
        if (videoPreview) {
            EditorGUILayout.HelpBox("Video preview is for debugging purpose. It requires extra CPU and GPU resources. Disable it for production build.", MessageType.Info);
            EditorGUILayout.PropertyField(previewPlane_Prop, new GUIContent("Video Preview Holder"));
        }
        EditorGUILayout.PropertyField(videoFPS_Prop, new GUIContent("Video FPS Holder (optional)"));

        serializedObject.ApplyModifiedProperties();
    }
}
