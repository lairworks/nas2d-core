// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2012 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Timer.h"

#include "SDL/sdl.h"


/**
 * C'tor
 */
Timer::Timer(): mLastTick(0)
{
}


/**
 * Resets the timer.
 */
void Timer::reset()
{
	
}

/**
 * Gets the current tick count.
 */
int Timer::tick()
{
	mLastTick = SDL_GetTicks();
	return mLastTick;
}


/**
 * Gets the change in time since the last update.
 */
float Timer::delta()
{
	int tick = SDL_GetTicks();
	float delta = tick - mLastTick;
	
	if(delta < 0)
		delta = 0;
	
	mLastTick = tick;
	
	return delta;
}

unsigned int Timer::ms() const
{
	
}

unsigned int Timer::s() const
{
	
}

unsigned int Timer::delta() const
{
	
}