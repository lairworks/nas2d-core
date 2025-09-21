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
	mCurrentAction{&mAnimationSet.frames(initialAction)},
	mCurrentFrame{0},
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
	return (*mCurrentAction).frame(mCurrentFrame).bounds.size;
}


Point<int> Sprite::origin(Point<int> point) const
{
	return point - (*mCurrentAction).frame(mCurrentFrame).anchorOffset;
}


bool Sprite::isPaused() const
{
	return mPaused || (*mCurrentAction).frame(mCurrentFrame).isStopFrame();
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
	mCurrentAction = &mAnimationSet.frames(action);
	mCurrentFrame = 0;
	mTimer.reset();
	resume();
}


void Sprite::setFrame(std::size_t frameIndex)
{
	mCurrentFrame = frameIndex % mCurrentAction->frameCount();
}


void Sprite::update()
{
	mTimer.adjustStartTick(advanceByTimeDelta(mTimer.elapsedTicks()));
}


void Sprite::draw(Point<float> position) const
{
	const auto& frame = (*mCurrentAction).frame(mCurrentFrame);
	const auto drawPosition = position - frame.anchorOffset.to<float>();
	const auto frameBounds = frame.bounds.to<float>();
	Utility<Renderer>::get().drawSubImage(frame.image, drawPosition, frameBounds, mTintColor);
}


void Sprite::draw(Point<float> position, Angle rotation) const
{
	const auto& frame = (*mCurrentAction).frame(mCurrentFrame);
	const auto drawPosition = position - frame.anchorOffset.to<float>();
	const auto frameBounds = frame.bounds.to<float>();
	Utility<Renderer>::get().drawSubImageRotated(frame.image, drawPosition, frameBounds, rotation, mTintColor);
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

	const auto& frames = *mCurrentAction;
	for (;;)
	{
		const auto& frame = frames.frame(mCurrentFrame);

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
		mCurrentFrame++;
		if (mCurrentFrame >= frames.frameCount())
		{
			mCurrentFrame = 0;
		}
	}
}
