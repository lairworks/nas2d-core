# NAS2D: Core

[![AppVeyor](https://ci.appveyor.com/api/projects/status/github/lairworks/nas2d-core?branch=master&svg=true)](https://ci.appveyor.com/project/OPU/nas2d-core)
[![CircleCI](https://dl.circleci.com/status-badge/img/gh/lairworks/nas2d-core/tree/main.svg?style=shield )](https://dl.circleci.com/status-badge/redirect/gh/lairworks/nas2d-core/tree/main)

*NAS2D* is an open source, object oriented 2D game development framework written in portable C++. It was designed to make the development of games and other applications that needed a 2D interface easier by providing a high-level interface. It handles the low-level tasks of setting up the video display, input, sound mixing, file loading, etc.

One of the primary goals of *NAS2D* was to be cross-platform. A lot of effort has gone into the code to make it as platform independent as possible. The few areas that are different from platform to platform are hidden from the interface and is virtually transparent to the user.

## Why another 2D API

*NAS2D* isn't just another 2D renderer. It's a complete set of classes and functions that let you jump into building a game right away.

## What Platforms are Supported

Automated builds are done for Windows, macOS, and Linux (Ubuntu and Arch). We provide Visual Studio project files for Windows, and a `makefile` based build for macOS and Linux. Dependencies are managed using platform specific tools: Vcpkg (Windows), Homebrew (macOS), apt (Ubuntu), pacman (Arch).

We are not currently providing precompiled binaries of recent builds for any platform. A source build is the expected way to use NAS2D.

## Windows Dependencies

Dependencies are managed using [Vcpkg](https://vcpkg.io/en/). Current dependencies can be found in [`vcpkg.json`](vcpkg.json).

Vcpkg integrates with the Visual Studio build environment to download dependencies automatically as part of the build. It requires a one time setup using:
```
vcpkg integrate install
```

Vcpkg updates the `AdditionalIncludeDirectories` preprocessor setting so header files of dependencies can be found, and the `AdditionalDependencies` linker setting so the `.lib` files of dependencies can be found.

If you see build errors about missing header files of dependencies, such as SDL, it's probably because `vcpkg integrate install` hasn't been run.

If you wish to avoid using Vcpkg, then you'll need to download and update dependencies yourself, and ensure proper settings for `AdditionalIncludeDirectories` and `AdditionalDependencies`.

## Linux and macOS Dependencies

To install dependencies for a [`makefile`](makefile) based build, there is a helper target that attempts to detect the platform and install dependencies using an appropriate platform specific package manager:
```sh
make install-dependencies
```

Typically `sudo` is used to run the command with root privilege, which will be required to run the system package manager to install the dependencies.

## NAS2D's History

*NAS2D* was born from the development efforts of another LairWorks project, [The Legend of Mazzeroth](http://lom.lairworks.com). After several months of development it became clear that core code that *LoM* was built on didn't change very much. We cleaned it up, pulled it out of the LoM project, repackaged it and the first version of NAS2D was released.

## License

NAS2D  is licensed under the zlib license. See LICENSE.txt for details.
