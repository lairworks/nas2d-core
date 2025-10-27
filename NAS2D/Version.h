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
	/**
	* Gets a string containing the version of NAS2D being used.
	*/
	const std::string versionString();

	/**
	* Gets version major.
	*/
	constexpr int versionMajor();

	/**
	* Gets version minor.
	*/
	constexpr int versionMinor();

	/**
	* Gets version patch.
	*/
	constexpr int versionPatch();
}
