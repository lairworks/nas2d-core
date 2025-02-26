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


union SDL_Event;


namespace NAS2D
{
	enum class KeyModifier : uint16_t;
	enum class KeyCode : uint32_t;
	enum class MouseButton;

	extern template class Signal<KeyCode, KeyModifier>;
	extern template class Signal<KeyCode, KeyModifier, bool>;
	extern template class Signal<MouseButton, Point<int>>;
	extern template class Signal<Point<int>, Vector<int>>;
	extern template class Signal<Vector<int>>;
	extern template class Signal<int, int, Vector<int>>;


	void postQuitEvent();


	/**
	 * Handles and dispatches low-level events.
	 */
	class EventHandler
	{
	public:
		using ActivateSignal = Signal<bool>;
		using WindowHiddenSignal = Signal<bool>;
		using WindowExposedSignal = Signal<>;
		using WindowMinimizedSignal = Signal<>;
		using WindowMaximizedSignal = Signal<>;
		using WindowRestoredSignal = Signal<>;
		using WindowResizedSignal = Signal<Vector<int>>;
		using WindowMouseEnterSignal = Signal<>;
		using WindowMouseLeaveSignal = Signal<>;

		using JoystickAxisMotionSignal = Signal<int, int, int>;
		using JoystickBallMotionSignal = Signal<int, int, Vector<int>>;
		using JoystickButtonSignal = Signal<int, int>;
		using JoystickHatMotionSignal = Signal<int, int, int>;

		using KeyDownSignal = Signal<KeyCode, KeyModifier, bool>;
		using KeyUpSignal = Signal<KeyCode, KeyModifier>;
		using TextInputSignal = Signal<const std::string&>;

		using MouseButtonSignal = Signal<MouseButton, Point<int>>;
		using MouseMotionSignal = Signal<Point<int>, Vector<int>>;
		using MouseWheelSignal = Signal<Vector<int>>;

		using QuitSignal = Signal<>;

	public:
		virtual ~EventHandler() = default;

		ActivateSignal::Source& activate();

		WindowHiddenSignal::Source& windowHidden();
		WindowExposedSignal::Source& windowExposed();

		WindowMinimizedSignal::Source& windowMinimized();
		WindowMaximizedSignal::Source& windowMaximized();
		WindowRestoredSignal::Source& windowRestored();
		WindowResizedSignal::Source& windowResized();

		WindowMouseEnterSignal::Source& windowMouseEnter();
		WindowMouseLeaveSignal::Source& windowMouseLeave();

		JoystickAxisMotionSignal::Source& joystickAxisMotion();
		JoystickBallMotionSignal::Source& joystickBallMotion();
		JoystickButtonSignal::Source& joystickButtonUp();
		JoystickButtonSignal::Source& joystickButtonDown();
		JoystickHatMotionSignal::Source& joystickHatMotion();

		KeyUpSignal::Source& keyUp();
		KeyDownSignal::Source& keyDown();

		TextInputSignal::Source& textInput();

		MouseButtonSignal::Source& mouseButtonUp();
		MouseButtonSignal::Source& mouseButtonDown();
		MouseButtonSignal::Source& mouseDoubleClick();
		MouseMotionSignal::Source& mouseMotion();
		MouseWheelSignal::Source& mouseWheel();

		QuitSignal::Source& quit();

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

		virtual void pump();

		void disconnectAll();

	protected:
		void onMessage(const SDL_Event& event);

	private:
		ActivateSignal mActivateSignal{};

		WindowHiddenSignal mWindowHiddenSignal{};
		WindowExposedSignal mWindowExposedSignal{};
		WindowMinimizedSignal mWindowMinimizedSignal{};
		WindowMaximizedSignal mWindowMaximizedSignal{};
		WindowRestoredSignal mWindowRestoredSignal{};
		WindowResizedSignal mWindowResizedSignal{};
		WindowMouseEnterSignal mWindowMouseEnterSignal{};
		WindowMouseLeaveSignal mWindowMouseLeaveSignal{};

		JoystickAxisMotionSignal mJoystickAxisMotionSignal{};
		JoystickBallMotionSignal mJoystickBallMotionSignal{};
		JoystickButtonSignal mJoystickButtonUpSignal{};
		JoystickButtonSignal mJoystickButtonDownSignal{};
		JoystickHatMotionSignal mJoystickHatMotionSignal{};

		KeyDownSignal mKeyDownSignal{};
		KeyUpSignal mKeyUpSignal{};

		TextInputSignal mTextInputSignal{};

		MouseButtonSignal mMouseButtonDownSignal{};
		MouseButtonSignal mMouseButtonUpSignal{};
		MouseButtonSignal mMouseDoubleClickSignal{};
		MouseMotionSignal mMouseMotionSignal{};
		MouseWheelSignal mMouseWheelSignal{};

		QuitSignal mQuitSignal{};
	};
} // namespace
