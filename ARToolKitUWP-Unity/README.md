ARToolKitUWP-Unity
===
Unity3D package interfacing ARToolKitUWP, with C# scripts, pre-built plugin, customized inspector property, and some default streaming assets.

Author: [Long Qian](http://longqian.me/aboutme)

Date: 2019-01-11

## Scripts

- ```ARUWPNative.cs``` directly interfaces with ARToolKitUWP wrapper. It defines the entry point of native functions with parameter correctly marshaled, and various constants used for tracking configuration.
- ```ARUWPUtils.cs``` provides various utility functions serving the package.
- ```ARUWPMarker.cs``` defines a marker to be tracked by ARToolKitUWP, and relates tracking result with particular GameObjects in the scene.
	* ```Editor/ARUWPMarkerEditor.cs``` customizes the Unity inspector of ARUWPMarker objects.
- ```ARUWPVideo.cs``` controls the video pipeline that runs asychronously with Unity UI. It makes uses of Windows.MediaCapture APIs that are part of Universal Windows SDK.
	* ```Editor/ARUWPVideoEditor.cs``` customizes the Unity inspector of ARUWPVideo objects.
- ```ARUWPController.cs``` takes the overall control of the application, including marker management, video capture management, configures the behavior of native ARToolKitUWP tracking.
	* ```Editor/ARUWPControllerEditor.cs``` customizes the Unity inspector of ARUWPController objects.

## Streaming Assets
Files in this folder are shipped with the application, which means they will also be accessible during runtime. Marker configurations, pattern definition, camera calibration files are placed here.

```hololens896x504.dat``` is the camera calibration file of my HoloLens locatable camera. Details can be found in [HoloLensCamCalib](https://github.com/qian256/HoloLensCamCalib/tree/master).

## How to use the scripts?

- [ARUWPController options v0.2](http://longqian.me/2017/05/15/hololens-artoolkit-controller-v02/)
- [ARUWPMarker options v0.2](http://longqian.me/2017/05/15/hololens-artoolkit-marker-v02/)
- [ARUWPVideo options v0.2](http://longqian.me/2017/05/15/hololens-artoolkit-video-v02/)



---

## License
ARToolKitUWP-Unity (including samples) is open for use in compliance with [GNU Lesser General Public License (LGPL v3.0)](https://www.gnu.org/licenses/lgpl-3.0.en.html). Please see COPYING and COPYING.LESSER for license details.

