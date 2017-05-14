HoloLens with ARToolKit v0.2
===
Author: [Long Qian](http://longqian.me/aboutme)

Date: 2017-05-13

## Overview
**HoloLensARToolKit** is a successful experiment to integrate [ARToolKit](http://artoolkit.org/) (v5.3.2) with [HoloLens](https://www.microsoft.com/microsoft-hololens/en-us), or generally UWP (Universal Windows Platform).

With fiducial marker tracking provided by ARToolKit, plus the indoor localization of HoloLens, many Augmented Reality applications would be made possible.

This repository contains:
- Wrapper of [ARToolKit5](https://github.com/artoolkit/artoolkit5) for UWP
- Unity package to interface the native library
- Some sample Unity scenes to demo the use of them
- Link to camera calibration of HoloLens

## Example

The following example videos are taken with HoloLensARToolKit **[v0.1](https://github.com/qian256/HoloLensARToolKit/releases/tag/v0.1)**.

<table border=0>
<tr>
	<td align="center" width="50%"><img src="http://longqian.me/public/image/artoolkit-hololens-minion-thumb.png" /></td>
	<td align="center" width="50%"><img src="http://longqian.me/public/image/artoolkit-hololens-samples-thumb.png" /></td>
</tr>
<tr>
	<td align="center">Minion on the Cube (<a href="https://youtu.be/cMzNyJkr3X0"><b>Youtube Video</b></a>)</td>
	<td align="center">HoloLensARToolKit Samples (<a href="https://youtu.be/PqT90QfgP-U"><b>Youtube Video</b></a>)</td>
</tr>
</table>

## Run the sample
1. Make sure you have [HoloLens development tools](https://developer.microsoft.com/en-us/windows/mixed-reality/install_the_tools) installed.
	* HoloLens Emulator and Vuforia are not required.
2. Launch [Unity3D](https://unity3d.com/), open project folder ```HoloLensARSample```.
3. Import the unity package ```ARToolKitUWP.unitypackage```.
4. Open one of the sample scenes
	* ```Assets/Sample/HoloLensARToolKitSingle.unity```
	* ```Assets/Sample/HoloLensARToolKitCube.unity```
	* ```Assets/Sample/HoloLensARToolKitMulti.unity```
5. Configure HoloLens deployment settings and deploy to HoloLens.
	* See Chapter 4 and 5 of [Holograms 100](https://developer.microsoft.com/en-us/windows/mixed-reality/holograms_100).
6. Prepare the correspondent printed marker of ARToolKit.
	* Marker images are [Here](https://github.com/artoolkit/artoolkit5/tree/master/doc/patterns).
	* Make sure the printed marker size is same as configured in Unity.
7. See the effect.
	* **HoloLens clicker** or **air tap** is able to disable and enable video preview.

## Compatibility Tested
1. Unity 5.6.0f3
2. Visual Studio 2017 (Toolset v141)
3. Windows 10 SDK 10.0.10240.0 to 10.0.14393.0

## Understand HoloLensARToolKit v0.2
- [Overview: ARToolKit on HoloLens](http://longqian.me/2017/01/20/artoolkit-on-hololens/)
- [Unity native programming](http://longqian.me/2017/01/29/unity-native-programming/)
- [ARUWPController options v0.2]()
- [ARUWPMarker options v0.2]()
- [Coordinate systems in HoloLensARToolKit v0.2]()
- [Asynchronous computation in .NET]()


---

## Academic Citation

If this work helps your research, please cite the following [paper](https://arxiv.org/abs/1703.05834):

```tex
@online{1703.05834,
  Author = {Long Qian and Ehsan Azimi and Peter Kazanzides and Nassir Navab},
  Title = {Comprehensive Tracker Based Display Calibration for Holographic Optical See-Through Head-Mounted Display},
  Year = {2017},
  Eprint = {1703.05834},
  Eprinttype = {arXiv},
}
```


---

## License
HoloLensARToolKit is composed of two separate projects: ARToolKitUWP and ARToolKitUWP-Unity (including samples). Both of them are open for use in compliance with [GNU Lesser General Public License (LGPL v3.0)](https://www.gnu.org/licenses/lgpl-3.0.en.html). Please see COPYING and COPYING.LESSER for license details.

ARToolKitUWP is a modified version of ARToolKit, statically links against ARToolKit binaries, and therefore complies with the license restrictions of ARToolKit (see [details](https://github.com/artoolkit/artoolkit5)).



