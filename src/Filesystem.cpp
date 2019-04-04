// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2018 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Filesystem.h"
#include "NAS2D/Exception.h"

#if defined(__APPLE__)
#define PLATFORM_APPLE
#include <CoreFoundation/CoreFoundation.h>
#endif

#include <climits>
#include <cstring>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>


#if defined(_WIN32) || defined(_WIN64)
#define PLATFORM_WINDOWS
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
    namespace FS = std::filesystem;
    if(mIsInit) { throw filesystem_already_initialized(); }

	std::cout << "Initializing Filesystem... ";

    mOrganizationName = organizationName;
    mAppName = appName;
    mDataPath = FS::path{ dataPath };
    mWorkingDirectory = mDataPath / FS::path{ mOrganizationName } / FS::path{ mAppName };
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
    namespace FS = std::filesystem;
    if(!isInit()) { throw filesystem_not_initialized(); }

    auto p = FS::path{ path };

    if(!exists(p)) {
        std::cout << "Couldn't add '" << path << "' to search path. Path: " << path << " could not be found." << std::endl;
        return false;
    }

	if (mVerbose) { std::cout << "Adding '" << path << "' to search path." << std::endl; }

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
    for(const auto& p : this->mSearchPath) {
        searchPath.push_back(p.string());
    }
	return searchPath;
}

/**
 * Returns a list of files within a given directory.
 *
 * \param	dir	Directory to search within the searchpath.
 * \param	filter		Optional extension filter comma-separated list. Only use the extension without a wildcard (*) character (e.g., '.png' vs '*.png').
 *
 */
StringList Filesystem::directoryList(const std::string& dir, const std::string& filter) const
{
    namespace FS = std::filesystem;
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
    if(!isInit()) { throw filesystem_not_initialized(); }

    namespace FS = std::filesystem;
    try {
        FS::remove(FS::path{ filename });
	    return true;
    } catch(const FS::filesystem_error& e) {
        std::cout << "Filesystem::del(): std::filesystem reported an error:"
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
 * Opens a file.
 *
 * \param filename	Path of the file to load.
 *
 * \return Returns a File.
 */
File Filesystem::open(const std::string& filename) const
{
    if(!isInit()) { throw filesystem_not_initialized(); }

	if (mVerbose) { std::cout << "Attempting to load '" << filename << std::endl; }

    std::string fileBuffer{};
    if(!this->readBufferFromFile(fileBuffer, filename)) {
        std::cout << "Unable to load '" << filename << "'. " << std::endl;
        return File();
    }

	File file(std::string(fileBuffer.data(), fileBuffer.size()), filename);

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
    namespace FS = std::filesystem;
    if(this->isInit()) { throw filesystem_not_initialized(); }

    return FS::create_directories(FS::path{ path });
}


/**
 * Determines if a given path is a directory rather than a file.
 *
 * \param path	Path to check.
 */
bool Filesystem::isDirectory(const std::string& path) const
{
    namespace FS = std::filesystem;
    if(this->isInit()) { throw filesystem_not_initialized(); }

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
	if (!isInit()) { throw filesystem_not_initialized(); }
    return exists(std::filesystem::path{ filename });
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
 * Writes a file to disk.
 *
 * \param	file		A reference to a \c const \c File object.
 * \param	overwrite	Flag indicating if a file should be overwritten if it already exists. Default is true.
 *
 * \return Returns \c true if successful. Otherwise, returns \c false.
 */
bool Filesystem::write(const File& file, bool overwrite) const
{
    namespace FS = std::filesystem;
	if (!isInit()) { throw filesystem_not_initialized(); }

    const auto& filename = file.filename();
	if (file.empty())
	{
		std::cout << "Attempted to write empty file '" << filename << "'" << std::endl;
		return false;
	}

	if (!overwrite && exists(filename))
	{
		if (mVerbose) { std::cout << "Attempted to overwrite a file '" << filename << "' that already exists." << std::endl; }
		return false;
	}

    { //Temporary file
        auto status = FS::status(FS::directory_entry{ FS::absolute(FS::path{filename}.parent_path()) });
        auto perms = status.permissions();
        if(FS::perms::none == (perms & FS::perms::group_write | FS::perms::others_write | FS::perms::owner_write)) {
            if(mVerbose) { std::cout << "Could not open '" << filename << "' for writing. Access denied for directory location." << std::endl; }
        }
    }
    if(!writeBufferToFile(file.bytes(), filename)) {
        if(mVerbose) { std::cout << "Error occured while writing to file '" << filename << "'" << std::endl; }
        return false;
    }

    if(mVerbose) { std::cout << "Wrote '" << file.size() << "' bytes to file '" << filename << "'." << std::endl; }
	return true;
}

/**
 * Gets the base data path.
 */
std::string Filesystem::dataPath() const
{
	if (!isInit()) { throw filesystem_not_initialized(); }
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
	if (!isInit()) { throw filesystem_not_initialized(); }
    if(filename.empty()) {
        if(mVerbose) { std::cout << "Filesystem::workingPath(): empty string provided." << std::endl; }
        return std::string();
    }
    return std::filesystem::path{ filename }.parent_path().string();
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
    namespace FS = std::filesystem;
	if (!isInit()) { throw filesystem_not_initialized(); }

    auto p = FS::path{ path };
    if(!p.has_extension()) {
        if(mVerbose) { std::cout << "Filesystem::extension(): File '" << path << "' has no extension." << std::endl; }
        return std::string();
    }

    if(FS::is_directory(p)) {
        if(mVerbose) { std::cout << "Filesystem::extension(): Given path '" << path << "' is a directory, not a file." << std::endl; }
        return std::string();
    }

    return p.extension().string();
}

void Filesystem::setWorkingDirectory(const std::filesystem::path& p)
{
    try {
        std::filesystem::current_path(p);
        mWorkingDirectory = p;
    } catch(const std::filesystem::filesystem_error& e) {
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


std::filesystem::path Filesystem::getWorkingDirectory()
{
    //Update the cache if it isn't correct.
    const auto& cur_path = std::filesystem::current_path();
    if(mWorkingDirectory != cur_path) {
        setWorkingDirectory(cur_path);
        mWorkingDirectory = cur_path;
    }
    return mWorkingDirectory;
}

bool Filesystem::isInit() const
{
    return mIsInit;
}

bool Filesystem::exists(const std::filesystem::path& p) const
{
    return std::filesystem::exists(p);
}

void Filesystem::forEachFileInFolder(const std::filesystem::path& folderpath, const std::string& validExtensionList /*= std::string{}*/, const std::function<void(const std::filesystem::path&)>& callback /*= [](const std::filesystem::path& p) { (void)p; }*/, bool recursive /*= false*/) const {
    namespace FS = std::filesystem;
    auto preferred_folderpath = folderpath;
    preferred_folderpath.make_preferred();
    bool exists = FS::exists(preferred_folderpath);
    bool is_directory = FS::is_directory(preferred_folderpath);
    bool is_folder = exists && is_directory;
    if(!is_folder) {
        return;
    }
    auto validExtensions = split(toLowercase(validExtensionList));
    if(!recursive) {
        forEachFileInFolders<FS::directory_iterator>(preferred_folderpath, validExtensions, callback);
    } else {
        forEachFileInFolders<FS::recursive_directory_iterator>(preferred_folderpath, validExtensions, callback);
    }
}

void Filesystem::forEachFileInFolder(const std::filesystem::path& folderpath, const std::string& validExtensionList /*= std::string{}*/, const std::function<void(const std::filesystem::path&)>& callback /*= [](const std::filesystem::path& p) { (void)p; }*/, bool recursive /*= false*/) {
    static_cast<const Filesystem&>(*this).forEachFileInFolder(folderpath, validExtensionList, callback, recursive);
}

bool Filesystem::readBufferFromFile(std::vector<unsigned char>& out_buffer, const std::string& filePath) const {
    namespace FS = std::filesystem;
    FS::path p(filePath);
    p.make_preferred();
    bool path_is_directory = FS::is_directory(p);
    bool path_not_exist = !FS::exists(p);
    bool not_valid_path = path_is_directory || path_not_exist;
    if(not_valid_path) {
        return false;
    }

    auto byte_size = static_cast<std::size_t>(FS::file_size(p));
    out_buffer.resize(byte_size);
    std::ifstream ifs;
    ifs.open(p, std::ios_base::binary);
    ifs.read(reinterpret_cast<char*>(out_buffer.data()), out_buffer.size());
    ifs.close();
    out_buffer.shrink_to_fit();
    return true;

}

bool Filesystem::readBufferFromFile(std::string& out_buffer, const std::string& filePath) const {
    namespace FS = std::filesystem;
    FS::path p(filePath);
    p.make_preferred();
    bool path_is_directory = FS::is_directory(p);
    bool path_not_exist = !FS::exists(p);
    bool not_valid_path = path_is_directory || path_not_exist;
    if(not_valid_path) {
        return false;
    }

    std::ifstream ifs{ p };
    //Dump ifstream buffer directly into stringstream and convert to string
    out_buffer = std::string(static_cast<const std::stringstream&>(std::stringstream() << ifs.rdbuf()).str());
    return true;
}

//TODO: Needs Linux/MacOSX implementations!
std::filesystem::path NAS2D::Filesystem::getExePath() const {
    namespace FS = std::filesystem;
    FS::path result{};
#ifdef PLATFORM_WINDOWS
    {
        TCHAR filename[MAX_PATH];
        ::GetModuleFileName(nullptr, filename, MAX_PATH);
        result = FS::path(filename);
        result.make_preferred();
    }
    return result;
#elif PLATFORM_APPLE
    return FS::current_path();
#elif PLATFORM_LINUX
    return FS::current_path();
#endif
}

bool NAS2D::Filesystem::writeBufferToFile(void* buffer, std::size_t size, const std::string& filePath) const {
    namespace FS = std::filesystem;
    FS::path p(filePath);
    p.make_preferred();
    bool not_valid_path = FS::is_directory(p);
    bool invalid = not_valid_path;
    if(invalid) {
        return false;
    }

    std::ofstream ofs;
    ofs.open(p.string(), std::ios_base::binary);
    if(ofs) {
        ofs.write(reinterpret_cast<const char*>(buffer), size);
        ofs.close();
        return true;
    }
    return false;
}

bool NAS2D::Filesystem::writeBufferToFile(const std::string& buffer, const std::string& filePath) const {
    namespace FS = std::filesystem;
    FS::path p(filePath);
    p.make_preferred();
    bool not_valid_path = FS::is_directory(p);
    bool invalid = not_valid_path;
    if(invalid) {
        return false;
    }

    std::ofstream ofs{ p };
    if(ofs) {
        ofs.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
        ofs.close();
        return true;
    }
    return false;
}
