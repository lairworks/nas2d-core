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

#include <SDL2/SDL.h>
#include <SDL2/SDL_filesystem.h>

#include <algorithm>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <memory>
#include <system_error>


using namespace NAS2D;


namespace
{
	using SdlStringDeleter = decltype([](char* string) { SDL_free(string); });
	using SdlString = std::unique_ptr<char, SdlStringDeleter>;


	std::string getBasePath()
	{
		const auto pathPtr = SdlString{SDL_GetBasePath()};
		if (pathPtr.get() == nullptr)
		{
			throw std::runtime_error("Error getting BasePath: " + std::string{SDL_GetError()});
		}
		return pathPtr.get();
	}


	std::string getPrefPath(const std::string& appName, const std::string& organizationName)
	{
		const auto pathPtr = SdlString{SDL_GetPrefPath(organizationName.c_str(), appName.c_str())};
		if (pathPtr.get() == nullptr)
		{
			throw std::runtime_error("Error getting PrefPath: " + std::string{SDL_GetError()});
		}
		return pathPtr.get();
	}


	bool hasFileSuffix(const std::string& filePath, const std::string& suffix)
	{
		return filePath.rfind(suffix, filePath.length() - suffix.length()) != std::string::npos;
	}


	std::string findFirstPath(const std::filesystem::path& path, const std::vector<std::filesystem::path>& searchPaths)
	{
		for (const auto& searchPath : searchPaths)
		{
			const auto& filePath = searchPath / path;
			if (std::filesystem::exists(filePath))
			{
				return filePath.string();
			}
		}
		return {};
	}


	std::string errorDescription()
	{
		return std::error_code{errno, std::generic_category()}.message();
	}
}


/**
 * Gets the dir separator for the current platform
 *
 * The dir separator separates the directories within a path. This is typically either "\\" for Windows, or "/" for Linux.
 *
 * \note The path separator may be more than one character
 */
std::string Filesystem::dirSeparator()
{
	return {std::filesystem::path::preferred_separator};
}


/**
 * Convenience function to get the parent directory of a file.
 *
 * \param filePath A file path.
 *
 * \return The path up to and including the last '/', or empty string if no '/'
 */
std::string Filesystem::parentPath(std::string_view filePath)
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
std::string Filesystem::extension(std::string_view filePath)
{
	return std::filesystem::path{filePath}.extension().string();
}


Filesystem::Filesystem(const std::string& appName, const std::string& organizationName) :
	mBasePath{getBasePath()},
	mPrefPath{getPrefPath(appName, organizationName)},
	mWritePath{},
	mSearchPaths{}
{
}


/**
 * Determines the path to the folder where the executable is located
 *
 * The base path may or may not be the current working directory.
 */
std::filesystem::path Filesystem::basePath() const
{
	return mBasePath;
}


/**
 * Determines the path to the folder where user preferences are stored
 *
 * The user should have write access to the preferences folder, which is generally under their user folder.
 *
 * \note This path is dependent on the Operating System (OS).
 */
std::filesystem::path Filesystem::prefPath() const
{
	return mPrefPath;
}


/**
 * Mount a folder with read access
 *
 * \param path	File path to add.
 *
 * \return Nonzero on success, zero on error.
 */
int Filesystem::mountSoftFail(const std::filesystem::path& path)
{
	mSearchPaths.push_back(path);
	return std::filesystem::exists(path);
}


/**
 * Mount a folder with read access
 *
 * \param path	File path to add.
 */
void Filesystem::mount(const std::filesystem::path& path)
{
	if (mountSoftFail(path) == 0)
	{
		throw std::runtime_error("Error mounting search path: " + path.string() + " : " + errorDescription());
	}
}


/**
 * Mount a folder with read and write access
 *
 * \param path	File path to add.
 */
void Filesystem::mountReadWrite(const std::filesystem::path& path)
{
	std::filesystem::create_directories(path);
	mWritePath = path;

	// Mount for read access
	mount(path);
}


/**
 * Removes a directory from the Search Path.
 *
 * \param path	File path to remove.
 */
void Filesystem::unmount(const std::filesystem::path& path)
{
	mSearchPaths.erase(std::remove(mSearchPaths.begin(), mSearchPaths.end(), path), mSearchPaths.end());
}


/**
 * Returns a list of directories in the Search Path.
 */
std::vector<std::filesystem::path> Filesystem::searchPath() const
{
	return mSearchPaths;
}


/**
 * Returns a list of files within a given directory.
 *
 * \param	dir	Directory to search within the searchpath.
 * \param	filter		Optional extension filter. Only use the extension without a wildcard (*) character or period (e.g., 'png' vs '*.png' or '.png').
 *
 * \note	This function will also return the names of any directories in a specified search path
 */
std::vector<std::filesystem::path> Filesystem::directoryList(const std::filesystem::path& dir, const std::string& filter) const
{
	std::vector<std::filesystem::path> fileList;

	for (const auto& searchPath : mSearchPaths)
	{
		const auto dirPath = searchPath / dir;
		if (std::filesystem::is_directory(dirPath))
		{
			for (const auto& dirEntry : std::filesystem::directory_iterator(dirPath))
			{
				const auto& filePath = dirEntry.path().filename().generic_string();
				if (hasFileSuffix(filePath, filter))
				{
					fileList.push_back(filePath);
				}
			}
		}
	}

	return fileList;
}


/**
 * Determines if a given path is a directory rather than a file.
 *
 * \param path	Path to check.
 */
bool Filesystem::isDirectory(const std::filesystem::path& path) const
{
	const auto& filePath = findFirstPath(path, mSearchPaths);
	return std::filesystem::is_directory(filePath);
}


/**
 * Creates a new directory within the primary search path.
 *
 * \param path	Path of the directory to create.
 */
void Filesystem::makeDirectory(const std::filesystem::path& path)
{
	const auto& filePath = mWritePath / path;
	std::filesystem::create_directories(filePath);
}


/**
 * Checks for the existence of a file.
 *
 * \param	path	File path to check.
 *
 * Returns Returns \c true if the specified file or directory exists. Otherwise, returns \c false.
 */
bool Filesystem::exists(const std::filesystem::path& path) const
{
	const auto& filePath = findFirstPath(path, mSearchPaths);
	return !filePath.empty();
}


/**
 * Deletes a specified file.
 *
 * \param	filename	Path of the file to delete relative to the Filesystem root directory.
 */
void Filesystem::del(const std::filesystem::path& filename)
{
	const auto& filePath = mWritePath / filename;
	if (!std::filesystem::remove(filePath))
	{
		throw std::runtime_error("Error deleting file: " + filename.string() + " : " + errorDescription());
	}
}


std::string Filesystem::readFile(const std::filesystem::path& filename) const
{
	const auto& filePath = findFirstPath(filename, mSearchPaths);
	if (filePath.empty())
	{
		throw std::runtime_error("Error opening file: " + filename.string() + " : File does not exist");
	}

	std::ifstream file{filePath, std::ios::in | std::ios::binary};
	if (!file)
	{
		throw std::runtime_error("Error opening file: " + filename.string() + " : " + errorDescription());
	}

	const auto fileSize = std::filesystem::file_size(filePath);
	if constexpr (std::numeric_limits<decltype(fileSize)>::max() > std::numeric_limits<std::string::size_type>::max())
	{
		if (fileSize > std::numeric_limits<std::string::size_type>::max())
		{
			throw std::runtime_error("Error opening file: " + filename.string() + " : File too large");
		}
	}
	const auto bufferSize = static_cast<std::string::size_type>(fileSize);

	std::string fileBuffer;
	fileBuffer.resize(bufferSize);

	file.read(fileBuffer.data(), static_cast<std::streamsize>(bufferSize));
	if (!file)
	{
		throw std::runtime_error("Error reading file: " + filename.string() + " : " + errorDescription());
	}

	return fileBuffer;
}


void Filesystem::writeFile(const std::filesystem::path& filename, const std::string& data, WriteFlags flags)
{
	if (flags != WriteFlags::Overwrite && exists(filename))
	{
		throw std::runtime_error("Overwrite flag not specified and file already exists: " + filename.string());
	}

	const auto& filePath = mWritePath / filename;
	std::ofstream file{filePath, std::ios::out | std::ios::binary};
	if (!file)
	{
		throw std::runtime_error("Error opening file for writing: " + filename.string() + " : " + errorDescription());
	}

	file << data;
	if (!file)
	{
		throw std::runtime_error("Error writing file: " + filename.string() + " : " + errorDescription());
	}
}
