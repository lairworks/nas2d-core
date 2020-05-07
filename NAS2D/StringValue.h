#pragma once

#include "StringUtils.h"


namespace NAS2D {
	struct StringValue
	{
		std::string value;


		StringValue() = default;
		template <typename T>
		explicit StringValue(T newValue) : value{stringFrom<T>(newValue)} {}

		template <typename T>
		operator T() const { return stringTo<T>(value); }

		template <typename T>
		T to() const { return stringTo<T>(value); }
		template <typename T>
		StringValue& from(T newValue) { value = stringFrom<T>(newValue); return *this; }
	};
}
