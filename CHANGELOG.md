# [1.3.0] - UNRELEASED

This version of NAS2D focuses on cleaning up the public interface headers and removing as much of the implementation details as possible. This is to help make NAS2D a lot easier to use to set up new projects and will reduce reliance on backend library interface headers unless the user specifically wants/needs direct access to these libraries.

## Added


## Changed

- Filesystem now throws an exception when any function is used before Filesystem::init() is called.
- Cleaned up the public interfaces for all objects removing backend library headers and implementation details from public classes.
- Moved signals into the NAS2D::Signals namespace.
- Clarified origin and licensing information for Signal.h and Delegate.h.
- Moved exceptions into NAS2D::exception namespace.
- Updated documentation on most interfaces.
- Cleaned out all `using namespace` directives from public interface headers.

## Deprecated

- Removed Exception class in favor of specific exception types derived from std::runtime_error. See [Exceptions](https://github.com/lairworks/nas2d-core/wiki/Exceptions-&-Exception-Handling-in-NAS2D).
- Removed Renderer::toggleCinematic().
- Removed Renderer::toggleLetterbox().
- Removed Random interface. See [Random Numbers](https://github.com/lairworks/nas2d-core/wiki/Random-Numbers).
- Removed MersenneTwister implementation. See [Random Numbers](https://github.com/lairworks/nas2d-core/wiki/Random-Numbers).

## Fixed


### IMPORTANT NOTES ABOUT BREAKING CHANGES

Since the Exception class has been removed, any code using NAS2D::Exception will need to be replaced. The easiest method to do this is to use `std::runtime_error()` or you may use a built-in exception type from the NAS2D::exception namespace. See [Issue #13](https://github.com/lairworks/nas2d-core/issues/13) for an explanation for the decision behind deprecating this class.

The Signals interface has been moved from the Gallant namespace to the NAS2D::Signals namespace. This will affect all code using custom Signals however using any of the built-in Signals (such as from the EventHandler or Sprite classes) will continue to function the same way without need for any changes.

Since all of the `using namespace` directives have been removed from all public interface headers, you will need to either use your own directive or specific they appropriate namespace for both `std::` and `NAS2D::` namespaces.

---

# [1.2.1] - 2017-03-02

## Added

- Added Visual Studio 2015 project files.
- Added version information coded into the library.
- Added several functions to get the numeric major/minor/patch versions of NAS2D.
- Added a function to get a complete version string.
- Added Double Click event.
- Added a Sprite::actions() function which returns a list of all available actions.
- Added increment/decrement frame functions to step through frames in a Sprite.
- Added Mixer::musicPlaying() to poll whether or not music is currently playing.
- Added a function to Sprite that allows for skipping frames.
- Added color tinting to Renderer::drawImage() and Renderer::drawSubImage().

## Changed

- Renamed SDL_Mixer to Mixer_SDL.
- Pulled out a lot of unnecessary debug code.
- StateManager no longer disconnects all event handlers when switching states.
- Improved implementation details encapsulation in OGL_Renderer and Mixer_SDL.
- Cleaned up superfluous comments in several modules including OGL_Renderer, Renderer and Mixer_SDL.
- Replaced all include guards with #pragma once to conform with newer C++11 standards.
- Updated copyright notice across all header files.
- Renderer::isFaded() now only returns true when fade is at full alpha instead of if alpha is any non-zero value.
- Game object will now default to a NULL Mixer if a Mixer_SDL fails to be instantiated.
- Updated OpenGL Renderer to use Triangle Strips for drawing primitives instead quads. This allows use of OpenGL Core context's without having to enable debug or compatibility modes.
- Moved raw pixel data in Image to the static resource table. Significantly improves memory overhead and load times when lots of Sprite's or Image's acre created.

## Fixed

- Fixed a mistake in Sprite which assumed that mCurrentFrame could be a negative (it is an unsigned int).
- Fixed a crash when terminating a Mixer_SDL when no audio output device is available.
- Fixed an edge case where loading an image from disk could result in mPixels being left NULL causing pixelColor() to fail unexpectedly.
- Fixed a crash in OGL_Renderer's init functions.
- Fixed a memory leak in Image Resource.
- Fixed color issues when using OGL_Renderer::drawImageStretched().
- Fixed the bugginess in Renderer::drawImageRect().

### IMPORTANT NOTES ABOUT BREAKING CHANGES

The StateManager no longer automatically disconnects listeners from the EventHandler when states are switched. This provides a fix for objects that connect themselves to the EventHandler before a State is fully initialized being disconnected.

This change requires that all objects that connect to the EventHandler, including States, manually disconnect themselves when they are destroyed. Mostly this means that every call to Event::Connect must have a matching Event::Disconnect. The easiest way to do this is to add the disconnect calls in the d'tors of objects that connect to the EventHandler.
