// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================


#include "Fade.h"
#include "Renderer.h"
#include "../Math/Rectangle.h"

#include <algorithm>
#include <stdexcept>


using namespace NAS2D;


namespace
{
	constexpr uint8_t alphaTransparent = 0;
	constexpr uint8_t alphaOpaque = 255;
}


Fade::Fade(DelegateType onFadeComplete) :
	Fade{Color::Black, onFadeComplete}
{
}


Fade::Fade(Color fadeColor, DelegateType onFadeComplete) :
	mFadeColor{fadeColor.alphaFade(alphaOpaque)},
	mDirection{FadeDirection::None},
	mDuration{},
	mFadeTimer{},
	mOnFadeComplete{onFadeComplete}
{
}


// Fade in from fadeColor
void Fade::fadeIn(Duration fadeTime)
{
	setDuration(fadeTime);
	mDirection = FadeDirection::In;
}


// Fade out to fadeColor
void Fade::fadeOut(Duration fadeTime)
{
	setDuration(fadeTime);
	mDirection = FadeDirection::Out;
}


bool Fade::isFading() const
{
	return (mDirection != FadeDirection::None);
}


bool Fade::isFaded() const
{
	return (mDirection == FadeDirection::None) && (mFadeColor.alpha == alphaOpaque);
}


void Fade::update()
{
	if (mDirection == FadeDirection::None)
	{
		return;
	}

	const auto currentMilliseconds = std::min(mFadeTimer.elapsedTicks(), mDuration.milliseconds);
	const auto step = static_cast<uint8_t>(currentMilliseconds * 255u / mDuration.milliseconds);
	mFadeColor.alpha = (mDirection == FadeDirection::In) ? 255 - step : step;

	if (currentMilliseconds >= mDuration.milliseconds)
	{
		mDirection = FadeDirection::None;
		if (!mOnFadeComplete.empty())
		{
			mOnFadeComplete();
		}
	}
}


void Fade::draw(Renderer& renderer) const
{
	if (mFadeColor.alpha != alphaTransparent)
	{
		const auto displayRect = Rectangle{{0, 0}, renderer.size()};
		renderer.drawBoxFilled(displayRect, mFadeColor);
	}
}


void Fade::setDuration(Duration newDuration)
{
	if (newDuration.milliseconds == 0)
	{
		throw std::domain_error("Fade duration must be positive");
	}

	mDuration = newDuration;
	mFadeTimer.reset();
}
