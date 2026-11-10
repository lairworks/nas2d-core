#pragma once

#include "Types.h"

#include <vector>


namespace NAS2D
{
	template<typename ElementType>
	void write(WriterArray& writer, const std::vector<ElementType>& collection)
	{
		for (const auto& element : collection)
		{
			writer.write(element);
		}
	}
}
