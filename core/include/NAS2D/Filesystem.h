// ==================================================================================
// = NAS2D
// = Copyright © 2008 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software as long under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#ifndef _NAS_FILESYSTEM_
#define _NAS_FILESYSTEM_

#include "Common.h"

#if defined(__linux__)
#include "physfs.h"
#else
#include "physfs/physfs.h"
#endif

#include <string>
#include <vector>
#include <iomanip>
#include <iostream>

#include "File.h"

/**
 * \class Filesystem
 * \brief Implements a virtual file system.
 *
 * Provides cross-platform and transparent archive Filesystem functions.
 */
class Filesystem
{
public:
	Filesystem(const std::string& argv_0, const std::string& startPath);
	Filesystem();
	~Filesystem();

	void init(const std::string& argv_0, const std::string& startPath);

	File getFile(const std::string& fileName) const;

	/**
	 * Gets the current User path.
	 */
	std::string getUserPath() const { return PHYSFS_getUserDir(); }

	/**
	 * Gets the base data path.
	 */
	std::string getDataPath() const { return mDataPath; }

	std::string getWorkingDir(const std::string& fileName) const;
	
	bool addToSearchPath(const std::string& pathName) const;

	StringList getSearchPath() const;

	StringList enumerateDir(const std::string& dir) const;
	StringList enumerateDir(const std::string& dir, const std::string& filter) const;

	bool writeFile(const File& file, bool overwrite = true) const;
	bool deleteFile(const std::string& filename) const;

	/**
	 * Creates a new directory within the primary search path.
	 *
	 * \return Returns \c true if successful. Otherwise, returns \c false.
	 */
	bool makeDirectory(const std::string& dirPath) const
	{
		return (PHYSFS_mkdir(dirPath.c_str()) == 0) ? false : true;
	}


	/**
	 * Determines if a given path is a directory rather than a file.
	 */
	bool isDirectory(const std::string& path) const
	{
		return (PHYSFS_isDirectory(path.c_str()) == 0) ? false : true;
	}


	/**
	 * Checks for the existence of a file.
	 *
	 * Returns Returns \c true if the specified file exists. Otherwise, returns \c false.
	 */
	bool exists(const std::string& fileName) const
	{
		return (PHYSFS_exists(fileName.c_str()) == 0) ? false : true;
	}


	/**
	 * Returns the last error that occurred.
	 */
	std::string getLastError() const
	{
		return mErrorMessages.back();
	}


	/*
	 * Toggles Verbose Mode.
	 *
	 * When Verbose mode is off, only critical messages are displayed.
	 * Verbose Mode is generally useful for debugging purposes.
	 */
	void toggleVerbose() const { mVerbose = !mVerbose; } // Will this work as expected on all platforms?

	void debug();

	std::string getFileExtension(const std::string path);

private:
	Filesystem(const Filesystem&);				// Intentionally left undefined.
	Filesystem& operator= (const Filesystem&);	// Intentionally left undefined.

	bool closeFile(PHYSFS_File *file) const;

	std::string			mDataPath;			/**< Data path string. Specific to each platform. */
	std::string			mStartPath;			/**< Path to start in. This will typically be 'data/'. */
	std::string			mDirSeparator;		/**< Platform dependant directory separator. */

	mutable bool		mVerbose;			/**< Displays lots of messages when true. Otherwise only critical messages are displayed. */

	mutable StringList	mErrorMessages;		/**< List of error messages. */

#ifdef __APPLE__
	std::string			mBundlePath;		/**< Apple Bundle Directory. */
#endif
};

#endif
