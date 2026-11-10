#pragma once

#include "../StringTo.h"

#include <string_view>


namespace NAS2D
{
	class ReaderObject;


	class ReaderScalar
	{
	public:
		template<typename Type>
		Type read() const
		{
			return stringTo<Type>(mValue);
		}

	protected:
		friend class ReaderObject;

		explicit ReaderScalar(std::string_view value);

	private:
		std::string_view mValue;
	};
}
