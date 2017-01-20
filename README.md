HoloLens with ARToolKit
===
Author: [Long Qian](https://longqian.me/aboutme)

Date: 2017-01-19

## Overview
HoloLensARToolKit is a successful experiment to integrate [ARToolKit](http://artoolkit.org/) with [HoloLens](https://www.microsoft.com/microsoft-hololens/en-us), or generally UWP (Universal Windows Platform).

With fiducial marker tracking provided by ARToolKit, plus the indoor localization of HoloLens, many Augmented Reality applications would be made possible.

This repository contains:
- Wrapper of [ARToolKit5](https://github.com/artoolkit/artoolkit5) for UWP
- Unity package to interface the native library
- A sample Unity application to demo the use of them
- Camera calibration of HoloLens

## Demo
Following are two screenshots taken in HoloLens Device Portal:
<table>
<tr>
	<td align="center" width="49.6%"><img src="https://cloud.githubusercontent.com/assets/8185982/22130805/ade4ac96-de7c-11e6-91fb-a3be5ad5d559.jpg" /></td>
	<td  align="center" width="50%"><img src="https://cloud.githubusercontent.com/assets/8185982/22130806/ade7e8ac-de7c-11e6-82ff-d1b31caef17f.jpg" /></td>
</tr>
</table>
The hiro marker (left) and kanji marker (right) are tracked by the HoloLens front-facing camera, and the rotation of the markers is associated with the two cubes on top. The 2D plane visualizes the current camera frame.
Frame rate of rendering and frame rate of tracking are displayed on the top right corner. The two green block shows the confidence of tracking of the two markers.

## Run the sample
1. Make sure you have [HoloLens development tools](https://developer.microsoft.com/en-us/windows/holographic/install_the_tools) installed.
	* HoloLens Emulator and Vuforia are not required.
2. Launch Unity3D, open project folder ```HoloLensARSample```.
3. Import the unity package ```ARToolKitUWP.unitypackage```.
4. Open sample scene ```Assets/Sample/HoloLensARToolKit.unity```.
5. Configure HoloLens deployment settings and deploy to HoloLens.
	* See Chapter 4 and 5 of [Holograms 100](https://developer.microsoft.com/en-us/windows/holographic/holograms_100).
6. Prepare [Hiro pattern](https://github.com/artoolkit/artoolkit5/blob/master/doc/patterns/Hiro%20pattern.pdf) and [Kanji pattern](https://github.com/artoolkit/artoolkit5/blob/master/doc/patterns/Kanji%20pattern.pdf) of ARToolKit.
7. See the effect.
	* HoloLens clicker is able to tune the visualization of camera frame image.

---

## Do more with HoloLensARToolKit
- Integrate HoloLensARToolKit package with your application.
- Customize native library of ARToolKitUWP.
- Customize Unity package ARToolKitUWP-Unity.
- Calibrate your HoloLens camera.


