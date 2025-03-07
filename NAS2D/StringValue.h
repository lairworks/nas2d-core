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

#include "StringFrom.h"
#include "StringTo.h"


namespace NAS2D
{
	struct StringValue
	{
		std::string value{};


		StringValue() = default;

		template <typename T>
		StringValue(T newValue) :
			value{stringFrom<T>(newValue)}
		{}

		bool operator==(const StringValue& other) const { return value == other.value; }
		bool operator!=(const StringValue& other) const { return !(*this == other); }

		template <typename T>
		operator T() const { return stringTo<T>(value); }

		template <typename T>
		T to() const { return stringTo<T>(value); }

		template <typename T>
		StringValue& from(T newValue)
		{
			value = stringFrom<T>(newValue);
			return *this;
		}
	};
}
