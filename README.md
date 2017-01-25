HoloLens with ARToolKit
===
Author: [Long Qian](https://longqian.me/aboutme)

Date: 2017-01-24

## Overview
**HoloLensARToolKit** is a successful experiment to integrate [ARToolKit](http://artoolkit.org/) with [HoloLens](https://www.microsoft.com/microsoft-hololens/en-us), or generally UWP (Universal Windows Platform).

With fiducial marker tracking provided by ARToolKit, plus the indoor localization of HoloLens, many Augmented Reality applications would be made possible.

This repository contains:
- Wrapper of [ARToolKit5](https://github.com/artoolkit/artoolkit5) for UWP
- Unity package to interface the native library
- A sample Unity application to demo the use of them
- Camera calibration of HoloLens

## Example
Following are two screenshots taken in HoloLens Device Portal:
<table border=0>
<tr>
	<td align="center" width="50%"><img src="https://cloud.githubusercontent.com/assets/8185982/22189679/0a6f8a6e-e0ec-11e6-8563-ecae01e7d294.jpg" /></td>
	<td align="center" width="50%"><img src="https://cloud.githubusercontent.com/assets/8185982/22190907/907d311c-e0f4-11e6-84b4-91b47586b008.jpg" /></td>
</tr>
<tr>
	<td align="center">Multiple Marker Tracking</td>
	<td align="center">Cube Marker Tracking</td>
</tr>
</table>
The top-left 2D plane visualizes the current camera frame. Frame rate of rendering and frame rate of tracking are displayed on the top-right corner.

## Run the sample
1. Make sure you have [HoloLens development tools](https://developer.microsoft.com/en-us/windows/holographic/install_the_tools) installed.
	* HoloLens Emulator and Vuforia are not required.
2. Launch Unity3D, open project folder ```HoloLensARSample```.
3. Import the unity package ```ARToolKitUWP.unitypackage```.
4. Open one of the sample scenes
	* ```Assets/Sample/HoloLensARToolKitSingle.unity```
	* ```Assets/Sample/HoloLensARToolKitMulti.unity```
	* ```Assets/Sample/HoloLensARToolKitCube.unity```
5. Configure HoloLens deployment settings and deploy to HoloLens.
	* See Chapter 4 and 5 of [Holograms 100](https://developer.microsoft.com/en-us/windows/holographic/holograms_100).
6. Prepare the correspondent printed marker of ARToolKit.
7. See the effect.
	* HoloLens clicker is able to tune the visualization of camera frame image.


---

## Understand HoloLensARToolKit
- [My blog post](https://longqian.me/2017/01/20/artoolkit-on-hololens/)
- [Coordinate systems in HoloLensARToolKit](https://github.com/qian256/HoloLensARToolKit/wiki/Coordinate-Systems)
- [Unity native programming](https://github.com/qian256/HoloLensARToolKit/wiki/Unity-Native-Coding)


## Do more with HoloLensARToolKit
- Integrate HoloLensARToolKit package with your application.
- Customize native library of ARToolKitUWP.
- Customize Unity package ARToolKitUWP-Unity.
- Calibrate your HoloLens camera.


---

## License
HoloLensARToolKit is composed of two separate projects: ARToolKitUWP and ARToolKitUWP-Unity (including samples). Both of them are open for use in compliance with [GNU Lesser General Public License (LGPL v3.0)](https://www.gnu.org/licenses/lgpl-3.0.en.html). Please see COPYING and COPYING.LESSER for license details.

ARToolKitUWP is a modified version of ARToolKit, statically links against ARToolKit binaries, and therefore complies with the license restrictions of ARToolKit (see [details](https://github.com/artoolkit/artoolkit5)).



