#pragma once

#include "../StringTo.h"

#include <string_view>


namespace NAS2D
{
	class ReaderScalar
	{
	public:
		explicit ReaderScalar(std::string_view value);

		template<typename Type>
		Type read() const
		{
			return stringTo<Type>(mValue);
		}

	private:
		std::string_view mValue;
	};
}
