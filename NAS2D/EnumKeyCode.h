#pragma once

#include <cstdint>


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
	enum class KeyCode : uint32_t
	{
		First = 0,
		Unknown = 0,
		Backspace = '\b',
		Tab = '\t',
		Return = '\r',
		Enter = '\r',
		Escape = '\033',
		Space = ' ',
		Exclaim = '!',
		Quote = '"',
		Hash = '#',
		Percent = '%',
		Dollar = '$',
		Ampersand = '&',
		SingleQuote = '\'',
		LeftParen = '(',
		RightParen = ')',
		Asterisk = '*',
		Plus = '+',
		Comma = ',',
		Minus = '-',
		Period = '.',
		Slash = '/',
		Num0 = '0',
		Num1 = '1',
		Num2 = '2',
		Num3 = '3',
		Num4 = '4',
		Num5 = '5',
		Num6 = '6',
		Num7 = '7',
		Num8 = '8',
		Num9 = '9',
		Colon = ':',
		Semicolon = ';',
		Less = '<',
		Equal = '=',
		Greater = '>',
		Question = '?',
		At = '@',

		// Special Symbols
		LeftBracket = '[',
		Backslash = '\\',
		RightBracket = ']',
		Caret = '^',
		Underscore = '_',
		Backquote = '`',

		A = 'a',
		B = 'b',
		C = 'c',
		D = 'd',
		E = 'e',
		F = 'f',
		G = 'g',
		H = 'h',
		I = 'i',
		J = 'j',
		K = 'k',
		L = 'l',
		M = 'm',
		N = 'n',
		O = 'o',
		P = 'p',
		Q = 'q',
		R = 'r',
		S = 's',
		T = 't',
		U = 'u',
		V = 'v',
		W = 'w',
		X = 'x',
		Y = 'y',
		Z = 'z',

		// Numeric Keypad
		Keypad1 = scanCodeToKeyCode(89),
		Keypad2 = scanCodeToKeyCode(90),
		Keypad3 = scanCodeToKeyCode(91),
		Keypad4 = scanCodeToKeyCode(92),
		Keypad5 = scanCodeToKeyCode(93),
		Keypad6 = scanCodeToKeyCode(94),
		Keypad7 = scanCodeToKeyCode(95),
		Keypad8 = scanCodeToKeyCode(96),
		Keypad9 = scanCodeToKeyCode(97),
		Keypad0 = scanCodeToKeyCode(98),
		KeypadPeriod = scanCodeToKeyCode(99),
		KeypadDivide = scanCodeToKeyCode(84),
		KeypadMultiply = scanCodeToKeyCode(85),
		KeypadMinus = scanCodeToKeyCode(86),
		KeypadPlus = scanCodeToKeyCode(87),
		KeypadEnter = scanCodeToKeyCode(88),
		KeypadEqual = scanCodeToKeyCode(103),
		KeypadComma = scanCodeToKeyCode(133),

		// Arrows/Home/End
		Up = scanCodeToKeyCode(82),
		Down = scanCodeToKeyCode(81),
		Right = scanCodeToKeyCode(79),
		Left = scanCodeToKeyCode(80),
		Insert = scanCodeToKeyCode(73),
		Pause = scanCodeToKeyCode(72),
		Home = scanCodeToKeyCode(74),
		End = scanCodeToKeyCode(77),
		Delete = '\177',
		PageUp = scanCodeToKeyCode(75),
		PageDown = scanCodeToKeyCode(78),

		// Function Keys
		F1 = scanCodeToKeyCode(58),
		F2 = scanCodeToKeyCode(59),
		F3 = scanCodeToKeyCode(60),
		F4 = scanCodeToKeyCode(61),
		F5 = scanCodeToKeyCode(62),
		F6 = scanCodeToKeyCode(63),
		F7 = scanCodeToKeyCode(64),
		F8 = scanCodeToKeyCode(65),
		F9 = scanCodeToKeyCode(66),
		F10 = scanCodeToKeyCode(67),
		F11 = scanCodeToKeyCode(68),
		F12 = scanCodeToKeyCode(69),
		F13 = scanCodeToKeyCode(104),
		F14 = scanCodeToKeyCode(105),
		F15 = scanCodeToKeyCode(106),
		F16 = scanCodeToKeyCode(107),
		F17 = scanCodeToKeyCode(108),
		F18 = scanCodeToKeyCode(109),
		F19 = scanCodeToKeyCode(110),
		F20 = scanCodeToKeyCode(111),
		F21 = scanCodeToKeyCode(112),
		F22 = scanCodeToKeyCode(113),
		F23 = scanCodeToKeyCode(114),
		F24 = scanCodeToKeyCode(115),

		// Key State Modifiers
		NumLock = scanCodeToKeyCode(83),
		CapsLock = scanCodeToKeyCode(57),
		ScrollLock = scanCodeToKeyCode(71),
		RightShift = scanCodeToKeyCode(229),
		LeftShift = scanCodeToKeyCode(225),
		RightCtrl = scanCodeToKeyCode(228),
		LeftCtrl = scanCodeToKeyCode(224),
		RightAlt = scanCodeToKeyCode(230),
		LeftAlt = scanCodeToKeyCode(226),
		RightGui = scanCodeToKeyCode(231),
		LeftGui = scanCodeToKeyCode(227),
		Mode = scanCodeToKeyCode(257),

		// Miscellaneous function keys
		Help = scanCodeToKeyCode(117),
		PrintScreen = scanCodeToKeyCode(70),
		AltErase = scanCodeToKeyCode(153),
		SysReq = scanCodeToKeyCode(154),
		Clear = scanCodeToKeyCode(159),
		Menu = scanCodeToKeyCode(118),
		Power = scanCodeToKeyCode(102),
		Undo = scanCodeToKeyCode(122),

		// Audio Control
		Mute = scanCodeToKeyCode(127),
		VolumeUp = scanCodeToKeyCode(128),
		VolumeDown = scanCodeToKeyCode(129),
		AudioNext = scanCodeToKeyCode(258),
		AudioPrev = scanCodeToKeyCode(259),
		AudioStop = scanCodeToKeyCode(260),
		AudioPlay = scanCodeToKeyCode(261),
		AudioMute = scanCodeToKeyCode(262),

		Last,
	};
}
