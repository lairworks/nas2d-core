#pragma once

namespace NAS2D
{
	//A structure represents a display description.
	struct DisplayDesc
	{
		int width{0};
		int height{0};
		int refreshHz{0};
	};

	bool operator==(const DisplayDesc& a, const DisplayDesc& b);
	bool operator!=(const DisplayDesc& a, const DisplayDesc& b);

	bool operator<(const DisplayDesc& a, const DisplayDesc& b);
	bool operator>(const DisplayDesc& a, const DisplayDesc& b);
	bool operator<=(const DisplayDesc& a, const DisplayDesc& b);
	bool operator>=(const DisplayDesc& a, const DisplayDesc& b);

} // namespace NAS2D
