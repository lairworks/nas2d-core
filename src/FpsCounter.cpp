// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/FpsCounter.h"

#include <SDL2/SDL.h>

using namespace NAS2D;


/**
 * Gets an average count of Frames per Second.
 */
unsigned int FpsCounter::fps()
{
	auto lastTick = currentTick;
	currentTick = SDL_GetTicks();

	auto tickDelta = currentTick - lastTick;

	if (tickDelta == 0) { tickDelta = 1; }

	fpsCounts[++fpsCountIndex] = 1000 / tickDelta;

	if (fpsCountIndex >= FpsCountsSize) { fpsCountIndex = 0; }

	unsigned int accumulator = 0;
	for (auto i : fpsCounts)
	{
		accumulator += i;
	}

	return accumulator / FpsCountsSize;
}
