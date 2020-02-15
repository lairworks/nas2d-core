#include "NAS2D/Renderer/DisplayDesc.h"

#include <tuple>

namespace NAS2D
{

	bool operator==(const DisplayDesc& a, const DisplayDesc& b)
	{
		return a.width == b.width && a.height == b.height && a.refreshHz == b.refreshHz;
	}

	bool operator!=(const DisplayDesc& a, const DisplayDesc& b)
	{
		return !(a == b);
	}


	bool operator<(const DisplayDesc& a, const DisplayDesc& b)
	{
		const auto a_tuple = std::make_tuple(a.width, a.height, a.refreshHz);
		const auto b_tuple = std::make_tuple(b.width, b.height, b.refreshHz);
		return a_tuple < b_tuple;
	}

	bool operator>(const DisplayDesc& a, const DisplayDesc& b)
	{
		return b < a;
	}

	bool operator<=(const DisplayDesc& a, const DisplayDesc& b)
	{
		return !(a > b);
	}

	bool operator>=(const DisplayDesc& a, const DisplayDesc& b)
	{
		return !(a < b);
	}

} // namespace NAS2D
