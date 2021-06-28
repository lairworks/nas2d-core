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
#include <SDL2/SDL.h>
#include <iostream>

// UGLY ASS HACK!
// This is required for mouse grabbing in the EventHandler class.
extern SDL_Window* underlyingWindow;


using namespace std;
using namespace NAS2D;

//TODO: Make this configurable?
const int MAX_MESSAGE_PROCESSING = 100;

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


EventHandler::EventHandler()
{}


EventHandler::~EventHandler()
{
	cout << "EventHandler Terminated." << endl;
}
EventHandler::ActivateEventSource& EventHandler::activate()
{
	return mActivateEvent;
}

EventHandler::WindowHiddenEventSource& EventHandler::windowHidden()
{
	return mWindowHiddenEvent;
}

EventHandler::WindowExposedEventSource& EventHandler::windowExposed()
{
	return mWindowExposedEvent;
}

EventHandler::WindowMinimizedEventSource& EventHandler::windowMinimized()
{
	return mWindowMinimizedEvent;
}

EventHandler::WindowMaximizedEventSource& EventHandler::windowMaximized()
{
	return mWindowMaximizedEvent;
}

EventHandler::WindowRestoredEventSource& EventHandler::windowRestored()
{
	return mWindowRestoredEvent;
}

EventHandler::WindowResizedEventSource& EventHandler::windowResized()
{
	return mWindowResizedEvent;
}

EventHandler::WindowMouseEnterEventSource& EventHandler::windowMouseEnter()
{
	return mWindowMouseEnterEvent;
}

EventHandler::WindowMouseLeaveEventSource& EventHandler::windowMouseLeave()
{
	return mWindowMouseLeaveEvent;
}

EventHandler::JoystickAxisMotionEventSource& EventHandler::joystickAxisMotion()
{
	return mJoystickAxisMotionEvent;
}

EventHandler::JoystickBallMotionEventSource& EventHandler::joystickBallMotion()
{
	return mJoystickBallMotionEvent;
}

EventHandler::JoystickButtonEventSource& EventHandler::joystickButtonUp()
{
	return mJoystickButtonUpEvent;
}

EventHandler::JoystickButtonEventSource& EventHandler::joystickButtonDown()
{
	return mJoystickButtonDownEvent;
}

EventHandler::JoystickHatMotionEventSource& EventHandler::joystickHatMotion()
{
	return mJoystickHatMotionEvent;
}

EventHandler::KeyDownEventSource& EventHandler::keyDown()
{
	return mKeyDownEvent;
}

EventHandler::KeyUpEventSource& EventHandler::keyUp()
{
	return mKeyUpEvent;
}

EventHandler::TextInputEventSource& EventHandler::textInput()
{
	return mTextInput;
}

EventHandler::MouseButtonEventSource& EventHandler::mouseButtonDown()
{
	return mMouseButtonDownEvent;
}

EventHandler::MouseButtonEventSource& EventHandler::mouseButtonUp()
{
	return mMouseButtonUpEvent;
}

EventHandler::MouseButtonEventSource& EventHandler::mouseDoubleClick()
{
	return mMouseDoubleClick;
}

EventHandler::MouseMotionEventSource& EventHandler::mouseMotion()
{
	return mMouseMotionEvent;
}

EventHandler::MouseWheelEventSource& EventHandler::mouseWheel()
{
	return mMouseWheelEvent;
}

EventHandler::QuitEventSource& EventHandler::quit()
{
	return mQuitEvent;
}

void EventHandler::grabMouse()
{
	if (underlyingWindow) { SDL_SetWindowGrab(underlyingWindow, SDL_TRUE); }
}

void EventHandler::releaseMouse()
{
	if (underlyingWindow) { SDL_SetWindowGrab(underlyingWindow, SDL_FALSE); }
}

void EventHandler::warpMouse(int x, int y)
{
	if (underlyingWindow)
	{
		SDL_WarpMouseInWindow(underlyingWindow, x, y);
		mMouseMotionEvent.emit(x, y, 0, 0);
	}
}

void EventHandler::mouseRelativeMode(bool rel)
{
	rel ? SDL_SetRelativeMouseMode(SDL_TRUE) : SDL_SetRelativeMouseMode(SDL_FALSE);
}

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
			//TODO: Convert to switch statement?
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
			break;
		}
		count++;
	}
}

void EventHandler::textInputMode(bool _b)
{
	if (_b) { SDL_StartTextInput(); }
	else { SDL_StopTextInput(); }
}

bool EventHandler::textInputMode()
{
	return SDL_IsTextInputActive() == SDL_TRUE;
}

bool EventHandler::shift(KeyModifier mod) const
{
	return KeyModifier::None != (mod & (KeyModifier::Shift | KeyModifier::Caps));
}

bool EventHandler::alt(KeyModifier mod) const
{
	return KeyModifier::None != (mod & KeyModifier::Alt);
}

bool EventHandler::numlock(KeyModifier mod) const
{
	return KeyModifier::None != (mod & KeyModifier::Num);
}

bool EventHandler::control(KeyModifier mod) const
{
	return KeyModifier::None != (mod & KeyModifier::Ctrl);
}

bool EventHandler::query_shift() const
{
	using underlying = std::underlying_type_t<KeyModifier>;
	return KeyModifier::None != static_cast<KeyModifier>(SDL_GetModState() & static_cast<underlying>(KeyModifier::Shift));
}

bool EventHandler::query_numlock() const
{
	using underlying = std::underlying_type_t<KeyModifier>;
	return KeyModifier::None != static_cast<KeyModifier>(SDL_GetModState() & static_cast<underlying>(KeyModifier::Num));
}

bool EventHandler::query_control() const
{
	using underlying = std::underlying_type_t<KeyModifier>;
	return KeyModifier::None != static_cast<KeyModifier>(SDL_GetModState() & static_cast<underlying>(KeyModifier::Ctrl));
}

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


namespace NAS2D {

	void postQuitEvent()
	{
		SDL_Event event;
		event.type = SDL_QUIT;
		SDL_PushEvent(&event);
	}
}
