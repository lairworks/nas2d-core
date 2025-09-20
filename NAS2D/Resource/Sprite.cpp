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
#include "../Math/Angle.h"
#include "../Renderer/Renderer.h"
#include "../Utility.h"

#include <utility>
#include <stdexcept>


using namespace NAS2D;


namespace
{
	using AnimationCache = ResourceCache<AnimationSet, std::string>;
	AnimationCache animationCache;
}


Sprite::Sprite(const std::string& filePath, const std::string& initialAction) :
	mAnimationSet{animationCache.load(filePath)},
	mCurrentAction{&mAnimationSet.frames(initialAction)}
{
}


Sprite::Sprite(const AnimationSet& animationSet, const std::string& initialAction) :
	mAnimationSet{animationSet},
	mCurrentAction{&mAnimationSet.frames(initialAction)}
{
}


Vector<int> Sprite::size() const
{
	return (*mCurrentAction)[mCurrentFrame].bounds.size;
}


Point<int> Sprite::origin(Point<int> point) const
{
	return point - (*mCurrentAction)[mCurrentFrame].anchorOffset;
}


/**
 * Gets a list of Actions available for the Sprite.
 *
 * \return	std::vector<std::string> of actions.
 */
std::vector<std::string> Sprite::actions() const
{
	return mAnimationSet.actionNames();
}


/**
 * Plays an action animation.
 *
 * \param	action	Name of the action to use for animation. Actions are not
 *			case sensitive. "Case", "caSe", "CASE", etc. will all be viewed
 *			as identical.
 *
 * \note	If the named action doesn't exist, a warning message will
 *			be written to the log and the default action will be used
 *			instead.
 */
void Sprite::play(const std::string& action)
{
	mCurrentAction = &mAnimationSet.frames(action);
	mCurrentFrame = 0;
	mTimer.reset();
	resume();
}


/**
 * Pauses animation for this Sprite.
 */
void Sprite::pause()
{
	mPaused = true;
}


/**
 * Resumes the action of the Sprite.
 */
void Sprite::resume()
{
	mPaused = false;
}


bool Sprite::isPaused() const
{
	return mPaused || (*mCurrentAction)[mCurrentFrame].isStopFrame();
}


/**
 * Sets the animation playback frame.
 *
 * \param	frameIndex	New frame index
 */
void Sprite::setFrame(std::size_t frameIndex)
{
	mCurrentFrame = frameIndex % mCurrentAction->size();
}


void Sprite::update()
{
	mTimer.adjustStartTick(advanceByTimeDelta(Duration{mTimer.elapsedTicks()}).milliseconds);
}


void Sprite::draw(Point<float> position) const
{
	const auto& frame = (*mCurrentAction)[mCurrentFrame];
	const auto drawPosition = position - frame.anchorOffset.to<float>();
	const auto frameBounds = frame.bounds.to<float>();
	Utility<Renderer>::get().drawSubImage(frame.image, drawPosition, frameBounds, mTintColor);
}


void Sprite::draw(Point<float> position, Angle rotation) const
{
	const auto& frame = (*mCurrentAction)[mCurrentFrame];
	const auto drawPosition = position - frame.anchorOffset.to<float>();
	const auto frameBounds = frame.bounds.to<float>();
	Utility<Renderer>::get().drawSubImageRotated(frame.image, drawPosition, frameBounds, rotation, mTintColor);
}


/**
 * Sets the alpha value for the Sprite.
 *
 * \param	alpha	Alpha value to set between 0 - 255.
 */
void Sprite::alpha(uint8_t alpha)
{
	mTintColor.alpha = alpha;
}


/**
 * Gets the alpha value for the Sprite.
 */
uint8_t Sprite::alpha() const
{
	return mTintColor.alpha;
}


/**
 * Sets the color tint of the Sprite.
 */
void Sprite::color(Color color)
{
	mTintColor = color;
}


/**
 * Gets the color tint of the Sprite.
 */
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
		const auto& frame = frames[mCurrentFrame];

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
		if (mCurrentFrame >= frames.size())
		{
			mCurrentFrame = 0;
		}
	}
}
