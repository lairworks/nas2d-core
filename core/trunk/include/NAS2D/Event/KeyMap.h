// ==================================================================================
// = NAS2D
// = Copyright � 2008 - 2014 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#ifndef _NAS_KEYCODES_
#define _NAS_KEYCODES_

namespace NAS2D {

#define SCANCODE_MASK (1<<30)
#define SCANCODE_TO_KEYCODE(X)	(X | SCANCODE_MASK)

/**
 * \enum KeyCode
 * \brief Keyboard scan codes.
 *
 * \note	These mappings are directly mapped from SDL's keyboard scan
 *			code enumerated list.
 */
enum KeyCode
{
	KEY_UNKNOWN			= 0,
	KEY_FIRST			= 0,
	KEY_BACKSPACE		= '\b',
	KEY_TAB				= '\t',
	KEY_RETURN			= '\r',
	KEY_ENTER			= '\r',
	KEY_ESCAPE			= '\033',
	KEY_SPACE			= ' ',
	KEY_EXCLAIM			= '!',
	KEY_QUOTEDBL		= '"',
	KEY_HASH			= '#',
	KEY_PERCENT			= '%',
	KEY_DOLLAR			= '$',
	KEY_AMPERSAND		= '&',
	KEY_QUOTE			= '\'',
	KEY_LEFTPAREN		= '(',
	KEY_RIGHTPAREN		= ')',
	KEY_ASTERISK		= '*',
	KEY_PLUS			= '+',
	KEY_COMMA			= ',',
	KEY_MINUS			= '-',
	KEY_PERIOD			= '.',
	KEY_SLASH			= '/',
	KEY_0				= '0',
	KEY_1				= '1',
	KEY_2				= '2',
	KEY_3				= '3',
	KEY_4				= '4',
	KEY_5				= '5',
	KEY_6				= '6',
	KEY_7				= '7',
	KEY_8				= '8',
	KEY_9				= '9',
	KEY_COLON			= ':',
	KEY_SEMICOLON		= ';',
	KEY_LESS			= '<',
	KEY_EQUALS			= '=',
	KEY_GREATER			= '>',
	KEY_QUESTION		= '?',
	KEY_AT				= '@',

	// Special Symbols 
	KEY_LEFTBRACKET		= '[',
	KEY_BACKSLASH		= '\\',
	KEY_RIGHTBRACKET	= ']',
	KEY_CARET			= '^',
	KEY_UNDERSCORE		= '_',
	KEY_BACKQUOTE		= '`',


	KEY_a				= 'a',
	KEY_b				= 'b',
	KEY_c				= 'c',
	KEY_d				= 'd',
	KEY_e				= 'e',
	KEY_f				= 'f',
	KEY_g				= 'g',
	KEY_h				= 'h',
	KEY_i				= 'i',
	KEY_j				= 'j',
	KEY_k				= 'k',
	KEY_l				= 'l',
	KEY_m				= 'm',
	KEY_n				= 'n',
	KEY_o				= 'o',
	KEY_p				= 'p',
	KEY_q				= 'q',
	KEY_r				= 'r',
	KEY_s				= 's',
	KEY_t				= 't',
	KEY_u				= 'u',
	KEY_v				= 'v',
	KEY_w				= 'w',
	KEY_x				= 'x',
	KEY_y				= 'y',
	KEY_z				= 'z',

	// Numeric Keypad
	KEY_KP1				= SCANCODE_TO_KEYCODE(89),
	KEY_KP2				= SCANCODE_TO_KEYCODE(90),
	KEY_KP3				= SCANCODE_TO_KEYCODE(91),
	KEY_KP4				= SCANCODE_TO_KEYCODE(92),
	KEY_KP5				= SCANCODE_TO_KEYCODE(93),
	KEY_KP6				= SCANCODE_TO_KEYCODE(94),
	KEY_KP7				= SCANCODE_TO_KEYCODE(95),
	KEY_KP8				= SCANCODE_TO_KEYCODE(96),
	KEY_KP9				= SCANCODE_TO_KEYCODE(97),
	KEY_KP0				= SCANCODE_TO_KEYCODE(98),
	KEY_KP_PERIOD		= SCANCODE_TO_KEYCODE(99),
	KEY_KP_DIVIDE		= SCANCODE_TO_KEYCODE(84),
	KEY_KP_MULTIPLY		= SCANCODE_TO_KEYCODE(85),
	KEY_KP_MINUS		= SCANCODE_TO_KEYCODE(86),
	KEY_KP_PLUS			= SCANCODE_TO_KEYCODE(87),
	KEY_KP_ENTER		= SCANCODE_TO_KEYCODE(88),
	KEY_KP_EQUALS		= SCANCODE_TO_KEYCODE(103),
	KEY_KP_COMMA		= SCANCODE_TO_KEYCODE(133),

	// Arrows/Home/End
	KEY_UP				= SCANCODE_TO_KEYCODE(82),
	KEY_DOWN			= SCANCODE_TO_KEYCODE(81),
	KEY_RIGHT			= SCANCODE_TO_KEYCODE(79),
	KEY_LEFT			= SCANCODE_TO_KEYCODE(80),
	KEY_INSERT			= SCANCODE_TO_KEYCODE(73),
	KEY_PAUSE			= SCANCODE_TO_KEYCODE(72),
	KEY_HOME			= SCANCODE_TO_KEYCODE(74),
	KEY_END				= SCANCODE_TO_KEYCODE(77),
	KEY_DELETE			= '\177',
	KEY_PAGEUP			= SCANCODE_TO_KEYCODE(75),
	KEY_PAGEDOWN		= SCANCODE_TO_KEYCODE(78),

	// Function Keys
	KEY_F1				= SCANCODE_TO_KEYCODE(58),
	KEY_F2				= SCANCODE_TO_KEYCODE(59),
	KEY_F3				= SCANCODE_TO_KEYCODE(60),
	KEY_F4				= SCANCODE_TO_KEYCODE(61),
	KEY_F5				= SCANCODE_TO_KEYCODE(62),
	KEY_F6				= SCANCODE_TO_KEYCODE(63),
	KEY_F7				= SCANCODE_TO_KEYCODE(64),
	KEY_F8				= SCANCODE_TO_KEYCODE(65),
	KEY_F9				= SCANCODE_TO_KEYCODE(66),
	KEY_F10				= SCANCODE_TO_KEYCODE(67),
	KEY_F11				= SCANCODE_TO_KEYCODE(68),
	KEY_F12				= SCANCODE_TO_KEYCODE(69),
	KEY_F13				= SCANCODE_TO_KEYCODE(104),
	KEY_F14				= SCANCODE_TO_KEYCODE(105),
	KEY_F15				= SCANCODE_TO_KEYCODE(106),
	KEY_F16				= SCANCODE_TO_KEYCODE(107),
	KEY_F17				= SCANCODE_TO_KEYCODE(108),
	KEY_F18				= SCANCODE_TO_KEYCODE(109),
	KEY_F19				= SCANCODE_TO_KEYCODE(110),
	KEY_F20				= SCANCODE_TO_KEYCODE(111),
	KEY_F21				= SCANCODE_TO_KEYCODE(112),
	KEY_F22				= SCANCODE_TO_KEYCODE(113),
	KEY_F23				= SCANCODE_TO_KEYCODE(114),
	KEY_F24				= SCANCODE_TO_KEYCODE(115),

	// Key State Modifiers
	KEY_NUMLOCK			= SCANCODE_TO_KEYCODE(83),
	KEY_CAPSLOCK		= SCANCODE_TO_KEYCODE(57),
	KEY_SCROLLOCK		= SCANCODE_TO_KEYCODE(71),
	KEY_RSHIFT			= SCANCODE_TO_KEYCODE(229),
	KEY_LSHIFT			= SCANCODE_TO_KEYCODE(225),
	KEY_RCTRL			= SCANCODE_TO_KEYCODE(228),
	KEY_LCTRL			= SCANCODE_TO_KEYCODE(224),
	KEY_RALT			= SCANCODE_TO_KEYCODE(230),
	KEY_LALT			= SCANCODE_TO_KEYCODE(226),
	KEY_RGUI			= SCANCODE_TO_KEYCODE(231),
	KEY_LGUI			= SCANCODE_TO_KEYCODE(227),
	KEY_MODE			= SCANCODE_TO_KEYCODE(257),

	// Miscellaneous function keys
	KEY_HELP			= SCANCODE_TO_KEYCODE(117),
	KEY_PRINTSCREEN		= SCANCODE_TO_KEYCODE(70),
	KEY_ALTERASE		= SCANCODE_TO_KEYCODE(153),
	KEY_SYSREQ			= SCANCODE_TO_KEYCODE(154),
	KEY_CLEAR			= SCANCODE_TO_KEYCODE(159),
	KEY_MENU			= SCANCODE_TO_KEYCODE(118),
	KEY_POWER			= SCANCODE_TO_KEYCODE(102),
	KEY_UNDO			= SCANCODE_TO_KEYCODE(122),

	// Audio Control
    KEY_MUTE			= SCANCODE_TO_KEYCODE(127),
    KEY_VOLUME_UP		= SCANCODE_TO_KEYCODE(128),
    KEY_VOLUME_DOWN		= SCANCODE_TO_KEYCODE(129),
    KEY_AUDIO_NEXT		= SCANCODE_TO_KEYCODE(258),
    KEY_AUDIO_PREV		= SCANCODE_TO_KEYCODE(259),
    KEY_AUDIO_STOP		= SCANCODE_TO_KEYCODE(260),
    KEY_AUDIO_PLAY		= SCANCODE_TO_KEYCODE(261),
    KEY_AUDIO_MUTE		= SCANCODE_TO_KEYCODE(262),


	// Additional Keys here
	KEY_LAST
};


/**
 * \enum KeyModifier
 * \brief Key Modifiers
 *
 * \note	Mapped directly from SDL's keyboard scan code
 *			enumerated list.
 */
enum KeyModifier
{
	KEY_MOD_NONE		= 0x0000,
	KEY_MOD_LSHIFT		= 0x0001,
	KEY_MOD_RSHIFT		= 0x0002,
	KEY_MOD_LCTRL		= 0x0040,
	KEY_MOD_RCTRL		= 0x0080,
	KEY_MOD_LALT		= 0x0100,
	KEY_MOD_RALT		= 0x0200,
	KEY_MOD_LMETA		= 0x0400,
	KEY_MOD_RMETA		= 0x0800,
	KEY_MOD_NUM			= 0x1000,
	KEY_MOD_CAPS		= 0x2000,
	KEY_MOD_MODE		= 0x4000,
	KEY_MOD_RESERVED	= 0x8000
};


const int KEY_MOD_CTRL	= KEY_MOD_LCTRL | KEY_MOD_RCTRL;
const int KEY_MOD_SHIFT	= KEY_MOD_LSHIFT | KEY_MOD_RSHIFT;
const int KEY_MOD_ALT	= KEY_MOD_LALT | KEY_MOD_RALT;
const int KEY_MOD_META	= KEY_MOD_LMETA | KEY_MOD_RMETA;

} // namespace

#endif