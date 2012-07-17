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
Timer::Timer():	mCurrentTick(0),
				mLastTick(0),
				mAccumulator(0)
{
}

/**
 * Gets the current tick.
 */
unsigned int Timer::tick() const
{
	return SDL_GetTicks();
}


/**
 * Gets the difference in time since the last call to delta().
 */
unsigned int Timer::delta()
{
	mLastTick = mCurrentTick;
	mCurrentTick = SDL_GetTicks();

	return mCurrentTick - mLastTick;
}


/**
 * Updates the Accumulator value.
 * 
 * \return	Returns an accumulator value.
 */
unsigned int Timer::accumulator()
{
	mAccumulator += delta();

	return mAccumulator;
}


/**
 * Adjusts the Accumulator value by a given amount.
 * 
 * \param	a	Amount to adjust the Accumulator by.
 */
void Timer::adjust_accumulator(unsigned int a)
{
	mAccumulator -= a;
}


/**
 * Resets the accumulator.
 */
void Timer::reset()
{
	mAccumulator = 0;
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