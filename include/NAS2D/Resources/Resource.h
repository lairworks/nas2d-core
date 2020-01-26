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
	Resource() = default;
	explicit Resource(const std::string& filePath);
	virtual ~Resource();

	const std::string& name() const;

	bool loaded() const;

protected:
	void loaded(bool loaded);
	void name(const std::string& name);

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

	std::string mResourceName{"Default Resource"};		/**< File path and internal identifier. */

	bool mIsLoaded{false};					/**< Flag indicating whether or not this Resource has loaded properly. */
};

} // namespace
