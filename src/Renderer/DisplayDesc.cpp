#include "NAS2D/Renderer/DisplayDesc.h"


namespace NAS2D
{

	DisplayDesc::operator std::string() const
	{
		return std::to_string(width) + 'x' + std::to_string(height) + 'x' + std::to_string(refreshHz);
	}


	bool operator==(const DisplayDesc& a, const DisplayDesc& b)
	{
		return a.width == b.width && a.height == b.height && a.refreshHz == b.refreshHz;
	}

	bool operator!=(const DisplayDesc& a, const DisplayDesc& b)
	{
		return !(a == b);
	}


	bool operator<=(const DisplayDesc& a, const DisplayDesc& b)
	{
		return (a.width <= b.width) && (a.height <= b.height) && (a.refreshHz <= b.refreshHz);
	}

	bool operator>=(const DisplayDesc& a, const DisplayDesc& b)
	{
		return (b <= a);
	}

	bool operator<(const DisplayDesc& a, const DisplayDesc& b)
	{
		return (a <= b) && (a != b);
	}

	bool operator>(const DisplayDesc& a, const DisplayDesc& b)
	{
		return (b < a);
	}

} // namespace NAS2D
