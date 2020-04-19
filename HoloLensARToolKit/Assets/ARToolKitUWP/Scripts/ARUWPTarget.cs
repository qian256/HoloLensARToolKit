/*
*  ARUWPTarget.cs
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
using UnityEngine;
using UnityEngine.UI;


/// <summary>
/// The ARUWPTarget class represents the virtual object for rendering, that
/// should be registered with a particular marker. It needs to be attached 
/// to Unity gameobject that is in the scene root. One should not attempt
/// to set it to a child of other gameobject.
///
/// Author:     Long Qian
/// Email:      lqian8@jhu.edu
/// </summary>
public class ARUWPTarget : MonoBehaviour {


    /// <summary>
    /// Whether to apply smoothing to the visualization target, if set to true, it will make the  
    /// transition of the visualization smooth, e.g. prevent jittering. [public use]
    /// </summary>
    public bool smoothing = true;

    /// <summary>
    /// The lerp coefficient to smooth the pose transition. [public use]
    /// </summary>
    public float lerp = 0.15f;


    private static float positionJumpThreshold = 0.08f;
    private static float rotationJumpThreshold = 24f;
    private static float positionRecoverThreshold = 0.04f;
    private static float rotationRecoverThreshold = 12f;
    private static int maxPendingList = 15;
    private List<Vector3> pendingPositionList = new List<Vector3>();
    private List<Quaternion> pendingRotationList = new List<Quaternion>();


    /// <summary>
    /// This function is called by the associated ARUWPMarker.cs to update the pose of this 
    /// virtual object. When smoothing is enabled, the new pose will be filtered with current
    /// pose using lerp. Big sudden change of 6-DOF pose will be prohibited. [public use]
    /// </summary>
    public void SetMatrix4x4(Matrix4x4 localToWorldMatrix) {
        Vector3 previousPosition = transform.localPosition;
        Quaternion previousRotation = transform.localRotation;
        Vector3 targetPosition = ARUWPUtils.PositionFromMatrix(localToWorldMatrix);
        Quaternion targetRotation = ARUWPUtils.QuaternionFromMatrix(localToWorldMatrix);
        if (!smoothing) {
            transform.localRotation = targetRotation;
            transform.localPosition = targetPosition;
        }
        else {
            float positionDiff = Vector3.Distance(targetPosition, previousPosition);
            float rotationDiff = Quaternion.Angle(targetRotation, previousRotation);

            if (Mathf.Abs(positionDiff) < positionJumpThreshold && Mathf.Abs(rotationDiff) < rotationJumpThreshold) {
                transform.localRotation = Quaternion.Slerp(previousRotation, targetRotation, lerp);
                transform.localPosition = Vector3.Lerp(previousPosition, targetPosition, lerp);
                pendingPositionList.Clear();
                pendingRotationList.Clear();
            }
            else {
                // maybe there is a jump
                pendingPositionList.Add(targetPosition);
                pendingRotationList.Add(targetRotation);
                bool confirmJump = true;
                if (pendingPositionList.Count > maxPendingList) {
                    for (int i = 0; i < maxPendingList - 1; i++) {
                        float tempPositionDiff = Vector3.Distance(pendingPositionList[pendingPositionList.Count - i - 1], pendingPositionList[pendingPositionList.Count - i - 2]);
                        float tempRotationDiff = Quaternion.Angle(pendingRotationList[pendingRotationList.Count - i - 1], pendingRotationList[pendingRotationList.Count - i - 2]);
                        if (Mathf.Abs(tempPositionDiff) > positionRecoverThreshold || Mathf.Abs(tempRotationDiff) > rotationRecoverThreshold) {
                            confirmJump = false;
                            break;
                        }
                    }
                    if (confirmJump) {
                        transform.localRotation = targetRotation;
                        transform.localPosition = targetPosition;
                        pendingPositionList.Clear();
                        pendingRotationList.Clear();
                    }
                }
            }
        }
    }

}
