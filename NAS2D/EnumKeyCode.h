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
		KeyQuote = '"',
		KeyHash = '#',
		KeyPercent = '%',
		KeyDollar = '$',
		KeyAmpersand = '&',
		KeySingleQuote = '\'',
		KeyLeftParen = '(',
		KeyRightParen = ')',
		KeyAsterisk = '*',
		KeyPlus = '+',
		KeyComma = ',',
		KeyMinus = '-',
		KeyPeriod = '.',
		KeySlash = '/',
		KeyNum0 = '0',
		KeyNum1 = '1',
		KeyNum2 = '2',
		KeyNum3 = '3',
		KeyNum4 = '4',
		KeyNum5 = '5',
		KeyNum6 = '6',
		KeyNum7 = '7',
		KeyNum8 = '8',
		KeyNum9 = '9',
		KeyColon = ':',
		KeySemicolon = ';',
		KeyLess = '<',
		KeyEqual = '=',
		KeyGreater = '>',
		KeyQuestion = '?',
		KeyAt = '@',

		// Special Symbols
		KeyLeftBracket = '[',
		KeyBackslash = '\\',
		KeyRightBracket = ']',
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
		KeyKeypad1 = scanCodeToKeyCode(89),
		KeyKeypad2 = scanCodeToKeyCode(90),
		KeyKeypad3 = scanCodeToKeyCode(91),
		KeyKeypad4 = scanCodeToKeyCode(92),
		KeyKeypad5 = scanCodeToKeyCode(93),
		KeyKeypad6 = scanCodeToKeyCode(94),
		KeyKeypad7 = scanCodeToKeyCode(95),
		KeyKeypad8 = scanCodeToKeyCode(96),
		KeyKeypad9 = scanCodeToKeyCode(97),
		KeyKeypad0 = scanCodeToKeyCode(98),
		KeyKeypadPeriod = scanCodeToKeyCode(99),
		KeyKeypadDivide = scanCodeToKeyCode(84),
		KeyKeypadMultiply = scanCodeToKeyCode(85),
		KeyKeypadMinus = scanCodeToKeyCode(86),
		KeyKeypadPlus = scanCodeToKeyCode(87),
		KeyKeypadEnter = scanCodeToKeyCode(88),
		KeyKeypadEqual = scanCodeToKeyCode(103),
		KeyKeypadComma = scanCodeToKeyCode(133),

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
		KeyPageUp = scanCodeToKeyCode(75),
		KeyPageDown = scanCodeToKeyCode(78),

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
		KeyNumLock = scanCodeToKeyCode(83),
		KeyCapsLock = scanCodeToKeyCode(57),
		KeyScrollLock = scanCodeToKeyCode(71),
		KeyRightShift = scanCodeToKeyCode(229),
		KeyLeftShift = scanCodeToKeyCode(225),
		KeyRightCtrl = scanCodeToKeyCode(228),
		KeyLeftCtrl = scanCodeToKeyCode(224),
		KeyRightAlt = scanCodeToKeyCode(230),
		KeyLeftAlt = scanCodeToKeyCode(226),
		KeyRightGui = scanCodeToKeyCode(231),
		KeyLeftGui = scanCodeToKeyCode(227),
		KeyMode = scanCodeToKeyCode(257),

		// Miscellaneous function keys
		KeyHelp = scanCodeToKeyCode(117),
		KeyPrintScreen = scanCodeToKeyCode(70),
		KeyAltErase = scanCodeToKeyCode(153),
		KeySysReq = scanCodeToKeyCode(154),
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
