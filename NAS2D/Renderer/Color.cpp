// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "Color.h"

using namespace NAS2D;

const Color Color::Black{0, 0, 0};
const Color Color::Blue{0, 0, 255};
const Color Color::Green{0, 255, 0};
const Color Color::Cyan{0, 255, 255};
const Color Color::DarkGreen{0, 128, 0};
const Color Color::DarkGray{64, 64, 64};
const Color Color::Gray{128, 128, 128};
const Color Color::LightGray{192, 192, 192};
const Color Color::CoolGray{128, 128, 143};
const Color Color::CoolLightGray{192, 192, 207};
const Color Color::CoolDarkGray{64, 64, 79};
const Color Color::WarmGray{143, 143, 128};
const Color Color::WarmLightGray{207, 207, 192};
const Color Color::WarmDarkGray{79, 79, 64};
const Color Color::Magenta{255, 0, 255};
const Color Color::Navy{35, 60, 85};
const Color Color::Orange{255, 128, 0};
const Color Color::Red{255, 0, 0};
const Color Color::Silver{192, 192, 192};
const Color Color::White{255, 255, 255};
const Color Color::Yellow{255, 255, 0};
const Color Color::Normal{255, 255, 255};
const Color Color::NormalZ{128, 128, 255};
const Color Color::NoAlpha{0, 0, 0, 0};



// ==================================================================================
// = Color Implementation
// ==================================================================================

/**
 * C'tor
 *
 * Instantiates a Color with a given RGBA value set.
 *
 * \param r	Red compontent. Valid values are 0 - 255.
 * \param g	Green compontent. Valid values are 0 - 255.
 * \param b	Blue compontent. Valid values are 0 - 255.
 * \param a	Alpha compontent. Valid values are 0 - 255.
 */
Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : mR(r), mG(g), mB(b), mA(a)
{}


bool Color::operator==(Color other) const
{
	return (mR == other.mR) && (mG == other.mG) && (mB == other.mB) && (mA == other.mA);
}


bool Color::operator!=(Color other) const
{
	return !(*this == other);
}


/**
 * Sets a Color with a given RGBA value set.
 *
 * \param r	Red compontent. Valid values are 0 - 255.
 * \param g	Green compontent. Valid values are 0 - 255.
 * \param b	Blue compontent. Valid values are 0 - 255.
 * \param a	Alpha compontent. Valid values are 0 - 255.
 */
void Color::operator()(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	red(r);
	green(g);
	blue(b);
	alpha(a);
}


/**
 * Gets the red component value of the color.
 */
uint8_t Color::red() const
{
	return mR;
}


/**
 * Gets the green component value of the color.
 */
uint8_t Color::green() const
{
	return mG;
}


/**
 * Gets the blue component value of the color.
 */
uint8_t Color::blue() const
{
	return mB;
}


/**
 * Gets the alpha component value of the color.
 */
uint8_t Color::alpha() const
{
	return mA;
}


/**
 * Sets the red component value of the color.
 *
 * \param red Red value. Valid values are 0 - 255.
 */
void Color::red(uint8_t red)
{
	mR = red;
}


/**
 * Sets the green component value of the color.
 *
 * \param green Green value. Valid values are 0 - 255.
 */
void Color::green(uint8_t green)
{
	mG = green;
}


/**
 * Sets the blue component value of the color.
 *
 * \param blue Blue value. Valid values are 0 - 255.
 */
void Color::blue(uint8_t blue)
{
	mB = blue;
}


/**
 * Sets the alpha component value of the color.
 *
 * \param alpha Alpha value. Valid values are 0 - 255.
 */
void Color::alpha(uint8_t alpha)
{
	mA = alpha;
}
