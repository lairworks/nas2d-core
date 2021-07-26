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
#include "Rectangle.h"

#include <algorithm>


using namespace NAS2D;


Fade::Fade(Color fadeColor) :
	mFadeColor{fadeColor.alphaFade(255)}
{}


SignalSource<>& Fade::fadeComplete()
{
	return mFadeComplete;
}


void Fade::fadeIn(unsigned int durationInMilliseconds)
{
	setDuration(durationInMilliseconds);
	mDirection = FadeDirection::In;
}


void Fade::fadeOut(unsigned int durationInMilliseconds)
{
	setDuration(durationInMilliseconds);
	mDirection = FadeDirection::Out;
}


bool Fade::isFading() const
{
	return (mDirection != FadeDirection::None);
}


bool Fade::isFaded() const
{
	return (mFadeColor.alpha == 255);
}


void Fade::update()
{
	if (mDirection == FadeDirection::None)
	{
		return;
	}

	const auto step = static_cast<uint8_t>(std::clamp(mFadeTimer.accumulator() * 255u / mDuration, 0u, 255u));
	mFadeColor.alpha = (mDirection == FadeDirection::In) ? 255 - step : step;

	if (step == 255)
	{
		mDirection = FadeDirection::None;
		mFadeComplete();
	}
}


void Fade::draw(Renderer& renderer) const
{
	if (mFadeColor.alpha > 0)
	{
		const auto displayRect = Rectangle<int>::Create({0, 0}, renderer.size());
		renderer.drawBoxFilled(displayRect, mFadeColor);
	}
}


void Fade::setDuration(unsigned int durationInMilliseconds)
{
	if (durationInMilliseconds == 0)
	{
		throw std::runtime_error("Fade duration must be positive");
	}

	mDuration = durationInMilliseconds;
	mFadeTimer.reset();
}
