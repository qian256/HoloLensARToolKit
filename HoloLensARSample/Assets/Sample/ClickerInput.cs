/*
*  ClickerInput.cs
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
using UnityEngine.VR.WSA.Input;
using UnityEngine;
using UnityEngine.Events;

/// <summary>
/// The ClickerInput class recognize the clicker event of HoloLens
/// clicker, the event callback can be configured in Unity Inspector.
///
/// Author:     Long Qian
/// Email:      lqian8@jhu.edu
/// </summary>
public class ClickerInput : MonoBehaviour {
    
    private string TAG = "ClickerInput";

    [System.Serializable]
    public class EventClickerClick : UnityEvent { };

    public EventClickerClick eventClickerClick;

    public static ClickerInput Instance { get; private set; }
    GestureRecognizer recognizer;

    // Use this for initialization
    void Awake() {
        Instance = this;

        // Set up a GestureRecognizer to detect Select gestures.
        recognizer = new GestureRecognizer();
        recognizer.TappedEvent += (source, tapCount, ray) => {
            eventClickerClick.Invoke();
        };
        recognizer.StartCapturingGestures();
    }

    // Use this for initialization
    void Start() {
        ;
    }

    // Update is called once per frame
    void Update() {
        ;
    }
}
