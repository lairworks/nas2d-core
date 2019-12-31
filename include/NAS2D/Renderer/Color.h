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

#include <cstdint>

namespace NAS2D {

/**
 * \class	Color_4ub
 * \brief	RGBA Color.
 */
class Color_4ub
{
public:
	static const Color_4ub Black;
	static const Color_4ub Blue;
	static const Color_4ub Bright_green;
	static const Color_4ub Cyan;
	static const Color_4ub Green;
	static const Color_4ub Grey;
	static const Color_4ub Magenta;
	static const Color_4ub Navy;
	static const Color_4ub Orange;
	static const Color_4ub Red;
	static const Color_4ub Silver;
	static const Color_4ub White;
	static const Color_4ub Yellow;
	static const Color_4ub Normal;
	static const Color_4ub NormalZ;
	static const Color_4ub NoAlpha;

	Color_4ub() = default;
	Color_4ub(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

public:
	void operator()(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

	uint8_t red() const;
	uint8_t green() const;
	uint8_t blue() const;
	uint8_t alpha() const;

	void red(uint8_t red);
	void green(uint8_t green);
	void blue(uint8_t blue);
	void alpha(uint8_t alpha);

private:
	uint8_t mR = 255;
	uint8_t mG = 255;
	uint8_t mB = 255;
	uint8_t mA = 255;
};

} // namespace
