// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================
#pragma once

#include "Signal/Signal.h"
#include "Math/Point.h"
#include "Math/Vector.h"

#include <string>
#include <cstdint>


namespace NAS2D
{

	/**
	 * Handles and dispatches low-level events.
	 */
	class EventHandler
	{
	public:
		enum class KeyModifier : uint16_t;
		enum class KeyCode : uint32_t;
		enum class MouseButton;


		using ActivateEvent = Signal<bool>;
		using WindowHiddenEvent = Signal<bool>;
		using WindowExposedEvent = Signal<>;
		using WindowMinimizedEvent = Signal<>;
		using WindowMaximizedEvent = Signal<>;
		using WindowRestoredEvent = Signal<>;
		using WindowResizedEvent = Signal<Vector<int>>;
		using WindowMouseEnterEvent = Signal<>;
		using WindowMouseLeaveEvent = Signal<>;

		using JoystickAxisMotionEvent = Signal<int, int, int>;
		using JoystickBallMotionEvent = Signal<int, int, Vector<int>>;
		using JoystickButtonEvent = Signal<int, int>;
		using JoystickHatMotionEvent = Signal<int, int, int>;

		using KeyDownEvent = Signal<KeyCode, KeyModifier, bool>;
		using KeyUpEvent = Signal<KeyCode, KeyModifier>;
		using TextInputEvent = Signal<const std::string&>;

		using MouseButtonEvent = Signal<MouseButton, Point<int>>;
		using MouseMotionEvent = Signal<Point<int>, Vector<int>>;
		using MouseWheelEvent = Signal<Vector<int>>;

		using QuitEvent = Signal<>;

	public:
		ActivateEvent::Source& activate();

		WindowHiddenEvent::Source& windowHidden();
		WindowExposedEvent::Source& windowExposed();

		WindowMinimizedEvent::Source& windowMinimized();
		WindowMaximizedEvent::Source& windowMaximized();
		WindowRestoredEvent::Source& windowRestored();
		WindowResizedEvent::Source& windowResized();

		WindowMouseEnterEvent::Source& windowMouseEnter();
		WindowMouseLeaveEvent::Source& windowMouseLeave();

		JoystickAxisMotionEvent::Source& joystickAxisMotion();
		JoystickBallMotionEvent::Source& joystickBallMotion();
		JoystickButtonEvent::Source& joystickButtonUp();
		JoystickButtonEvent::Source& joystickButtonDown();
		JoystickHatMotionEvent::Source& joystickHatMotion();

		KeyUpEvent::Source& keyUp();
		KeyDownEvent::Source& keyDown();

		TextInputEvent::Source& textInput();

		MouseButtonEvent::Source& mouseButtonUp();
		MouseButtonEvent::Source& mouseButtonDown();
		MouseButtonEvent::Source& mouseDoubleClick();
		MouseMotionEvent::Source& mouseMotion();
		MouseWheelEvent::Source& mouseWheel();

		QuitEvent::Source& quit();

		void grabMouse();
		void releaseMouse();
		void warpMouse(int x, int y);
		void mouseRelativeMode(bool rel);

		void textInputMode(bool);
		bool textInputMode();

		static bool shift(KeyModifier mod);
		static bool numlock(KeyModifier mod);
		static bool control(KeyModifier mod);
		static bool alt(KeyModifier mod);

		bool shift() const;
		bool numlock() const;
		bool control() const;

		void pump();

		void disconnectAll();

	private:
		ActivateEvent mActivateEvent{};

		WindowHiddenEvent mWindowHiddenEvent{};
		WindowExposedEvent mWindowExposedEvent{};
		WindowMinimizedEvent mWindowMinimizedEvent{};
		WindowMaximizedEvent mWindowMaximizedEvent{};
		WindowRestoredEvent mWindowRestoredEvent{};
		WindowResizedEvent mWindowResizedEvent{};
		WindowMouseEnterEvent mWindowMouseEnterEvent{};
		WindowMouseLeaveEvent mWindowMouseLeaveEvent{};

		JoystickAxisMotionEvent mJoystickAxisMotionEvent{};
		JoystickBallMotionEvent mJoystickBallMotionEvent{};
		JoystickButtonEvent mJoystickButtonUpEvent{};
		JoystickButtonEvent mJoystickButtonDownEvent{};
		JoystickHatMotionEvent mJoystickHatMotionEvent{};

		KeyDownEvent mKeyDownEvent{};
		KeyUpEvent mKeyUpEvent{};

		TextInputEvent mTextInput{};

		MouseButtonEvent mMouseButtonDownEvent{};
		MouseButtonEvent mMouseButtonUpEvent{};
		MouseButtonEvent mMouseDoubleClick{};
		MouseMotionEvent mMouseMotionEvent{};
		MouseWheelEvent mMouseWheelEvent{};

		QuitEvent mQuitEvent{};
	};

	void postQuitEvent();


	EventHandler::KeyModifier& operator|=(EventHandler::KeyModifier& a, const EventHandler::KeyModifier& b) noexcept;
	EventHandler::KeyModifier& operator&=(EventHandler::KeyModifier& a, const EventHandler::KeyModifier& b) noexcept;
	EventHandler::KeyModifier operator|(EventHandler::KeyModifier a, const EventHandler::KeyModifier& b) noexcept;
	EventHandler::KeyModifier operator&(EventHandler::KeyModifier a, const EventHandler::KeyModifier& b) noexcept;


} // namespace
