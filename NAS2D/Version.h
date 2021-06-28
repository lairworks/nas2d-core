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

	constexpr int NAS2D_MAJOR_VERSION = 1;
	constexpr int NAS2D_MINOR_VERSION = 4;
	constexpr int NAS2D_PATCH_VERSION = 2;

	const std::string versionString();
	constexpr int versionMajor();
	constexpr int versionMinor();
	constexpr int versionPatch();

} // namespace NAS2D
