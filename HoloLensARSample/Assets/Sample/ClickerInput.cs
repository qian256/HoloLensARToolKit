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
