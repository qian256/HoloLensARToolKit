HoloLens with ARToolKit
===
Author: [Long Qian](https://longqian.me/aboutme)

Date: 2017-01-19

## Overview
HoloLensARToolKit is a successful experiment to integrate [ARToolKit](http://artoolkit.org/) with [HoloLens](https://www.microsoft.com/microsoft-hololens/en-us), or generally UWP (Universal Windows Platform).

With fiducial marker tracking provided by ARToolKit, plus the indoor localization of HoloLens, many Augmented Reality applications would be made possible.

This repository contains:
- Wrapper of ARToolKit5 for UWP
- Unity package to interface the native library
- A sample Unity application to demo the use of them
- Camera calibration of HoloLens

## Demo
Following are two screenshots taken in HoloLens Device Portal:
<table>
<tr>
	<td align="center" width="50%"><img src="https://cloud.githubusercontent.com/assets/8185982/22130805/ade4ac96-de7c-11e6-91fb-a3be5ad5d559.jpg" /></td>
	<td  align="center" width="50%"><img src="https://cloud.githubusercontent.com/assets/8185982/22130806/ade7e8ac-de7c-11e6-82ff-d1b31caef17f.jpg" /></td>
</tr>
</table>
The hiro marker (left) and kanji marker (right) are tracked by the HoloLens front-facing camera, and the rotation is associated with the two cubes on top. The 2D plane visualizes the current camera frame.

Frame rate of rendering and frame rate of tracking are displayed on the top right corner. The two green block shows the confidence of tracking of the two markers.




