// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2017 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Filesystem.h"
#include "NAS2D/Exception.h"

#include <physfs.h>

#if defined(__APPLE__)
#include <CoreFoundation/CoreFoundation.h>
#endif

#include <climits>
#include <cstring>
#include <string>
#include <iostream>
#include <sstream>

using namespace NAS2D;
using namespace NAS2D::Exception;

bool FILESYSTEM_INITIALIZED = false;

enum MountPosition
{
	MOUNT_PREPEND = 0,
	MOUNT_APPEND = 1,
};


/**
 * Default c'tor.
 */
Filesystem::Filesystem(): mVerbose(false)
{}


/**
 * Shuts down PhysFS and cleans up.
 */
Filesystem::~Filesystem()
{
	PHYSFS_deinit();
	FILESYSTEM_INITIALIZED = false;
	std::cout << "Filesystem Terminated." << std::endl;
}


/**
 * Shuts down PhysFS and cleans up.
 */
void Filesystem::init(const std::string& argv_0, const std::string& appName, const std::string& organizationName, const std::string& dataPath)
{
	if (FILESYSTEM_INITIALIZED) { throw filesystem_already_initialized(); }

	std::cout << "Initializing Filesystem... ";

	if (PHYSFS_init(argv_0.c_str()) == 0)
	{
		throw filesystem_backend_init_failure(PHYSFS_getLastError());
	}

	mStartPath = dataPath;
	mDirSeparator = PHYSFS_getDirSeparator();

#if defined(WINDOWS) || defined(__APPLE__)
	std::string basePath = PHYSFS_getBaseDir();

	// Note: Multiple trailing dir separators are safely ignored
	mDataPath = basePath + mStartPath + mDirSeparator;

#elif defined(__linux__)
	std::string userDir = PHYSFS_getUserDir();
	std::string appUserDataDir = ".lom/data/";
	mDataPath = userDir + appUserDataDir;

	// Must set write directory before we can modify filesystem
	PHYSFS_setWriteDir(userDir.c_str());
	// Create directory if it does not exist
	PHYSFS_mkdir(appUserDataDir.c_str());
#endif

	PHYSFS_setWriteDir(mDataPath.c_str());

	if (PHYSFS_mount(mDataPath.c_str(), "/", MountPosition::MOUNT_PREPEND) == 0)
	{
		//mErrorMessages.push_back(PHYSFS_getLastError());
		std::cout << std::endl << "(FSYS) Couldn't find data path '" << mDataPath << "'. " << PHYSFS_getLastError() << "." << std::endl;
	}

	FILESYSTEM_INITIALIZED = true;

	std::cout << "done." << std::endl;
}


/**
 * Adds a directory or supported archive to the Search Path.
 *
 * \param path	File path to add.
 *
 * \return Returns \c true if successful. Otherwise, returns \c false.
 */
bool Filesystem::addToSearchPath(const std::string& path) const
{
	if (!FILESYSTEM_INITIALIZED) { throw filesystem_not_initialized(); }

	if (mVerbose) { std::cout << "Adding '" << path << "' to search path." << std::endl; }

	std::string searchPath(mDataPath + path);

	if (PHYSFS_mount(searchPath.c_str(), "/", MountPosition::MOUNT_APPEND) == 0)
	{
		std::cout << "Couldn't add '" << path << "' to search path. " << PHYSFS_getLastError() << "." << std::endl;
		return false;
	}

	if (mVerbose) { std::cout << "Added '" << path << "' to search path." << std::endl; }

	return true;
}


/**
 * Returns a list of directories in the Search Path.
 */
StringList Filesystem::searchPath() const
{
	if (!FILESYSTEM_INITIALIZED) { throw filesystem_not_initialized(); }

	StringList searchPath;

	for (char **i = PHYSFS_getSearchPath(); *i != nullptr; i++)
	{
		searchPath.push_back(*i);
	}

	return searchPath;
}


/**
 * Returns a list of files within a given directory.
 *
 * \param	dir	Directory to search within the searchpath.
 *
 * \note	This function will also return the names of any directories in a specified search path
 */
StringList Filesystem::directoryList(const std::string& dir) const
{
	return directoryList(dir, std::string(""));
}


/**
 * Returns a list of files within a given directory.
 *
 * \param	dir	Directory to search within the searchpath.
 * \param	filter		Optional extension filter. Only use the extension without a wildcard (*) character or period (e.g., 'png' vs '*.png' or '.png').
 *
 * \note	This function will also return the names of any directories in a specified search path
 */
StringList Filesystem::directoryList(const std::string& dir, const std::string& filter) const
{
	if (!FILESYSTEM_INITIALIZED) { throw filesystem_not_initialized(); }

	char **rc = PHYSFS_enumerateFiles(dir.c_str());

	StringList fileList;
	if (filter.empty())
	{
		for (char **i = rc; *i != nullptr; i++)
		{
			fileList.push_back(*i);
		}
	}
	else
	{
		size_t filterLen = filter.size();
		for (char **i = rc; *i != nullptr; i++)
		{
			std::string tmpStr = *i;
			if (tmpStr.rfind(filter, strlen(*i) - filterLen) != std::string::npos)
			{
				fileList.push_back(*i);
			}
		}
	}

	PHYSFS_freeList(rc);

	return fileList;
}


/**
 * Deletes a specified file.
 *
 * \param	filename	Path of the file to delete relative to the Filesystem root directory.
 *
 * \note	This function is not named 'delete' due to
 *			language limitations.
 */
bool Filesystem::del(const std::string& filename) const
{
	if (!FILESYSTEM_INITIALIZED) { throw filesystem_not_initialized(); }

	if (PHYSFS_delete(filename.c_str()) == 0)
	{
		std::cout << "Unable to delete '" << filename << "':" << PHYSFS_getLastError() << std::endl;
		return false;
	}

	return true;
}


/**
 * Opens a file.
 *
 * \param filename	Path of the file to load.
 *
 * \return Returns a File.
 */
File Filesystem::open(const std::string& filename) const
{
	if (!FILESYSTEM_INITIALIZED) { throw filesystem_not_initialized(); }

	if (mVerbose) { std::cout << "Attempting to load '" << filename << std::endl; }

	PHYSFS_file* myFile = PHYSFS_openRead(filename.c_str());
	if (!myFile)
	{
		std::cout << "Unable to load '" << filename << "'. " << PHYSFS_getLastError() << "." << std::endl;
		closeFile(myFile);
		return File();
	}

	// Ensure that the file size is greater than zero and can fit in a 32-bit integer.
	PHYSFS_sint64 len = PHYSFS_fileLength(myFile);
	if (len < 0 || len > UINT_MAX)
	{
		std::cout << "File '" << filename << "' is too large to load." << std::endl;
		closeFile(myFile);
		return File();
	}

	// Create a char* buffer large enough to hold the entire file.
	PHYSFS_uint32 fileLength = static_cast<PHYSFS_uint32>(len);
	char *fileBuffer = new char[fileLength + 1];

	// If we read less then the file length, return an empty File object, log a message and free any used memory.
	if (PHYSFS_readBytes(myFile, fileBuffer, fileLength) < fileLength)
	{
		std::cout << "Unable to load '" << filename << "'. " << PHYSFS_getLastError() << "." << std::endl;
		delete[] fileBuffer;
		closeFile(myFile);
		return File();
	}

	File file(std::string(fileBuffer, fileLength), filename);
	closeFile(myFile);
	delete[] fileBuffer;

	if (mVerbose) { std::cout << "Loaded '" << filename << "' successfully." << std::endl; }

	return file;
}


/**
 * Creates a new directory within the primary search path.
 *
 * \param path	Path of the directory to create.
 *
 * \return Returns \c true if successful. Otherwise, returns \c false.
 */
bool Filesystem::makeDirectory(const std::string& path) const
{
	if (!FILESYSTEM_INITIALIZED) { throw filesystem_not_initialized(); }
	return PHYSFS_mkdir(path.c_str()) != 0;
}


/**
 * Determines if a given path is a directory rather than a file.
 *
 * \param path	Path to check.
 */
bool Filesystem::isDirectory(const std::string& path) const
{
	if (!FILESYSTEM_INITIALIZED) { throw filesystem_not_initialized(); }

	PHYSFS_Stat stat;
	return (PHYSFS_stat(path.c_str(), &stat) != 0) && (stat.filetype == PHYSFS_FILETYPE_DIRECTORY);
}


/**
 * Checks for the existence of a file.
 *
 * \param	filename	File path to check.
 *
 * Returns Returns \c true if the specified file exists. Otherwise, returns \c false.
 */
bool Filesystem::exists(const std::string& filename) const
{
	if (!FILESYSTEM_INITIALIZED) { throw filesystem_not_initialized(); }

	return PHYSFS_exists(filename.c_str()) != 0;
}


/**
 * Toggles Verbose Mode.
 *
 * When Verbose mode is off, only critical messages are displayed.
 * Verbose Mode is generally useful for debugging purposes.
 */
void Filesystem::toggleVerbose() const
{
	mVerbose = !mVerbose;
}


/**
 * Closes a file handle.
 *
 * \param	file	A handle to a PHYSFS_file.
 *
 * \return	True on success, false otherwise.
 */
bool Filesystem::closeFile(void* file) const
{
	if (!FILESYSTEM_INITIALIZED) { throw filesystem_not_initialized(); }

	if (!file) { return false; }

	if (PHYSFS_close(static_cast<PHYSFS_File*>(file)) != 0) { return true; }

	throw filesystem_file_handle_still_open(PHYSFS_getLastError());
}


/**
 * Writes a file to disk.
 *
 * \param	file		A reference to a \c const \c File object.
 * \param	overwrite	Flag indicating if a file should be overwritten if it already exists. Default is true.
 *
 * \return Returns \c true if successful. Otherwise, returns \c false.
 */
bool Filesystem::write(const File& file, bool overwrite) const
{
	if (!FILESYSTEM_INITIALIZED) { throw filesystem_not_initialized(); }

	if (file.empty())
	{
		std::cout << "Attempted to write empty file '" << file.filename() << "'" << std::endl;
		return false;
	}

	if (!overwrite && exists(file.filename()))
	{
		if (mVerbose) { std::cout << "Attempted to overwrite a file '" << file.filename() << "' that already exists." << std::endl; }
		return false;
	}

	PHYSFS_file* myFile = PHYSFS_openWrite(file.filename().c_str());
	if (!myFile)
	{
		if (mVerbose) { std::cout << "Couldn't open '" << file.filename() << "' for writing: " << PHYSFS_getLastError() << std::endl; }
		return false;
	}

	if (PHYSFS_writeBytes(myFile, file.bytes().c_str(), static_cast<PHYSFS_uint32>(file.size())) < static_cast<PHYSFS_sint64>(file.size()))
	{
		if (mVerbose) { std::cout << "Error occured while writing to file '" << file.filename() << "': " << PHYSFS_getLastError() << std::endl; }
		closeFile(myFile);
		return false;
	}
	else
	{
		closeFile(myFile);
		if (mVerbose) { std::cout << "Wrote '" << file.size() << "' bytes to file '" << file.filename() << "'." << std::endl; }
	}

	return true;
}


/**
 * Gets the current User path.
 */
std::string Filesystem::userPath() const
{
	if (!FILESYSTEM_INITIALIZED) { throw filesystem_not_initialized(); }
	return PHYSFS_getUserDir();
}


/**
 * Gets the base data path.
 */
std::string Filesystem::dataPath() const
{
	if (!FILESYSTEM_INITIALIZED) { throw filesystem_not_initialized(); }
	return mDataPath;
}


/**
 * Convenience function to get the working directory of a file.
 *
 * \param filename	A file path.
 *
 * \note	File paths should not have any trailing '/' characters.
 */
std::string Filesystem::workingPath(const std::string& filename) const
{
	if (!FILESYSTEM_INITIALIZED) { throw filesystem_not_initialized(); }
	if (!filename.empty())
	{
		std::string tmpStr(filename);
		size_t pos = tmpStr.rfind("/");
		tmpStr = tmpStr.substr(0, pos + 1);
		return tmpStr;
	}
	else
	{
		if (mVerbose) { std::cout << "Filesystem::workingPath(): empty string provided." << std::endl; }
		return std::string();
	}
}


/**
 * Gets the extension of a given file path.
 *
 * \param	path	Path to check for an extension.
 *
 * \return	Returns a string containing the file extension. An empty string will be
 *			returned if the file has no extension or if it's a directory.
 */
std::string Filesystem::extension(const std::string& path)
{
	if (!FILESYSTEM_INITIALIZED) { throw filesystem_not_initialized(); }

	// This is a naive approach but works for most cases.
	size_t pos = path.find_last_of(".");

	if (pos != std::string::npos)
	{
		return path.substr(pos + 1);
	}
	else if (isDirectory(path))
	{
		if (mVerbose) { std::cout << "Filesystem::extension(): Given path '" << path << "' is a directory, not a file." << std::endl; }
		return std::string();
	}
	else
	{
		if (mVerbose) { std::cout << "Filesystem::extension(): File '" << path << "' has no extension." << std::endl; }
		return std::string();
	}
}
