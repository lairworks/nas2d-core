// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2012 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#ifndef _LOM_FPS_COUNTER_
#define _LOM_FPS_COUNTER_

#include <vector>

#include "SDL/SDL.h"
#include "Common.h"

/**
 * \class FpsCounter
 * \brief Frames Per Second Class
 */
class FpsCounter
{
public:
	FpsCounter();

	unsigned int getFps();

private:
	inline int update();

	std::vector<unsigned int>	mFrameTimes;	/**< An array to store frame times. */
	unsigned int				mFrameLastTick;	/**< Difference between the current frame and the last frame. */
	unsigned int				mFrameCount;	/**< Frame Count. */
};

#endif