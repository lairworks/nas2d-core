// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================
#include "Renderer.h"
#include "Rectangle.h"

#include <iostream>
#include <algorithm>


using namespace NAS2D;

Renderer::Renderer(const std::string& appTitle) : mTitle(appTitle)
{}


Renderer::~Renderer()
{
	fadeCompleteSignal.clear();
	std::cout << "Renderer Terminated." << std::endl;
}

const std::string& Renderer::title() const
{
	return mTitle;
}

const std::string& Renderer::driverName() const
{
	return mDriverName;
}

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

void Renderer::fadeColor(Color color)
{
	mFadeColor = color;
}

//TODO: Rename delay to delayMs?
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

	fadeTimer.delta();
}

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

	fadeTimer.delta();
}

bool Renderer::isFading() const
{
	return (mCurrentFadeType != FadeType::None);
}

bool Renderer::isFaded() const
{
	return (mCurrentFade == 255.0f);
}

SignalSource<>& Renderer::fadeComplete()
{
	return fadeCompleteSignal;
}

Point<int> Renderer::center() const
{
	return Point{0, 0} + mResolution / 2;
}

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

void Renderer::driverName(const std::string& name)
{
	mDriverName = name;
}
