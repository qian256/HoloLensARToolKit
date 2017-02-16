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

using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

/// <summary>
/// ARUWPUtils class provides utility functions for ARUWP package
///
/// Author:     Long Qian
/// Email:      lqian8@jhu.edu
/// </summary>
public static class ARUWPUtils {

    private static string TAG = "ARUWPUtils";

    // Delegate type declaration.
    public delegate void LogCallback([MarshalAs(UnmanagedType.LPStr)] string msg);

    // Delegate instance.
    private static LogCallback logCallback = null;
    private static GCHandle logCallbackGCH;

    public static void aruwpRegisterLogCallback(LogCallback lcb) {
        if (lcb != null) {
            logCallback = lcb;
            logCallbackGCH = GCHandle.Alloc(logCallback); // Does not need to be pinned, see http://stackoverflow.com/a/19866119/316487 
        }
        ARUWP.aruwpRegisterLogCallback(logCallback);
        if (lcb == null) {
            logCallback = null;
            logCallbackGCH.Free();
        }
    }

    public static void aruwpSetLogLevel(int logLevel) {
        ARUWP.aruwpSetLogLevel(logLevel);
    }



    /// <summary>
    /// Convert row major 3x4 matrix to Matrix4x4
    /// </summary>
    public static Matrix4x4 ConvertARUWPFloatArrayToMatrix4x4(float[] t) {
        Matrix4x4 m = new Matrix4x4();
        m.SetRow(0, new Vector4(t[0], t[1], t[2], t[3]/1000.0f));
        m.SetRow(1, new Vector4(t[4], t[5], t[6], t[7]/1000.0f));
        m.SetRow(2, new Vector4(t[8], t[9], t[10], t[11]/1000.0f));
        m.SetRow(3, new Vector4(0, 0, 0, 1));
        return m;
    }
    
    /// <summary>
    /// Extract Quaternion representation from Matrix4x4 obejct
    /// </summary>
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
    /// Extract Quaternion representation from Matrix4x4 obejct
    /// </summary>
    public static Quaternion QuaternionFromMatrix(Matrix4x4 m) {
        // Trap the case where the matrix passed in has an invalid rotation submatrix.
        if (m.GetColumn(2) == Vector4.zero) {
            Debug.Log(TAG + ": Quaternion got zero matrix");
            return Quaternion.identity;
        }
        return Quaternion.LookRotation(m.GetColumn(2), m.GetColumn(1));
    }

    /// <summary>
    /// Extract Vector3 representation of translation from Matrix4x4 obejct
    /// unit: meter
    /// </summary>
    //public static Vector3 PositionFromMatrixMeter(Matrix4x4 m) {
    //    return m.GetColumn(3) / 1000f;
    //}


    /// <summary>
    /// Extract Vector3 representation of translation from Matrix4x4 obejct
    /// unit: millimeter
    /// </summary>
    public static Vector3 PositionFromMatrix(Matrix4x4 m) {
        return m.GetColumn(3);
    }


    /// <summary>
    /// Extract Vector3 representation of scale from Matrix4x4 obejct
    /// </summary>
    public static Vector3 ScaleFromMatrix(Matrix4x4 m) {
        var x = Mathf.Sqrt(m.m00 * m.m00 + m.m01 * m.m01 + m.m02 * m.m02);
        var y = Mathf.Sqrt(m.m10 * m.m10 + m.m11 * m.m11 + m.m12 * m.m12);
        var z = Mathf.Sqrt(m.m20 * m.m20 + m.m21 * m.m21 + m.m22 * m.m22);

        return new Vector3(x, y, z);
    }



    /// <summary>
    /// Logging 4x4 row major matrix
    /// </summary>
    public static void LogMatrixRowMajor(float[] mat) {
        var str1 = string.Format("{0:0.0000}, {1:0.0000}, {2:0.0000}, {3:0.0000}\n", mat[0], mat[1], mat[2], mat[3]);
        var str2 = string.Format("{0:0.0000}, {1:0.0000}, {2:0.0000}, {3:0.0000}\n", mat[4], mat[5], mat[6], mat[7]);
        var str3 = string.Format("{0:0.0000}, {1:0.0000}, {2:0.0000}, {3:0.0000}\n", mat[8], mat[9], mat[10], mat[11]);
        var str4 = string.Format("{0:0.0000}, {1:0.0000}, {2:0.0000}, {3:0.0000}\n", mat[12], mat[13], mat[14], mat[15]);
        Debug.Log(str1 + str2 + str3 + str4);
    }

    /// <summary>
    /// Logging 4x4 column major matrix.
    /// ARToolKit returns column major matrix for ARPatterns
    /// </summary>
    public static void LogMatrixColumnMajor(float[] mat) {
        var str1 = string.Format("{0:0.0000}, {1:0.0000}, {2:0.0000}, {3:0.0000}\n", mat[0], mat[4], mat[8], mat[12]);
        var str2 = string.Format("{0:0.0000}, {1:0.0000}, {2:0.0000}, {3:0.0000}\n", mat[1], mat[5], mat[9], mat[13]);
        var str3 = string.Format("{0:0.0000}, {1:0.0000}, {2:0.0000}, {3:0.0000}\n", mat[2], mat[6], mat[10], mat[14]);
        var str4 = string.Format("{0:0.0000}, {1:0.0000}, {2:0.0000}, {3:0.0000}\n", mat[3], mat[7], mat[11], mat[15]);
        Debug.Log(str1 + str2 + str3 + str4);
    }

    /// <summary>
    /// Logging 4x4 Matrix4x4
    /// </summary>
    public static void LogMatrix4x4(Matrix4x4 mat) {
        var str1 = string.Format("{0:0.0000}, {1:0.0000}, {2:0.0000}, {3:0.0000}\n", mat[0], mat[4], mat[8], mat[12]);
        var str2 = string.Format("{0:0.0000}, {1:0.0000}, {2:0.0000}, {3:0.0000}\n", mat[1], mat[5], mat[9], mat[13]);
        var str3 = string.Format("{0:0.0000}, {1:0.0000}, {2:0.0000}, {3:0.0000}\n", mat[2], mat[6], mat[10], mat[14]);
        var str4 = string.Format("{0:0.0000}, {1:0.0000}, {2:0.0000}, {3:0.0000}\n", mat[3], mat[7], mat[11], mat[15]);
        Debug.Log(str1 + str2 + str3 + str4);
    }


    /// <summary>
    /// Convert ARUWP matrix to unity matrix, the trick here is to change unit from millimeter to meter
    /// </summary>
    //public static Matrix4x4 ConvertARUWPMatrixToMatrix4x4(Matrix4x4 m) {
    //    Matrix4x4 n = m * Matrix4x4.identity;
    //    n.m03 /= 1000f;
    //    n.m13 /= 1000f;
    //    n.m23 /= 1000f;
    //    return n;
    //}


    /// <summary>
    /// Convert Unity Transform object to a Matrix4x4 object
    /// </summary>
    public static Matrix4x4 ConvertTransformToMatrix4x4(Transform t) {
        return Matrix4x4.TRS(t.localPosition, t.localRotation, t.localScale);
    }


    /// <summary>
    /// Apply a transformation represented by Matrix4x4 to a Transform object
    /// </summary>
    public static void SetMatrix4x4ToTransform(ref Transform t, Matrix4x4 m) {
        t.localPosition = PositionFromMatrix(m);
        t.localRotation = QuaternionFromMatrix(m);
        t.localScale = ScaleFromMatrix(m);
    }

    /// <summary>
    /// Apply a transformation represented by Matrix4x4 to a GameObject
    /// </summary>
    public static void SetMatrix4x4ToGameObject(ref GameObject o, Matrix4x4 m) {
        o.transform.localPosition = PositionFromMatrix(m);
        o.transform.localRotation = QuaternionFromMatrix(m);
        o.transform.localScale = ScaleFromMatrix(m);
    }


    /// <summary>
    /// Adapted from arunity package:
    /// Convert from right-hand coordinate system with <normal vector> in direction of +x,
    /// <orthorgonal vector> in direction of +y, and <approach vector> in direction of +z,
    /// to Unity's left-hand coordinate system with <normal vector> in direction of +x,
    /// <orthorgonal vector> in direction of +y, and <approach vector> in direction of +z.
    /// This is equivalent to negating row 2, and then negating column 2.
    /// </summary>
    //public static Matrix4x4 LHMatrixFromRHMatrix(Matrix4x4 rhm) {
    //    Matrix4x4 lhm = new Matrix4x4(); 

    //    // Column 0.
    //    lhm[0, 0] = rhm[0, 0];
    //    lhm[1, 0] = rhm[1, 0];
    //    lhm[2, 0] = -rhm[2, 0];
    //    lhm[3, 0] = rhm[3, 0];

    //    // Column 1.
    //    lhm[0, 1] = rhm[0, 1];
    //    lhm[1, 1] = rhm[1, 1];
    //    lhm[2, 1] = -rhm[2, 1];
    //    lhm[3, 1] = rhm[3, 1];

    //    // Column 2.
    //    lhm[0, 2] = -rhm[0, 2];
    //    lhm[1, 2] = -rhm[1, 2];
    //    lhm[2, 2] = rhm[2, 2];
    //    lhm[3, 2] = -rhm[3, 2];

    //    // Column 3.
    //    lhm[0, 3] = rhm[0, 3];
    //    lhm[1, 3] = rhm[1, 3];
    //    lhm[2, 3] = -rhm[2, 3];
    //    lhm[3, 3] = rhm[3, 3];

    //    return lhm;
    //}
}
