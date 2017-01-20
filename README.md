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
	<td align="center" width="100%"><img src="https://cloud.githubusercontent.com/assets/8185982/22130509/6e2915e4-de7a-11e6-8205-7521bd5ee1bf.jpg" /></td>
	<td  align="center" width="100%"><img src="https://cloud.githubusercontent.com/assets/8185982/22130508/6e283ffc-de7a-11e6-98de-c13662b76842.jpg" /></td>
</tr>
</table>
The hiro marker and kanji marker are tracked by the HoloLens front-facing camera, and the rotation is associated with the two cubes on top. The image plane visualizes the current camera frame.

Frame rate of rendering and frame rate of tracking are displayed on the top right corner. The two green block shows the confidence of tracking of the two markers.




