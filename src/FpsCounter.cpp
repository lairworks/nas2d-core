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

#include <SDL.h>

using namespace NAS2D;

const unsigned int FPS_COUNTS_SIZE = 25;
unsigned int FPS_COUNTS[FPS_COUNTS_SIZE] = { 0 };

unsigned int	CURRENT_TICK		= 0;
unsigned int	LAST_TICK			= 0;
unsigned int	TICK_DELTA			= 0;
unsigned int	INDEX				= 0;
unsigned int	ACCUMULATOR			= 0;


/**
 * FpsCounter c'tor
 */
FpsCounter::FpsCounter()
{}


/**
 * Gets an average count of Frames per Second.
 */
unsigned int FpsCounter::fps()
{
	LAST_TICK = CURRENT_TICK;
	CURRENT_TICK = SDL_GetTicks();

	TICK_DELTA = CURRENT_TICK - LAST_TICK;

	if (TICK_DELTA == 0) { TICK_DELTA = 1; }

	FPS_COUNTS[++INDEX] = 1000 / TICK_DELTA;

	if (INDEX >= FPS_COUNTS_SIZE) { INDEX = 0; }

	ACCUMULATOR = 0;
	for (auto i : FPS_COUNTS)
	{
		ACCUMULATOR += i;
	}

	return ACCUMULATOR / FPS_COUNTS_SIZE;
}
