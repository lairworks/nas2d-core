// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2017 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#pragma once

#include "Common.h"

#if defined(__linux__)
#include "physfs.h"
#elif __APPLE__
#include "physfs.h"
#else
#include "physfs.h"
#endif

#ifdef __linux__
#include <cstring>
#endif

#include <string>
#include <vector>
#include <iomanip>
#include <iostream>

#include "File.h"

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

	void init(const std::string& argv_0, const std::string& startPath);

	std::string userPath() const;
	std::string dataPath() const;
	std::string workingPath(const std::string& filename) const;
	StringList searchPath() const;
	bool addToSearchPath(const std::string& path) const;

	StringList directoryList(const std::string& dir) const;
	StringList directoryList(const std::string& dir, const std::string& filter) const;

	File open(const std::string& filename) const;
	bool write(const File& file, bool overwrite = true) const;
	bool del(const std::string& path) const;
	bool exists(const std::string& filename) const;

	std::string extension(const std::string path);

	bool isDirectory(const std::string& path) const;
	bool makeDirectory(const std::string& path) const;


	void toggleVerbose() const;

private:
	Filesystem(const Filesystem&);				// Intentionally left undefined.
	Filesystem& operator= (const Filesystem&);	// Intentionally left undefined.

	bool closeFile(PHYSFS_File *file) const;

private:
	std::string			mDataPath;			/**< Data path string. Specific to each platform. */
	std::string			mStartPath;			/**< Path to start in. This will typically be 'data/'. */
	std::string			mDirSeparator;		/**< Platform dependant directory separator. */

	#ifdef __APPLE__
	std::string			mBundlePath;		/**< Apple Bundle Directory. */
	#endif

	mutable bool		mVerbose;			/**< Displays lots of messages when true. Otherwise only critical messages are displayed. */
};

};
