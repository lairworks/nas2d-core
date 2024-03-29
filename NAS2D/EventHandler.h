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


namespace
{
	constexpr int scanCodeToKeyCode(int scanCode)
	{
		const auto scanCodeMask = 1 << 30;
		const auto keyCode = scanCode | scanCodeMask;
		return keyCode;
	}
}


namespace NAS2D
{

	/**
	 * Handles and dispatches low-level events.
	 */
	class EventHandler
	{
	public:
		/**
		* Key Modifiers
		*/
		enum class KeyModifier : uint16_t
		{
			None = 0x0000,
			ShiftLeft = 0x0001,
			ShiftRight = 0x0002,
			Shift = ShiftLeft | ShiftRight,
			CtrlLeft = 0x0040,
			CtrlRight = 0x0080,
			Ctrl = CtrlLeft | CtrlRight,
			AltLeft = 0x0100,
			AltRight = 0x0200,
			Alt = AltLeft | AltRight,
			MetaLeft = 0x0400,
			MetaRight = 0x0800,
			Meta = MetaLeft | MetaRight,
			Num = 0x1000,
			Caps = 0x2000,
			Mode = 0x4000,
			Reserved = 0x8000
		};


		/**
		* Keyboard scan codes.
		*/
		enum class KeyCode : uint32_t
		{
			KEY_UNKNOWN = 0,
			KEY_FIRST = 0,
			KEY_BACKSPACE = '\b',
			KEY_TAB = '\t',
			KEY_RETURN = '\r',
			KEY_ENTER = '\r',
			KEY_ESCAPE = '\033',
			KEY_SPACE = ' ',
			KEY_EXCLAIM = '!',
			KEY_QUOTEDBL = '"',
			KEY_HASH = '#',
			KEY_PERCENT = '%',
			KEY_DOLLAR = '$',
			KEY_AMPERSAND = '&',
			KEY_QUOTE = '\'',
			KEY_LEFTPAREN = '(',
			KEY_RIGHTPAREN = ')',
			KEY_ASTERISK = '*',
			KEY_PLUS = '+',
			KEY_COMMA = ',',
			KEY_MINUS = '-',
			KEY_PERIOD = '.',
			KEY_SLASH = '/',
			KEY_0 = '0',
			KEY_1 = '1',
			KEY_2 = '2',
			KEY_3 = '3',
			KEY_4 = '4',
			KEY_5 = '5',
			KEY_6 = '6',
			KEY_7 = '7',
			KEY_8 = '8',
			KEY_9 = '9',
			KEY_COLON = ':',
			KEY_SEMICOLON = ';',
			KEY_LESS = '<',
			KEY_EQUALS = '=',
			KEY_GREATER = '>',
			KEY_QUESTION = '?',
			KEY_AT = '@',

			// Special Symbols
			KEY_LEFTBRACKET = '[',
			KEY_BACKSLASH = '\\',
			KEY_RIGHTBRACKET = ']',
			KEY_CARET = '^',
			KEY_UNDERSCORE = '_',
			KEY_BACKQUOTE = '`',

			KEY_a = 'a',
			KEY_b = 'b',
			KEY_c = 'c',
			KEY_d = 'd',
			KEY_e = 'e',
			KEY_f = 'f',
			KEY_g = 'g',
			KEY_h = 'h',
			KEY_i = 'i',
			KEY_j = 'j',
			KEY_k = 'k',
			KEY_l = 'l',
			KEY_m = 'm',
			KEY_n = 'n',
			KEY_o = 'o',
			KEY_p = 'p',
			KEY_q = 'q',
			KEY_r = 'r',
			KEY_s = 's',
			KEY_t = 't',
			KEY_u = 'u',
			KEY_v = 'v',
			KEY_w = 'w',
			KEY_x = 'x',
			KEY_y = 'y',
			KEY_z = 'z',

			// Numeric Keypad
			KEY_KP1 = scanCodeToKeyCode(89),
			KEY_KP2 = scanCodeToKeyCode(90),
			KEY_KP3 = scanCodeToKeyCode(91),
			KEY_KP4 = scanCodeToKeyCode(92),
			KEY_KP5 = scanCodeToKeyCode(93),
			KEY_KP6 = scanCodeToKeyCode(94),
			KEY_KP7 = scanCodeToKeyCode(95),
			KEY_KP8 = scanCodeToKeyCode(96),
			KEY_KP9 = scanCodeToKeyCode(97),
			KEY_KP0 = scanCodeToKeyCode(98),
			KEY_KP_PERIOD = scanCodeToKeyCode(99),
			KEY_KP_DIVIDE = scanCodeToKeyCode(84),
			KEY_KP_MULTIPLY = scanCodeToKeyCode(85),
			KEY_KP_MINUS = scanCodeToKeyCode(86),
			KEY_KP_PLUS = scanCodeToKeyCode(87),
			KEY_KP_ENTER = scanCodeToKeyCode(88),
			KEY_KP_EQUALS = scanCodeToKeyCode(103),
			KEY_KP_COMMA = scanCodeToKeyCode(133),

			// Arrows/Home/End
			KEY_UP = scanCodeToKeyCode(82),
			KEY_DOWN = scanCodeToKeyCode(81),
			KEY_RIGHT = scanCodeToKeyCode(79),
			KEY_LEFT = scanCodeToKeyCode(80),
			KEY_INSERT = scanCodeToKeyCode(73),
			KEY_PAUSE = scanCodeToKeyCode(72),
			KEY_HOME = scanCodeToKeyCode(74),
			KEY_END = scanCodeToKeyCode(77),
			KEY_DELETE = '\177',
			KEY_PAGEUP = scanCodeToKeyCode(75),
			KEY_PAGEDOWN = scanCodeToKeyCode(78),

			// Function Keys
			KEY_F1 = scanCodeToKeyCode(58),
			KEY_F2 = scanCodeToKeyCode(59),
			KEY_F3 = scanCodeToKeyCode(60),
			KEY_F4 = scanCodeToKeyCode(61),
			KEY_F5 = scanCodeToKeyCode(62),
			KEY_F6 = scanCodeToKeyCode(63),
			KEY_F7 = scanCodeToKeyCode(64),
			KEY_F8 = scanCodeToKeyCode(65),
			KEY_F9 = scanCodeToKeyCode(66),
			KEY_F10 = scanCodeToKeyCode(67),
			KEY_F11 = scanCodeToKeyCode(68),
			KEY_F12 = scanCodeToKeyCode(69),
			KEY_F13 = scanCodeToKeyCode(104),
			KEY_F14 = scanCodeToKeyCode(105),
			KEY_F15 = scanCodeToKeyCode(106),
			KEY_F16 = scanCodeToKeyCode(107),
			KEY_F17 = scanCodeToKeyCode(108),
			KEY_F18 = scanCodeToKeyCode(109),
			KEY_F19 = scanCodeToKeyCode(110),
			KEY_F20 = scanCodeToKeyCode(111),
			KEY_F21 = scanCodeToKeyCode(112),
			KEY_F22 = scanCodeToKeyCode(113),
			KEY_F23 = scanCodeToKeyCode(114),
			KEY_F24 = scanCodeToKeyCode(115),

			// Key State Modifiers
			KEY_NUMLOCK = scanCodeToKeyCode(83),
			KEY_CAPSLOCK = scanCodeToKeyCode(57),
			KEY_SCROLLOCK = scanCodeToKeyCode(71),
			KEY_RSHIFT = scanCodeToKeyCode(229),
			KEY_LSHIFT = scanCodeToKeyCode(225),
			KEY_RCTRL = scanCodeToKeyCode(228),
			KEY_LCTRL = scanCodeToKeyCode(224),
			KEY_RALT = scanCodeToKeyCode(230),
			KEY_LALT = scanCodeToKeyCode(226),
			KEY_RGUI = scanCodeToKeyCode(231),
			KEY_LGUI = scanCodeToKeyCode(227),
			KEY_MODE = scanCodeToKeyCode(257),

			// Miscellaneous function keys
			KEY_HELP = scanCodeToKeyCode(117),
			KEY_PRINTSCREEN = scanCodeToKeyCode(70),
			KEY_ALTERASE = scanCodeToKeyCode(153),
			KEY_SYSREQ = scanCodeToKeyCode(154),
			KEY_CLEAR = scanCodeToKeyCode(159),
			KEY_MENU = scanCodeToKeyCode(118),
			KEY_POWER = scanCodeToKeyCode(102),
			KEY_UNDO = scanCodeToKeyCode(122),

			// Audio Control
			KEY_MUTE = scanCodeToKeyCode(127),
			KEY_VOLUME_UP = scanCodeToKeyCode(128),
			KEY_VOLUME_DOWN = scanCodeToKeyCode(129),
			KEY_AUDIO_NEXT = scanCodeToKeyCode(258),
			KEY_AUDIO_PREV = scanCodeToKeyCode(259),
			KEY_AUDIO_STOP = scanCodeToKeyCode(260),
			KEY_AUDIO_PLAY = scanCodeToKeyCode(261),
			KEY_AUDIO_MUTE = scanCodeToKeyCode(262),

			// Additional Keys here
			KEY_LAST
		};

		/**
		* Mouse button identifiers.
		*/
		enum class MouseButton
		{
			None,
			Left,
			Middle,
			Right
		};


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
