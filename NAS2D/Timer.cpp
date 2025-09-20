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

#include <SDL2/SDL.h>


using namespace NAS2D;


uint32_t Timer::tick()
{
	return SDL_GetTicks();
}


Timer::Timer() :
	Timer{Timer::tick()}
{}


Timer::Timer(uint32_t startTick) :
	mStartTick{startTick}
{}


uint32_t Timer::elapsedTicks() const
{
	return tick() - mStartTick;
}


uint32_t Timer::delta()
{
	const auto elapsed = elapsedTicks();
	adjustStartTick(elapsed);
	return elapsed;
}


void Timer::adjustStartTick(uint32_t ticksForward)
{
	mStartTick += ticksForward;
}


void Timer::reset()
{
	mStartTick = tick();
}
