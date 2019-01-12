HoloLens with ARToolKit v0.2 (Feature Grayscale)
===
Author: [Long Qian](http://longqian.me/aboutme)

Date: 2019-01-11

## Overview

This **feature-grayscale** branch significantly improves the system performance. With this feature, tracking with ```1344x756x30fps``` video is able to maintain constantly 30 FPS, and rendering constantly at 60 FPS. Lower latency is achieved.

For general overview of **HoloLensARToolKit**, please refer to the [README](https://github.com/qian256/HoloLensARToolKit/blob/master/README.md) of master branch.

## Differences

- Grayscale image is provided to ARToolKit instead of RGBA image.
- With a new incoming video frame of NV12 format, the preceding ```width * height``` bytes are directly copied to a byte buffer, which is used for tracking and preview. These bytes are illuminance plane of NV12 image. Please refer to [Windows YUV Formats](https://docs.microsoft.com/en-us/windows/desktop/medfound/recommended-8-bit-yuv-formats-for-video-rendering#420-formats-12-bits-per-pixel) for more information. Multiple copies of SoftwareBitmap are avoided. 
- As a result, the preview image is also only grayscale. A custom Unity shader is provided to handle single-channel grayscale image. 


## Compatibility Tested

1. Unity 2018.3.1.f1
2. Visual Studio 2017 (Toolset v141)
3. ARToolKit 5.3.2
4. Windows 10 SDK 10.0.10240.0 to 10.0.17134.0


---

## Academic Citation

If this work helps your research, please cite the following paper:

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
HoloLensARToolKit is composed of two separate projects: ARToolKitUWP and ARToolKitUWP-Unity (including samples). Both of them are open for use in compliance with [GNU Lesser General Public License (LGPL v3.0)](https://www.gnu.org/licenses/lgpl-3.0.en.html). Please see COPYING and COPYING.LESSER for license details.

ARToolKitUWP is a modified version of ARToolKit, statically links against ARToolKit binaries, and therefore complies with the license restrictions of ARToolKit (see [details](https://github.com/artoolkit/artoolkit5)).



