// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================
#include "EventHandler.h"

#include "EnumKeyModifier.h"

#include <SDL2/SDL.h>


// UGLY ASS HACK!
// This is required for mouse grabbing in the EventHandler class.
extern SDL_Window* underlyingWindow;


using namespace NAS2D;


namespace NAS2D
{
	/**
	 * Posts a quit event to the event system.
	 */
	void postQuitEvent()
	{
		SDL_Event event;
		event.type = SDL_QUIT;
		SDL_PushEvent(&event);
	}
}


/**
 * Triggered whenever the application gains or loses focus.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect({this, &Object::function});
 * \endcode
 *
 * \code
 * void function(bool gained);
 * \endcode
 *
 * \arg \c gained Bool value indicating whether or not the app lost focus.
 */
EventHandler::ActivateSignal::Source& EventHandler::activate()
{
	return mActivateSignal;
}


/**
 * Triggered whenever the application's window is hidden or exposed.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect({this, &Object::function});
 * \endcode
 *
 * \code
 * void function(bool hidden);
 * \endcode
 *
 * \arg \c gained Bool value indicating whether or not the window was hidden.
 */
EventHandler::WindowHiddenSignal::Source& EventHandler::windowHidden()
{
	return mWindowHiddenSignal;
}


/**
 * Triggered whenever the application's window is exposed.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect({this, &Object::function});
 * \endcode
 *
 * \code
 * void function(void);
 * \endcode
 */
EventHandler::WindowExposedSignal::Source& EventHandler::windowExposed()
{
	return mWindowExposedSignal;
}


/**
 * Triggered whenever the application's window is minimized.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect({this, &Object::function});
 * \endcode
 *
 * \code
 * void function(void);
 * \endcode
 */
EventHandler::WindowMinimizedSignal::Source& EventHandler::windowMinimized()
{
	return mWindowMinimizedSignal;
}


/**
 * Triggered whenever the application's window is maximized.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect({this, &Object::function});
 * \endcode
 *
 * \code
 * void function(void);
 * \endcode
 */
EventHandler::WindowMaximizedSignal::Source& EventHandler::windowMaximized()
{
	return mWindowMaximizedSignal;
}


/**
 * Triggered whenever the application's window is restored to its original size and location.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect({this, &Object::function});
 * \endcode
 *
 * \code
 * void function(void);
 * \endcode
 */
EventHandler::WindowRestoredSignal::Source& EventHandler::windowRestored()
{
	return mWindowRestoredSignal;
}


/**
 * Triggered whenever the window is resized.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect({this, &Object::function});
 * \endcode
 *
 * \code
 * void function(void);
 * \endcode
 */
EventHandler::WindowResizedSignal::Source& EventHandler::windowResized()
{
	return mWindowResizedSignal;
}


/**
 * Triggered whenever the mouse enters the application's window.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect({this, &Object::function});
 * \endcode
 *
 * \code
 * void function(void);
 * \endcode
 */
EventHandler::WindowMouseEnterSignal::Source& EventHandler::windowMouseEnter()
{
	return mWindowMouseEnterSignal;
}


/**
 * Triggered whenever the mouse leaves the application's window.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect({this, &Object::function});
 * \endcode
 *
 * \code
 * void function(void);
 * \endcode
 */
EventHandler::WindowMouseLeaveSignal::Source& EventHandler::windowMouseLeave()
{
	return mWindowMouseLeaveSignal;
}


/**
 * Joystick axis events generate three values:
 * Device ID, Axis ID and Axis Position.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect({this, &Object::function});
 * \endcode
 *
 * \code
 * void function(int deviceId, int axisId, int pos);
 * \endcode
 *
 * \arg \c deviceId Joystick ID which this event was generated from.
 * \arg \c axisId Axis ID. Axis 0 is x, Axis 1 is Y.
 * Some joysticks use additional axis as buttons.
 * \arg \c pos Current position of the axis.
 */
EventHandler::JoystickAxisMotionSignal::Source& EventHandler::joystickAxisMotion()
{
	return mJoystickAxisMotionSignal;
}


/**
 * Joystick trackball events generate three values:
 * Device ID, Ball ID, X and Y motion change.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect({this, &Object::function});
 * \endcode
 *
 * \code
 * void function(int deviceId, int ballId, NAS2D::Vector<int> change);
 * \endcode
 *
 * \arg \c deviceId Joystick ID which this event was generated from.
 * \arg \c ballId Trackball ID.
 * \arg \c change Change in relative position.
 */
EventHandler::JoystickBallMotionSignal::Source& EventHandler::joystickBallMotion()
{
	return mJoystickBallMotionSignal;
}


/**
 * Joystick button events generate two values:
 * Device ID, and Button ID
 * Button events occur when a joystick button is pressed or released.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect({this, &Object::function});
 * \endcode
 *
 * \code
 * void function(int deviceId, int buttonId);
 * \endcode
 *
 * \arg \c deviceId	Joystick ID which this event was generated from.
 * \arg \c buttonId	Button ID which the event was generated from.
 */
EventHandler::JoystickButtonSignal::Source& EventHandler::joystickButtonUp()
{
	return mJoystickButtonUpSignal;
}


/**
 * Joystick button events generate two values:
 * Device ID, and Button ID
 * Button events occur when a joystick button is pressed or released.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect({this, &Object::function});
 * \endcode
 *
 * \code
 * void function(int deviceId, int buttonId);
 * \endcode
 *
 * \arg \c deviceId	Joystick ID which this event was generated from.
 * \arg \c buttonId	Button ID which the event was generated from.
 */
EventHandler::JoystickButtonSignal::Source& EventHandler::joystickButtonDown()
{
	return mJoystickButtonDownSignal;
}


/**
 * Joystick hat motion events generate three values:
 * Device ID, Hat ID, and Hat Position.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect({this, &Object::function});
 * \endcode
 *
 * \code
 * void function(int deviceId, int hatId, int pos);
 * \endcode
 *
 * \arg \c deviceId	Joystick ID which this event was generated from.
 * \arg \c hatId	Hat ID.
 * \arg \c pos		Current position of the hat.
 */
EventHandler::JoystickHatMotionSignal::Source& EventHandler::joystickHatMotion()
{
	return mJoystickHatMotionSignal;
}


/**
 * Keyboard key down events generate three values:
 * Keyboard scan code, key modifier, and a repeat flag.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect({this, &Object::function});
 * \endcode
 *
 * \code
 * void function(KeyCode key, KeyModifier mod, bool repeat);
 * \endcode
 *
 * \arg \c key		KeyCode representing a key on the keyboard.
 * \arg \c mod		Keyboard modifier.
 * \arg \c repeat	Indicates that this event is a repeat and not an initial key event.
 */
EventHandler::KeyDownSignal::Source& EventHandler::keyDown()
{
	return mKeyDownSignal;
}


/**
 * Keyboard key up events generate three values:
 * Keyboard scan code, key modifier, and a repeat flag.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect({this, &Object::function});
 * \endcode
 *
 * \code
 * void function(KeyCode key, KeyModifier mod);
 * \endcode
 *
 * \arg \c key		KeyCode representing a key on the keyboard.
 * \arg \c mod		Keyboard modifier.
 */
EventHandler::KeyUpSignal::Source& EventHandler::keyUp()
{
	return mKeyUpSignal;
}


/**
 * Fires whenever text input events occur.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect({this, &Object::function});
 * \endcode
 *
 * \code
 * void function(const std::string&);
 * \endcode
 */
EventHandler::TextInputSignal::Source& EventHandler::textInput()
{
	return mTextInputSignal;
}


/**
 * Mouse button events generate two values:
 * MouseButton, position of mouse.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect({this, &Object::function});
 * \endcode
 *
 * \code
 * void function(MouseButton button, NAS2D::Point<int> position);
 * \endcode
 *
 * \arg \c button: MouseButton value indicating which button is pressed.
 * \arg \c position: Position of the mouse button event.
 */
EventHandler::MouseButtonSignal::Source& EventHandler::mouseButtonDown()
{
	return mMouseButtonDownSignal;
}


/**
 * Mouse button events generate two values:
 * MouseButton, position of mouse.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect({this, &Object::function});
 * \endcode
 *
 * \code
 * void function(MouseButton button, NAS2D::Point<int> position);
 * \endcode
 *
 * \arg \c button: MouseButton value indicating which button is pressed.
 * \arg \c position: Position of the mouse button event.
 */
EventHandler::MouseButtonSignal::Source& EventHandler::mouseButtonUp()
{
	return mMouseButtonUpSignal;
}


/**
 * Mouse button events generate two values:
 * MouseButton, position of mouse.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect({this, &Object::function});
 * \endcode
 *
 * \code
 * void function(MouseButton button, NAS2D::Point<int> position);
 * \endcode
 *
 * \arg \c button: MouseButton value indicating which button is pressed.
 * \arg \c position: Position of the mouse button event.
 */
EventHandler::MouseButtonSignal::Source& EventHandler::mouseDoubleClick()
{
	return mMouseDoubleClickSignal;
}


/**
 * Mouse motion events generate two values:
 * mouse position, and relative position change.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect({this, &Object::function});
 * \endcode
 *
 * \code
 * void function(NAS2D::Point<int> position, NAS2D::Vector<int> change);
 * \endcode
 *
 * \arg \c position: Absolute position of the mouse.
 * \arg \c change: position of the mouse relative to its last position.
 */
EventHandler::MouseMotionSignal::Source& EventHandler::mouseMotion()
{
	return mMouseMotionSignal;
}


/**
 * Mouse wheel events generate one value:
 * change in the x and y motion values.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect({this, &Object::function});
 * \endcode
 *
 * \code
 * void function(NAS2D::Vector<int> change);
 * \endcode
 *
 * \arg \c change: Change along the X and Y axis.
 *
 * \note The value given in the axis parameters is the value
 * of a single 'click' of the mouse wheel. This may be
 * more than one (on Windows this value is typical 120,
 * not 1).
 */
EventHandler::MouseWheelSignal::Source& EventHandler::mouseWheel()
{
	return mMouseWheelSignal;
}


/**
 * Triggered whenever a Quit message is posted.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect({this, &Object::function});
 * \endcode
 *
 * \code
 * void function(void);
 * \endcode
 */
EventHandler::QuitSignal::Source& EventHandler::quit()
{
	return mQuitSignal;
}


/**
 * Sets the mouse pointer to a specified location within the application window.
 *
 * \param x X-Coordinate.
 * \param y Y-Coordinate.
 *
 * \note	Coordinates can only be set within the bounds of the application's
 *			window. Coordinates will be clamped for all other values.
 */
void EventHandler::warpMouse(int x, int y)
{
	if (underlyingWindow)
	{
		SDL_WarpMouseInWindow(underlyingWindow, x, y);
		mMouseMotionSignal.emit({x, y}, {0, 0});
	}
}


/**
 * Sets mouse updates to relative or absolute motion.
 *
 * \param	rel		True for relative mode. False for absolute mode.
 */
void EventHandler::mouseRelativeMode(bool rel)
{
	rel ? SDL_SetRelativeMouseMode(SDL_TRUE) : SDL_SetRelativeMouseMode(SDL_FALSE);
}


/**
 * Updates the event pump and calls any associated callbacks.
 */
void EventHandler::pump()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		onMessage(event);
	}
}


void EventHandler::onMessage(const SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_MOUSEMOTION:
		mMouseMotionSignal({event.motion.x, event.motion.y}, {event.motion.xrel, event.motion.yrel});
		break;

	case SDL_KEYDOWN:
		mKeyDownSignal(static_cast<KeyCode>(event.key.keysym.sym), static_cast<KeyModifier>(event.key.keysym.mod), event.key.repeat != 0 ? true : false);
		break;

	case SDL_KEYUP:
		mKeyUpSignal(static_cast<KeyCode>(event.key.keysym.sym), static_cast<KeyModifier>(event.key.keysym.mod));
		break;

	case SDL_TEXTINPUT:
		mTextInputSignal(event.text.text);
		break;

	case SDL_MOUSEBUTTONDOWN:
		if (event.button.clicks == 2)
		{
			mMouseDoubleClickSignal(static_cast<MouseButton>(event.button.button), {event.button.x, event.button.y});
		}

		mMouseButtonDownSignal(static_cast<MouseButton>(event.button.button), {event.button.x, event.button.y});
		break;

	case SDL_MOUSEBUTTONUP:
		mMouseButtonUpSignal(static_cast<MouseButton>(event.button.button), {event.button.x, event.button.y});
		break;

	case SDL_MOUSEWHEEL:
		mMouseWheelSignal({event.wheel.x, event.wheel.y});
		break;

	case SDL_JOYAXISMOTION:
		mJoystickAxisMotionSignal(event.jaxis.which, event.jaxis.axis, event.jaxis.value);
		break;

	case SDL_JOYBALLMOTION:
		mJoystickBallMotionSignal(event.jball.which, event.jball.ball, {event.jball.xrel, event.jball.yrel});
		break;

	case SDL_JOYHATMOTION:
		mJoystickHatMotionSignal(event.jhat.which, event.jhat.hat, event.jhat.value);
		break;

	case SDL_JOYBUTTONDOWN:
		mJoystickButtonDownSignal(event.jbutton.which, event.jbutton.button);
		break;

	case SDL_JOYBUTTONUP:
		mJoystickButtonUpSignal(event.jbutton.which, event.jbutton.button);
		break;

	case SDL_WINDOWEVENT:
		// Not completely happy with this but meh, it works.
		if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) { mActivateSignal(true); }
		else if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) { mActivateSignal(false); }
		else if (event.window.event == SDL_WINDOWEVENT_SHOWN) { mWindowHiddenSignal(false); }
		else if (event.window.event == SDL_WINDOWEVENT_HIDDEN) { mWindowHiddenSignal(true); }
		else if (event.window.event == SDL_WINDOWEVENT_EXPOSED) { mWindowExposedSignal(); }
		else if (event.window.event == SDL_WINDOWEVENT_MINIMIZED) { mWindowMinimizedSignal(); }
		else if (event.window.event == SDL_WINDOWEVENT_MAXIMIZED) { mWindowMaximizedSignal(); }
		else if (event.window.event == SDL_WINDOWEVENT_RESTORED) { mWindowRestoredSignal(); }
		else if (event.window.event == SDL_WINDOWEVENT_ENTER) { mWindowMouseEnterSignal(); }
		else if (event.window.event == SDL_WINDOWEVENT_LEAVE) { mWindowMouseLeaveSignal(); }
		else if (event.window.event == SDL_WINDOWEVENT_RESIZED) { mWindowResizedSignal({event.window.data1, event.window.data2}); }
		break;

	case SDL_QUIT:
		mQuitSignal();
		break;

	default:
		// Ignore any cases not handled.
		break;
	}
}


/**
 * Turns on/off text input mode.
 *
 * \param _b Boolean value. True to turn on text input, false to turn it off.
 */
void EventHandler::textInputMode(bool _b)
{
	if (_b) { SDL_StartTextInput(); }
	else { SDL_StopTextInput(); }
}


/**
 * Queries text input mode.
 */
bool EventHandler::textInputMode()
{
	return SDL_IsTextInputActive() == SDL_TRUE;
}


/**
 * Decodes a KeyModifier and determines if the Shift keymod is applied.
 *
 * \param mod	Modifier value to decode.
 */
bool EventHandler::shift(KeyModifier mod)
{
	return KeyModifier::None != (mod & (KeyModifier::Shift));
}


/**
 * Decodes a KeyModifier and determines if the Shift keymod is applied.
 *
 * \param mod	Modifier value to decode.
 */
bool EventHandler::alt(KeyModifier mod)
{
	return KeyModifier::None != (mod & KeyModifier::Alt);
}


/**
 * Decodes a KeyModifier and determines if the Numlock keymod is applied.
 *
 * \param mod	Modifier value to decode.
 */
bool EventHandler::numlock(KeyModifier mod)
{
	return KeyModifier::None != (mod & KeyModifier::Num);
}


/**
 * Decodes a KeyModifier and determines if the Control keymod is applied.
 *
 * \param mod	Modifier value to decode.
 */
bool EventHandler::control(KeyModifier mod)
{
	return KeyModifier::None != (mod & KeyModifier::Ctrl);
}


/**
 * Queries state of the Shift key modifier.
 */
bool EventHandler::shift() const
{
	return shift(static_cast<KeyModifier>(SDL_GetModState()));
}


/**
 * Queries state of the Shift key modifier.
 */
bool EventHandler::numlock() const
{
	return numlock(static_cast<KeyModifier>(SDL_GetModState()));
}


/**
 * Queries state of the Shift key modifier.
 */
bool EventHandler::control() const
{
	return control(static_cast<KeyModifier>(SDL_GetModState()));
}


/**
 * Disconnects all connected signal handlers.
 */
void EventHandler::disconnectAll()
{
	mActivateSignal.clear();
	mWindowHiddenSignal.clear();
	mWindowExposedSignal.clear();
	mWindowMinimizedSignal.clear();
	mWindowMaximizedSignal.clear();
	mWindowRestoredSignal.clear();
	mWindowResizedSignal.clear();
	mWindowMouseEnterSignal.clear();
	mWindowMouseLeaveSignal.clear();
	mJoystickAxisMotionSignal.clear();
	mJoystickBallMotionSignal.clear();
	mJoystickButtonUpSignal.clear();
	mJoystickButtonDownSignal.clear();
	mJoystickHatMotionSignal.clear();
	mKeyDownSignal.clear();
	mKeyUpSignal.clear();
	mTextInputSignal.clear();
	mMouseButtonDownSignal.clear();
	mMouseButtonUpSignal.clear();
	mMouseDoubleClickSignal.clear();
	mMouseMotionSignal.clear();
	mMouseWheelSignal.clear();
	mQuitSignal.clear();
}
