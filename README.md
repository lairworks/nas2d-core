# NAS2D: Core

*NAS2D* is an open source, object oriented 2D game development framework written in portable C++. It was designed to make the development of games and other applications that needed a 2D interface easier by providing a high-level interface. It handles the low-level tasks of setting up the video display, input, sound mixing, file loading, etc.

One of the primary goals of *NAS2D* was to be cross-platform. A lot of effort has gone into the code to make it as platform independant as possible. The few areas that are different from platform to platform are hidden from the interface and is virtually transparent to the user.

## Why another 2D API?

*NAS2D* isn't just another 2D renderer. It's a complete set of classes and functions that let you jump into building a game right away.

## What Platforms are Supported?

Officially, *NAS2D* is supported on Windows (Vista+) and MacOS X (10.8+). Binaries, source code and IDE Project files are provided and maintained for these platforms.

*NAS2D* has been tested and works on Linux and BSD but there are no official maintainers for these platforms.

## NAS2D's History

*NAS2D* was born from the development efforts of another LairWorks project, [The Legend of Mazzeroth](http://lom.lairworks.com). After several months of development it became clear that core code that *LoM* was built on didn't change very much. We cleaned it up, pulled it out of the LoM project, repackaged it and the first version of NAS2D was released.
