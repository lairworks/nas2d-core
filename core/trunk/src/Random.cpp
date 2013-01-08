// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2012 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Random.h"

MersenneTwister Random::mMT;


/**
 * Gets an integer value between 0 and 2,147,483,647.
 */
int Random::integer()
{
	return mMT.random_int();
}


/**
 * Gets an integer between upper and lower bounds.
 * 
 * \param lower	Lower bounds of the range.
 * \param upper Highest number of the range.
 */
int Random::ranged_integer(int lower, int upper)
{
	return (mMT.random_int() % upper) + lower;
}


/**
 * Gets a floating point value between 0.0 and 1.0.
 */
double Random::floating_point()
{
	return mMT.random_double();
}

