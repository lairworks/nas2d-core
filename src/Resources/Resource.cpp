// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2017 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Resources/Resource.h"

using namespace std;
using namespace NAS2D;

MersenneTwister Resource::mMt = MersenneTwister();


Resource::Resource(const std::string& filePath):	mResourceName(filePath),
													mIsLoaded(false),
													mId(mMt.random_int())
									
{}

Resource::Resource():	mResourceName("Default Resource"),
						mIsLoaded(false),
						mId(mMt.random_int())
{}


Resource::~Resource()
{}


/**
 * Returns the name of the Resource as a file name with directory path.
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


/**
 * Gets the resource Id.
 */
int Resource::id() const
{
	return mId;
}
