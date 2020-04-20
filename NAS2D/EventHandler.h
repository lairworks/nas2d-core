// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================
#pragma once

#include "Signal.h"

#include <string>


namespace NAS2D {

#define SCANCODE_MASK (1<<30)
#define SCANCODE_TO_KEYCODE(X) (X | SCANCODE_MASK)


/**
 * \class	EventHandler
 * \brief	Handles and dispatches low-level events.
 *
 */
class EventHandler
{
public:
	/**
	* \enum KeyModifier
	* \brief Key Modifiers
	*/
	enum class KeyModifier : uint16_t
	{
		KEY_MOD_NONE = 0x0000,
		KEY_MOD_LSHIFT = 0x0001,
		KEY_MOD_RSHIFT = 0x0002,
		KEY_MOD_SHIFT = KEY_MOD_LSHIFT | KEY_MOD_RSHIFT,
		KEY_MOD_LCTRL = 0x0040,
		KEY_MOD_RCTRL = 0x0080,
		KEY_MOD_CTRL = KEY_MOD_LCTRL | KEY_MOD_RCTRL,
		KEY_MOD_LALT = 0x0100,
		KEY_MOD_RALT = 0x0200,
		KEY_MOD_ALT = KEY_MOD_LALT | KEY_MOD_RALT,
		KEY_MOD_LMETA = 0x0400,
		KEY_MOD_RMETA = 0x0800,
		KEY_MOD_META = KEY_MOD_LMETA | KEY_MOD_RMETA,
		KEY_MOD_NUM = 0x1000,
		KEY_MOD_CAPS = 0x2000,
		KEY_MOD_MODE = 0x4000,
		KEY_MOD_RESERVED = 0x8000
	};


		/**
		* \enum KeyCode
		* \brief Keyboard scan codes.
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
		KEY_KP1 = SCANCODE_TO_KEYCODE(89),
		KEY_KP2 = SCANCODE_TO_KEYCODE(90),
		KEY_KP3 = SCANCODE_TO_KEYCODE(91),
		KEY_KP4 = SCANCODE_TO_KEYCODE(92),
		KEY_KP5 = SCANCODE_TO_KEYCODE(93),
		KEY_KP6 = SCANCODE_TO_KEYCODE(94),
		KEY_KP7 = SCANCODE_TO_KEYCODE(95),
		KEY_KP8 = SCANCODE_TO_KEYCODE(96),
		KEY_KP9 = SCANCODE_TO_KEYCODE(97),
		KEY_KP0 = SCANCODE_TO_KEYCODE(98),
		KEY_KP_PERIOD = SCANCODE_TO_KEYCODE(99),
		KEY_KP_DIVIDE = SCANCODE_TO_KEYCODE(84),
		KEY_KP_MULTIPLY = SCANCODE_TO_KEYCODE(85),
		KEY_KP_MINUS = SCANCODE_TO_KEYCODE(86),
		KEY_KP_PLUS = SCANCODE_TO_KEYCODE(87),
		KEY_KP_ENTER = SCANCODE_TO_KEYCODE(88),
		KEY_KP_EQUALS = SCANCODE_TO_KEYCODE(103),
		KEY_KP_COMMA = SCANCODE_TO_KEYCODE(133),

		// Arrows/Home/End
		KEY_UP = SCANCODE_TO_KEYCODE(82),
		KEY_DOWN = SCANCODE_TO_KEYCODE(81),
		KEY_RIGHT = SCANCODE_TO_KEYCODE(79),
		KEY_LEFT = SCANCODE_TO_KEYCODE(80),
		KEY_INSERT = SCANCODE_TO_KEYCODE(73),
		KEY_PAUSE = SCANCODE_TO_KEYCODE(72),
		KEY_HOME = SCANCODE_TO_KEYCODE(74),
		KEY_END = SCANCODE_TO_KEYCODE(77),
		KEY_DELETE = '\177',
		KEY_PAGEUP = SCANCODE_TO_KEYCODE(75),
		KEY_PAGEDOWN = SCANCODE_TO_KEYCODE(78),

		// Function Keys
		KEY_F1 = SCANCODE_TO_KEYCODE(58),
		KEY_F2 = SCANCODE_TO_KEYCODE(59),
		KEY_F3 = SCANCODE_TO_KEYCODE(60),
		KEY_F4 = SCANCODE_TO_KEYCODE(61),
		KEY_F5 = SCANCODE_TO_KEYCODE(62),
		KEY_F6 = SCANCODE_TO_KEYCODE(63),
		KEY_F7 = SCANCODE_TO_KEYCODE(64),
		KEY_F8 = SCANCODE_TO_KEYCODE(65),
		KEY_F9 = SCANCODE_TO_KEYCODE(66),
		KEY_F10 = SCANCODE_TO_KEYCODE(67),
		KEY_F11 = SCANCODE_TO_KEYCODE(68),
		KEY_F12 = SCANCODE_TO_KEYCODE(69),
		KEY_F13 = SCANCODE_TO_KEYCODE(104),
		KEY_F14 = SCANCODE_TO_KEYCODE(105),
		KEY_F15 = SCANCODE_TO_KEYCODE(106),
		KEY_F16 = SCANCODE_TO_KEYCODE(107),
		KEY_F17 = SCANCODE_TO_KEYCODE(108),
		KEY_F18 = SCANCODE_TO_KEYCODE(109),
		KEY_F19 = SCANCODE_TO_KEYCODE(110),
		KEY_F20 = SCANCODE_TO_KEYCODE(111),
		KEY_F21 = SCANCODE_TO_KEYCODE(112),
		KEY_F22 = SCANCODE_TO_KEYCODE(113),
		KEY_F23 = SCANCODE_TO_KEYCODE(114),
		KEY_F24 = SCANCODE_TO_KEYCODE(115),

		// Key State Modifiers
		KEY_NUMLOCK = SCANCODE_TO_KEYCODE(83),
		KEY_CAPSLOCK = SCANCODE_TO_KEYCODE(57),
		KEY_SCROLLOCK = SCANCODE_TO_KEYCODE(71),
		KEY_RSHIFT = SCANCODE_TO_KEYCODE(229),
		KEY_LSHIFT = SCANCODE_TO_KEYCODE(225),
		KEY_RCTRL = SCANCODE_TO_KEYCODE(228),
		KEY_LCTRL = SCANCODE_TO_KEYCODE(224),
		KEY_RALT = SCANCODE_TO_KEYCODE(230),
		KEY_LALT = SCANCODE_TO_KEYCODE(226),
		KEY_RGUI = SCANCODE_TO_KEYCODE(231),
		KEY_LGUI = SCANCODE_TO_KEYCODE(227),
		KEY_MODE = SCANCODE_TO_KEYCODE(257),

		// Miscellaneous function keys
		KEY_HELP = SCANCODE_TO_KEYCODE(117),
		KEY_PRINTSCREEN = SCANCODE_TO_KEYCODE(70),
		KEY_ALTERASE = SCANCODE_TO_KEYCODE(153),
		KEY_SYSREQ = SCANCODE_TO_KEYCODE(154),
		KEY_CLEAR = SCANCODE_TO_KEYCODE(159),
		KEY_MENU = SCANCODE_TO_KEYCODE(118),
		KEY_POWER = SCANCODE_TO_KEYCODE(102),
		KEY_UNDO = SCANCODE_TO_KEYCODE(122),

		// Audio Control
		KEY_MUTE = SCANCODE_TO_KEYCODE(127),
		KEY_VOLUME_UP = SCANCODE_TO_KEYCODE(128),
		KEY_VOLUME_DOWN = SCANCODE_TO_KEYCODE(129),
		KEY_AUDIO_NEXT = SCANCODE_TO_KEYCODE(258),
		KEY_AUDIO_PREV = SCANCODE_TO_KEYCODE(259),
		KEY_AUDIO_STOP = SCANCODE_TO_KEYCODE(260),
		KEY_AUDIO_PLAY = SCANCODE_TO_KEYCODE(261),
		KEY_AUDIO_MUTE = SCANCODE_TO_KEYCODE(262),

		// Additional Keys here
		KEY_LAST
	};

	/**
	* \enum		MouseButton
	* \brief	Mouse button identifiers.
	*/
	enum class MouseButton
	{
		BUTTON_NONE,
		BUTTON_LEFT,
		BUTTON_MIDDLE,
		BUTTON_RIGHT
	};


	/**
	 * \typedef	ActivateEventCallback
	 * \brief	Triggered whenever the application gains or loses focus.
	 *
	 * The callback function expects one bool paramter.
	 *
	 * \code
	 * void function(bool gained);
	 * \endcode
	 *
	 * \arg \c gained	Bool value indicating whether or not the app lost focus.
	 */
	using ActivateEventCallback = Signals::Signal<bool>;

	/**
	 * \typedef	WindowHiddenEventCallback
	 * \brief	Triggered whenever the application's window is hidden or exposed.
	 *
	 * The callback function expects one bool paramter.
	 *
	 * \code
	 * void function(bool hidden);
	 * \endcode
	 *
	 * \arg \c gained	Bool value indicating whether or not the app lost focus.
	 */
	using WindowHiddenEventCallback = Signals::Signal<bool>;

	/**
	 * \typedef	WindowExposedEventCallback
	 * \brief	Triggered whenever the application's window is exposed.
	 *
	 * \code
	 * void function(void);
	 * \endcode
	 */
	using WindowExposedEventCallback = Signals::Signal<>;

	/**
	 * \typedef	WindowMinimizedEventCallback
	 * \brief	Triggered whenever the application's window is minimized.
	 *
	 * \code
	 * void function(void);
	 * \endcode
	 */
	using WindowMinimizedEventCallback = Signals::Signal<>;

	/**
	 * \typedef	WindowMaximizedEventCallback
	 * \brief	Triggered whenever the application's window is maximized.
	 *
	 * \code
	 * void function(void);
	 * \endcode
	 */
	using WindowMaximizedEventCallback = Signals::Signal<>;

	/**
	 * \typedef	WindowRestoredEventCallback
	 * \brief	Triggered whenever the application's window is restored to its original size and location.
	 *
	 * \code
	 * void function(void);
	 * \endcode
	 */
	using WindowRestoredEventCallback = Signals::Signal<>;

	/**
	 * \typedef	WindowMouseEnterEventCallback
	 * \brief	Triggered whenever the mouse enters the application's window.
	 *
	 * \code
	 * void function(void);
	 * \endcode
	 */
	using WindowMouseEnterEventCallback = Signals::Signal<>;

	/**
	 * \typedef	WindowMouseLeaveEventCallback
	 * \brief	Triggered whenever the mouse leaves the application's window.
	 *
	 * \code
	 * void function(void);
	 * \endcode
	 */
	using WindowMouseLeaveEventCallback = Signals::Signal<>;

	/**
	 * \typedef	WindowResizedEventCallback
	 * \brief	Triggered whenever the window is resized.
	 *
	 * \code
	 * void function(int width, int height);
	 * \endcode
	 *
	 * \arg \c width	Width of the resized window.
	 * \arg \c height	Height of the resized window.
	 */
	using WindowResizedEventCallback = Signals::Signal<int, int>;

	/**
	 * \typedef	JoystickAxisMotionEventCallback
	 * \brief	Joystic axis events generate three values: Device ID,
	 *			Axis ID and Axis Position.
	 *
	 * The callback function expects three int paramter.
	 *
	 * \code
	 * void function(int deviceId, int axisId, int pos);
	 * \endcode
	 *
	 * \arg \c deviceId	Joystick ID which this event was generated from.
	 * \arg \c axisId	Axis ID. Axis 0 is x, Axis 1 is Y. Some joysticks
						use additional axis as buttons.
	 * \arg \c pos		Current position of the axis.
	 */
	using JoystickAxisMotionEventCallback = Signals::Signal<int, int, int>;

	/**
	 * \typedef	JoystickBallMotionEventCallback
	 * \brief	Joystick trackball events generate four values: Device ID,
	 *			Ball ID, X motion change and Y motion change.
	 *
	 * The callback function expects four int paramter.
	 *
	 * \code
	 * void function(int deviceId, int ballId, int xChange, int yChange);
	 * \endcode
	 *
	 * \arg \c deviceId	Joystick ID which this event was generated from.
	 * \arg \c ballId	Trackball ID.
	 * \arg \c xChange	Change in relative position of the X position.
	 * \arg \c yChange	Change in relative position of the Y position.
	 */
	using JoystickBallMotionEventCallback = Signals::Signal<int, int, int, int>;

	/**
	 * \typedef	JoystickButtonEventCallback
	 * \brief	Joystick button events generate two values: Device ID
	 *			and Button ID. Button events occure when a joystick
	 *			button is pressed or released.
	 *
	 * The callback function expects two int paramters.
	 *
	 * \code
	 * void function(int deviceId, int buttonId);
	 * \endcode
	 *
	 * \arg \c deviceId	Joystick ID which this event was generated from.
	 * \arg \c buttonId	Button ID which the event was generated from.
	 */
	using JoystickButtonEventCallback = Signals::Signal<int, int>;

	/**
	 * \typedef	JoystickHatMotionEventCallback
	 * \brief	Joystick hat motion events generate three values: Device ID,
	 *			Hat ID, and Hat Position.
	 *
	 * The callback function expects three int paramters.
	 *
	 * \code
	 * void function(int deviceId, int hatId, int pos);
	 * \endcode
	 *
	 * \arg \c deviceId	Joystick ID which this event was generated from.
	 * \arg \c hatId	Hat ID.
	 * \arg \c pos		Current position of the hat.
	 */
	using JoystickHatMotionEventCallback = Signals::Signal<int, int, int>;

	/**
	 * \typedef	KeyDownEventCallback
	 * \brief	Keyboard key down events generate three values: Keyboard
	 *			scan code, key modifier and a repeat flag.
	 *
	 * The callback function expects one KeyCode paramter, one KeyModifier paramter
	 * and one bool parameter.
	 *
	 * \code
	 * void function(KeyCode key, KeyModifier mod, bool repeat);
	 * \endcode
	 *
	 * \arg \c key		KeyCode representing a key on the keyboard.
	 * \arg \c mod		Keyboard modifier.
	 * \arg \c repeat	Indicates that this event is a repeat and not an initial key event.
	 */
	using KeyDownEventCallback = Signals::Signal<KeyCode, KeyModifier, bool>;

	/**
	 * \typedef	KeyUpEventCallback
	 * \brief	Keyboard key up events generate two values: Keyboard
	 *			scan code and key modifier.
	 *
	 * The callback function expects one KeyCode paramter and one KeyModifier paramter.
	 *
	 * \code
	 * void function(KeyCode key, KeyModifier mod, bool repeat);
	 * \endcode
	 *
	 * \arg \c key		KeyCode representing a key on the keyboard.
	 * \arg \c mod		Keyboard modifier.
	 * \arg \c repeat	Indicates that this event is a repeat and not an initial key event.
	 */
	using KeyUpEventCallback = Signals::Signal<KeyCode, KeyModifier>;

	/**
	 * \typedef	MouseButtonEventCallback
	 * \brief	Mouse button events generate three values: MouseButton, x
	 *			and y.
	 *
	 * The callback function expects one MouseButton and two int parameters.
	 *
	 * \code
	 * void function(MouseButton button, int x, int y);
	 * \endcode
	 *
	 * \arg \c button:	MouseButton value indicating which button is pressed.
	 * \arg	\c x:		X position of the mouse button event.
	 * \arg	\c y:		Y position of the mouse button event.
	 */
	using MouseButtonEventCallback = Signals::Signal<MouseButton, int, int>;

	/**
	 * \typedef	MouseMotionEventCallback
	 * \brief	Mouse motion events generate four values: x, y, relative x
	 *			and relative y values.
	 *
	 * The callback function expects four int paramters.
	 *
	 * \code
	 * void function(int x, int y, int relX, int relY);
	 * \endcode
	 *
	 * \arg	\c x:		Absolute x position of the mouse.
	 * \arg	\c y:		Absolute y position of the mouse.
	 * \arg	\c relX:	X position of the mouse relative to its last position.
	 * \arg	\c relY;	Y position of the mouse relative to its last position.
	 */
	using MouseMotionEventCallback = Signals::Signal<int, int, int, int>;

	/**
	 * \typedef	MouseWheelEventCallback
	 * \brief	Mouse wheel events generate two values: x and y
	 *			motion values.
	 *
	 * The callback function expects two int parameters.
	 *
	 * \code
	 * void function(int x, int y);
	 * \endcode
	 *
	 * \arg	\c x:		Change along the X-Axis.
	 * \arg	\c y:		Change along the Y-Axis.
	 *
	 * \note	The value given in the axis parameters is the value
	 * 			of a single 'click' of the mouse wheel. This may be
	 * 			more than one (on Windows this value is typical 120,
	 * 			not 1).
	 */
	using MouseWheelEventCallback = Signals::Signal<int, int>;

	/**
	 * \typedef	TextInputEventCallback
	 * \brief	Fires whenever text input events occur.
	 *
	 * \code
	 * void function(const std::string&);
	 * \endcode
	 */
	using TextInputEventCallback = Signals::Signal<const std::string&>;

	/**
	 * \typedef	QuitEventCallback
	 * \brief	Triggered whenever a Quit message is posted.
	 *
	 * A Quit callback function requires no paramters.
	 *
	 * \code
	 * void function(void);
	 * \endcode
	 */
	using QuitEventCallback = Signals::Signal<>;

public:
	EventHandler();
	~EventHandler();

	ActivateEventCallback& activate();

	WindowHiddenEventCallback& windowHidden();
	WindowExposedEventCallback& windowExposed();

	WindowMinimizedEventCallback& windowMinimized();
	WindowMaximizedEventCallback& windowMaximized();
	WindowRestoredEventCallback& windowRestored();
	WindowResizedEventCallback& windowResized();

	WindowMouseEnterEventCallback& windowMouseEnter();
	WindowMouseLeaveEventCallback& windowMouseLeave();

	JoystickAxisMotionEventCallback& joystickAxisMotion();
	JoystickBallMotionEventCallback& joystickBallMotion();
	JoystickButtonEventCallback& joystickButtonUp();
	JoystickButtonEventCallback& joystickButtonDown();
	JoystickHatMotionEventCallback& joystickHatMotion();

	KeyUpEventCallback& keyUp();
	KeyDownEventCallback& keyDown();

	TextInputEventCallback& textInput();

	MouseButtonEventCallback& mouseButtonUp();
	MouseButtonEventCallback& mouseButtonDown();
	MouseButtonEventCallback& mouseDoubleClick();
	MouseMotionEventCallback& mouseMotion();
	MouseWheelEventCallback& mouseWheel();

	QuitEventCallback& quit();

	void grabMouse();
	void releaseMouse();
	void warpMouse(int x, int y);
	void mouseRelativeMode(bool rel);

	void textInputMode(bool);
	bool textInputMode();

	bool shift(KeyModifier mod) const;
	bool numlock(KeyModifier mod) const;
	bool control(KeyModifier mod) const;
	bool alt(KeyModifier mod) const;

	bool query_shift() const;
	bool query_numlock() const;
	bool query_control() const;

	void pump();

	void disconnectAll();

private:
	ActivateEventCallback mActivateEvent;

	WindowHiddenEventCallback mWindowHiddenEventCallback;
	WindowExposedEventCallback mWindowExposedEventCallback;
	WindowMinimizedEventCallback mWindowMinimizedEventCallback;
	WindowMaximizedEventCallback mWindowMaximizedEventCallback;
	WindowRestoredEventCallback mWindowRestoredEventCallback;
	WindowResizedEventCallback mWindowResizedEventCallback;
	WindowMouseEnterEventCallback mWindowMouseEnterEventCallback;
	WindowMouseLeaveEventCallback mWindowMouseLeaveEventCallback;

	JoystickAxisMotionEventCallback mJoystickAxisMotionEvent;
	JoystickBallMotionEventCallback mJoystickBallMotionEvent;
	JoystickButtonEventCallback mJoystickButtonUpEvent;
	JoystickButtonEventCallback mJoystickButtonDownEvent;
	JoystickHatMotionEventCallback mJoystickHatMotionEvent;

	KeyUpEventCallback mKeyUpEvent;
	KeyDownEventCallback mKeyDownEvent;

	TextInputEventCallback mTextInput;

	MouseButtonEventCallback mMouseButtonUpEvent;
	MouseButtonEventCallback mMouseButtonDownEvent;
	MouseButtonEventCallback mMouseDoubleClick;
	MouseMotionEventCallback mMouseMotionEvent;
	MouseWheelEventCallback mMouseWheelEvent;

	QuitEventCallback mQuitEvent;
};

void postQuitEvent();


EventHandler::KeyModifier& operator|=(EventHandler::KeyModifier& a, const EventHandler::KeyModifier& b) noexcept;
EventHandler::KeyModifier& operator&=(EventHandler::KeyModifier& a, const EventHandler::KeyModifier& b) noexcept;
EventHandler::KeyModifier operator|(EventHandler::KeyModifier a, const EventHandler::KeyModifier& b) noexcept;
EventHandler::KeyModifier operator&(EventHandler::KeyModifier a, const EventHandler::KeyModifier& b) noexcept;


} // namespace
