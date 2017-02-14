HoloLens with ARToolKit
===
Author: [Long Qian](http://longqian.me/aboutme)

Date: 2017-02-14

## Overview
**HoloLensARToolKit** is a successful experiment to integrate [ARToolKit](http://artoolkit.org/) with [HoloLens](https://www.microsoft.com/microsoft-hololens/en-us), or generally UWP (Universal Windows Platform).

With fiducial marker tracking provided by ARToolKit, plus the indoor localization of HoloLens, many Augmented Reality applications would be made possible.

This repository contains:
- Wrapper of [ARToolKit5](https://github.com/artoolkit/artoolkit5) for UWP
- Unity package to interface the native library
- Some sample Unity scenes to demo the use of them
- Link to camera calibration of HoloLens

## Example

<table border=0>
<tr>
	<td align="center" width="50%"><img src="http://longqian.me/public/image/artoolkit-hololens-minion-thumb.png" /></td>
	<td align="center" width="50%"><img src="http://longqian.me/public/image/artoolkit-hololens-samples-thumb.png" /></td>
</tr>
<tr>
	<td align="center">Minion on the Cube (<a href="https://youtu.be/cMzNyJkr3X0">Watch on Youtube</a>)</td>
	<td align="center">HoloLensARToolKit Samples(<a href="https://youtu.be/PqT90QfgP-U">Watch on Youtube</a>)</td>
</tr>
</table>

## Run the sample
1. Make sure you have [HoloLens development tools](https://developer.microsoft.com/en-us/windows/holographic/install_the_tools) installed.
	* HoloLens Emulator and Vuforia are not required.
2. Launch Unity3D, open project folder ```HoloLensARSample```.
3. Import the unity package ```ARToolKitUWP.unitypackage```.
4. Open one of the sample scenes
	* ```Assets/Sample/HoloLensARToolKitSingle.unity```
	* ```Assets/Sample/HoloLensARToolKitCube.unity```
	* ```Assets/Sample/HoloLensARToolKitMulti.unity```
5. Configure HoloLens deployment settings and deploy to HoloLens.
	* See Chapter 4 and 5 of [Holograms 100](https://developer.microsoft.com/en-us/windows/holographic/holograms_100).
6. Prepare the correspondent printed marker of ARToolKit.
	* Marker images are [Here](https://github.com/artoolkit/artoolkit5/tree/master/doc/patterns).
	* Make sure the printed marker size is same as configured in Unity.
7. See the effect.
	* **HoloLens clicker** or **air tap** is able to tune the visualization of camera frame.


---

## Understand HoloLensARToolKit
- [ARToolKit on HoloLens](http://longqian.me/2017/01/20/artoolkit-on-hololens/)
- [ARToolKitUWP controller options]()
- [ARToolKitUWP marker options]()
- [Unity native programming](http://longqian.me/2017/01/29/unity-native-programming/)
- [Coordinate systems in HoloLensARToolKit]()


## Do more with HoloLensARToolKit
- Integrate HoloLensARToolKit package with your application.
- Customize native library of ARToolKitUWP.
- Customize Unity package ARToolKitUWP-Unity.
- Calibrate your HoloLens camera.


---

## License
HoloLensARToolKit is composed of two separate projects: ARToolKitUWP and ARToolKitUWP-Unity (including samples). Both of them are open for use in compliance with [GNU Lesser General Public License (LGPL v3.0)](https://www.gnu.org/licenses/lgpl-3.0.en.html). Please see COPYING and COPYING.LESSER for license details.

ARToolKitUWP is a modified version of ARToolKit, statically links against ARToolKit binaries, and therefore complies with the license restrictions of ARToolKit (see [details](https://github.com/artoolkit/artoolkit5)).



