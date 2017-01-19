using UnityEngine;
using UnityEngine.UI;

/// <summary>
/// Calculate frame rate of Unity applications, and visualize via
/// UnityEngine.UI.Text
///
/// Author:     Long Qian
/// Email:      lqian8@jhu.edu
/// </summary>
public class FpsDisplayHUD : MonoBehaviour
{
    public Text Text;

    private float deltaTime;

    private void Update()
    {
        deltaTime += (Time.deltaTime - deltaTime) * 0.1f;
        var msec = deltaTime * 1000.0f;
        var fps = 1.0f / deltaTime;
        var text = string.Format("{0:0.0} ms ({1:0.} fps)", msec, fps);
        Text.text = text;
    }
}
