#include "IsZero.h"

#include <cmath>
#include <stdexcept>


namespace NAS2D
{
	bool isZero(float value)
	{
		return std::fpclassify(value) == FP_ZERO;
	}


	bool isZero(double value)
	{
		return std::fpclassify(value) == FP_ZERO;
	}


	bool isZero(long double value)
	{
		return std::fpclassify(value) == FP_ZERO;
	}


	// Defined in source file to limit include of `<stdexcept>`
	void throwDomainError(const char* errorMessage)
	{
		throw std::domain_error(errorMessage);
	}
}
