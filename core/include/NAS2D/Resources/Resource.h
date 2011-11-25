// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2011 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
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

	Resource(const std::string& filePath);
	Resource();
	virtual ~Resource();

	const std::string& name() const;

	const std::string& errorMessage() const;

	bool loaded() const;

	int id() const;

protected:
	void errorMessage(const std::string& message);
	void loaded(bool loaded);

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

	std::string mResourceName;		/**< File path and internal identifier. */
	std::string mErrorDescription;	/**< Internal Error Message. */
	
	bool mIsLoaded;					/**< Flag indicating whether or not this Resource has loaded properly. */

	static MersenneTwister mMt;

	int mId;
};

#endif


