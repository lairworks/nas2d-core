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


using namespace NAS2D;


Fade::Fade() :
	Fade(Color::Black)
{
}


Fade::Fade(Color fadeColor) :
	mFadeColor{fadeColor.alphaFade(255)},
	mDirection{FadeDirection::None},
	mDuration{},
	mFadeTimer{},
	mFadeComplete{}
{}


Fade::Fade(FadeCompleteSignal::DelegateType onFadeComplete) :
	Fade()
{
	mFadeComplete.connect(onFadeComplete);
}


Fade::Fade(Color fadeColor, FadeCompleteSignal::DelegateType onFadeComplete) :
	Fade(fadeColor)
{
	mFadeComplete.connect(onFadeComplete);
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
	return (mFadeColor.alpha == 255);
}


void Fade::update()
{
	if (mDirection == FadeDirection::None)
	{
		return;
	}

	const auto step = static_cast<uint8_t>(std::clamp(mFadeTimer.elapsedTicks() * 255u / static_cast<unsigned int>(mDuration.milliseconds), 0u, 255u));
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
		const auto displayRect = Rectangle{{0, 0}, renderer.size()};
		renderer.drawBoxFilled(displayRect, mFadeColor);
	}
}


void Fade::setDuration(Duration newDuration)
{
	if (newDuration.milliseconds == 0)
	{
		throw std::runtime_error("Fade duration must be positive");
	}

	mDuration = newDuration;
	mFadeTimer.reset();
}
