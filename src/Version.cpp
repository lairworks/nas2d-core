// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================

#include "NAS2D/Version.h"

#include <sstream>

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
