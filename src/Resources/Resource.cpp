// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Resources/Resource.h"

using namespace NAS2D;

Resource::~Resource()
{
	/* DO NOTHING */
}

/**
 * C'tor.
 *
 * \param filePath Sets the name of the Resource to \c filePath.
 */
Resource::Resource(const std::string& filePath) :
	mResourceName(filePath)
{}

/**
 * Gets the name of the Resource as a file name with directory path.
 */
const std::string& Resource::name() const
{
	return mResourceName;
}


/**
 * Sets the name of the resource.
 */
void Resource::name(const std::string& name)
{
	mResourceName = name;
}


/**
 * Returns true if this Resource loaded properly.
 */
bool Resource::loaded() const
{
	return mIsLoaded;
}


/**
 * Sets whether or not the Resource is loaded.
 *
 * \note	This is an internal function only and is provided for use
 *			by derived Resource types.
 *
 * \param	loaded	Boolean value.
 */
void Resource::loaded(bool loaded)
{
	mIsLoaded = loaded;
}
