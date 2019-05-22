# Releases

## \[1.5.0\] - 2019-05-19

This is a feature release.

### Added

-   Added EventHandler::alt() which returns true if the key modifier is either alt key (or both).
-   Added a clip area function to the renderer (clipRect series of functions).
-   Added center_x/center_y functions to Image. This maintains a previously computed value based on the last loaded Image.
-   Visual Studio 2019 solution and project files have been added.
-   A null Renderer (RendererNull) has been added for fallback and testing purposes.
-   A null Mixer (MixerNull) has been added for fallback and testing purposes.

### Changed

-   All prior versions of Visual Studio solutions/projects have been removed in favor of Visual Studio 2019.
-   Visual Studio solution now takes advantage of NuGet. PhysFS and GLEW still need to be provided by the user as the NuGet versions are very out of date. We recommend using vcpkg.
-   OGL_Renderer has been renamed to RendererOpenGL.
-   Mixer_SDL has been renamed to MixerSDL.

### Fixed

-   OGL_Renderer::drawImageRepeated() now draws a texture repeated across a given face as expected.
-   Fixed an issue with XmlNode::lastChild(const std::string&) functions that would test against an internal value string instead of the value passed in as a parameter.
-   Fixed a mistake in OGL_Renderer::drawImage() that ignored the 'scale' paramter.
-   Renderer::drawImageToImage() will no longer clear the previously loaded image before a call to this function was made.
-   A defect in the Filesystem was corrected related to edge cases where the underlying library PhysFS is asked to deinit itself before it was ever initialized.

---

## \[1.4.2\] - 2017-05-15

This is a small patch to NAS2D that corrects a few minor issues not caught in the 1.4.1 release.

### Fixed

-   Rectangle_2d/Rectangle_2df center_x/center_y functions were not marked as const causing a variety of issues when attempting to use these functions.

---

## \[1.4.1\]  2017-05-14

This is a small patch to NAS2D that corrects a few minor issues not caught in the 1.4.0 release.

### Fixed

-   Version information has been corrected.
-   Window will now center itself when switching out of fullscreen mode.

---

## \[1.4.0\] - 2017-05-14

This version of NAS2D introduces several new features to make it easier to use geometric primitives, switch between fullscreen and windowed modes and resize the window.

### Added

-   Added Window Resize events to the EventHandler.
-   Added center_x/center_y functions to Rectangle_2d/Rectangle_2df.
-   Added ability to set an applications window icon.
-   Added ability to change the size of the application window.
-   Applications can now switch to fullscreen and windowed modes.
-   Added ability to set the minimum size for an applications window.
-   Added the ability to set an application window as resizeable.

### Changed

-   Renamed Renderer::screenSize() to Renderer::size();
-   Renamed Renderer::screenCenterX()/Renderer::screenCenterY() to Renderer::center_x()/Renderer::center_y().
-   Renderer will now respond to window resize events as they happen.

#### IMPORTANT NOTES ABOUT BREAKING CHANGES

Very little has changed internally but some functions have been renamed, particularly in the Renderer (`screenSize`, `screenCenterX` and `screenCenterY`). Your applications will need to be adjusted to account for this.

This release now links to SDL 2.0.5, be sure to use the updated SDL binary files included in the download package.

---

## \[1.3.1\] - 2017-04-05

This is a patch to NAS2D that fixes a few minor bugs.

### Changed

-   Improved Renderer's handling of fading and fade callbacks.

### Fixed

-   Fixed a mistake in Configuration's XML processing of Options tags.
-   Fixed an issue in OGL_Renderer's drawText that could result in an out of bounds exception being thrown.

---

## \[1.3.0\] - 2017-03-23

This version of NAS2D focuses on cleaning up the public interface headers and removing as much of the implementation details as possible. This is to help make NAS2D a lot easier to use to set up new projects and will reduce reliance on backend library interface headers unless the user specifically wants/needs direct access to these libraries.

### Added

-   Renderer fade completed signal.
-   Mixer music completed signal.
-   Added EventHandler::textInputMode(bool) to turn on and off text input modes.
-   Added EventHandler::textInput() to query whether or not text input mode is turned on.
-   Added EventHandller::query_control(), EventHandller::query_numlock() and EventHandller::query_shift() to query key modifier states.
-   Added Renderer::showSystemPointer() method.

### Changed

-   Clarified origin and licensing information for Signal.h and Delegate.h.
-   Updated documentation on most interfaces.
-   Filesystem now throws an exception when any function is used before Filesystem::init() is called.
-   Cleaned up the public interfaces for all objects removing backend library headers and implementation details from public classes.
-   Cleaned out all `using namespace` directives from public interface headers.
-   Moved exceptions into NAS2D::exception namespace.
-   Moved signals into the NAS2D::Signals namespace.
-   Merged MouseButton enumeration to EventHandler class declaration.
-   Merged KeyCode into EventHandler class instead of being stand alone.
-   Merged KeyTranslator::control(KeyModifier), KeyTranslator::numlock(KeyModifier) and KeyTranslator::shift(KeyModifier) into EventHandler.
-   Renamed all functions in Signal0 - Signal5 to lowercase names to match std:: and NAS2D library conventions.
-   Renamed ::w() and ::h() methods in in Rectangle_2d/Rectangle_2df to ::width() and ::height().
-   OGL_Renderer sets line quality for circle drawing code based on configuration's texture quality.
-   Merged TinyXML into the NAS2D namespace. See [Task #14](https://github.com/lairworks/nas2d-core/issues/14) for additional information.

### Deprecated

-   Removed Exception class in favor of specific exception types derived from std::runtime_error. See [Exceptions](https://github.com/lairworks/nas2d-core/wiki/Exceptions-&-Exception-Handling-in-NAS2D).
-   Removed Renderer::toggleCinematic().
-   Removed Renderer::toggleLetterbox().
-   Removed Renderer::drawTextClamped() and Renderer::drawTextClampedShadow().
-   Removed Random interface. See [Random Numbers](https://github.com/lairworks/nas2d-core/wiki/Random-Numbers).
-   Removed MersenneTwister implementation. See [Random Numbers](https://github.com/lairworks/nas2d-core/wiki/Random-Numbers).
-   Removed Image::rect().
-   Removed XmlAttributeParser.
-   Removed Sprite::debug().
-   Texture Quality configuration option has been removed.

### Fixed

-   Fixed a bug in Font::width(const std::string&) that would report incorrect widths when using TrueType or OpenType fonts.
-   Fixed a possible resource leak in Font copy assignment operator.
-   Fixed a possible resource leak in Image copy assignment operator.
-   Fixed a possible resource leak in Music copy assignment operator.
-   Fixed OGL_Renderer::drawGradient() showing incorrect colors.

#### IMPORTANT NOTES ABOUT BREAKING CHANGES

Since the `Exception` class has been removed, any code using `NAS2D::Exception` will need to be replaced. The easiest method to do this is to use `std::runtime_error()` or you may use a built-in exception type from the `NAS2D::exception` namespace. See [Issue #13](https://github.com/lairworks/nas2d-core/issues/13) for an explanation of the decision behind deprecating this class.

The Signals interface has been moved from the `Gallant` namespace to the `NAS2D::Signals` namespace. This will affect all code using custom Signals however using any of the built-in Signals (such as from the `EventHandler` or `Sprite` classes) will continue to function the same way without need for any changes. Additionally, all method names have been renamed to all lowercase names to match `NAS2D` and `std::` library conventions.

All `::w()` and `::h()` method names across the entire interface have been renamed to `::width()` and `::height()`.

Since all of the `using namespace` directives have been removed from all public interface headers, you will need to either use your own directive or specific the appropriate namespace for both `std::` and `NAS2D::` namespaces.

The `KeyTranslator` object has been removed. Useful functionality has been merged into the `EventHandler` class. Additionally, the `KeyCode` and `MouseButton` enumerators were also merged into the `EventHandler`. This means that you will either need to append `EventHander::` to your calls or add a `using namespace NAS2D::EventHandler` to your source files. See [Issue #23](https://github.com/lairworks/nas2d-core/issues/23) for an in-depth explanation.

TinyXML's interface has been merged into the `NAS2D::Xml` namespace, all objects have been renamed and its interface has been updated to reflect NAS2D naming conventions. Documentation has been thoroughly updated and is now included as part of NAS2D's main documentation. See [Task #14](https://github.com/lairworks/nas2d-core/issues/14) for additional information.

---

## \[1.2.1\] - 2017-03-02

### Added

-   Added Visual Studio 2015 project files.
-   Added version information coded into the library.
-   Added several functions to get the numeric major/minor/patch versions of NAS2D.
-   Added a function to get a complete version string.
-   Added Double Click event.
-   Added a Sprite::actions() function which returns a list of all available actions.
-   Added increment/decrement frame functions to step through frames in a Sprite.
-   Added Mixer::musicPlaying() to poll whether or not music is currently playing.
-   Added a function to Sprite that allows for skipping frames.
-   Added color tinting to Renderer::drawImage() and Renderer::drawSubImage().

### Changed

-   Renamed SDL_Mixer to Mixer_SDL.
-   Pulled out a lot of unnecessary debug code.
-   StateManager no longer disconnects all event handlers when switching states.
-   Improved implementation details encapsulation in OGL_Renderer and Mixer_SDL.
-   Cleaned up superfluous comments in several modules including OGL_Renderer, Renderer and Mixer_SDL.
-   Replaced all include guards with #pragma once to conform with newer C++11 standards.
-   Updated copyright notice across all header files.
-   Renderer::isFaded() now only returns true when fade is at full alpha instead of if alpha is any non-zero value.
-   Game object will now default to a NULL Mixer if a Mixer_SDL fails to be instantiated.
-   Updated OpenGL Renderer to use Triangle Strips for drawing primitives instead quads. This allows use of OpenGL Core context's without having to enable debug or compatibility modes.
-   Moved raw pixel data in Image to the static resource table. Significantly improves memory overhead and load times when lots of Sprite's or Image's acre created.

### Fixed

-   Fixed a mistake in Sprite which assumed that mCurrentFrame could be a negative (it is an unsigned int).
-   Fixed a crash when terminating a Mixer_SDL when no audio output device is available.
-   Fixed an edge case where loading an image from disk could result in mPixels being left NULL causing pixelColor() to fail unexpectedly.
-   Fixed a crash in OGL_Renderer's init functions.
-   Fixed a memory leak in Image Resource.
-   Fixed color issues when using OGL_Renderer::drawImageStretched().
-   Fixed the bugginess in Renderer::drawImageRect().

#### IMPORTANT NOTES ABOUT BREAKING CHANGES

The StateManager no longer automatically disconnects listeners from the EventHandler when states are switched. This provides a fix for objects that connect themselves to the EventHandler before a State is fully initialized being disconnected.

This change requires that all objects that connect to the EventHandler, including States, manually disconnect themselves when they are destroyed. Mostly this means that every call to Event::Connect must have a matching Event::Disconnect. The easiest way to do this is to add the disconnect calls in the d'tors of objects that connect to the EventHandler.
