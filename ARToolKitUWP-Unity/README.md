ARToolKitUWP-Unity
===
Unity3D package interfacing ARToolKitUWP, with C# scripts, pre-built plugin, customized inspector property, and some default streaming assets.

Author: [Long Qian](http://longqian.me/aboutme)

Date: 2017-02-14

## Scripts
- ```ARUWPNative.cs``` directly interfaces with ARToolKitUWP wrapper. It defines the entry point of native functions with parameter correctly marshaled, and various constants used for tracking configuration.
- ```ARUWPMarker.cs``` defines a marker to be tracked by ARToolKitUWP, and relates tracking result with particular GameObjects in the scene.
	* ```Editor/ARUWPMarkerEditor.cs``` customizes the Unity inspector of ARUWPMarker objects.
- ```ARUWPController.cs``` configures the behavior of ARToolKitUWP.
- ```ARUWPUtils.cs``` provides various utility functions serving the package.

## Streaming Assets
Files in this folder are shipped with the application, which means they will also be accessible during runtime. Marker configurations, pattern definition, camera calibration files are placed here.

```hololens896x504.dat``` is the camera calibration file of my HoloLens locatable camera. Details can be found in this [repository](https://github.com/qian256/HoloLensCamCalib/tree/master).

## How to use the scripts?

- [ARToolKitUWP controller options]()
- [ARToolKitUWP marker options]()



---

## License
ARToolKitUWP-Unity (including samples) is open for use in compliance with [GNU Lesser General Public License (LGPL v3.0)](https://www.gnu.org/licenses/lgpl-3.0.en.html). Please see COPYING and COPYING.LESSER for license details.

