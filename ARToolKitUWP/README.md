ARToolKitUWP
===
ARToolKit Wrapper for UWP (Universal Windows Platform)

Author: [Long Qian](http://longqian.me/aboutme)

Date: 2017-01-24

## Overview
ARWrapper library provided by [ARToolKit](https://github.com/artoolkit/artoolkit5) is compatible with multiple platforms, but not with UWP yet. This project intends to fill this gap. The native library built by this project consumes UWP runtime, and can be accessed by .NET applications via [PInovoke](http://www.pinvoke.net/).

## What's in?
```ARToolKitUWP.sln``` solution builds a dynamic link library for x86, namely, ```ARToolKitUWP.dll```. This library incorperates the following ARToolKit components:  
- AR.lib
- AR2.lib
- ARICP.lib
- ARMulti.lib

Therefore, this wrapper is capable of detecting single-pattern markers and multi-pattern markers. NFT (Natural Feature Marker) is not supported right now, because of its various unsatisfied dependencies on UWP.

## Exported APIs
The exported functions from this dll are listed in ```ARToolKitUWP.h```.

## Build
- Make sure you have [Visual Studio 2015 update 3](https://www.visualstudio.com/en-us/news/releasenotes/vs2015-update3-vs) and [Windows 10 SDK](https://developer.microsoft.com/en-us/windows/downloads/windows-10-sdk) installed.
- Open solution file ```ARToolKitUWP.sln```, and hit ```Build Solution```.
- Find the dll file. Simple.

## For Unity3D and HoloLens
Place the built library in Unity project folder: ```Assets/Plugins/WSA/x86/```.


---

## License
ARToolKitUWP is open for use in compliance with [GNU Lesser General Public License (LGPL v3.0)](https://www.gnu.org/licenses/lgpl-3.0.en.html). Please see COPYING and COPYING.LESSER for license details.

ARToolKitUWP is a modified version of ARToolKit, statically links against ARToolKit binaries, and therefore complies with the license restrictions of ARToolKit (see [details](https://github.com/artoolkit/artoolkit5)).



