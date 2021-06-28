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

namespace NAS2D {

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
