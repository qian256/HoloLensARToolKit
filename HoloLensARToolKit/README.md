HoloLensARToolKit
===
A Unity3D application using ARToolKitUWP. It has already imported ```ARToolKitUWP.unitypackage``` and ```ARToolKitUWP-Sample.unitypackage```.

Author: [Long Qian](http://longqian.me/aboutme)

Date: 2020-04-19

## Overview

This folder contains samples that utilize ARToolKitUWP to build applications based on Unity3D platform. It works with both HoloLens 1 and HoloLens 2. The samples unity scenes for deployment are located at:

* For HoloLens 1:
	* ```Assets/Sample/HL1ARToolKitSingle.unity```
	* ```Assets/Sample/HL1ARToolKitCube.unity```
	* ```Assets/Sample/HL1ARToolKitMulti.unity```
	* ```Assets/Sample/HL1ARToolKitCoords.unity```

* For HoloLens 2:
	* ```Assets/Sample/HL2ARToolKitSingle.unity```
	* ```Assets/Sample/HL2ARToolKitCube.unity```
	* ```Assets/Sample/HL2ARToolKitMulti.unity```
	* ```Assets/Sample/HL2ARToolKitCoords.unity```


## Build to HoloLens

1. Choose a sample scene based on your device
2. Click on the GameObject ```ARUWP Controller```, and verify that
	* ```ARUWP Video / Video Parameter``` matches your device
	* ```ARUWP Controller / Camera Param Filename``` matches the above video parameter, and the calibration file exists in the ```StreamingAssets/``` folder
3. Open ```File / Build Settings```, and verify that
	* Build target is **Universal Windows Platform**
	* The selected sample scene is added for build
	* Target Device is **HoloLens**
	* Architecture is set to **x86** for HoloLens 1, or **ARM64** for HoloLens 2
	* Other general settings for HoloLens project should apply
4. Hit **Build**, and specify a path for the Visual Studio solution
5. Open the Visual Studio Solution, and change the following settings:
	* Configuration is set to **Release, x86** for HoloLens 1, or **Release, ARM64** for HoloLens 2
	* Deploy to **Device** or **Remote Machine**
6. The Unity application will automatically start on the device
7. Prepare a paper marker as the tracking target based on the sample application
	* The marker for print is in ```../Marker``` folder
8. You will see the **Preview Video** at the top left corner, and applications stats at the top right corner. Once the paper marker is present, the virtual content will appear.


## Further Notice

* HoloLensARToolKit may not work with Mixed Reality Capture of HoloLens.
* The ```ARUWP Video / Enable Video Preview``` checkbox in the Unity inspector should be turned off in real applications (when performance matters).
* When the virtual content is not registered well, you shoud consider:
	* Calibrate your HoloLens camera, following **[HoloLensCamCalib](https://github.com/qian256/HoloLensCamCalib)** repository, then point to the customized calibration file in ```ARUWP Controller / Camera Param Filename```.
	* Set the offset matrix in ```ARUWPMarker::calibrationMatrix```. The Matrix4x4 will be automatically left-multiplied to whatever tracking results. For example, if you want to chage the position of the virtual content, you can modify the last column of the calibration matrix.


## Compatibility

The following versions of the software are used for development. It should support older (but not too old) version.

* Unity 2019.2.21f1
* Windows 10 SDK 10.10240.0 +
* Visual Studio 2017 for deployment to HoloLens 1
* Visaul Studio 2019 for deployment to HoloLens 2

---

## License
These samples and its dependent project, HoloLensARToolKit, are open for use in compliance with [GNU Lesser General Public License (LGPL v3.0)](https://www.gnu.org/licenses/lgpl-3.0.en.html). Please see COPYING and COPYING.LESSER for license details.
