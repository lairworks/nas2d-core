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
 * \class	Color
 * \brief	RGBA Color.
 */
class Color
{
public:
	static const Color Black;
	static const Color Blue;
	static const Color Green;
	static const Color Cyan;
	static const Color DarkGreen;
	static const Color DarkGray;
	static const Color Gray;
	static const Color LightGray;
	static const Color CoolGray;
	static const Color CoolLightGray;
	static const Color CoolDarkGray;
	static const Color WarmGray;
	static const Color WarmLightGray;
	static const Color WarmDarkGray;
	static const Color Magenta;
	static const Color Navy;
	static const Color Orange;
	static const Color Red;
	static const Color Silver;
	static const Color White;
	static const Color Yellow;
	static const Color Normal;
	static const Color NormalZ;
	static const Color NoAlpha;

	Color() = default;
	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

	bool operator==(Color other) const;
	bool operator!=(Color other) const;

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
