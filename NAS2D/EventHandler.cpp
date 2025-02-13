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

#include "EventHandlerKeyModifier.h"

#include <SDL2/SDL.h>


// UGLY ASS HACK!
// This is required for mouse grabbing in the EventHandler class.
extern SDL_Window* underlyingWindow;


using namespace NAS2D;

/**
 * Maximum number of events to process each frame.
 *
 * \todo	Make this configurable?
 */
const int MAX_MESSAGE_PROCESSING = 100;

namespace NAS2D
{
	EventHandler::KeyModifier& operator|=(EventHandler::KeyModifier& a, const EventHandler::KeyModifier& b) noexcept
	{
		using underlying = std::underlying_type_t<EventHandler::KeyModifier>;
		auto underlying_a = static_cast<underlying>(a);
		auto underlying_b = static_cast<underlying>(b);
		a = static_cast<EventHandler::KeyModifier>(underlying_a | underlying_b);
		return a;
	}

	EventHandler::KeyModifier& operator&=(EventHandler::KeyModifier& a, const EventHandler::KeyModifier& b) noexcept
	{
		using underlying = std::underlying_type_t<EventHandler::KeyModifier>;
		auto underlying_a = static_cast<underlying>(a);
		auto underlying_b = static_cast<underlying>(b);
		a = static_cast<EventHandler::KeyModifier>(underlying_a & underlying_b);
		return a;
	}

	EventHandler::KeyModifier operator|(EventHandler::KeyModifier a, const EventHandler::KeyModifier& b) noexcept
	{
		a |= b;
		return a;
	}

	EventHandler::KeyModifier operator&(EventHandler::KeyModifier a, const EventHandler::KeyModifier& b) noexcept
	{
		a &= b;
		return a;
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
EventHandler::ActivateEvent::Source& EventHandler::activate()
{
	return mActivateEvent;
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
EventHandler::WindowHiddenEvent::Source& EventHandler::windowHidden()
{
	return mWindowHiddenEvent;
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
EventHandler::WindowExposedEvent::Source& EventHandler::windowExposed()
{
	return mWindowExposedEvent;
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
EventHandler::WindowMinimizedEvent::Source& EventHandler::windowMinimized()
{
	return mWindowMinimizedEvent;
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
EventHandler::WindowMaximizedEvent::Source& EventHandler::windowMaximized()
{
	return mWindowMaximizedEvent;
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
EventHandler::WindowRestoredEvent::Source& EventHandler::windowRestored()
{
	return mWindowRestoredEvent;
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
EventHandler::WindowResizedEvent::Source& EventHandler::windowResized()
{
	return mWindowResizedEvent;
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
EventHandler::WindowMouseEnterEvent::Source& EventHandler::windowMouseEnter()
{
	return mWindowMouseEnterEvent;
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
EventHandler::WindowMouseLeaveEvent::Source& EventHandler::windowMouseLeave()
{
	return mWindowMouseLeaveEvent;
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
EventHandler::JoystickAxisMotionEvent::Source& EventHandler::joystickAxisMotion()
{
	return mJoystickAxisMotionEvent;
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
EventHandler::JoystickBallMotionEvent::Source& EventHandler::joystickBallMotion()
{
	return mJoystickBallMotionEvent;
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
EventHandler::JoystickButtonEvent::Source& EventHandler::joystickButtonUp()
{
	return mJoystickButtonUpEvent;
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
EventHandler::JoystickButtonEvent::Source& EventHandler::joystickButtonDown()
{
	return mJoystickButtonDownEvent;
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
EventHandler::JoystickHatMotionEvent::Source& EventHandler::joystickHatMotion()
{
	return mJoystickHatMotionEvent;
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
EventHandler::KeyDownEvent::Source& EventHandler::keyDown()
{
	return mKeyDownEvent;
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
EventHandler::KeyUpEvent::Source& EventHandler::keyUp()
{
	return mKeyUpEvent;
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
EventHandler::TextInputEvent::Source& EventHandler::textInput()
{
	return mTextInputEvent;
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
EventHandler::MouseButtonEvent::Source& EventHandler::mouseButtonDown()
{
	return mMouseButtonDownEvent;
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
EventHandler::MouseButtonEvent::Source& EventHandler::mouseButtonUp()
{
	return mMouseButtonUpEvent;
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
EventHandler::MouseButtonEvent::Source& EventHandler::mouseDoubleClick()
{
	return mMouseDoubleClickEvent;
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
EventHandler::MouseMotionEvent::Source& EventHandler::mouseMotion()
{
	return mMouseMotionEvent;
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
EventHandler::MouseWheelEvent::Source& EventHandler::mouseWheel()
{
	return mMouseWheelEvent;
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
EventHandler::QuitEvent::Source& EventHandler::quit()
{
	return mQuitEvent;
}


/**
 * Grabs exclusive mouse input.
 */
void EventHandler::grabMouse()
{
	if (underlyingWindow) { SDL_SetWindowGrab(underlyingWindow, SDL_TRUE); }
}


/**
 * Releases exclusive mouse input.
 */
void EventHandler::releaseMouse()
{
	if (underlyingWindow) { SDL_SetWindowGrab(underlyingWindow, SDL_FALSE); }
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
		mMouseMotionEvent.emit({x, y}, {0, 0});
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

	int count = 0;
	while ((SDL_PollEvent(&event) != 0) && (count < MAX_MESSAGE_PROCESSING))
	{
		switch (event.type)
		{
		case SDL_MOUSEMOTION:
			mMouseMotionEvent({event.motion.x, event.motion.y}, {event.motion.xrel, event.motion.yrel});
			break;

		case SDL_KEYDOWN:
			mKeyDownEvent(static_cast<KeyCode>(event.key.keysym.sym), static_cast<KeyModifier>(event.key.keysym.mod), event.key.repeat != 0 ? true : false);
			break;

		case SDL_KEYUP:
			mKeyUpEvent(static_cast<KeyCode>(event.key.keysym.sym), static_cast<KeyModifier>(event.key.keysym.mod));
			break;

		case SDL_TEXTINPUT:
			mTextInputEvent(event.text.text);
			break;

		case SDL_MOUSEBUTTONDOWN:
			if (event.button.clicks == 2)
			{
				mMouseDoubleClickEvent(static_cast<MouseButton>(event.button.button), {event.button.x, event.button.y});
			}

			mMouseButtonDownEvent(static_cast<MouseButton>(event.button.button), {event.button.x, event.button.y});
			break;

		case SDL_MOUSEBUTTONUP:
			mMouseButtonUpEvent(static_cast<MouseButton>(event.button.button), {event.button.x, event.button.y});
			break;

		case SDL_MOUSEWHEEL:
			mMouseWheelEvent({event.wheel.x, event.wheel.y});
			break;

		case SDL_JOYAXISMOTION:
			mJoystickAxisMotionEvent(event.jaxis.which, event.jaxis.axis, event.jaxis.value);
			break;

		case SDL_JOYBALLMOTION:
			mJoystickBallMotionEvent(event.jball.which, event.jball.ball, {event.jball.xrel, event.jball.yrel});
			break;

		case SDL_JOYHATMOTION:
			mJoystickHatMotionEvent(event.jhat.which, event.jhat.hat, event.jhat.value);
			break;

		case SDL_JOYBUTTONDOWN:
			mJoystickButtonDownEvent(event.jbutton.which, event.jbutton.button);
			break;

		case SDL_JOYBUTTONUP:
			mJoystickButtonUpEvent(event.jbutton.which, event.jbutton.button);
			break;

		case SDL_WINDOWEVENT:
			// Not completely happy with this but meh, it works.
			if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) { mActivateEvent(true); }
			else if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) { mActivateEvent(false); }
			else if (event.window.event == SDL_WINDOWEVENT_SHOWN) { mWindowHiddenEvent(false); }
			else if (event.window.event == SDL_WINDOWEVENT_HIDDEN) { mWindowHiddenEvent(true); }
			else if (event.window.event == SDL_WINDOWEVENT_EXPOSED) { mWindowExposedEvent(); }
			else if (event.window.event == SDL_WINDOWEVENT_MINIMIZED) { mWindowMinimizedEvent(); }
			else if (event.window.event == SDL_WINDOWEVENT_MAXIMIZED) { mWindowMaximizedEvent(); }
			else if (event.window.event == SDL_WINDOWEVENT_RESTORED) { mWindowRestoredEvent(); }
			else if (event.window.event == SDL_WINDOWEVENT_ENTER) { mWindowMouseEnterEvent(); }
			else if (event.window.event == SDL_WINDOWEVENT_LEAVE) { mWindowMouseLeaveEvent(); }
			else if (event.window.event == SDL_WINDOWEVENT_RESIZED) { mWindowResizedEvent({event.window.data1, event.window.data2}); }
			break;

		case SDL_QUIT:
			mQuitEvent();
			break;

		default:
			// Ignore any cases not handled.
			break;
		}
		count++;
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
	mActivateEvent.clear();
	mJoystickAxisMotionEvent.clear();
	mJoystickBallMotionEvent.clear();
	mJoystickButtonUpEvent.clear();
	mJoystickButtonDownEvent.clear();
	mJoystickHatMotionEvent.clear();
	mKeyUpEvent.clear();
	mKeyDownEvent.clear();
	mTextInputEvent.clear();
	mMouseButtonUpEvent.clear();
	mMouseButtonDownEvent.clear();
	mMouseMotionEvent.clear();
	mMouseWheelEvent.clear();
	mQuitEvent.clear();
}


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
