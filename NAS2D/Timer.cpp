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
	unsigned int mLastTick = mCurrentTick;
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
