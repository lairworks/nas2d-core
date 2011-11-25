// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2011 New Age Software
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
 * Gets the current tick count in miliseconds.
 */
unsigned int Timer::ms() const
{
	mLastTick = SDL_GetTicks();
	return mLastTick;
}


/**
 * Gets the current tick count in seconds.
 */
unsigned int Timer::s() const
{
	mLastTick = SDL_GetTicks();
	return mLastTick / 1000;
}


/**
 * Gets the change in time since the last update.
 */
unsigned int Timer::delta() const
{
	int tick = SDL_GetTicks();
	int delta = tick - mLastTick;

	if(delta < 0)
		delta = 0;

	mLastTick = tick;

	return delta;
}