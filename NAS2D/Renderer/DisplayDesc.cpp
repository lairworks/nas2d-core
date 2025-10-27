// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================

#include "DisplayDesc.h"


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

}
