#pragma once

#include <cstdint>


enum class NAS2D::KeyModifier : uint16_t
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
