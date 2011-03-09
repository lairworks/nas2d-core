// ==================================================================================
// = NAS2D
// = Copyright © 2008 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software as long under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#ifndef _NAS_RESOURCE_
#define _NAS_RESOURCE_

#include "NAS2D/Common.h"
#include "NAS2D/Filesystem.h"
#include "NAS2D/Singleton.h"

#include "NAS2D/MersenneTwister.h"

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

	Resource(const std::string& filePath):	mResourceName(filePath),
											mErrorDescription(""),
											mIsLoaded(false),
											mId(mMt.random_int())
										
	{}
	
	Resource():	mResourceName(""),
				mErrorDescription("Resource Default Construction."),
				mIsLoaded(false)
	{}

	virtual ~Resource() {};

	/**
	 * Returns the name of the Resource as a file name with directory path.
	 */
	const std::string& getName() const { return mResourceName; }

	/**
	 * Returns the last error message.
	 */
	const std::string& getErrorMessage() const { return mErrorDescription; }

	/**
	 * Returns true if this Resource loaded properly.
	 */
	bool isLoaded() const { return mIsLoaded; }

	int id() const { return mId; }

protected:
	std::string mResourceName;		/**< File path and internal identifier. */
	std::string mErrorDescription;	/**< Internal Error Message. */
	
	bool mIsLoaded;					/**< Flag indicating whether or not this Resource has loaded properly. */

private:
	/**
	 * Performs the necessary operations to load a Resource.
	 *
	 * \note	This is a pure virtual function and so must be
	 *			overridden by all derived classes.
	 *
	 * \note	It is the responsibility of the derived class to
	 *			call this function.
	 */
	virtual void load() = 0;

	static MersenneTwister mMt;

	int mId;
};

#endif


