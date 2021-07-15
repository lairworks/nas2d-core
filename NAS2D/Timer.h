// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================

#pragma once

namespace NAS2D
{

	/**
	 * A timing class that provides high-resolution, millisecond-precision timing services.
	 *
	 * The Timer class provides three different method for getting and managing timing.
	 *
	 * \section Raw Tick
	 *
	 * By far the simplest method, raw ticks provide the current tick in milliseconds since the application
	 * started. You can get this value by calling the function <tt>unsigned int tick() const</tt>.
	 *
	 * As a note, according to SDL documentation, values will roll back over to 0 if the application runs
	 * for more than 49 days. In practice this should cause no noticable side effects (as all values are
	 * in <tt>unsigned int</tt>'s).
	 *
	 * \section Delta Change
	 *
	 * This is a basic method of getting delta times, or the difference in time from the last time <tt>delta()</tt>
	 * was called to the current call.
	 *
	 * This is the simplest timing method in that physics simulations can use a raw delta value like this
	 * to determine how far to advance the simulation, etc.
	 *
	 * \section Accumulators
	 *
	 * The third and final method for handling timing is through the use of an Accumulator. In essence, an accumulator
	 * is just what it sounds like. It accumulates the elapsed time every time it is updated. The accumulator will continue
	 * to report the current accumulated time in milliseconds since the Timer was created or since the last call to
	 * <tt>void reset()</tt>.
	 *
	 * Additionally, the accumulator provides a method to adjust the accumulated time. Calling <tt>void adjust_accumulator(unsigned int a)</tt>
	 * will adjust the accumulated time counter by the amount indicated by \c 'a'. E.g., a positive value will adjust the
	 * accumulator forward vs. a negative value which will adjust it backward.
	 *
	 * Using accumulators are very useful when it comes to timing things like animation accurately to account for
	 * jumps and gaps in time, for instance if the application stalls for a few seconds.
	 *
	 * Accumulators are used internally in the Sprite animation handling to account for time desynchs between frames. As
	 * time passes the frame counter loses accuracy. Using an accumulator corrects for these inaccuracies by skipping
	 * frames whenever needed.
	 */
	class Timer
	{
	public:
		Timer() = default;

		unsigned int tick() const;
		unsigned int delta();

		unsigned int accumulator();
		void adjust_accumulator(unsigned int a);

		void reset();

	private:
		unsigned int mCurrentTick = 0;
		unsigned int mAccumulator = 0;
	};

} // namespace
