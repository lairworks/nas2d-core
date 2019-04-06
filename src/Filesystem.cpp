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

#ifdef PLATFORM_APPLE
#include <CoreFoundation/CoreFoundation.h>
#endif

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
    if(!isInit()) { throw filesystem_not_initialized(); }

    auto p = FS::path{ path };

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
    for(const auto& p : this->mSearchPath) { searchPath.push_back(p.string()); }
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

    try {
        FS::remove(FS::path{ filename });
	    return true;
    }
    catch(const FS::filesystem_error& e)
    {
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
    if(!this->readBufferFromFile(fileBuffer, filename))
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
	if (!isInit()) { throw filesystem_not_initialized(); }
    return exists(FS::path{ filename });
}

/**
 * Writes a file to disk.
 *
 * \param	file		A reference to a \c const \c File object.
 * \param	overwrite	Flag indicating if a file should be overwritten if it already exists. Default is true.
 *
 * \return Returns \c true if successful. Otherwise, returns \c false.
 */
bool Filesystem::write(const File& file) const
{
	if (!isInit()) { throw filesystem_not_initialized(); }

    const auto& filename = file.filename();
	if (file.empty())
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
    if(filename.empty())
    {
        return std::string();
    }
    return FS::path{ filename }.parent_path().string();
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
	if (!isInit()) { throw filesystem_not_initialized(); }

    auto p = FS::path{ path };
    if(!p.has_extension())
    {
        return std::string();
    }

    if(FS::is_directory(p))
    {
        return std::string();
    }

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
        mWorkingDirectory = cur_path;
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

void Filesystem::forEachFileInFolder(const FS::path& folderpath, const std::string& validExtensionList /*= std::string{}*/, const std::function<void(const FS::path&)>& callback /*= [](const FS::path& p) { (void)p; }*/, bool recursive /*= false*/) const
{
    auto preferred_folderpath = folderpath;
    preferred_folderpath.make_preferred();
    bool exists = FS::exists(preferred_folderpath);
    bool is_directory = FS::is_directory(preferred_folderpath);
    bool is_folder = exists && is_directory;
    if(!is_folder) { return; }
    
    auto validExtensions = split(toLowercase(validExtensionList));
    
    if(!recursive)
    {
        forEachFileInFolders<FS::directory_iterator>(preferred_folderpath, validExtensions, callback);
    }
    else
    {
        forEachFileInFolders<FS::recursive_directory_iterator>(preferred_folderpath, validExtensions, callback);
    }
}

void Filesystem::forEachFileInFolder(const FS::path& folderpath, const std::string& validExtensionList /*= std::string{}*/, const std::function<void(const FS::path&)>& callback /*= [](const FS::path& p) { (void)p; }*/, bool recursive /*= false*/)
{
    static_cast<const Filesystem&>(*this).forEachFileInFolder(folderpath, validExtensionList, callback, recursive);
}

bool Filesystem::readBufferFromFile(std::vector<unsigned char>& out_buffer, const std::string& filePath) const
{
    FS::path p(filePath);
    p.make_preferred();
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

bool Filesystem::readBufferFromFile(std::string& out_buffer, const std::string& filePath) const
{
    FS::path p(filePath);
    p.make_preferred();
    bool path_is_directory = FS::is_directory(p);
    bool path_not_exist = !FS::exists(p);
    bool not_valid_path = path_is_directory || path_not_exist;
    if(not_valid_path) { return false; }

    std::ifstream ifs{ p };
    //Dump ifstream buffer directly into stringstream and convert to string
    out_buffer = std::string(static_cast<const std::stringstream&>(std::stringstream() << ifs.rdbuf()).str());
    return true;
}

FS::path NAS2D::Filesystem::getExePath() const
{
    //Cache result because OS calls may dynamically allocate
    //and that can get expensive.
    if(mExePath.empty())
    {
#ifdef PLATFORM_WINDOWS
        mExePath = DoWindowsQueryExePath();
#elif PLATFORM_APPLE
        mExePath = DoAppleQueryExePath();
#elif PLATFORM_LINUX
        mExePath = DoLinuxQueryExePath();
#endif
    }
    return mExePath;
}

#ifdef PLATFORM_WINDOWS
FS::path NAS2D::Filesystem::DoWindowsQueryExePath() const
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
FS::path NAS2D::Filesystem::DoWindowsQueryExePath() const
{
    return FS::path{};
}
#endif

#ifdef PLATFORM_LINUX
FS::path NAS2D::Filesystem::DoLinuxQueryExePath() const
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
FS::path NAS2D::Filesystem::DoLinuxQueryExePath() const
{
    return FS::path{};
}
#endif

#ifdef PLATFORM_APPLE
FS::path NAS2D::Filesystem::DoAppleQueryExePath() const
{
    uint32_t size = 0;
    std::string path{};
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
FS::path NAS2D::Filesystem::DoAppleQueryExePath() const
{
    return FS::path{};
}
#endif

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

bool NAS2D::Filesystem::writeBufferToFile(const std::string& buffer, const std::string& filePath) const
{
    FS::path p(filePath);
    p.make_preferred();
    bool not_valid_path = FS::is_directory(p);
    bool invalid = not_valid_path;
    if(invalid) { return false; }

    std::ofstream ofs{ p };
    if(ofs)
    {
        ofs.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
        return true;
    }
    return false;
}
