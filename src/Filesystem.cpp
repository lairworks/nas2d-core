// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2018 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================

#include "NAS2D/Filesystem.h"

//Hacky fix until Apple gets off its butt.
#if defined(PLATFORM_APPLE)

#include "NAS2D/Exception.h"


#if defined PLATFORM_APPLE
#include <physfs.h>
#include <CoreFoundation/CoreFoundation.h>
#endif

#include <climits>
#include <cstring>
#include <string>
#include <iostream>
#include <sstream>

using namespace NAS2D;
using namespace NAS2D::Exception;


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
#ifdef PLATFORM_APPLE
	if (PHYSFS_isInit()) { PHYSFS_deinit(); }
#endif
	std::cout << "Filesystem Terminated." << std::endl;
}


/**
 * Shuts down PhysFS and cleans up.
 */
void Filesystem::init(const std::string& argv_0, const std::string& appName, const std::string& organizationName, const std::string& dataPath)
{
	std::cout << "Initializing Filesystem... ";
#ifdef PLATFORM_APPLE
	if (PHYSFS_isInit()) { throw filesystem_already_initialized(); }

	if (PHYSFS_init(argv_0.c_str()) == 0)
	{
		throw filesystem_backend_init_failure(getLastPhysfsError());
	}

	if (PHYSFS_setSaneConfig(organizationName.c_str(), appName.c_str(), nullptr, false, false) == 0)
	{
		std::cout << std::endl << "(FSYS) Error setting sane config. " << getLastPhysfsError() << "." << std::endl;
	}

	mDataPath = dataPath;
	if (PHYSFS_mount(mDataPath.c_str(), "/", MountPosition::MOUNT_PREPEND) == 0)
	{
		std::cout << std::endl << "(FSYS) Couldn't find data path '" << mDataPath << "'. " << getLastPhysfsError() << "." << std::endl;
	}
#endif

	std::cout << "done." << std::endl;
}


/**
 * Adds a directory or supported archive to the Search Path.
 *
 * \param path	File path to add.
 *
 * \return Returns \c true if successful. Otherwise, returns \c false.
 */
bool Filesystem::mount(const std::string& path) const
{
#ifdef PLATFORM_APPLE
	if (!PHYSFS_isInit()) { throw filesystem_not_initialized(); }

	if (mVerbose) { std::cout << "Adding '" << path << "' to search path." << std::endl; }

	std::string searchPath(mDataPath + path);

	if (PHYSFS_mount(searchPath.c_str(), "/", MountPosition::MOUNT_APPEND) == 0)
	{
		std::cout << "Couldn't add '" << path << "' to search path. " << getLastPhysfsError() << "." << std::endl;
		return false;
	}

	return true;
#endif
}


/**
 * Returns a list of directories in the Search Path.
 */
StringList Filesystem::searchPath() const
{
#ifdef PLATFORM_APPLE
	if (!PHYSFS_isInit()) { throw filesystem_not_initialized(); }

	StringList searchPath;

	auto searchPathList = PHYSFS_getSearchPath();
	for (char **i = searchPathList; *i != nullptr; ++i)
	{
		searchPath.push_back(*i);
	}
	PHYSFS_freeList(searchPathList);

	return searchPath;
#endif
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
#ifdef PLATFORM_APPLE
	if (!PHYSFS_isInit()) { throw filesystem_not_initialized(); }

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
#endif
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
#ifdef PLATFORM_APPLE
	if (!PHYSFS_isInit()) { throw filesystem_not_initialized(); }

	if (PHYSFS_delete(filename.c_str()) == 0)
	{
		std::cout << "Unable to delete '" << filename << "':" << getLastPhysfsError() << std::endl;
		return false;
	}

	return true;
#endif
}

/**
 * Removes a specified file.
 *
 * \param	filename	Path of the file to delete relative to the Filesystem root directory.
 *
 */
bool Filesystem::remove(const std::string& filename) const
{
	return del(filename);
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
#ifdef PLATFORM_APPLE
	if (!PHYSFS_isInit()) { throw filesystem_not_initialized(); }

	if (mVerbose) { std::cout << "Attempting to load '" << filename << std::endl; }

	PHYSFS_file* myFile = PHYSFS_openRead(filename.c_str());
	if (!myFile)
	{
		std::cout << "Unable to load '" << filename << "'. " << getLastPhysfsError() << "." << std::endl;
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
		std::cout << "Unable to load '" << filename << "'. " << getLastPhysfsError() << "." << std::endl;
		delete[] fileBuffer;
		closeFile(myFile);
		return File();
	}

	File file(std::string(fileBuffer, fileLength), filename);
	closeFile(myFile);
	delete[] fileBuffer;

	if (mVerbose) { std::cout << "Loaded '" << filename << "' successfully." << std::endl; }

	return file;
#endif
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
#ifdef PLATFORM_APPLE
	if (!PHYSFS_isInit()) { throw filesystem_not_initialized(); }
	return PHYSFS_mkdir(path.c_str()) != 0;
#endif
}


/**
 * Determines if a given path is a directory rather than a file.
 *
 * \param path	Path to check.
 */
bool Filesystem::isDirectory(const std::string& path) const
{
#ifdef PLATFORM_APPLE
	if (!PHYSFS_isInit()) { throw filesystem_not_initialized(); }

	PHYSFS_Stat stat;
	return (PHYSFS_stat(path.c_str(), &stat) != 0) && (stat.filetype == PHYSFS_FILETYPE_DIRECTORY);
#endif
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
#ifdef PLATFORM_APPLE
	if (!PHYSFS_isInit()) { throw filesystem_not_initialized(); }

	return PHYSFS_exists(filename.c_str()) != 0;
#endif
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


#ifdef PLATFORM_APPLE
/**
 * Closes a file handle.
 *
 * \param	file	A handle to a PHYSFS_file.
 *
 * \return	True on success, false otherwise.
 */
bool Filesystem::closeFile(void* file) const
{
	if (!PHYSFS_isInit()) { throw filesystem_not_initialized(); }

	if (!file) { return false; }

	if (PHYSFS_close(static_cast<PHYSFS_File*>(file)) != 0) { return true; }

	throw filesystem_file_handle_still_open(getLastPhysfsError());
}
#endif


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
#ifdef PLATFORM_APPLE
	if (!PHYSFS_isInit()) { throw filesystem_not_initialized(); }

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
		if (mVerbose) { std::cout << "Couldn't open '" << file.filename() << "' for writing: " << getLastPhysfsError() << std::endl; }
		return false;
	}

	if (PHYSFS_writeBytes(myFile, file.bytes().c_str(), static_cast<PHYSFS_uint32>(file.size())) < static_cast<PHYSFS_sint64>(file.size()))
	{
		if (mVerbose) { std::cout << "Error occured while writing to file '" << file.filename() << "': " << getLastPhysfsError() << std::endl; }
		closeFile(myFile);
		return false;
	}
	else
	{
		closeFile(myFile);
		if (mVerbose) { std::cout << "Wrote '" << file.size() << "' bytes to file '" << file.filename() << "'." << std::endl; }
	}

	return true;
#endif
}


/**
 * Gets the current User path.
 */
std::string Filesystem::userPath() const
{
#ifdef PLATFORM_APPLE
	if (!PHYSFS_isInit()) { throw filesystem_not_initialized(); }
	return PHYSFS_getUserDir();
#endif
}


/**
 * Gets the base data path.
 */
std::string Filesystem::dataPath() const
{
#ifdef PLATFORM_APPLE
	if (!PHYSFS_isInit()) { throw filesystem_not_initialized(); }
	return mDataPath;
#endif
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
#ifdef PLATFORM_APPLE
	if (!PHYSFS_isInit()) { throw filesystem_not_initialized(); }
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
#endif
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
#ifdef PLATFORM_APPLE
	if (!PHYSFS_isInit()) { throw filesystem_not_initialized(); }

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
#endif
}

#ifdef PLATFORM_APPLE
const char* Filesystem::getLastPhysfsError() const
{
	return PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
}
#endif















#else

#include "NAS2D/Exception.h"

#include <climits>
#include <cstring>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

#ifdef PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

using namespace NAS2D;
using namespace NAS2D::Exception;


enum MountPosition
{
    MOUNT_PREPEND = 0,
    MOUNT_APPEND = 1,
};

/**
 * Shuts down PhysFS and cleans up.
 */
Filesystem::~Filesystem()
{
    std::cout << "Filesystem Terminated." << std::endl;
}


/**
 * Initializes PhysFS
 */
void Filesystem::init(const std::string& /*argv_0*/, const std::string& appName, const std::string& organizationName, const std::string& dataPath)
{
    if(isInit()) { throw filesystem_already_initialized(); }

    std::cout << "Initializing Filesystem... ";

    mOrganizationName = organizationName;
    mAppName = appName;
    mDataPath = FS::path{ dataPath }.make_preferred();
    mWorkingDirectory = FS::current_path().make_preferred();
    mOrgAppPath = (FS::path{ mOrganizationName } / FS::path{ mAppName }).make_preferred();
    mExePath = getExePath();
    mIsInit = true;

    std::cout << "done." << std::endl;
}


/**
 * Adds a directory or supported archive to the Search Path.
 *
 * \param path	File path to add.
 *
 * \return Returns \c true if successful. Otherwise, returns \c false.
 */
bool Filesystem::mount(const std::string& path) const
{
    if(!isInit()) { throw filesystem_not_initialized(); }

    auto p = FS::path{ path }.make_preferred();

    if(!exists(p))
    {
        std::cout << "Couldn't add '" << path << "' to search path. Path: " << path << " could not be found." << std::endl;
        return false;
    }

    mSearchPath.push_back(p);
    return true;
}


/**
 * Returns a list of directories in the Search Path.
 */
StringList Filesystem::searchPath() const
{
    if(!isInit()) { throw filesystem_not_initialized(); }

    StringList searchPath{};
    for(const auto& p : mSearchPath) { searchPath.push_back(p.string()); }
    return searchPath;
}

/**
 * Returns a list of files within a given directory.
 *
 * \param	dir         Directory to search.
 * \param	filter		Optional extension filter comma-separated list. Only use the extension without a wildcard (*) character (e.g., '.png' vs '*.png').
 *
 */
StringList Filesystem::directoryList(const std::string& dir, const std::string& filter) const
{
    if(!isInit()) { throw filesystem_not_initialized(); }
    StringList paths{};
    auto add_path_cb = [&paths](const FS::path& p) { paths.push_back(p.string()); };
    forEachFileInFolder(dir, filter, add_path_cb, false);
    return paths;
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
    return remove(filename);
}


/**
 * Reads a file from disk into memory
 *
 * \param filename	Path of the file to load.
 *
 * \return Returns a File.
 */
File Filesystem::read(const std::string& filename) const
{
    if(!isInit()) { throw filesystem_not_initialized(); }

    std::string fileBuffer{};
    if(!readBufferFromFile(fileBuffer, filename))
    {
        std::cout << "Unable to load '" << filename << "'. " << std::endl;
        return File();
    }

    File file(std::string(fileBuffer.data(), fileBuffer.size()), filename);

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
    if(!isInit()) { throw filesystem_not_initialized(); }

    return FS::create_directories(FS::path{ path });
}


/**
 * Determines if a given path is a directory rather than a file.
 *
 * \param path	Path to check.
 */
bool Filesystem::isDirectory(const std::string& path) const
{
    if(!isInit()) { throw filesystem_not_initialized(); }

    return FS::is_directory(FS::path{ path });
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
    if(!isInit()) { throw filesystem_not_initialized(); }
    return exists(FS::path{ filename });
}

/**
 * Writes a file to disk.
 *
 * \param	file		A reference to a \c const \c File object.
 *
 * \return Returns \c true if successful. Otherwise, returns \c false.
 */
bool Filesystem::write(const File& file) const
{
    if(!isInit()) { throw filesystem_not_initialized(); }

    const auto& filename = file.filename();
    if(file.empty())
    {
        std::cout << "Attempted to write empty file '" << filename << "'" << std::endl;
        return false;
    }
    if(!writeBufferToFile(file.bytes(), filename))
    {
        return false;
    }
    return true;
}

/**
 * Gets the base data path.
 */
std::string Filesystem::dataPath() const
{
    if(!isInit()) { throw filesystem_not_initialized(); }
    return mDataPath.string();
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
    if(!isInit()) { throw filesystem_not_initialized(); }
    if(filename.empty())
    {
        return std::string();
    }
    return FS::path{ filename }.parent_path().make_preferred().string();
}


/**
 * Gets the extension of a given file path.
 *
 * \param	path	Path to check for an extension.
 *
 * \return	Returns a string containing the file extension in the form ".ext".
            An empty string will be returned if the file has no extension or if it's a directory.
 */
std::string Filesystem::extension(const std::string& path)
{
    if(!isInit()) { throw filesystem_not_initialized(); }

    auto p = FS::path{ path }.make_preferred();
    if(!p.has_extension()) { return std::string{}; }
    if(FS::is_directory(p)) { return std::string{}; }

    return p.extension().string();
}

void Filesystem::setWorkingDirectory(const FS::path& p)
{
    static_cast<const Filesystem&>(*this).setWorkingDirectory(p);
}

void Filesystem::setWorkingDirectory(const FS::path& p) const
{
    try
    {
        FS::current_path(p);
        mWorkingDirectory = p;
    }
    catch(const FS::filesystem_error& e)
    {
        std::cout << "Filesystem::extension(): std::filesystem reported an error:"
            << "\n  What: " << e.what()
            << "\n  Code: " << e.code()
            << "\n  Path1: \"" << e.path1() << '\"'
            << "\n  Path2: \"" << e.path2() << '\"'
            << "\nRethrowing as NAS2D::Exception::filesystem_file_not_found";
        std::cout.flush();
        std::string desc{"Path 1: " + e.path1().string() + "Path 2: " + e.path2().string() };
        throw NAS2D::Exception::filesystem_file_not_found(desc);
    }
}

FS::path Filesystem::getWorkingDirectory() const
{
    //Update the cache if it isn't correct.
    const auto& cur_path = FS::current_path();
    if(mWorkingDirectory != cur_path)
    {
        setWorkingDirectory(cur_path);
    }
    return mWorkingDirectory;
}

bool Filesystem::isInit() const
{
    return mIsInit;
}

bool Filesystem::exists(const FS::path& p) const
{
    return FS::exists(p);
}


/**
 * Calls a delegate callback for each file in a directory, optionally recursing to subdirectories
 *
 * \param	folderpath         The root folder to search.
 * \param   validExtentionList CSV list of valid extensions to call [callback] on. pattern is ".txt,.pdf,.png" etc, note the leading dot.
                               Leave empty to search every file.
 * \param   callback           A free function, std::function or lambda that
                               returns void and takes a single std::filesystem::path by const-ref
                               representing the current file being touched.
 * \param   recursive          Should I search subdirectories?
 */
void Filesystem::forEachFileInFolder(const FS::path& folderpath, const std::string& validExtensionList /*= std::string{}*/, const std::function<void(const FS::path&)>& callback /*= [](const FS::path& p) { (void)p; }*/, bool recursive /*= false*/) const
{
	bool exists = FS::exists(folderpath);
	if(!exists) { return; }
	auto fp_copy = folderpath;
	fp_copy = FS::canonical(fp_copy);
	fp_copy.make_preferred();
    bool is_directory = FS::is_directory(fp_copy);
    if(!is_directory) { return; }

    auto validExtensions = split(toLowercase(validExtensionList));

    if(!recursive)
    {
		forEachFileInFolders<FS::directory_iterator>(fp_copy, validExtensions, callback);
    }
    else
    {
		forEachFileInFolders<FS::recursive_directory_iterator>(fp_copy, validExtensions, callback);
    }
}

void Filesystem::forEachFileInFolder(const FS::path& folderpath, const std::string& validExtensionList /*= std::string{}*/, const std::function<void(const FS::path&)>& callback /*= [](const FS::path& p) { (void)p; }*/, bool recursive /*= false*/)
{
    static_cast<const Filesystem&>(*this).forEachFileInFolder(folderpath, validExtensionList, callback, recursive);
}

/**
 * Reads binary file into memory.
 *
 * \param	out_buffer      a std::vector of binary data containing the contents of the file.
 * \param   filePath        the path to read from. Must exist. Must not be a directory.
 * \return  Returns true if the file was successfully read. False otherwise.
 */
bool Filesystem::readBufferFromFile(std::vector<unsigned char>& out_buffer, const std::string& filePath) const
{
    auto p{ FS::path(filePath).make_preferred() };
    bool path_is_directory = FS::is_directory(p);
    bool path_not_exist = !FS::exists(p);
    bool not_valid_path = path_is_directory || path_not_exist;
    if(not_valid_path) { return false; }

    auto byte_size = static_cast<std::size_t>(FS::file_size(p));
    out_buffer.resize(byte_size);
    std::ifstream ifs;
    ifs.open(p, std::ios_base::binary);
    ifs.read(reinterpret_cast<char*>(out_buffer.data()), out_buffer.size());
    ifs.close();
    out_buffer.shrink_to_fit();
    return true;

}

/**
 * Reads a text file into memory.
 *
 * \param	out_buffer      a string containing the contents of the file.
 * \param   filePath the    path to read from. Must exist. Must not be a directory.
 * \return  Returns true if the file was successfully read. False otherwise.
 */
bool Filesystem::readBufferFromFile(std::string& out_buffer, const std::string& filePath) const
{
    auto p{ FS::path(filePath).make_preferred() };
    bool path_is_directory = FS::is_directory(p);
    bool path_not_exist = !FS::exists(p);
    bool not_valid_path = path_is_directory || path_not_exist;
    if(not_valid_path) { return false; }

    std::ifstream ifs{ p };
    //Dump ifstream buffer directly into stringstream and convert to string
    out_buffer = std::string(static_cast<const std::stringstream&>(std::stringstream() << ifs.rdbuf()).str());
    return true;
}

/**
 * Removes a specified file.
 *
 * \param	filename	Path of the file to delete.
 * \note    Must be either a file or an empty directory.
 *          Non-empty directories will throw an error.
 *          To delete an entire directory and all its contents use FileSystem::removeDirectory.
 *
 */
bool NAS2D::Filesystem::remove(const std::string& path) const
{
    if(!isInit()) { throw filesystem_not_initialized(); }

    try
    {
        FS::remove(FS::path{ path });
        return true;
    }
    catch(const FS::filesystem_error& e)
    {
        std::cout << "Filesystem::remove(): std::filesystem reported an error:"
            << "\n  What: " << e.what()
            << "\n  Code: " << e.code()
            << "\n  Path1: \"" << e.path1() << '\"'
            << "\n  Path2: \"" << e.path2() << '\"'
            << "\nRethrowing as NAS2D::Exception::filesystem_file_not_found";
        std::cout.flush();
        std::string desc{ "Path 1: " + e.path1().string() + "Path 2: " + e.path2().string() };
        throw NAS2D::Exception::filesystem_file_not_found(desc);
    }
}


/**
 * Removes a directory and all its contents.
 *
 * \param	filename	Path of the directory to remove.
 * \return  True if successful. False otherwise.
 *
 */
bool NAS2D::Filesystem::removeDirectory(const std::string& path) const
{
    if(!isInit()) { throw filesystem_not_initialized(); };

    try
    {
        auto removedFilesAndDirectoriesCount = FS::remove_all(FS::path{ path });
        std::cout << "Removed " << removedFilesAndDirectoriesCount << "files/folders" << std::endl;
        return true;
    }
    catch(const FS::filesystem_error& e)
    {
        std::cout << "Filesystem::removeDirectory(): std::filesystem reported an error:"
            << "\n  What: " << e.what()
            << "\n  Code: " << e.code()
            << "\n  Path1: \"" << e.path1() << '\"'
            << "\n  Path2: \"" << e.path2() << '\"'
            << "\nRethrowing as NAS2D::Exception::filesystem_file_not_found";
        std::cout.flush();
        std::string desc{ "Path 1: " + e.path1().string() + "Path 2: " + e.path2().string() };
        throw NAS2D::Exception::filesystem_file_not_found(desc);
    }
}

/**
 * Gets the path of the binary we are running.
 *
 * \return	The path to the binary or an empty path if there was an error.
 */
FS::path NAS2D::Filesystem::getExePath() const
{
    //Cache result because OS calls may dynamically allocate
    //and that can get expensive.
    if(mExePath.empty())
    {
#if defined(PLATFORM_WINDOWS)
        mExePath = doWindowsQueryExePath();
#elif defined(PLATFORM_APPLE)
        mExePath = doAppleQueryExePath();
#elif defined(PLATFORM_LINUX)
        mExePath = doLinuxQueryExePath();
#endif
    }
    return mExePath;
}

#ifdef PLATFORM_WINDOWS
FS::path NAS2D::Filesystem::doWindowsQueryExePath() const
{
    TCHAR filename[MAX_PATH];
    ::GetModuleFileName(nullptr, filename, MAX_PATH);
    auto result = FS::path(filename);
    //Canonical converts to absolute first "because the standard says so."
    //So there's no need to explicitly call absolute.
    result = FS::canonical(result);
    result.make_preferred();
    return result;
}
#else
FS::path NAS2D::Filesystem::doWindowsQueryExePath() const
{
    return FS::path{};
}
#endif

#ifdef PLATFORM_LINUX
FS::path NAS2D::Filesystem::doLinuxQueryExePath() const
{
    FS::path p{ "/proc/self/exe" };
    if(!FS::exists(p)) { return FS::path{}; }
    //Follow the symbolic link to the actual file.
    if(FS::is_symlink(p)) { p = FS::read_symlink(p); }
    //Canonical converts to absolute first "because the standard says so."
    //So there's no need to explicitly call absolute.
    p = FS::canonical(p);
    p.make_preferred();
    return p;
}
#else
FS::path NAS2D::Filesystem::doLinuxQueryExePath() const
{
    return FS::path{};
}
#endif

#ifdef PLATFORM_APPLE
FS::path NAS2D::Filesystem::doAppleQueryExePath() const
{
    uint32_t size = 0;
    std::string path{};
    //This will intentionally fail but fill 'size' with the required file size
    //and since we are immediately calling it again with the correct size
    //there's no need to check the return codes.
    _NSGetExecutablePath(path.data(), &size);
    path.resize(size);
    _NSGetExecutablePath(path.data(), &size);
    FS::path p{ path };
    //Canonical converts to absolute first "because the standard says so."
    //So there's no need to explicitly call absolute.
    p = FS::canonical(p);
    p.make_preferred();
    return p;
}
#else
FS::path NAS2D::Filesystem::doAppleQueryExePath() const
{
    return FS::path{};
}
#endif


/**
 * Writes a binary data buffer to a file.
 *
 * \param	buffer	a binary-only buffer that contains the data to write.
 * \param   filePath the path to write to. Must not be a directory.
 * \return	Returns true if the file was successfully written. False otherwise.
 */
bool NAS2D::Filesystem::writeBufferToFile(void* buffer, std::size_t size, const std::string& filePath) const
{
    FS::path p(filePath);
    p.make_preferred();
    bool not_valid_path = FS::is_directory(p);
    bool invalid = not_valid_path;
    if(invalid) { return false; }

    std::ofstream ofs;
    ofs.open(p.string(), std::ios_base::binary);
    if(ofs)
    {
        ofs.write(reinterpret_cast<const char*>(buffer), size);
        return true;
    }
    return false;
}


/**
 * Writes a text buffer/string to a file.
 *
 * \param	buffer	a text-only, non-binary buffer that contains the data to write.
 * \param   filePath the path to write to. Must not be a directory.
 * \return	Returns true if the file was successfully written. False otherwise.
 */
bool NAS2D::Filesystem::writeBufferToFile(const std::string& buffer, const std::string& filePath) const
{
    FS::path p{ filePath };
    p.make_preferred();
    bool not_valid_path = FS::is_directory(p);
    bool invalid = not_valid_path;
    if(invalid) { return false; }

    std::ofstream ofs{ p };
    if(ofs)
    {
        //Reviewer: cugone
        //TODO: Might need to change this to output stream operator because explicitly dealing with non-binary data.
        //TODO: and line-breaks are not normalized across OSes when .write is called like with the stream operators.
        //TODO: Until otherwise proven with a Unit test, I'm gonna leave it.
        ofs.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
        return true;
    }
    return false;
}
#endif