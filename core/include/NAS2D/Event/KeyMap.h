// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2012 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#ifndef _NAS_KEYCODES_
#define _NAS_KEYCODES_

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
	KEY_BACKSPACE		= 8,
	KEY_TAB				= 9,
	KEY_CLEAR			= 12,
	KEY_RETURN			= 13,
	KEY_ENTER			= 13,
	KEY_PAUSE			= 19,
	KEY_ESCAPE			= 27,
	KEY_SPACE			= 32,
	KEY_EXCLAIM			= 33,
	KEY_QUOTEDBL		= 34,
	KEY_HASH			= 35,
	KEY_DOLLAR			= 36,
	KEY_AMPERSAND		= 38,
	KEY_QUOTE			= 39,
	KEY_LEFTPAREN		= 40,
	KEY_RIGHTPAREN		= 41,
	KEY_ASTERISK		= 42,
	KEY_PLUS			= 43,
	KEY_COMMA			= 44,
	KEY_MINUS			= 45,
	KEY_PERIOD			= 46,
	KEY_SLASH			= 47,
	KEY_0				= 48,
	KEY_1				= 49,
	KEY_2				= 50,
	KEY_3				= 51,
	KEY_4				= 52,
	KEY_5				= 53,
	KEY_6				= 54,
	KEY_7				= 55,
	KEY_8				= 56,
	KEY_9				= 57,
	KEY_COLON			= 58,
	KEY_SEMICOLON		= 59,
	KEY_LESS			= 60,
	KEY_EQUALS			= 61,
	KEY_GREATER			= 62,
	KEY_QUESTION		= 63,
	KEY_AT				= 64,

	// Special Symbols 
	KEY_LEFTBRACKET		= 91,
	KEY_BACKSLASH		= 92,
	KEY_RIGHTBRACKET	= 93,
	KEY_CARET			= 94,
	KEY_UNDERSCORE		= 95,
	KEY_BACKQUOTE		= 96,


	KEY_a				= 97,
	KEY_b				= 98,
	KEY_c				= 99,
	KEY_d				= 100,
	KEY_e				= 101,
	KEY_f				= 102,
	KEY_g				= 103,
	KEY_h				= 104,
	KEY_i				= 105,
	KEY_j				= 106,
	KEY_k				= 107,
	KEY_l				= 108,
	KEY_m				= 109,
	KEY_n				= 110,
	KEY_o				= 111,
	KEY_p				= 112,
	KEY_q				= 113,
	KEY_r				= 114,
	KEY_s				= 115,
	KEY_t				= 116,
	KEY_u				= 117,
	KEY_v				= 118,
	KEY_w				= 119,
	KEY_x				= 120,
	KEY_y				= 121,
	KEY_z				= 122,
	KEY_DELETE			= 127,

	// Numeric Keypad
	KEY_KP0				= 256,
	KEY_KP1				= 257,
	KEY_KP2				= 258,
	KEY_KP3				= 259,
	KEY_KP4				= 260,
	KEY_KP5				= 261,
	KEY_KP6				= 262,
	KEY_KP7				= 263,
	KEY_KP8				= 264,
	KEY_KP9				= 265,
	KEY_KP_PERIOD		= 266,
	KEY_KP_DIVIDE		= 267,
	KEY_KP_MULTIPLY		= 268,
	KEY_KP_MINUS		= 269,
	KEY_KP_PLUS			= 270,
	KEY_KP_ENTER		= 271,
	KEY_KP_EQUALS		= 272,

	// Arrows/Home/End
	KEY_UP				= 273,
	KEY_DOWN			= 274,
	KEY_RIGHT			= 275,
	KEY_LEFT			= 276,
	KEY_INSERT			= 277,
	KEY_HOME			= 278,
	KEY_END				= 279,
	KEY_PAGEUP			= 280,
	KEY_PAGEDOWN		= 281,

	// Function Keys
	KEY_F1				= 282,
	KEY_F2				= 283,
	KEY_F3				= 284,
	KEY_F4				= 285,
	KEY_F5				= 286,
	KEY_F6				= 287,
	KEY_F7				= 288,
	KEY_F8				= 289,
	KEY_F9				= 290,
	KEY_F10				= 291,
	KEY_F11				= 292,
	KEY_F12				= 293,
	KEY_F13				= 294,
	KEY_F14				= 295,
	KEY_F15				= 296,

	// Key State Modifiers
	KEY_NUMLOCK			= 300,
	KEY_CAPSLOCK		= 301,
	KEY_SCROLLOCK		= 302,
	KEY_RSHIFT			= 303,
	KEY_LSHIFT			= 304,
	KEY_RCTRL			= 305,
	KEY_LCTRL			= 306,
	KEY_RALT			= 307,
	KEY_LALT			= 308,
	KEY_RMETA			= 309,
	KEY_LMETA			= 310,
	KEY_LSUPER			= 311,		// Left "Windows" key
	KEY_RSUPER			= 312,		// Right "Windows" key
	KEY_MODE			= 313,		// "Alt Gr" key
	KEY_COMPOSE			= 314,		// Multi-key compose key

	// Miscellaneous function keys
	KEY_HELP			= 315,
	KEY_PRINT			= 316,
	KEY_SYSREQ			= 317,
	KEY_BREAK			= 318,
	KEY_MENU			= 319,
	KEY_POWER			= 320,		// Power Macintosh power key
	KEY_EURO			= 321,		// Some european keyboards
	KEY_UNDO			= 322,		// Atari keyboard has Undo

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

#endif