// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2017 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/FpsCounter.h"

#ifdef __APPLE__
#include <SDL2/SDL.h>
#elif __linux__
#include "SDL2/SDL.h"
#else
#include "SDL.h"
#endif

using namespace NAS2D;

const unsigned int FPS_COUNTS_SIZE = 10;
unsigned int FPS_COUNTS[FPS_COUNTS_SIZE] = { 0 };

/**
 * Basic Constructor
 */
FpsCounter::FpsCounter():	mCurrentTick(0),
							mLastTick(0),
							mDelta(0),
							mArrayIndex(0),
							mAccumulator(0)
{}


/**
 * Returns an approximate FPS value.
 */
unsigned int FpsCounter::fps()
{
	mLastTick = mCurrentTick;
	mCurrentTick = SDL_GetTicks();

	mDelta = mCurrentTick - mLastTick;
	
	// Avoid a division by 0
	if(mDelta == 0)
		mDelta = 1;

	// Using a modulus here to eliminate maintenance overhead
	FPS_COUNTS[mArrayIndex] = 1000 / mDelta;
	mArrayIndex++;
	if(mArrayIndex >= FPS_COUNTS_SIZE)
		mArrayIndex = 0;

	mAccumulator = 0;
	for(unsigned int i = 0; i < FPS_COUNTS_SIZE; i++)
		mAccumulator += FPS_COUNTS[i];

	return mAccumulator / FPS_COUNTS_SIZE;
}
