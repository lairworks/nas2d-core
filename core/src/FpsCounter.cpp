// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2012 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/FpsCounter.h"


/**
 * Basic Constructor
 */
FpsCounter::FpsCounter():	mCurrentTick(0),
							mLastTick(0),
							mDelta(0),
							mLastDelta(0)
{}


/**
 * Returns an approximate FPS value.
 */
unsigned int FpsCounter::fps()
{
	mCurrentTick = SDL_GetTicks();

	unsigned int d = mCurrentTick - mLastTick;
	mLastTick = mCurrentTick;

	mDelta = d * 0.05 + mLastDelta * 0.95;
	mLastDelta = d;
	
	// Avoid a division by 0
	if(mDelta == 0)
		return 0;

	return 1000 / mDelta;
}
