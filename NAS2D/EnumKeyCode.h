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
		KeyFirst = 0,
		KeyUnknown = 0,
		KeyBackspace = '\b',
		KeyTab = '\t',
		KeyReturn = '\r',
		KeyEnter = '\r',
		KeyEscape = '\033',
		KeySpace = ' ',
		KeyExclaim = '!',
		KeyQuotedbl = '"',
		KeyHash = '#',
		KeyPercent = '%',
		KeyDollar = '$',
		KeyAmpersand = '&',
		KeyQuote = '\'',
		KeyLeftparen = '(',
		KeyRightparen = ')',
		KeyAsterisk = '*',
		KeyPlus = '+',
		KeyComma = ',',
		KeyMinus = '-',
		KeyPeriod = '.',
		KeySlash = '/',
		Key0 = '0',
		Key1 = '1',
		Key2 = '2',
		Key3 = '3',
		Key4 = '4',
		Key5 = '5',
		Key6 = '6',
		Key7 = '7',
		Key8 = '8',
		Key9 = '9',
		KeyColon = ':',
		KeySemicolon = ';',
		KeyLess = '<',
		KeyEquals = '=',
		KeyGreater = '>',
		KeyQuestion = '?',
		KeyAt = '@',

		// Special Symbols
		KeyLeftbracket = '[',
		KeyBackslash = '\\',
		KeyRightbracket = ']',
		KeyCaret = '^',
		KeyUnderscore = '_',
		KeyBackquote = '`',

		KeyA = 'a',
		KeyB = 'b',
		KeyC = 'c',
		KeyD = 'd',
		KeyE = 'e',
		KeyF = 'f',
		KeyG = 'g',
		KeyH = 'h',
		KeyI = 'i',
		KeyJ = 'j',
		KeyK = 'k',
		KeyL = 'l',
		KeyM = 'm',
		KeyN = 'n',
		KeyO = 'o',
		KeyP = 'p',
		KeyQ = 'q',
		KeyR = 'r',
		KeyS = 's',
		KeyT = 't',
		KeyU = 'u',
		KeyV = 'v',
		KeyW = 'w',
		KeyX = 'x',
		KeyY = 'y',
		KeyZ = 'z',

		// Numeric Keypad
		KeyKp1 = scanCodeToKeyCode(89),
		KeyKp2 = scanCodeToKeyCode(90),
		KeyKp3 = scanCodeToKeyCode(91),
		KeyKp4 = scanCodeToKeyCode(92),
		KeyKp5 = scanCodeToKeyCode(93),
		KeyKp6 = scanCodeToKeyCode(94),
		KeyKp7 = scanCodeToKeyCode(95),
		KeyKp8 = scanCodeToKeyCode(96),
		KeyKp9 = scanCodeToKeyCode(97),
		KeyKp0 = scanCodeToKeyCode(98),
		KeyKpPeriod = scanCodeToKeyCode(99),
		KeyKpDivide = scanCodeToKeyCode(84),
		KeyKpMultiply = scanCodeToKeyCode(85),
		KeyKpMinus = scanCodeToKeyCode(86),
		KeyKpPlus = scanCodeToKeyCode(87),
		KeyKpEnter = scanCodeToKeyCode(88),
		KeyKpEquals = scanCodeToKeyCode(103),
		KeyKpComma = scanCodeToKeyCode(133),

		// Arrows/Home/End
		KeyUp = scanCodeToKeyCode(82),
		KeyDown = scanCodeToKeyCode(81),
		KeyRight = scanCodeToKeyCode(79),
		KeyLeft = scanCodeToKeyCode(80),
		KeyInsert = scanCodeToKeyCode(73),
		KeyPause = scanCodeToKeyCode(72),
		KeyHome = scanCodeToKeyCode(74),
		KeyEnd = scanCodeToKeyCode(77),
		KeyDelete = '\177',
		KeyPageup = scanCodeToKeyCode(75),
		KeyPagedown = scanCodeToKeyCode(78),

		// Function Keys
		KeyF1 = scanCodeToKeyCode(58),
		KeyF2 = scanCodeToKeyCode(59),
		KeyF3 = scanCodeToKeyCode(60),
		KeyF4 = scanCodeToKeyCode(61),
		KeyF5 = scanCodeToKeyCode(62),
		KeyF6 = scanCodeToKeyCode(63),
		KeyF7 = scanCodeToKeyCode(64),
		KeyF8 = scanCodeToKeyCode(65),
		KeyF9 = scanCodeToKeyCode(66),
		KeyF10 = scanCodeToKeyCode(67),
		KeyF11 = scanCodeToKeyCode(68),
		KeyF12 = scanCodeToKeyCode(69),
		KeyF13 = scanCodeToKeyCode(104),
		KeyF14 = scanCodeToKeyCode(105),
		KeyF15 = scanCodeToKeyCode(106),
		KeyF16 = scanCodeToKeyCode(107),
		KeyF17 = scanCodeToKeyCode(108),
		KeyF18 = scanCodeToKeyCode(109),
		KeyF19 = scanCodeToKeyCode(110),
		KeyF20 = scanCodeToKeyCode(111),
		KeyF21 = scanCodeToKeyCode(112),
		KeyF22 = scanCodeToKeyCode(113),
		KeyF23 = scanCodeToKeyCode(114),
		KeyF24 = scanCodeToKeyCode(115),

		// Key State Modifiers
		KeyNumlock = scanCodeToKeyCode(83),
		KeyCapslock = scanCodeToKeyCode(57),
		KeyScrollock = scanCodeToKeyCode(71),
		KeyRshift = scanCodeToKeyCode(229),
		KeyLshift = scanCodeToKeyCode(225),
		KeyRctrl = scanCodeToKeyCode(228),
		KeyLctrl = scanCodeToKeyCode(224),
		KeyRalt = scanCodeToKeyCode(230),
		KeyLalt = scanCodeToKeyCode(226),
		KeyRgui = scanCodeToKeyCode(231),
		KeyLgui = scanCodeToKeyCode(227),
		KeyMode = scanCodeToKeyCode(257),

		// Miscellaneous function keys
		KeyHelp = scanCodeToKeyCode(117),
		KeyPrintscreen = scanCodeToKeyCode(70),
		KeyAlterase = scanCodeToKeyCode(153),
		KeySysreq = scanCodeToKeyCode(154),
		KeyClear = scanCodeToKeyCode(159),
		KeyMenu = scanCodeToKeyCode(118),
		KeyPower = scanCodeToKeyCode(102),
		KeyUndo = scanCodeToKeyCode(122),

		// Audio Control
		KeyMute = scanCodeToKeyCode(127),
		KeyVolumeUp = scanCodeToKeyCode(128),
		KeyVolumeDown = scanCodeToKeyCode(129),
		KeyAudioNext = scanCodeToKeyCode(258),
		KeyAudioPrev = scanCodeToKeyCode(259),
		KeyAudioStop = scanCodeToKeyCode(260),
		KeyAudioPlay = scanCodeToKeyCode(261),
		KeyAudioMute = scanCodeToKeyCode(262),

		KeyLast,
	};
}
