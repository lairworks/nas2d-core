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

#include <string>

namespace NAS2D
{
	struct DisplayDesc
	{
		int width{0};
		int height{0};
		int refreshHz{0};

		operator std::string() const;
	};

	bool operator==(const DisplayDesc& a, const DisplayDesc& b);
	bool operator!=(const DisplayDesc& a, const DisplayDesc& b);

	bool operator<=(const DisplayDesc& a, const DisplayDesc& b);
	bool operator>=(const DisplayDesc& a, const DisplayDesc& b);
	bool operator<(const DisplayDesc& a, const DisplayDesc& b);
	bool operator>(const DisplayDesc& a, const DisplayDesc& b);

}
