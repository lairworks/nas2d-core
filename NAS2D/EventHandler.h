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


		using ActivateEventSource = SignalSource<bool>;
		using WindowHiddenEventSource = SignalSource<bool>;
		using WindowExposedEventSource = SignalSource<>;
		using WindowMinimizedEventSource = SignalSource<>;
		using WindowMaximizedEventSource = SignalSource<>;
		using WindowRestoredEventSource = SignalSource<>;
		using WindowResizedEventSource = SignalSource<Vector<int>>;
		using WindowMouseEnterEventSource = SignalSource<>;
		using WindowMouseLeaveEventSource = SignalSource<>;

		using JoystickAxisMotionEventSource = SignalSource<int, int, int>;
		using JoystickBallMotionEventSource = SignalSource<int, int, Vector<int>>;
		using JoystickButtonEventSource = SignalSource<int, int>;
		using JoystickHatMotionEventSource = SignalSource<int, int, int>;

		using KeyDownEventSource = SignalSource<KeyCode, KeyModifier, bool>;
		using KeyUpEventSource = SignalSource<KeyCode, KeyModifier>;
		using TextInputEventSource = SignalSource<const std::string&>;

		using MouseButtonEventSource = SignalSource<MouseButton, Point<int>>;
		using MouseMotionEventSource = SignalSource<Point<int>, Vector<int>>;
		using MouseWheelEventSource = SignalSource<Vector<int>>;

		using QuitEventSource = SignalSource<>;

	public:
		ActivateEventSource& activate();

		WindowHiddenEventSource& windowHidden();
		WindowExposedEventSource& windowExposed();

		WindowMinimizedEventSource& windowMinimized();
		WindowMaximizedEventSource& windowMaximized();
		WindowRestoredEventSource& windowRestored();
		WindowResizedEventSource& windowResized();

		WindowMouseEnterEventSource& windowMouseEnter();
		WindowMouseLeaveEventSource& windowMouseLeave();

		JoystickAxisMotionEventSource& joystickAxisMotion();
		JoystickBallMotionEventSource& joystickBallMotion();
		JoystickButtonEventSource& joystickButtonUp();
		JoystickButtonEventSource& joystickButtonDown();
		JoystickHatMotionEventSource& joystickHatMotion();

		KeyUpEventSource& keyUp();
		KeyDownEventSource& keyDown();

		TextInputEventSource& textInput();

		MouseButtonEventSource& mouseButtonUp();
		MouseButtonEventSource& mouseButtonDown();
		MouseButtonEventSource& mouseDoubleClick();
		MouseMotionEventSource& mouseMotion();
		MouseWheelEventSource& mouseWheel();

		QuitEventSource& quit();

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
		Signal<bool> mActivateEvent{};

		Signal<bool> mWindowHiddenEvent{};
		Signal<> mWindowExposedEvent{};
		Signal<> mWindowMinimizedEvent{};
		Signal<> mWindowMaximizedEvent{};
		Signal<> mWindowRestoredEvent{};
		Signal<Vector<int>> mWindowResizedEvent{};
		Signal<> mWindowMouseEnterEvent{};
		Signal<> mWindowMouseLeaveEvent{};

		Signal<int, int, int> mJoystickAxisMotionEvent{};
		Signal<int, int, Vector<int>> mJoystickBallMotionEvent{};
		Signal<int, int> mJoystickButtonUpEvent{};
		Signal<int, int> mJoystickButtonDownEvent{};
		Signal<int, int, int> mJoystickHatMotionEvent{};

		Signal<KeyCode, KeyModifier, bool> mKeyDownEvent{};
		Signal<KeyCode, KeyModifier> mKeyUpEvent{};

		Signal<const std::string&> mTextInput{};

		Signal<MouseButton, Point<int>> mMouseButtonDownEvent{};
		Signal<MouseButton, Point<int>> mMouseButtonUpEvent{};
		Signal<MouseButton, Point<int>> mMouseDoubleClick{};
		Signal<Point<int>, Vector<int>> mMouseMotionEvent{};
		Signal<Vector<int>> mMouseWheelEvent{};

		Signal<> mQuitEvent{};
	};

	void postQuitEvent();


	EventHandler::KeyModifier& operator|=(EventHandler::KeyModifier& a, const EventHandler::KeyModifier& b) noexcept;
	EventHandler::KeyModifier& operator&=(EventHandler::KeyModifier& a, const EventHandler::KeyModifier& b) noexcept;
	EventHandler::KeyModifier operator|(EventHandler::KeyModifier a, const EventHandler::KeyModifier& b) noexcept;
	EventHandler::KeyModifier operator&(EventHandler::KeyModifier a, const EventHandler::KeyModifier& b) noexcept;


} // namespace
