// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================

#include "Filesystem.h"

#include <physfs.h>

#include <limits>
#include <stdexcept>


using namespace NAS2D;


namespace {
	std::string getLastPhysfsError()
	{
		return PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
	}


	bool closeFile(void* file)
	{
		if (!file) { return false; }

		if (PHYSFS_close(static_cast<PHYSFS_File*>(file)) != 0) { return true; }

		throw std::runtime_error("Unable to close file handle: " + getLastPhysfsError());
	}
}


enum MountPosition
{
	MOUNT_PREPEND = 0,
	MOUNT_APPEND = 1,
};


Filesystem::Filesystem(const std::string& argv_0, const std::string& appName, const std::string& organizationName) :
	mAppName(appName),
	mOrganizationName(organizationName)
{
	if (PHYSFS_isInit()) { throw std::runtime_error("Filesystem is already initialized"); }

	if (PHYSFS_init(argv_0.c_str()) == 0)
	{
		throw std::runtime_error("Error initializing filesystem library: " + getLastPhysfsError());
	}
}


/**
 * Shuts down PhysFS and cleans up.
 */
Filesystem::~Filesystem()
{
	PHYSFS_deinit();
}


/**
 * Determines the path to the folder where the executable is located
 *
 * The base path may or may not be the current working directory.
 */
std::string Filesystem::basePath() const
{
	return PHYSFS_getBaseDir();
}


/**
 * Determines the path to the folder where user preferences are stored
 *
 * The user should have write access to the preferences folder, which is generally under their user folder.
 *
 * \note This path is dependent on the Operating System (OS).
 * \note Path may be empty if the folder could not be created.
 */
std::string Filesystem::prefPath() const
{
	auto prefDir = PHYSFS_getPrefDir(mOrganizationName.c_str(), mAppName.c_str());
	return (prefDir != nullptr) ? prefDir : "";
}


/**
 * Mount a folder with read access
 *
 * \param path	File path to add.
 *
 * \return Nonzero on success, zero on error.
 */
int Filesystem::mountSoftFail(const std::string& path) const
{
	return PHYSFS_mount(path.c_str(), "/", MountPosition::MOUNT_APPEND);
}


/**
 * Mount a folder with read access
 *
 * \param path	File path to add.
 */
void Filesystem::mount(const std::string& path) const
{
	if (mountSoftFail(path) == 0)
	{
		throw std::runtime_error("Error mounting search path: " + path + " : " + getLastPhysfsError());
	}
}


/**
 * Mount a folder with read and write access
 *
 * \param path	File path to add.
 */
void Filesystem::mountReadWrite(const std::string& path) const
{
	// Mount for read access
	mount(path);

	// Mount for write access
	if (PHYSFS_setWriteDir(path.c_str()) == 0)
	{
		throw std::runtime_error("Error setting write folder: " + path + " : " + getLastPhysfsError());
	}
}


/**
 * Removes a directory or supported archive from the Search Path.
 *
 * \param path	File path to remove.
 */
void Filesystem::unmount(const std::string& path) const
{
	if (PHYSFS_unmount(path.c_str()) == 0)
	{
		throw std::runtime_error("Error unmounting search path: " + path + " : " + getLastPhysfsError());
	}
}


/**
 * Returns a list of directories in the Search Path.
 */
std::vector<std::string> Filesystem::searchPath() const
{
	std::vector<std::string> searchPath;

	auto searchPathList = PHYSFS_getSearchPath();
	for (auto i = searchPathList; *i != nullptr; ++i)
	{
		searchPath.push_back(*i);
	}
	PHYSFS_freeList(searchPathList);

	return searchPath;
}


/**
 * Returns a list of files within a given directory.
 *
 * \param	dir	Directory to search within the searchpath.
 * \param	filter		Optional extension filter. Only use the extension without a wildcard (*) character or period (e.g., 'png' vs '*.png' or '.png').
 *
 * \note	This function will also return the names of any directories in a specified search path
 */
std::vector<std::string> Filesystem::directoryList(const std::string& dir, const std::string& filter) const
{
	auto rc = PHYSFS_enumerateFiles(dir.c_str());

	std::vector<std::string> fileList;
	if (filter.empty())
	{
		for (auto i = rc; *i != nullptr; i++)
		{
			fileList.push_back(*i);
		}
	}
	else
	{
		const auto filterLen = filter.size();
		for (auto i = rc; *i != nullptr; i++)
		{
			std::string tmpStr = *i;
			if (tmpStr.rfind(filter, tmpStr.length() - filterLen) != std::string::npos)
			{
				fileList.push_back(*i);
			}
		}
	}

	PHYSFS_freeList(rc);

	return fileList;
}


/**
 * Determines if a given path is a directory rather than a file.
 *
 * \param path	Path to check.
 */
bool Filesystem::isDirectory(const std::string& path) const
{
	PHYSFS_Stat stat;
	return (PHYSFS_stat(path.c_str(), &stat) != 0) && (stat.filetype == PHYSFS_FILETYPE_DIRECTORY);
}


/**
 * Creates a new directory within the primary search path.
 *
 * \param path	Path of the directory to create.
 */
void Filesystem::makeDirectory(const std::string& path) const
{
	if (PHYSFS_mkdir(path.c_str()) == 0)
	{
		throw std::runtime_error("Error creating directory: " + path + " : " + getLastPhysfsError());
	}
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
	return PHYSFS_exists(filename.c_str()) != 0;
}


/**
 * Deletes a specified file.
 *
 * \param	filename	Path of the file to delete relative to the Filesystem root directory.
 */
void Filesystem::del(const std::string& filename) const
{
	if (PHYSFS_delete(filename.c_str()) == 0)
	{
		throw std::runtime_error("Error deleting file: " + filename + " : " + getLastPhysfsError());
	}
}


std::string Filesystem::read(const std::string& filename) const
{
	PHYSFS_file* myFile = PHYSFS_openRead(filename.c_str());
	if (!myFile)
	{
		closeFile(myFile);
		throw std::runtime_error("Error opening file for reading: " + filename + " : " + getLastPhysfsError());
	}

	// Ensure that the file size is greater than zero and can fit in a std::string::size_type
	auto fileLength = PHYSFS_fileLength(myFile);
	if (fileLength < 0 || static_cast<PHYSFS_uint64>(fileLength) > std::numeric_limits<std::string::size_type>::max())
	{
		closeFile(myFile);
		throw std::runtime_error("Error determining length of file or file too large: " + filename + " : Length = " + std::to_string(fileLength));
	}

	// Create buffer large enough to hold entire file
	const auto bufferSize = static_cast<std::string::size_type>(fileLength);
	std::string fileBuffer;
	fileBuffer.resize(bufferSize);

	// Read file data into buffer and close file
	const auto actualReadLength = PHYSFS_readBytes(myFile, fileBuffer.data(), bufferSize);
	closeFile(myFile);

	// Ensure we read the expected length
	if (actualReadLength < fileLength)
	{
		throw std::runtime_error("Error reading file data: " + filename + " : " + getLastPhysfsError());
	}

	return fileBuffer;
}


void Filesystem::write(const std::string& filename, const std::string& data, WriteFlags flags) const
{
	if (flags != WriteFlags::Overwrite && exists(filename))
	{
		throw std::runtime_error("Overwrite flag not specified and file already exists: " + filename);
	}

	PHYSFS_file* myFile = PHYSFS_openWrite(filename.c_str());
	if (!myFile)
	{
		throw std::runtime_error("Error opening file for writing: " + filename + " : " + getLastPhysfsError());
	}

	if (PHYSFS_writeBytes(myFile, data.c_str(), static_cast<PHYSFS_uint64>(data.size())) < static_cast<PHYSFS_sint64>(data.size()))
	{
		closeFile(myFile);
		throw std::runtime_error("Error writing file: " + filename + " : " + getLastPhysfsError());
	}

	closeFile(myFile);
}


/**
 * Gets the dir separator for the current platform
 *
 * The dir separator separates the directories within a path. This is typically either "\\" for Windows, or "/" for Linux.
 *
 * \note The path separator may be more than one character
 */
std::string Filesystem::dirSeparator() const
{
	return PHYSFS_getDirSeparator();
}


/**
 * Convenience function to get the parent directory of a file.
 *
 * \param filePath A file path.
 *
 * \return The path up to and including the last '/', or empty string if no '/'
 */
std::string Filesystem::parentPath(std::string_view filePath) const
{
	return std::string{filePath.substr(0, filePath.rfind('/') + 1)};
}


/**
 * Gets the extension of a given file path.
 *
 * \param	filePath	Path to check for an extension.
 *
 * \return	Returns a string containing the file extension, including the dot (".").
 *			An empty string will be returned if the file has no extension.
 */
std::string Filesystem::extension(std::string_view filePath) const
{
	const auto fileName = filePath.substr(filePath.rfind('/') + 1);
	const auto pos = fileName.rfind('.');

	if (pos != std::string::npos)
	{
		return std::string{fileName.substr(pos)};
	}
	return std::string{};
}
