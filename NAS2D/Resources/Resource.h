// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#pragma once

#include <string>

namespace NAS2D {

/**
 * \class	Resource
 * \brief	A basic Resource.
 *
 *  A base Resource class from which all other resources are derived. Provides the
 *  basic framework for Resources to load and initialize themselves.
 */
class Resource
{
public:
	explicit Resource(const std::string& filePath);
};

} // namespace
