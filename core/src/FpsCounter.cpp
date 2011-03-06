// ==================================================================================
// = NAS2D
// = Copyright © 2008 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software as long under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/FpsCounter.h"

const size_t FRAME_VALUES = 32;		/**
									 * Defines the number of values to store when calculating
									 * FPS. The higher the number, the smoother the change in
									 * FPS count.
									 *	 
									 * \todo	Make this a configurable paramter?
									 */


/**
 * Basic Constructor
 */
FpsCounter::FpsCounter() :	mFrameLastTick(SDL_GetTicks()),
							mFrameCount(0)
{
	mFrameTimes.resize(FRAME_VALUES);
}


/**
 * Returns an approximate FPS value.
 */
unsigned int FpsCounter::getFps()
{
	return update();
}


/**
 * Calculate FPS.
 *
 * Calculates an approximate FPS value and returns it as an integer.
 */
inline int FpsCounter::update()
{
	Uint32 frameTimesIndex;		// Index in frame value array.
	frameTimesIndex = mFrameCount % FRAME_VALUES;

	Uint32 currentTick;
	currentTick = SDL_GetTicks();

	// Store Value in array
	mFrameTimes.at(frameTimesIndex) = currentTick - mFrameLastTick;
	mFrameLastTick = currentTick;
	mFrameCount++;
	
	// Get an average of all values.
	int framesPerSecond = 0;
	for(size_t i = 0; i < FRAME_VALUES; i++)
		framesPerSecond += mFrameTimes[i];

	framesPerSecond = framesPerSecond / FRAME_VALUES;

	// Calculate FPS value.
	/// \todo Document magic number.
	if(framesPerSecond != 0)
		framesPerSecond = 1000 / framesPerSecond;

	return framesPerSecond;
}