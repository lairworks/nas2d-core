# NAS2D: Core

[![AppVeyor](https://ci.appveyor.com/api/projects/status/github/lairworks/nas2d-core?branch=master&svg=true)](https://ci.appveyor.com/project/OPU/nas2d-core)
[![CircleCI](https://circleci.com/gh/lairworks/nas2d-core/tree/master.svg?style=svg)](https://circleci.com/gh/lairworks/nas2d-core/tree/master)
[![Codacy](https://api.codacy.com/project/badge/Grade/bcf32f1b0a864f3ead9d45ae6099d07c)](https://www.codacy.com/app/ldicker83/nas2d-core)

*NAS2D* is an open source, object oriented 2D game development framework written in portable C++. It was designed to make the development of games and other applications that needed a 2D interface easier by providing a high-level interface. It handles the low-level tasks of setting up the video display, input, sound mixing, file loading, etc.

One of the primary goals of *NAS2D* was to be cross-platform. A lot of effort has gone into the code to make it as platform independant as possible. The few areas that are different from platform to platform are hidden from the interface and is virtually transparent to the user.

## Why another 2D API

*NAS2D* isn't just another 2D renderer. It's a complete set of classes and functions that let you jump into building a game right away.

## What Platforms are Supported

Officially, *NAS2D* is supported on Windows (Vista+) and MacOS X (10.8+). Binaries, source code and IDE Project files are provided and maintained for these platforms.

*NAS2D* has been tested and works on Linux and BSD but there are no official maintainers for these platforms.

## NAS2D's History

*NAS2D* was born from the development efforts of another LairWorks project, [The Legend of Mazzeroth](http://lom.lairworks.com). After several months of development it became clear that core code that *LoM* was built on didn't change very much. We cleaned it up, pulled it out of the LoM project, repackaged it and the first version of NAS2D was released.

## License

NAS2D  is licensed under the zlib license. See LICENSE.txt for details.

---

## Vcpkg integration

The NAS2D project uses Vcpkg to integrate any dependencies without explicitly adding the source to the project directly. Vcpkg can be installed in two different forms: user-wide and project-specific.

### User-wide integration

1.  Clone `vcpkg`.
2.  Run the command `./bootstrap-vcpkg.bat`.
3.  Run the command `vcpkg integrate install`.
4.  Build the NAS2D solution to install all dependencies.

### Project-specific integration

If you do not want to install vcpkg in a user-wide context, vcpkg allows for a local NuGet package to be created. The following steps are specific to NAS2D:

1.  Clone `vcpkg`.
2.  Run the command `./bootstrap-vcpkg.bat`.
3.  Run the command `vcpkg integrate project`.
4.  Copy the generated NuGet install command.
5.  Open Visual Studio's Package Manager Console (Tools > NuGet Package Manager > Package Manager Console).
6.  Paste and Run the generated NuGet install command.
7.  Build the NAS2D solution to install all dependencies.

From this point any missing headers should be found and future compilation will be successful.

> <span style="color:red">**WARNING** Do not commit the NuGet-specific changes made to the project in `.vcxproj`, `.vcxproj.filters`, `packages.config`, or the folder `./packages/...`. In addition to these changes being local to your system, they may contain potentially sensitive or personally identifiable information in the generated absolute path!</span>

A workaround to work with the project file when adding or removing files or filters is to stash the changes with an appropriate name.

For best results, you should stash the changes immediately after installing the local NuGet package so as to not include any unrelated changes.

> <span style="color:red">**NOTICE** Committing the NuGet changes to the project will break it for all other developers due to the local absolute path.</span>