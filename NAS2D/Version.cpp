// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================

#include "Version.h"

#include <sstream>


namespace
{
	constexpr int NAS2D_MAJOR_VERSION = 1;
	constexpr int NAS2D_MINOR_VERSION = 4;
	constexpr int NAS2D_PATCH_VERSION = 2;
}


namespace NAS2D
{
	const std::string versionString()
	{
		std::ostringstream ss;
		ss << versionMajor() << "." << versionMinor() << "." << versionPatch();
		return ss.str();
	}

	constexpr int versionMajor()
	{
		return NAS2D_MAJOR_VERSION;
	}

	constexpr int versionMinor()
	{
		return NAS2D_MINOR_VERSION;
	}

	constexpr int versionPatch()
	{
		return NAS2D_PATCH_VERSION;
	}
} // namespace NAS2D
