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
 * \brief Implements a basic timer class getting.
 */
class Timer
{
public:
	Timer();
	
	unsigned int tick() const;
	unsigned int delta();

	unsigned int accumulator();
	void adjust_accumulator(unsigned int a);

	void reset();

private:
	
	unsigned int	mCurrentTick;
	unsigned int	mLastTick;

	unsigned int	mAccumulator;
};


#endif