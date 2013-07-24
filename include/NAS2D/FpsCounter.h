// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2013 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#ifndef _LOM_FPS_COUNTER_
#define _LOM_FPS_COUNTER_

#include <vector>

#include "Common.h"

/**
 * \class FpsCounter
 * \brief Implements a basic FPS Counter.
 * 
 * FPS values are only approximates. As the FPS count gets higher, the
 * returned value becomes a more average count.
 */
class FpsCounter
{
public:
	FpsCounter();

	unsigned int fps();

private:

	unsigned int	mCurrentTick;
	unsigned int	mLastTick;
	unsigned int	mDelta;

	unsigned int	mArrayIndex;
	unsigned int	mAccumulator;
};

#endif