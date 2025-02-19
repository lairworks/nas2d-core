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
	constexpr int VersionMajor = 1;
	constexpr int VersionMinor = 4;
	constexpr int VersionPatch = 2;
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
		return VersionMajor;
	}

	constexpr int versionMinor()
	{
		return VersionMinor;
	}

	constexpr int versionPatch()
	{
		return VersionPatch;
	}
} // namespace NAS2D
