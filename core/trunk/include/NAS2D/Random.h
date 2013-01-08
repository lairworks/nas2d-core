// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2012 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#ifndef _NAS_RANDOM_GENERATOR_
#define _NAS_RANDOM_GENERATOR_

#include "MersenneTwister.h"


/**
 * \class	Random
 * \brief	Provides a clean, simple way to generate high quality
 *			psuedo-random numbers.
 *
 * Random is a static class and cannot be instantiated. Usage is
 * straight-forward; simply call one of the available functions
 * using the scope operator:
 * 
 * \code{.cpp}
 * Random::integer();					// Gets a number between 0 and 2,147,483,647.
 * Random::ranged_integer(10, 100);		// Gets a number between 10 and 100.
 * Random::floating_point();			// Gets a number between 0.0f and 1.0f.
 * \endcode
 */
class Random
{
public:

	static int integer();
	static int ranged_integer(int lower, int upper);

	static double floating_point();

private:
	Random() {}
	Random(const Random&) {}
	const Random& operator=(const Random&) { return *this; }

	static MersenneTwister		mMT;
};

#endif