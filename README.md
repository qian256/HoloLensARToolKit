HoloLens with ARToolKit v0.3
===
Author: [Long Qian](http://longqian.me/aboutme)

Date: 2020-04-19

## Overview

**HoloLensARToolKit** is a successful experiment to integrate [ARToolKit](http://artoolkit.org/) (v5.3.2) with [HoloLens](https://www.microsoft.com/microsoft-hololens/en-us) (**HoloLens 2 support added in v0.3**), or generally UWP (Universal Windows Platform). With fiducial marker tracking provided by ARToolKit, plus the indoor localization of HoloLens, many Augmented Reality applications would be made possible.

This repository contains:
- `ARToolKitUWP/`: Wrapper of [ARToolKit5](https://github.com/artoolkit/artoolkit5) for UWP
- `HoloLensARToolKit/`: Unity project with sample scenes and full package to demo this project
- `HoloLensCamCalib/`: Link to camera calibration of HoloLens
- `Markers/`: ARToolKit markers used for the samples
- `ARToolKitUWP.unitypackage`: Unity package with core libraries and scripts to interface the native library
- `ARToolKitUWP-Sample.unitypackage`: Unity package with sample scenes

## What's new in v0.3?

* HoloLensARToolKit v0.3 starts to support **HoloLens 2**, meaning that the core library ```ARToolKitUWP.dll``` for **ARM64** is available, in addition to **x86** for HoloLens 1. The video pipeline for HoloLens 2 is integrated into the Unity project API, currently supporting video configuration **1504 x 846 x 60 Hz** and **1504 x 846 x 30 Hz**. Sample camera calibration file for HoloLens 2 is provided.

* Grayscale video parsing, previously implemented in [feature-grayscale](https://github.com/qian256/HoloLensARToolKit/tree/feature-grayscale) branch is now forced in v0.3. It significantly reduces CPU memory copy, and therefore improves fiducial tracking throughput.

* A new script ```ARUWPTarget.cs``` is added to the Unity project API, which should be attached to root of virtual content registered with each marker. It also supports 6-DOF **smoothing**, preventing sudden jump due to tracking errors. 

* A calibration matrix, ```ARUWPMarker::calibrationMatrix```, allows users to offset the virtual rendering in 6-DOF, replacing the magic functions in older versions. 

* **IL2CPP** becomes the default scripting backend.



## How to use HoloLensARToolKit

Please refer to: **[Instructions to run HoloLens v0.3 samples](https://github.com/qian256/HoloLensARToolKit/blob/master/HoloLensARToolKit/README.md)**.

To use HoloLensARToolKit with your own project, simply import ```ARToolKitUWP.unitypackage``` to your Unity project.



## Examples from v0.1

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

## Understand HoloLensARToolKit

**The posts are created for HoloLensARToolKit v0.2, therefore some contents may be deprecated.**

- [Overview: ARToolKit on HoloLens](http://longqian.me/2017/01/20/artoolkit-on-hololens/)
- [Unity native programming](http://longqian.me/2017/01/29/unity-native-programming/)
- [ARUWPController options v0.2](http://longqian.me/2017/05/15/hololens-artoolkit-controller-v02/)
- [ARUWPMarker options v0.2](http://longqian.me/2017/05/15/hololens-artoolkit-marker-v02/)
- [ARUWPVideo options v0.2](http://longqian.me/2017/05/15/hololens-artoolkit-video-v02/)
- [Coordinate systems in HoloLensARToolKit v0.2](http://longqian.me/2017/05/15/hololens-artoolkit-coordinates-v02/)


---

## Academic Citation

If this work helps your research, please cite the following paper:

```tex
@article{qian2018arssist,
  title={ARssist: augmented reality on a head-mounted display for the first assistant in robotic surgery},
  author={Qian, Long and Deguet, Anton and Kazanzides, Peter},
  journal={Healthcare technology letters},
  volume={5},
  number={5},
  pages={194--200},
  year={2018},
  publisher={IET}
}
```

If you use HoloLensARToolKit v0.2, please cite:

```tex
@article{azimiqian2018alignment,
  title={Alignment of the Virtual Scene to the 3D Display Space of a Mixed Reality Head-Mounted Display},
  author={Azimi, Ehsan and Qian, Long and Navab, Nassir and Kazanzides, Peter},
  journal={arXiv preprint arXiv:1703.05834},
  year={2018}
}
```


---

## License

HoloLensARToolKit is open for use in compliance with [GNU Lesser General Public License (LGPL v3.0)](https://www.gnu.org/licenses/lgpl-3.0.en.html). Please see COPYING and COPYING.LESSER for license details.


