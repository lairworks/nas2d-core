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
#include "ResourceCache.h"
#include "../Renderer/Renderer.h"
#include "../Utility.h"

#include <utility>
#include <iostream>
#include <stdexcept>


using namespace NAS2D;


namespace {
	const auto FRAME_PAUSE = unsigned(-1);

	using AnimationCache = ResourceCache<AnimationSet, std::string>;
	AnimationCache animationCache;
}

Sprite::Sprite(const std::string& filePath, const std::string& initialAction) :
	mSpriteName{filePath},
	mAnimationSet{&animationCache.load(filePath)},
	mCurrentAction{&mAnimationSet->frames(initialAction)}
{
}


Vector<int> Sprite::size() const
{
	return (*mCurrentAction)[mCurrentFrame].bounds.size();
}


Point<int> Sprite::origin(Point<int> point) const
{
	return point - (*mCurrentAction)[mCurrentFrame].anchorOffset;
}

std::vector<std::string> Sprite::actions() const
{
	return mAnimationSet->actionNames();
}

void Sprite::play(const std::string& action)
{
	mCurrentAction = &mAnimationSet->frames(action);
	mCurrentFrame = 0;
	mTimer.reset();
	resume();
}

void Sprite::pause()
{
	mPaused = true;
}

void Sprite::resume()
{
	mPaused = false;
}

void Sprite::setFrame(std::size_t frameIndex)
{
	mCurrentFrame = frameIndex % mCurrentAction->size();
}

void Sprite::incrementFrame()
{
	setFrame(mCurrentFrame + 1);
}

void Sprite::decrementFrame()
{
	setFrame(mCurrentFrame - 1);
}


void Sprite::update(Point<float> position)
{
	const auto& frame = (*mCurrentAction)[mCurrentFrame];

	if (!mPaused && (frame.frameDelay != FRAME_PAUSE))
	{
		while (frame.frameDelay > 0 && mTimer.accumulator() >= frame.frameDelay)
		{
			mTimer.adjust_accumulator(frame.frameDelay);
			mCurrentFrame++;
		}

		if (mCurrentFrame >= mCurrentAction->size())
		{
			mCurrentFrame = 0;
			mAnimationCompleteSignal();
		}
	}
	else if (frame.frameDelay == FRAME_PAUSE)
	{
		mAnimationCompleteSignal();
	}

	const auto drawPosition = position - frame.anchorOffset.to<float>();
	const auto frameBounds = frame.bounds.to<float>();
	Utility<Renderer>::get().drawSubImageRotated(frame.image, drawPosition, frameBounds, mRotationAngle, mColor);
}

void Sprite::rotation(float angle)
{
	mRotationAngle = angle;
}

float Sprite::rotation() const
{
	return mRotationAngle;
}

void Sprite::alpha(uint8_t alpha)
{
	mColor.alpha = alpha;
}

uint8_t Sprite::alpha() const
{
	return mColor.alpha;
}

void Sprite::color(Color color)
{
	mColor = color;
}

Color Sprite::color() const
{
	return mColor;
}


Sprite::AnimationCompleteSignal::Source& Sprite::animationCompleteSignalSource()
{
	return mAnimationCompleteSignal;
}
