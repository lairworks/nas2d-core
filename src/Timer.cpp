// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2017 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Timer.h"

#ifdef __APPLE__
#include <SDL2/SDL.h>
#elif __linux__
#include "SDL2/SDL.h"
#else
#include "SDL.h"
#endif

using namespace NAS2D;

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
 * Resets the accumulator and updates the tick counter.
 */
void Timer::reset()
{
	delta();
	mAccumulator = 0;
}
