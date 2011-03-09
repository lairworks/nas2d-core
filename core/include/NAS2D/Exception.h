// ==================================================================================
// = NAS2D
// = Copyright © 2008 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software as long under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#ifndef _LOM_EXCEPTION_
#define _LOM_EXCEPTION_

#include <string>

using namespace std;

/**
 * \class Exception
 * \brief Thrown when Exceptions occur.
 *
 * Thrown when exception occur. Provides an error code and a description.
 */
class Exception
{
public:
	Exception(int code, const std::string& brief, const std::string& description):	mErrorBrief(brief),
																					mErrorDescription(description),
																					mErrorCode(code)
	{}

	
	/**
	 * Gets a brief description of the Exception.
	 */
	const std::string& getBriefDescription() const		{ return mErrorBrief; }

	/**
	 * Gets a full description of the Exception.
	 */
	const std::string& getDescription() const 			{ return mErrorBrief; }

	/**
	 * Gets an Exception ID.
	 */
	int getCode() const									{ return mErrorCode; }

private:
	std::string		mErrorBrief;			/**< Brief description of the error. */
	std::string		mErrorDescription;		/**< Full description of the Error. */
	int				mErrorCode;				/**< Error Code */
};

#endif
