#include "AnimatedImage.h"

#include "AnimationSequence.h"
#include "AnimationFrame.h"
#include "../Math/Point.h"
#include "../Math/Angle.h"
#include "../Renderer/Color.h"
#include "../Renderer/Renderer.h"

#include <stdexcept>


using namespace NAS2D;


AnimatedImage::AnimatedImage(const AnimationSequence& animationSequence) :
	mAnimationSequence{&animationSequence},
	mFrameIndex{0}
{
	if (mAnimationSequence->empty())
	{
		throw std::runtime_error("AnimatedImage initialized with empty AnimationSequence");
	}
}


const AnimationSequence& AnimatedImage::sequence() const
{
	return *mAnimationSequence;
}


const AnimationFrame& AnimatedImage::frame() const
{
	return mAnimationSequence->frame(mFrameIndex);
}


std::size_t AnimatedImage::frameCount() const
{
	return mAnimationSequence->frameCount();
}


std::size_t AnimatedImage::frameIndex() const
{
	return mFrameIndex;
}


void AnimatedImage::setFrame(std::size_t frameIndex)
{
	mFrameIndex = frameIndex % mAnimationSequence->frameCount();
}


void AnimatedImage::advanceFrame()
{
	if (mAnimationSequence->frame(mFrameIndex).isStopFrame()) { return; }

	++mFrameIndex;
	if (mFrameIndex >= mAnimationSequence->frameCount())
	{
		mFrameIndex = 0;
	}
}


void AnimatedImage::draw(Renderer& renderer, Point<int> position) const
{
	draw(renderer, position, Color::White);
}


void AnimatedImage::draw(Renderer& renderer, Point<int> position, Color tintColor) const
{
	const auto& frame = mAnimationSequence->frame(mFrameIndex);
	const auto drawPosition = position - frame.anchorOffset;
	const auto frameBounds = frame.bounds;
	renderer.drawSubImage(frame.image, drawPosition, frameBounds, tintColor);
}


void AnimatedImage::draw(Renderer& renderer, Point<int> position, Color tintColor, Angle rotation) const
{
	const auto& frame = mAnimationSequence->frame(mFrameIndex);
	const auto drawPosition = position - frame.anchorOffset;
	const auto frameBounds = frame.bounds;
	renderer.drawSubImageRotated(frame.image, drawPosition, frameBounds, rotation, tintColor);
}
