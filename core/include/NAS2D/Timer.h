// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2012 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#ifndef __NAS_TIMER__
#define __NAS_TIMER__


/**
 * \class Timer
 * \brief Convenience class to return high-resolution timer values.
 *
 * Provides functions to get system ticks in Miliseconds or in Seconds.
 *
 * \todo	Should we add in functions to calculate delta times for framerate
 *			independant animation or is that too specific for what this class
 *			should be used for?
 */
class Timer
{
public:
	Timer();

	unsigned int ms() const;
	unsigned int s() const;

	unsigned int delta() const;

private:
	mutable unsigned int	mLastTick;
};


#endif