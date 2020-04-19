/*
*  ARUWPTargetEditor.cs
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
/// The ARUWPTargetEditor class controles the Unity inspector options of
/// each ARUWPTarget object.
///
/// Author:     Long Qian
/// Email:      lqian8@jhu.edu
/// </summary>
[CustomEditor(typeof(ARUWPTarget))]
[CanEditMultipleObjects]
public class ARUWPTargetEditor : Editor {
    public SerializedProperty smoothing_Prop, lerp_Prop;

    void OnEnable() {
        smoothing_Prop = serializedObject.FindProperty("smoothing");
        lerp_Prop = serializedObject.FindProperty("lerp");
    }

    public override void OnInspectorGUI() {
        serializedObject.Update();
        EditorGUILayout.HelpBox("This script must be attached to GameObjects in the scene root. The transformation of this GameObject is managed by HoloLensARToolKit at runtime.", MessageType.Info);
        EditorGUILayout.PropertyField(smoothing_Prop, new GUIContent("Visualization Smoothing"));
        bool smoothing = smoothing_Prop.boolValue;
        if (smoothing) {
            EditorGUILayout.PropertyField(lerp_Prop, new GUIContent("Lerp Parameter"));
        }

        serializedObject.ApplyModifiedProperties();
    }
}
