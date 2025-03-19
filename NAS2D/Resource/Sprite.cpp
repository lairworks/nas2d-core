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


Sprite::Sprite(const std::string& filePath, const std::string& initialAction, Delegate<void()> animationCompleteHandler) :
	Sprite(filePath, initialAction)
{
	mAnimationCompleteSignal.connect(animationCompleteHandler);
}


Sprite::Sprite(const AnimationSet& animationSet, const std::string& initialAction, Delegate<void()> animationCompleteHandler) :
	Sprite(animationSet, initialAction)
{
	mAnimationCompleteSignal.connect(animationCompleteHandler);
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
	mTimer.adjustStartTick(advanceByTimeDelta(mTimer.elapsedTicks()));
}


void Sprite::draw(Point<float> position) const
{
	const auto& frame = (*mCurrentAction)[mCurrentFrame];
	const auto drawPosition = position - frame.anchorOffset.to<float>();
	const auto frameBounds = frame.bounds.to<float>();
	Utility<Renderer>::get().drawSubImageRotated(frame.image, drawPosition, frameBounds, mRotationAngle, mTintColor);
}


/**
 * Sets the rotation angle of the Sprite.
 *
 * \param	angle	Angle of rotation.
 */
void Sprite::rotation(Angle angle)
{
	mRotationAngle = angle;
}


/**
 * Gets the rotation angle of the Sprite.
 *
 * \return	Angle of rotation.
 */
Angle Sprite::rotation() const
{
	return mRotationAngle;
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


Sprite::AnimationCompleteSignal::Source& Sprite::animationCompleteSignalSource()
{
	return mAnimationCompleteSignal;
}


unsigned int Sprite::advanceByTimeDelta(unsigned int timeDelta)
{
	unsigned int accumulator = 0;

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
			mAnimationCompleteSignal();
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
			mAnimationCompleteSignal();
		}
	}
}
