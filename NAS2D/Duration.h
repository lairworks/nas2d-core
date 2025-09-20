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

	inline Duration& operator+=(Duration& duration1, Duration duration2) { duration1.milliseconds += duration2.milliseconds; return duration1; }
	inline Duration& operator-=(Duration& duration1, Duration duration2) { duration1.milliseconds -= duration2.milliseconds; return duration1; }

	inline Duration operator+(Duration duration1, Duration duration2) { return Duration{duration1.milliseconds + duration2.milliseconds}; }
	inline Duration operator-(Duration duration1, Duration duration2) { return Duration{duration1.milliseconds - duration2.milliseconds}; }

	inline Duration operator*(Duration duration, uint32_t scalar) { return Duration{duration.milliseconds * scalar}; }

	inline Duration operator/(Duration duration, uint32_t scalar) { return Duration{duration.milliseconds / scalar}; }
	inline uint32_t operator/(Duration duration, Duration duration2) { return duration.milliseconds / duration2.milliseconds; }

	inline Duration operator%(Duration duration, uint32_t scalar) { return Duration{duration.milliseconds % scalar}; }
}
