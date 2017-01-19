using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// ARUWPUtils class provides utility functions for ARUWP package
///
/// Author:     Long Qian
/// Email:      lqian8@jhu.edu
/// </summary>
public static class ARUWPUtils {

    /// <summary>
    /// Convert row major 3x4 matrix to Matrix4x4
    /// </summary>
    public static Matrix4x4 ConvertFloatArrayToMatrix4x4(float[] t) {
        Matrix4x4 m = new Matrix4x4();
        m.SetRow(0, new Vector4(t[0], t[1], t[2], t[3]));
        m.SetRow(1, new Vector4(t[4], t[5], t[6], t[7]));
        m.SetRow(2, new Vector4(t[8], t[9], t[10], t[11]));
        m.SetRow(3, new Vector4(0, 0, 0, 1));
        return m;
    }
    
    /// <summary>
    /// Extract Quaternion representation from Matrix4x4 obejct
    /// </summary>
    public static Quaternion QuaternionFromMatrix(Matrix4x4 m) {
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
    /// Extract Vector3 representation of translation from Matrix4x4 obejct
    /// </summary>
    public static Vector3 PositionFromMatrix(Matrix4x4 m) {
        return new Vector3(m[0, 3], m[1, 3], m[2, 3]);
    }



    /// <summary>
    /// Logging 4x4 row major matrix
    /// </summary>
    private static void LogMatrixRowMajor(float[] mat) {
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
    private static void LogMatrixColumnMajor(float[] mat) {
        var str1 = string.Format("{0:0.0000}, {1:0.0000}, {2:0.0000}, {3:0.0000}\n", mat[0], mat[4], mat[8], mat[12]);
        var str2 = string.Format("{0:0.0000}, {1:0.0000}, {2:0.0000}, {3:0.0000}\n", mat[1], mat[5], mat[9], mat[13]);
        var str3 = string.Format("{0:0.0000}, {1:0.0000}, {2:0.0000}, {3:0.0000}\n", mat[2], mat[6], mat[10], mat[14]);
        var str4 = string.Format("{0:0.0000}, {1:0.0000}, {2:0.0000}, {3:0.0000}\n", mat[3], mat[7], mat[11], mat[15]);
        Debug.Log(str1 + str2 + str3 + str4);
    }


}
