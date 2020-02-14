#include "NAS2D/Renderer/DisplayDesc.h"

#include <utility>

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
		const auto adims = std::make_pair(a.width, a.height);
		const auto ap = std::make_pair(adims, a.refreshHz);
		const auto bdims = std::make_pair(b.width, b.height);
		const auto bp = std::make_pair(bdims, b.refreshHz);
		return ap < bp;
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
