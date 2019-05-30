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

#pragma message("Filesystem Class is deprecated and wil be removed in a future release.")
#include "StringUtils.h"
#include "File.h"
#include <string>

namespace NAS2D {

/**
 * \class Filesystem
 * \brief Implements a virtual file system.
 *
 * Provides cross-platform and transparent archive Filesystem functions.
 */
class Filesystem
{
public:
	Filesystem();
	~Filesystem();

	void init(const std::string& argv_0, const std::string& appName, const std::string& organizationName, const std::string& dataPath);

	std::string dataPath() const;
	std::string workingPath(const std::string& filename) const;
	StringList searchPath() const;
	bool mount(const std::string& path) const;

	StringList directoryList(const std::string& dir, const std::string& filter = "") const;

	File open(const std::string& filename) const;
	bool write(const File& file, bool overwrite = true) const;
	bool del(const std::string& path) const;
	bool exists(const std::string& filename) const;

	std::string extension(const std::string& path);

	bool isDirectory(const std::string& path) const;
	bool makeDirectory(const std::string& path) const;

	void toggleVerbose() const;

private:
	Filesystem(const Filesystem&) = delete;
	Filesystem& operator= (const Filesystem&) = delete;

	bool closeFile(void *file) const;
	const char* getLastPhysfsError() const;

private:
	std::string			mDataPath;			/**< Data path string. This will typically be 'data/'. */
	mutable bool		mVerbose;			/**< Displays lots of messages when true. Otherwise only critical messages are displayed. */
};

}
