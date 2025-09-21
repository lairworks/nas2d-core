#include "AnimationSequence.h"

#include "AnimationFrame.h"

#include <utility>


using namespace NAS2D;


AnimationSequence::AnimationSequence(std::vector<AnimationFrame> frames) :
	mFrames{std::move(frames)}
{
}


bool AnimationSequence::empty() const
{
	return mFrames.empty();
}


std::size_t AnimationSequence::frameCount() const
{
	return mFrames.size();
}


const AnimationFrame& AnimationSequence::frame(std::size_t index) const
{
	return mFrames.at(index);
}
