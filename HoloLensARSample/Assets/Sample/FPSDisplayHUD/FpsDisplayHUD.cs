/*
*  FpsDisplayHUD.cs
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
