#pragma once

#include "Types.h"

#include <vector>


namespace NAS2D
{
	template<typename ElementType>
	std::vector<ElementType> read(ReaderArray& reader)
	{
		std::vector<ElementType> collection;
		while(reader.hasNext())
		{
			collection.push_back(reader.read<ElementType>());
		}
		return collection;
	}
}
