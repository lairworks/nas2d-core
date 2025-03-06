#pragma once


namespace NAS2D
{
	// Smaller values would be promoted to at least an int during mathematical operations
	// Larger values could wrap around to 0 if coerced and truncated so handle them specially
	// Floats can produce warnings if compared directly so use their classification instead

	constexpr bool isZero(int value) { return value == 0; }
	constexpr bool isZero(long int value) { return value == 0; }
	constexpr bool isZero(long long int value) { return value == 0; }

	constexpr bool isZero(unsigned int value) { return value == 0; }
	constexpr bool isZero(unsigned long int value) { return value == 0; }
	constexpr bool isZero(unsigned long long int value) { return value == 0; }

	bool isZero(float value);
	bool isZero(double value);
	bool isZero(long double value);


	// Allow throwing specific exceptions without full include of `<stdexcept>`
	[[noreturn]] void throwDomainError(const char* errorMessage);


	template <typename NumericType>
	constexpr void verifyNotZero(NumericType value, const char* errorMessage)
	{
		if (isZero(value))
		{
			throwDomainError(errorMessage);
		}
	}


	template <typename NumericType>
	constexpr void verifyNotZero(NumericType value1, NumericType value2, const char* errorMessage)
	{
		if (isZero(value1) || isZero(value2))
		{
			throwDomainError(errorMessage);
		}
	}
}
