// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Filesystem.h"

#include "NAS2D/Exception.h"

#include <climits>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

#if defined(__APPLE__)
#include <limits.h>
#include <mach-o/dyld.h>
#endif

using namespace NAS2D;
using namespace NAS2D::Exception;

FS::path getPathToBinary([[maybe_unused]] const std::string& argv_0, [[maybe_unused]] const std::string& appName, [[maybe_unused]] const std::string& organizationName);
FS::path getPathToBinaryWindows();
FS::path getPathToBinaryApple();
FS::path getPathToBinaryLinux();


/**
 * Returns the canonical, preferred path to the current process/binary, or the current path if there was an error.
 * 
 */
FS::path getPathToBinary([[maybe_unused]] const std::string& argv_0, [[maybe_unused]] const std::string& appName, [[maybe_unused]] const std::string& organizationName)
{
	FS::path result{"./"};
#if defined(_WIN32)
	result = getPathToBinaryWindows();
#elif defined(__APPLE__)
	result = getPathToBinaryApple();
#elif defined(__linux__)
	result = getPathToBinaryLinux();
#endif
	result = FS::canonical(result);
	result = result.make_preferred();
	return result;
}

FS::path getPathToBinaryWindows()
{
	FS::path result{"./"};
#if defined(_WIN32)
	{
		std::basic_string<TCHAR> filename(MAX_PATH, '\0');
		while (DWORD buffer_length = ::GetModuleFileName(nullptr, filename.data(), static_cast<DWORD>(filename.size())))
		{
			if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			{
				filename.resize(filename.size() * 2);
				continue;
			}
			filename = filename.substr(0, buffer_length);
			result = FS::path(filename);
			break;
		}
	}
#endif
	return result;
}

FS::path getPathToBinaryApple()
{
	FS::path result{"./"};
#if defined(__APPLE__)
	{
		uint32_t size = 0;
		//Will return -1 at first because size is not large enough.
		//_NSGetExecutablePath sets size if it fails.
		_NSGetExecutablePath(nullptr, &size);
		{
			std::string filename{};
			filename.resize(size);
			if (_NSGetExecutablePath(filename.data(), &size) == 0)
			{
				result = FS::path{filename};
				//Path may be a symlink!
				while (FS::is_symlink(result))
				{
					result = FS::read_symlink(result);
				}
			}
		}
	}
#endif
	return result;
}

FS::path getPathToBinaryLinux()
{
	FS::path result{"./"};
#if defined(__linux__)
	{
		//Using '/proc/self/exe' isn't necessarily portable on mobile or older platforms.
		result = FS::path{"/proc/self/exe"};
		while (FS::is_symlink(result))
		{
			result = FS::read_symlink(result);
		}
	}
#endif
	return result;
}

NAS2D::Filesystem::Filesystem(const std::string& argv_0, const std::string& appName, const std::string& organizationName)
{
	mExePath = getPathToBinary(argv_0, appName, organizationName);
	//Path to executable is required to exist for program to be well-formed.
	//FS::canonical will automatically throw std::filesystem_error on failure.
	mWritePath = FS::absolute(mWritePath).make_preferred();

}

/**
 * Adds the default paths to the Search Path:
 * Default read path: the path to the application binary.
 * Default read/write path: ./data path next to the application binary.
 */
void NAS2D::Filesystem::mountDefault() noexcept
{
	mount(mExePath);
	mount(mWritePath);
}

/**
 * Adds a directory or supported archive to the Search Path.
 *
 * \param path	File path to add.
 */
bool Filesystem::mount(const FS::path& path) const noexcept
{
	std::clog << "Adding '" << path << "' to search path." << std::endl;

	bool does_exist = exists(path);
	if (does_exist)
	{
		auto [where, wasInserted] = mSearchPath.insert(path);
		if (!wasInserted)
		{
			std::cerr << "Couldn't add " << path << " to search path.\n";
			return false;
		}
		return true;
	}
	else
	{
		std::cerr << "Couldn't add " << path << " to search path.\n";
		return false;
	}
}

/**
 * Removes a directory or supported archive from the Search Path.
 *
 * \param path	File path to remove.
 *
 * \return Returns \c true if successful. Otherwise, returns \c false.
 */
bool NAS2D::Filesystem::unmount(const FS::path& path) const noexcept
{
	std::clog << "Removing '" << path << "' from search path." << std::endl;

	bool does_exist = exists(path);
	if (does_exist)
	{
		auto erase_count = mSearchPath.erase(path);
		if (!erase_count)
		{
			std::cerr << "Couldn't remove " << path << " from search path.\n";
			return false;
		}
		return true;
	}
	else
	{
		std::cerr << "Couldn't remove " << path << " from search path.\n";
		return false;
	}
}

/**
 * Returns a list of files within a given directory.
 *
 * \param	dir		Directory to search within the searchpath.
 * \param	filter	Extension filter. Provide an empty string for no filter.
 *
 * \note	This function will also return the names of any directories in a specified search path
 */
PathList Filesystem::directoryList(const FS::path& dir, const std::string& filter) const noexcept
{
	PathList result{};
	FS::path root{};

	if (dir.empty()) { root = FS::absolute(mExePath.parent_path()); }

	if (!FS::is_directory(root)) { return {}; }

	for (auto& f : FS::directory_iterator{root})
	{
		if (filter.empty())
		{
			if (FS::is_regular_file(f))
			{
				result.push_back(f.path().filename());
			}
		}
		else
		{
			if (FS::is_regular_file(f))
			{
				if (f.path().extension() == filter)
				{
					result.push_back(f.path().filename());
				}
			}
		}
	}

	return result;
}

/**
 * Returns a list of directories in the Search Path.
 */
PathList Filesystem::searchPath() const noexcept
{
	PathList searchPath{std::begin(mSearchPath), std::end(mSearchPath)};
	return searchPath;
}

/**
 * Opens a file.
 *
 * \param filename	Path of the file to load.
 *
 * \return Returns a File.
 */
File Filesystem::open(const FS::path& filename) const noexcept
{
#if defined(DEBUG)
	std::cerr << "Attempting to load '" << filename << "...";
#endif

	if (!exists(filename))
	{
#if defined(DEBUG)
		std::cerr << "Failed. File does not exist." << std::endl;
#endif

		return File();
	}

	std::error_code ec{};
	const auto p = FS::absolute(filename).make_preferred();
	const auto size = FS::file_size(p, ec);
	if (size == static_cast<std::uintmax_t>(-1))
	{
#if defined(DEBUG)
		std::cerr << "Failed. std::filesystem::file_size failed to get the size." << std::endl;
#endif
		return File();
	}

	std::ifstream ifs{p};
	std::string buffer = std::string(static_cast<const std::stringstream&>(std::stringstream() << ifs.rdbuf()).str());
	return File(buffer, p);
}

/**
 * Writes a file to disk.
 *
 * \param	file		A reference to a \c const \c File object.
 * \param	overwrite	Flag indicating if a file should be overwritten if it already exists. Default is true.
 */
bool Filesystem::write(const File& file, bool overwrite) const noexcept
{
	if (file.empty())
	{
		std::cerr << "Attempted to write empty file '" << file.filename() << std::endl;
		return false;
	}

	if (!overwrite && exists(file.filename()))
	{
#if defined(DEBUG)
		std::cerr << "Attempted to overwrite a file '" << file.filename() << "' that already exists." << std::endl;
#endif

		return false;
	}

	std::ofstream myFile{FS::path{file.filename()}, std::ios_base::binary};
	if (!myFile)
	{
#if defined(DEBUG)
		std::cerr << "Couldn't open '" << file.filename() << "' for writing." << std::endl;
#endif

		return false;
	}

	if (!myFile.write(file.raw_bytes(), file.bytes().size()))
	{
#if defined(DEBUG)
		std::cerr << "Error occured while writing to file '" << file.filename() << std::endl;
#endif
	}
	else
	{
#if defined(DEBUG)
		std::clog << "Wrote " << file.bytes().size() << " bytes to file '" << file.filename() << std::endl;
#endif
	}
	return true;
}

/**
 * Deletes a specified file.
 *
 * \param	filename	Path of the file to delete relative to the Filesystem root directory.
 *
 * \note	This function is not named 'delete' due to
 *			language limitations.
 */
bool Filesystem::del(const FS::path& filename) const noexcept
{
	std::error_code ec{};
	if (!FS::remove(filename, ec))
	{
		std::cerr << "Unable to delete " << filename << ".\n";
		return false;
	}

	return true;
}

/**
 * Creates a new directory within the primary search path.
 *
 * \param path	Path of the directory to create.
 *
 * \return Returns \c true if successful. Otherwise, returns \c false.
 */
bool Filesystem::makeDirectory(const FS::path& path) const noexcept
{
	std::error_code ec{};
	if (FS::create_directories(path, ec))
	{
		return true;
	}
	else
	{
		if (FS::exists(path))
		{
			return true;
		}
	}
	return false;
}

/**
 * Checks for the existence of a file.
 *
 * \param	filename	File path to check.
 *
 * Returns Returns \c true if the specified file exists. Otherwise, returns \c false.
 */
bool Filesystem::exists(FS::path filename) const noexcept
{
	std::error_code ec{};
	bool does_not_exist = !FS::exists(filename, ec);
	if (does_not_exist)
	{
		for (const auto& cur_path : mSearchPath)
		{
			const auto cur_path_with_filename = FS::path{cur_path} / filename;
			if (FS::exists(cur_path_with_filename, ec))
			{
				return true;
			}
		}
		return false;
	}

	return true;
}

/**
 * Convenience function to get the working directory of a file.
 *
 * \param filename	A file path.
 *
 * \note	File paths should not have any trailing '/' characters.
 */
FS::path Filesystem::workingPath(const FS::path& filename) const noexcept
{
	if (filename.empty())
	{
#if defined(DEBUG)
		std::cerr << "Filesystem::workingPath(): empty string provided.\n";
#endif

		return {};
	}

	return filename.parent_path().make_preferred();
}

/**
 * Gets the extension of a given file path.
 *
 * \param	path	Path to check for an extension.
 *
 * \return	Returns a string containing the file extension. An empty string will be
 *			returned if the file has no extension or if it's a directory.
 */
FS::path Filesystem::extension(const FS::path& path) noexcept
{
	if (FS::is_directory(path))
	{
		return {};
	}
	if (path.has_extension())
	{
		return path.extension();
	}
	if (path.stem() == path.filename())
	{
		if (path.stem().string().find_first_of('.') != std::string::npos)
		{
			return path.stem();
		}
	}

	return {};
}

/**
 * Determines if a given path is a directory rather than a file.
 *
 * \param path	Path to check.
 */
bool Filesystem::isDirectory(const FS::path& path) const noexcept
{
	return FS::is_directory(path);
}
