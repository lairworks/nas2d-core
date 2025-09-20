// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================

#include "Timer.h"

#include "Duration.h"

#include <SDL2/SDL.h>


using namespace NAS2D;


uint64_t Timer::tick()
{
	return SDL_GetTicks64();
}


Timer::Timer() :
	Timer{Timer::tick()}
{}


Timer::Timer(uint64_t startTick) :
	mStartTick{startTick}
{}


Duration Timer::elapsedTicks() const
{
	return {tick() - mStartTick};
}


Duration Timer::delta()
{
	const auto elapsed = elapsedTicks();
	adjustStartTick(elapsed);
	return elapsed;
}


void Timer::adjustStartTick(Duration ticksForward)
{
	mStartTick += ticksForward.milliseconds;
}


void Timer::reset()
{
	mStartTick = tick();
}
