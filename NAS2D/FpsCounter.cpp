// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================

#include "FpsCounter.h"

#include <SDL2/SDL.h>

#include <numeric>
#include <iterator>
#include <algorithm>


using namespace NAS2D;


/**
 * Gets an average count of Frames per Second.
 */
unsigned int FpsCounter::fps()
{
	const auto lastTick = currentTick;
	currentTick = SDL_GetTicks();

	const auto tickDelta = std::max(currentTick - lastTick, 1u);

	fpsCounts[fpsCountIndex] = 1000 / tickDelta;

	++fpsCountIndex;
	if (fpsCountIndex >= FpsCountsSize) { fpsCountIndex = 0; }

	const auto sum = std::accumulate(std::begin(fpsCounts), std::end(fpsCounts), 0u);
	return sum / FpsCountsSize;
}
