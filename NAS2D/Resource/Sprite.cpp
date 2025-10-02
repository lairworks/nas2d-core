// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================

#include "Sprite.h"

#include "AnimationFrame.h"
#include "AnimationSequence.h"
#include "AnimationSet.h"
#include "../Math/Angle.h"
#include "../Math/Point.h"
#include "../Math/Vector.h"
#include "../Renderer/Renderer.h"
#include "../Duration.h"
#include "../Utility.h"

#include <utility>
#include <stdexcept>


using namespace NAS2D;


Sprite::Sprite(const AnimationSet& animationSet, const std::string& initialAction) :
	mAnimationSet{animationSet},
	mAnimatedImage{mAnimationSet.frames(initialAction)},
	mPaused{false},
	mTimer{},
	mTintColor{Color::Normal}
{
}


const AnimationSet& Sprite::animationSet() const
{
	return mAnimationSet;
}


Vector<int> Sprite::size() const
{
	return mAnimatedImage.frame().bounds.size;
}


Point<int> Sprite::origin(Point<int> point) const
{
	return point - mAnimatedImage.frame().anchorOffset;
}


bool Sprite::isPaused() const
{
	return mPaused || mAnimatedImage.frame().isStopFrame();
}


void Sprite::pause()
{
	mPaused = true;
}


void Sprite::resume()
{
	mPaused = false;
}


void Sprite::play(const std::string& action)
{
	mAnimatedImage = AnimatedImage{mAnimationSet.frames(action)};
	mTimer.reset();
	resume();
}


void Sprite::setFrame(std::size_t frameIndex)
{
	mAnimatedImage.setFrame(frameIndex);
}


void Sprite::update()
{
	mTimer.adjustStartTick(advanceByTimeDelta(mTimer.elapsedTicks()));
}


void Sprite::draw(Point<int> position) const
{
	mAnimatedImage.draw(Utility<Renderer>::get(), position, mTintColor);
}


void Sprite::draw(Point<int> position, Angle rotation) const
{
	mAnimatedImage.draw(Utility<Renderer>::get(), position, mTintColor, rotation);
}


void Sprite::alpha(uint8_t alpha)
{
	mTintColor.alpha = alpha;
}


uint8_t Sprite::alpha() const
{
	return mTintColor.alpha;
}


void Sprite::color(Color color)
{
	mTintColor = color;
}


Color Sprite::color() const
{
	return mTintColor;
}


Duration Sprite::advanceByTimeDelta(Duration timeDelta)
{
	Duration accumulator{0};

	if (mPaused)
	{
		return accumulator;
	}

	for (;;)
	{
		const auto& frame = mAnimatedImage.frame();

		if (frame.isStopFrame())
		{
			mPaused = true;
			return accumulator;
		}

		if (timeDelta - accumulator < frame.frameDelay)
		{
			return accumulator;
		}

		accumulator += frame.frameDelay;
		mAnimatedImage.advanceFrame();
	}
}
