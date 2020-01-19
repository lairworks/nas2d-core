// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================
#include "NAS2D/EventHandler.h"
#include <SDL2/SDL.h>
#include <iostream>

// UGLY ASS HACK!
// This is required for mouse grabbing in the EventHandler class.
extern SDL_Window* underlyingWindow;


using namespace std;
using namespace NAS2D;

const int MAX_MESSAGE_PROCESSING = 100;	/**
										 * Maximum number of events to process each frame.
										 *
										 * \todo	Make this configurable?
										 */

namespace NAS2D {
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
 * Default c'tor.
 */
EventHandler::EventHandler()
{}


/**
 * Default d'tor.
 */
EventHandler::~EventHandler()
{
	cout << "EventHandler Terminated." << endl;
}

/**
 * \brief Gets the handler slot associated with this event.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect(this, &Object::function);
 * \endcode
 *
 * See EventHandler::ActivateEventCallback for paramter listing.
 */
EventHandler::ActivateEventCallback& EventHandler::activate()
{
	return mActivateEvent;
}


/**
 * \brief Gets the handler slot associated with this event.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect(this, &Object::function);
 * \endcode
 *
 * See EventHandler::WindowHiddenEventCallback for paramter listing.
 */
EventHandler::WindowHiddenEventCallback& EventHandler::windowHidden()
{
	return mWindowHiddenEventCallback;
}


/**
 * \brief Gets the handler slot associated with this event.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect(this, &Object::function);
 * \endcode
 *
 * See EventHandler::WindowExposedEventCallback for paramter listing.
 */
EventHandler::WindowExposedEventCallback& EventHandler::windowExposed()
{
	return mWindowExposedEventCallback;
}


/**
 * \brief Gets the handler slot associated with this event.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect(this, &Object::function);
 * \endcode
 *
 * See EventHandler::WindowMinimizedEventCallback for paramter listing.
 */
EventHandler::WindowMinimizedEventCallback& EventHandler::windowMinimized()
{
	return mWindowMinimizedEventCallback;
}


/**
 * \brief Gets the handler slot associated with this event.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect(this, &Object::function);
 * \endcode
 *
 * See EventHandler::WindowMaximizedEventCallback for paramter listing.
 */
EventHandler::WindowMaximizedEventCallback& EventHandler::windowMaximized()
{
	return mWindowMaximizedEventCallback;
}


/**
 * \brief Gets the handler slot associated with this event.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect(this, &Object::function);
 * \endcode
 *
 * See EventHandler::WindowRestoredEventCallback for paramter listing.
 */
EventHandler::WindowRestoredEventCallback& EventHandler::windowRestored()
{
	return mWindowRestoredEventCallback;
}


/**
 * \brief Gets the handler slot associated with this event.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect(this, &Object::function);
 * \endcode
 *
 * See EventHandler::WindowResizedEventCallback for paramter listing.
 */
EventHandler::WindowResizedEventCallback& EventHandler::windowResized()
{
	return mWindowResizedEventCallback;
}


/**
 * \brief Gets the handler slot associated with this event.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect(this, &Object::function);
 * \endcode
 *
 * See EventHandler::WindowMouseEnterEventCallback for paramter listing.
 */
EventHandler::WindowMouseEnterEventCallback& EventHandler::windowMouseEnter()
{
	return mWindowMouseEnterEventCallback;
}


/**
 * \brief Gets the handler slot associated with this event.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect(this, &Object::function);
 * \endcode
 *
 * See EventHandler::WindowMouseLeaveEventCallback for paramter listing.
 */
EventHandler::WindowMouseLeaveEventCallback& EventHandler::windowMouseLeave()
{
	return mWindowMouseLeaveEventCallback;
}


/**
 * \brief Gets the handler slot associated with this event.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect(this, &Object::function);
 * \endcode
 *
 * See EventHandler::JoystickAxisMotionEventCallback for paramter listing.
 */
EventHandler::JoystickAxisMotionEventCallback& EventHandler::joystickAxisMotion()
{
	return mJoystickAxisMotionEvent;
}


/**
 * \brief Gets the handler slot associated with this event.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect(this, &Object::function);
 * \endcode
 *
 * See EventHandler::JoystickBallMotionEventCallback for paramter listing.
 */
EventHandler::JoystickBallMotionEventCallback& EventHandler::joystickBallMotion()
{
	return mJoystickBallMotionEvent;
}


/**
 * \brief Gets the handler slot associated with this event.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect(this, &Object::function);
 * \endcode
 *
 * See EventHandler::JoystickButtonEventCallback for paramter listing.
 */
EventHandler::JoystickButtonEventCallback& EventHandler::joystickButtonUp()
{
	return mJoystickButtonUpEvent;
}


/**
 * \brief Gets the handler slot associated with this event.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect(this, &Object::function);
 * \endcode
 *
 * See EventHandler::JoystickButtonEventCallback for paramter listing.
 */
EventHandler::JoystickButtonEventCallback& EventHandler::joystickButtonDown()
{
	return mJoystickButtonDownEvent;
}


/**
 * \brief Gets the handler slot associated with this event.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect(this, &Object::function);
 * \endcode
 *
 * See EventHandler::JoystickHatMotionEventCallback for paramter listing.
 */
EventHandler::JoystickHatMotionEventCallback& EventHandler::joystickHatMotion()
{
	return mJoystickHatMotionEvent;
}


/**
 * \brief Gets the handler slot associated with this event.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect(this, &Object::function);
 * \endcode
 *
 * See EventHandler::KeyEventCallback for paramter listing.
 */
EventHandler::KeyUpEventCallback& EventHandler::keyUp()
{
	return mKeyUpEvent;
}


/**
 * \brief Gets the handler slot associated with this event.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect(this, &Object::function);
 * \endcode
 *
 * See EventHandler::KeyEventCallback for paramter listing.
 */
EventHandler::KeyDownEventCallback& EventHandler::keyDown()
{
	return mKeyDownEvent;
}


/**
* \brief Gets the handler slot associated with this event.
*
* To connect an event handler to this event, call the 'connect()'
* function as follows:
*
* \code
* connect(this, &Object::function);
* \endcode
*
* See EventHandler::TextInputEventCallback for paramter listing.
*/
EventHandler::TextInputEventCallback& EventHandler::textInput()
{
	return mTextInput;
}


/**
 * \brief Gets the handler slot associated with this event.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect(this, &Object::function);
 * \endcode
 *
 * See EventHandler::MouseButtonEventCallback for paramter listing.
 */
EventHandler::MouseButtonEventCallback& EventHandler::mouseButtonUp()
{
	return mMouseButtonUpEvent;
}


/**
 * \brief Gets the handler slot associated with this event.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect(this, &Object::function);
 * \endcode
 *
 * See EventHandler::MouseButtonEventCallback for paramter listing.
 */
EventHandler::MouseButtonEventCallback& EventHandler::mouseButtonDown()
{
	return mMouseButtonDownEvent;
}


/**
 * \brief Gets the handler slot associated with this event.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect(this, &Object::function);
 * \endcode
 *
 * See EventHandler::MouseButtonEventCallback for paramter listing.
 */
EventHandler::MouseButtonEventCallback& EventHandler::mouseDoubleClick()
{
	return mMouseDoubleClick;
}


/**
 * \brief Gets the handler slot associated with this event.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect(this, &Object::function);
 * \endcode
 *
 * See EventHandler::MouseMotionEventCallback for paramter listing.
 */
EventHandler::MouseMotionEventCallback& EventHandler::mouseMotion()
{
	return mMouseMotionEvent;
}


/**
 * \brief Gets the handler slot associated with this event.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect(this, &Object::function);
 * \endcode
 *
 * See EventHandler::MouseWheelEventCallback for paramter listing.
 */
EventHandler::MouseWheelEventCallback& EventHandler::mouseWheel()
{
	return mMouseWheelEvent;
}


/**
 * \brief Gets the handler slot associated with this event.
 *
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 *
 * \code
 * connect(this, &Object::function);
 * \endcode
 *
 * See EventHandler::QuitEventCallback for paramter listing.
 */
EventHandler::QuitEventCallback& EventHandler::quit()
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
	if (underlyingWindow) { SDL_WarpMouseInWindow(underlyingWindow, x, y); }
}


/**
 * Sets mouse updates to relative or absolute mostion.
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
			mMouseMotionEvent(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
			break;

		case SDL_KEYDOWN:
			mKeyDownEvent(static_cast<KeyCode>(event.key.keysym.sym), static_cast<KeyModifier>(event.key.keysym.mod), event.key.repeat != 0 ? true : false);
			break;

		case SDL_KEYUP:
			mKeyUpEvent(static_cast<KeyCode>(event.key.keysym.sym), static_cast<KeyModifier>(event.key.keysym.mod));
			break;

		case SDL_TEXTINPUT:
			mTextInput(event.text.text);
			break;

		case SDL_MOUSEBUTTONDOWN:
			if (event.button.clicks == 2)
			{
				mMouseDoubleClick(static_cast<MouseButton>(event.button.button), event.button.x, event.button.y);
			}

			mMouseButtonDownEvent(static_cast<MouseButton>(event.button.button), event.button.x, event.button.y);
			break;

		case SDL_MOUSEBUTTONUP:
			mMouseButtonUpEvent(static_cast<MouseButton>(event.button.button), event.button.x, event.button.y);
			break;

		case SDL_MOUSEWHEEL:
			mMouseWheelEvent(event.wheel.x, event.wheel.y);
			break;

		case SDL_JOYAXISMOTION:
			mJoystickAxisMotionEvent(event.jaxis.which, event.jaxis.axis, event.jaxis.value);
			break;

		case SDL_JOYBALLMOTION:
			mJoystickBallMotionEvent(event.jball.which, event.jball.ball, event.jball.xrel, event.jball.yrel);
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
			else if (event.window.event == SDL_WINDOWEVENT_SHOWN) { mWindowHiddenEventCallback(false); }
			else if (event.window.event == SDL_WINDOWEVENT_HIDDEN) { mWindowHiddenEventCallback(true); }
			else if (event.window.event == SDL_WINDOWEVENT_EXPOSED) { mWindowExposedEventCallback(); }
			else if (event.window.event == SDL_WINDOWEVENT_MINIMIZED) { mWindowMinimizedEventCallback(); }
			else if (event.window.event == SDL_WINDOWEVENT_MAXIMIZED) { mWindowMaximizedEventCallback(); }
			else if (event.window.event == SDL_WINDOWEVENT_RESTORED) { mWindowRestoredEventCallback(); }
			else if (event.window.event == SDL_WINDOWEVENT_ENTER) { mWindowMouseEnterEventCallback(); }
			else if (event.window.event == SDL_WINDOWEVENT_LEAVE) { mWindowMouseLeaveEventCallback(); }
			else if (event.window.event == SDL_WINDOWEVENT_RESIZED) { mWindowResizedEventCallback(event.window.data1, event.window.data2); }
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
bool EventHandler::shift(KeyModifier mod) const
{
	return KeyModifier::KEY_MOD_NONE != (mod & (KeyModifier::KEY_MOD_SHIFT | KeyModifier::KEY_MOD_CAPS));
}


/**
 * Decodes a KeyModifier and determines if the Shift keymod is applied.
 *
 * \param mod	Modifier value to decode.
 */
bool EventHandler::alt(KeyModifier mod) const
{
	return KeyModifier::KEY_MOD_NONE != (mod & KeyModifier::KEY_MOD_ALT);
}


/**
 * Decodes a KeyModifier and determines if the Numlock keymod is applied.
 *
 * \param mod	Modifier value to decode.
 */
bool EventHandler::numlock(KeyModifier mod) const
{
	return KeyModifier::KEY_MOD_NONE != (mod & KeyModifier::KEY_MOD_NUM);
}


/**
 * Decodes a KeyModifier and determines if the Control keymod is applied.
 *
 * \param mod	Modifier value to decode.
 */
bool EventHandler::control(KeyModifier mod) const
{
	return KeyModifier::KEY_MOD_NONE != (mod & KeyModifier::KEY_MOD_CTRL);
}


/**
 * Queries state of the Shift key modifier.
 */
bool EventHandler::query_shift() const
{
	using underlying = std::underlying_type_t<KeyModifier>;
	return KeyModifier::KEY_MOD_NONE != static_cast<KeyModifier>(SDL_GetModState() & static_cast<underlying>(KeyModifier::KEY_MOD_SHIFT));
}


/**
 * Queries state of the Shift key modifier.
 */
bool EventHandler::query_numlock() const
{
    using underlying = std::underlying_type_t<KeyModifier>;
	return KeyModifier::KEY_MOD_NONE != static_cast<KeyModifier>(SDL_GetModState() & static_cast<underlying>(KeyModifier::KEY_MOD_NUM));
}


/**
 * Queries state of the Shift key modifier.
 */
bool EventHandler::query_control() const
{
	using underlying = std::underlying_type_t<KeyModifier>;
	return KeyModifier::KEY_MOD_NONE != static_cast<KeyModifier>(SDL_GetModState() & static_cast<underlying>(KeyModifier::KEY_MOD_CTRL));
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
	mTextInput.clear();
	mMouseButtonUpEvent.clear();
	mMouseButtonDownEvent.clear();
	mMouseMotionEvent.clear();
	mMouseWheelEvent.clear();
	mQuitEvent.clear();
}


/**
 * Posts a quit event to the event system.
 */
void NAS2D::postQuitEvent()
{
	SDL_Event event;
	event.type = SDL_QUIT;
	SDL_PushEvent(&event);
}
