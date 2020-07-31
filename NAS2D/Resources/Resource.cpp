// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "Resource.h"

using namespace NAS2D;


/**
 * C'tor.
 *
 * \param filePath Sets the name of the Resource to \c filePath.
 */
Resource::Resource(const std::string& filePath) :
	mResourceName(filePath)
{}


/**
 * D'tor
 */
Resource::~Resource()
{}


/**
 * Gets the name of the Resource as a file name with directory path.
 */
const std::string& Resource::name() const
{
	return mResourceName;
}


/**
 * Returns true if this Resource loaded properly.
 */
bool Resource::loaded() const
{
	return mIsLoaded;
}
