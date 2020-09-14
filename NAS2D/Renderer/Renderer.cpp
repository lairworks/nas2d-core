// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "Renderer.h"
#include "Rectangle.h"

#include <iostream>
#include <algorithm>


using namespace NAS2D;


/**
 * Internal constructor used by derived types to set the name of the Renderer.
 *
 * This c'tor is not public and can't be invoked externally.
 */
Renderer::Renderer(const std::string& appTitle) : mTitle(appTitle)
{}


Renderer::~Renderer()
{
	fadeCompleteSignal.clear();
	std::cout << "Renderer Terminated." << std::endl;
}


/**
 * Returns the title of the application window.
 */
const std::string& Renderer::title() const
{
	return mTitle;
}


/**
 * Returns the name of the driver as named by the operating system.
 */
const std::string& Renderer::driverName() const
{
	return mDriverName;
}


/**
 * Sets the title of the application window.
 */
void Renderer::title(const std::string& title)
{
	mTitle = title;
}


void Renderer::drawTextShadow(const Font& font, std::string_view text, Point<float> position, Vector<float> shadowOffset, Color textColor, Color shadowColor)
{
	const auto shadowPosition = position + shadowOffset;
	drawText(font, text, shadowPosition, shadowColor);
	drawText(font, text, position, textColor);
}


/**
 * Sets the color of the fade.
 *
 * \param	color	A Color.
 */
void Renderer::fadeColor(Color color)
{
	mFadeColor = color;
}


/**
 * Non-blocking screen fade.
 *
 * \param	delay	Time in miliseconds the fade should last. A value of 0
 *					will instantly fade the screen in.
 */
void Renderer::fadeIn(float delay)
{
	if (delay == 0)
	{
		mCurrentFade = 0.0f;
		mCurrentFadeType = FadeType::None;
		return;
	}

	mCurrentFadeType = FadeType::In;
	mFadeStep = 255.0f / delay;

	fadeTimer.delta(); // clear timer
}


/**
 * Non-blocking screen fade.
 *
 * \param	delay	Time in miliseconds the fade should last. A value of 0
 *					will instantly fade the screen in.
 */
void Renderer::fadeOut(float delay)
{
	if (delay == 0)
	{
		mCurrentFade = 255.0f;
		mCurrentFadeType = FadeType::None;
		return;
	}

	mCurrentFadeType = FadeType::Out;
	mFadeStep = 255.0f / delay;

	fadeTimer.delta(); // clear timer
}


/**
 * Gets whether or not a fade is in progress.
 */
bool Renderer::isFading() const
{
	return (mCurrentFadeType != FadeType::None);
}


/**
 * Gets whether the screen is faded or not.
 */
bool Renderer::isFaded() const
{
	return (mCurrentFade == 255.0f);
}


/**
 * Gets a refernece to the callback signal for fade transitions.
 */
Signals::Signal<>& Renderer::fadeComplete()
{
	return fadeCompleteSignal;
}


/**
 * Gets the center coordinates of the screen.
 */
Point<int> Renderer::center() const
{
	return Point{0, 0} + mResolution / 2;
}


/**
 * Updates the screen.
 *
 * \note	All derived Renderer objects must call Renderer::update()
 *			before performing screen refreshes.
 */
void Renderer::update()
{
	if (mCurrentFadeType != FadeType::None)
	{
		float fade = (fadeTimer.delta() * mFadeStep) * static_cast<int>(mCurrentFadeType);

		mCurrentFade += fade;

		if (mCurrentFade < 0.0f || mCurrentFade > 255.0f)
		{
			mCurrentFade = std::clamp(mCurrentFade, 0.0f, 255.0f);
			mCurrentFadeType = FadeType::None;
			fadeCompleteSignal();
		}
	}

	if (mCurrentFade > 0.0f)
	{
		drawBoxFilled(Rectangle<float>::Create({0, 0}, size().to<float>()), mFadeColor.alphaFade(static_cast<uint8_t>(mCurrentFade)));
	}
}


void Renderer::setResolution(Vector<int> newResolution)
{
	if (!fullscreen())
	{
		mResolution = newResolution;
	}
}


/**
 * Sets the driver name.
 *
 * \note	Internal function used only by derived
 *			renderer types.
 */
void Renderer::driverName(const std::string& name)
{
	mDriverName = name;
}
