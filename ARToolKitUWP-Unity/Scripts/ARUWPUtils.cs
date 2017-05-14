/*
*  ARUWPUtils.cs
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

using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

/// <summary>
/// ARUWPUtils class provides utility functions for ARUWP package, including transformation
/// manipulation, FPS recording, logging.
/// </summary>
public static class ARUWPUtils {

    /// <summary>
    /// Class and object identifier for logging. [internal use]
    /// </summary>
    private static string TAG = "ARUWPUtils";

    /// <summary>
    /// The queue to record the timestamps of rendering, used to calculate render FPS. [internal use]
    /// </summary>
    private static Queue<long> qRenderTick = new Queue<long>();

    /// <summary>
    /// The queue to record the timestamps of video frames, used to calculate video FPS. [internal use]
    /// </summary>
    private static Queue<long> qVideoTick = new Queue<long>();
    
    /// <summary>
    /// The queue to record the timestamps of tracking performed, used to calculate tracking FPS. [internal use]
    /// </summary>
    private static Queue<long> qTrackTick = new Queue<long>();

    /// <summary>
    /// Record the current timestamp for rendering a frame. [internal use]
    /// </summary>
    public static void RenderTick() {
        while (qRenderTick.Count > 49) {
            qRenderTick.Dequeue();
        }
        qRenderTick.Enqueue(DateTime.Now.Ticks);
    }

    /// <summary>
    /// Get the average rendering period for the previous 50 occurrence. [public use]
    /// </summary>
    /// <returns>Average rendering period in millisecond</returns>
    public static float GetRenderDeltaTime() {
        if (qRenderTick.Count == 0) {
            return float.PositiveInfinity;
        }
        return (DateTime.Now.Ticks - qRenderTick.Peek()) / 500000.0f; 
    }


    /// <summary>
    /// Record the current timestamp for video frame arrival. [internal use]
    /// </summary>
    public static void VideoTick() {
        while (qVideoTick.Count > 49) {
            qVideoTick.Dequeue();
        }
        qVideoTick.Enqueue(DateTime.Now.Ticks);
    }

    /// <summary>
    /// Get the average video frame period for the previous 50 occurrence. [public use]
    /// </summary>
    /// <returns>Average video frame period in millisecond</returns>
    public static float GetVideoDeltaTime() {
        if (qVideoTick.Count == 0) {
            return float.PositiveInfinity;
        }
        return (DateTime.Now.Ticks - qVideoTick.Peek()) / 500000.0f;
    }

    /// <summary>
    /// Record the current timestamp for tracking performed. [internal use]
    /// </summary>
    public static void TrackTick() {
        while (qTrackTick.Count > 49) {
            qTrackTick.Dequeue();
        }
        qTrackTick.Enqueue(DateTime.Now.Ticks);
    }

    /// <summary>
    /// Get the average tracking period for the previous 50 occurrence. [public use]
    /// </summary>
    /// <returns>Average tracking period in millisecond</returns>
    public static float GetTrackDeltaTime() {
        if (qTrackTick.Count == 0) {
            return float.PositiveInfinity;
        }
        return (DateTime.Now.Ticks - qTrackTick.Peek()) / 500000.0f;
    }


    /// <summary>
    /// Convert row major 3x4 matrix returned by ARToolKitUWP to Matrix4x4 used in Unity.
    /// Right-hand coordinates to left-hand coordinates conversion is performed.
    /// That is, the Y-axis is flipped. Unit is changed from millimeter to meter. [internal use]
    /// </summary>
    /// <param name="t">Flat float array with length 12, obtained from ARToolKitUWP</param>
    /// <returns>The Matrix4x4 object representing the transformation in Unity</returns>
    public static Matrix4x4 ConvertARUWPFloatArrayToMatrix4x4(float[] t) {
        Matrix4x4 m = new Matrix4x4();
        m.SetRow(0, new Vector4(t[0], -t[1], t[2], t[3]/1000.0f));
        m.SetRow(1, new Vector4(-t[4], t[5], -t[6], -t[7]/1000.0f));
        m.SetRow(2, new Vector4(t[8], -t[9], t[10], t[11]/1000.0f));
        m.SetRow(3, new Vector4(0, 0, 0, 1));
        return m;
    }

    /// <summary>
    /// Extract Quaternion representation from Matrix4x4 object (Deprecated) [public use]
    /// </summary>
    /// <param name="m">Matrix4x4 object</param>
    /// <returns>Quaternion extracted from the 3x3 submatrix</returns>
    public static Quaternion QuaternionFromMatrix_Deprecated(Matrix4x4 m) {
        // Adapted from: http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/index.htm
        Quaternion q = new Quaternion();
        q.w = Mathf.Sqrt(Mathf.Max(0, 1 + m[0, 0] + m[1, 1] + m[2, 2])) / 2;
        q.x = Mathf.Sqrt(Mathf.Max(0, 1 + m[0, 0] - m[1, 1] - m[2, 2])) / 2;
        q.y = Mathf.Sqrt(Mathf.Max(0, 1 - m[0, 0] + m[1, 1] - m[2, 2])) / 2;
        q.z = Mathf.Sqrt(Mathf.Max(0, 1 - m[0, 0] - m[1, 1] + m[2, 2])) / 2;
        q.x *= Mathf.Sign(q.x * (m[2, 1] - m[1, 2]));
        q.y *= Mathf.Sign(q.y * (m[0, 2] - m[2, 0]));
        q.z *= Mathf.Sign(q.z * (m[1, 0] - m[0, 1]));
        return q;
    }


    /// <summary>
    /// Extract Quaternion representation from Matrix4x4 object, that is more robust against
    /// singularity. [public use]
    /// </summary>
    /// <param name="m">Matrix4x4 object</param>
    /// <returns>Quaternion extracted from the 3x3 submatrix</returns>
    public static Quaternion QuaternionFromMatrix(Matrix4x4 m) {
        // Trap the case where the matrix passed in has an invalid rotation submatrix.
        if (m.GetColumn(2) == Vector4.zero) {
            Debug.Log(TAG + ": Quaternion got zero matrix");
            return Quaternion.identity;
        }
        return Quaternion.LookRotation(m.GetColumn(2), m.GetColumn(1));
    }


    /// <summary>
    /// Extract Vector3 representation of translation from Matrix4x4 object. [public use]
    /// </summary>
    /// <param name="m">Matrix4x4 object</param>
    /// <returns>Translation represented in Vector3</returns>
    public static Vector3 PositionFromMatrix(Matrix4x4 m) {
        return m.GetColumn(3);
    }


    /// <summary>
    /// Extract Vector3 representation of scale from Matrix4x4 object. [public use]
    /// </summary>
    /// <param name="m">Matrix4x4 object</param>
    /// <returns>Scale represented in Vector3</returns>
    public static Vector3 ScaleFromMatrix(Matrix4x4 m) {
        var x = Mathf.Sqrt(m.m00 * m.m00 + m.m01 * m.m01 + m.m02 * m.m02);
        var y = Mathf.Sqrt(m.m10 * m.m10 + m.m11 * m.m11 + m.m12 * m.m12);
        var z = Mathf.Sqrt(m.m20 * m.m20 + m.m21 * m.m21 + m.m22 * m.m22);

        return new Vector3(x, y, z);
    }



    /// <summary>
    /// Logging 4x4 row major matrix, using Debug.Log support. [public use]
    /// </summary>
    /// <param name="mat">Flat float array representing row-major matrix</param>
    public static void LogMatrixRowMajor(float[] mat) {
        var str1 = string.Format("{0:0.0000}, {1:0.0000}, {2:0.0000}, {3:0.0000}\n", mat[0], mat[1], mat[2], mat[3]);
        var str2 = string.Format("{0:0.0000}, {1:0.0000}, {2:0.0000}, {3:0.0000}\n", mat[4], mat[5], mat[6], mat[7]);
        var str3 = string.Format("{0:0.0000}, {1:0.0000}, {2:0.0000}, {3:0.0000}\n", mat[8], mat[9], mat[10], mat[11]);
        var str4 = string.Format("{0:0.0000}, {1:0.0000}, {2:0.0000}, {3:0.0000}\n", mat[12], mat[13], mat[14], mat[15]);
        Debug.Log(str1 + str2 + str3 + str4);
    }


    /// <summary>
    /// Logging 4x4 column major matrix, using Debug.Log support. ARToolKit returns column-major matrix for ARPatterns. [public use]
    /// </summary>
    /// <param name="mat">Flat float array representing column-major matrix</param>
    public static void LogMatrixColumnMajor(float[] mat) {
        var str1 = string.Format("{0:0.0000}, {1:0.0000}, {2:0.0000}, {3:0.0000}\n", mat[0], mat[4], mat[8], mat[12]);
        var str2 = string.Format("{0:0.0000}, {1:0.0000}, {2:0.0000}, {3:0.0000}\n", mat[1], mat[5], mat[9], mat[13]);
        var str3 = string.Format("{0:0.0000}, {1:0.0000}, {2:0.0000}, {3:0.0000}\n", mat[2], mat[6], mat[10], mat[14]);
        var str4 = string.Format("{0:0.0000}, {1:0.0000}, {2:0.0000}, {3:0.0000}\n", mat[3], mat[7], mat[11], mat[15]);
        Debug.Log(str1 + str2 + str3 + str4);
    }


    /// <summary>
    /// Logging Matrix4x4 using Debug.Log support. [public use]
    /// </summary>
    /// <param name="mat">Matrix4x4 object</param>
    public static void LogMatrix4x4(Matrix4x4 mat) {
        var str1 = string.Format("{0:0.0000}, {1:0.0000}, {2:0.0000}, {3:0.0000}\n", mat[0], mat[4], mat[8], mat[12]);
        var str2 = string.Format("{0:0.0000}, {1:0.0000}, {2:0.0000}, {3:0.0000}\n", mat[1], mat[5], mat[9], mat[13]);
        var str3 = string.Format("{0:0.0000}, {1:0.0000}, {2:0.0000}, {3:0.0000}\n", mat[2], mat[6], mat[10], mat[14]);
        var str4 = string.Format("{0:0.0000}, {1:0.0000}, {2:0.0000}, {3:0.0000}\n", mat[3], mat[7], mat[11], mat[15]);
        Debug.Log(str1 + str2 + str3 + str4);
    }



    /// <summary>
    /// Convert Unity Transform object to a Matrix4x4 object. [public use]
    /// </summary>
    /// <param name="t">Transform of Unity GameObject</param>
    /// <returns>Matrix4x4 representation of the transform</returns>
    public static Matrix4x4 ConvertTransformToMatrix4x4(Transform t) {
        return Matrix4x4.TRS(t.localPosition, t.localRotation, t.localScale);
    }


    /// <summary>
    /// Set a transformation represented by Matrix4x4 to a GameObject localtransform. [public use]
    /// </summary>
    /// <param name="o">The GameObject to set</param>
    /// <param name="m">The Matrix4x4 object representing the target transformation</param>
    public static void SetMatrix4x4ToGameObject(ref GameObject o, Matrix4x4 m) {
        o.transform.localPosition = PositionFromMatrix(m);
        o.transform.localRotation = QuaternionFromMatrix(m);
        o.transform.localScale = ScaleFromMatrix(m);
    }



}
