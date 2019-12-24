// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Renderer/Color.h"

using namespace NAS2D;

// ==================================================================================
// = Color_4ub Implementation
// ==================================================================================


/**
 * C'tor
 *
 * Instantiates a Color_4ub with a given RGBA value set.
 *
 * \param r	Red compontent. Valid values are 0 - 255.
 * \param g	Green compontent. Valid values are 0 - 255.
 * \param b	Blue compontent. Valid values are 0 - 255.
 * \param a	Alpha compontent. Valid values are 0 - 255.
 */
Color_4ub::Color_4ub(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : mR(r), mG(g), mB(b), mA(a)
{}


/**
 * Sets a Color_4ub with a given RGBA value set.
 *
 * \param r	Red compontent. Valid values are 0 - 255.
 * \param g	Green compontent. Valid values are 0 - 255.
 * \param b	Blue compontent. Valid values are 0 - 255.
 * \param a	Alpha compontent. Valid values are 0 - 255.
 */
void Color_4ub::operator()(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	red(r);
	green(g);
	blue(b);
	alpha(a);
}


/**
 * Gets the red component value of the color.
 */
uint8_t Color_4ub::red() const
{
	return mR;
}


/**
 * Gets the green component value of the color.
 */
uint8_t Color_4ub::green() const
{
	return mG;
}


/**
 * Gets the blue component value of the color.
 */
uint8_t Color_4ub::blue() const
{
	return mB;
}


/**
 * Gets the alpha component value of the color.
 */
uint8_t Color_4ub::alpha() const
{
	return mA;
}


/**
 * Sets the red component value of the color.
 *
 * \param red Red value. Valid values are 0 - 255.
 */
void Color_4ub::red(uint8_t red)
{
	mR = red;
}


/**
 * Sets the green component value of the color.
 *
 * \param green Green value. Valid values are 0 - 255.
 */
void Color_4ub::green(uint8_t green)
{
	mG = green;
}


/**
 * Sets the blue component value of the color.
 *
 * \param blue Blue value. Valid values are 0 - 255.
 */
void Color_4ub::blue(uint8_t blue)
{
	mB = blue;
}


/**
 * Sets the alpha component value of the color.
 *
 * \param alpha Alpha value. Valid values are 0 - 255.
 */
void Color_4ub::alpha(uint8_t alpha)
{
	mA = alpha;
}
