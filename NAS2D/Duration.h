#pragma once

#include <cstdint>


namespace NAS2D
{
	struct Duration
	{
		uint32_t milliseconds;
	};


	inline bool operator==(Duration duration1, Duration duration2) { return duration1.milliseconds == duration2.milliseconds; }
	inline bool operator!=(Duration duration1, Duration duration2) { return duration1.milliseconds != duration2.milliseconds; }

	inline bool operator<(Duration duration1, Duration duration2) { return duration1.milliseconds < duration2.milliseconds; }
	inline bool operator>(Duration duration1, Duration duration2) { return duration1.milliseconds > duration2.milliseconds; }
	inline bool operator<=(Duration duration1, Duration duration2) { return duration1.milliseconds <= duration2.milliseconds; }
	inline bool operator>=(Duration duration1, Duration duration2) { return duration1.milliseconds >= duration2.milliseconds; }
}
