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

#include <physfs.h>

#if defined(__APPLE__)
#include <CoreFoundation/CoreFoundation.h>
#endif

#include <climits>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace NAS2D;
using namespace NAS2D::Exception;

FS::path getPathToBinary(const std::string& argv_0, const std::string& appName, const std::string& organizationName);

FS::path getPathToBinary(const std::string& /*argv_0*/, const std::string& /*appName*/, const std::string& /*organizationName*/) {
	//TODO: Get path to binary from OS.
	return {"./"};
}

NAS2D::Filesystem::Filesystem(const std::string& argv_0, const std::string& appName, const std::string& organizationName)
{
	mExePath = getPathToBinary(argv_0, appName, organizationName);
	//Path to executable is required to exist for program to be well-formed.
	//FS::canonical will automatically throw std::filesystem_error on failure.
	mExePath = FS::canonical(mExePath);
	mExePath = mExePath.make_preferred();
	mSearchPath.insert(mExePath.string());
}

/**
 * Adds a directory or supported archive to the Search Path.
 *
 * \param path	File path to add.
 *
 * \return Returns \c true if successful. Otherwise, returns \c false.
 */
bool Filesystem::mount(const std::string& path) const noexcept
{
	std::clog << "Adding '" << path << "' to search path." << std::endl;

	FS::path p{FS::path{mExePath} / path};
	bool does_exist = exists(p.string());
	if (does_exist)
	{
		auto [where, wasInserted] = mSearchPath.insert(p.string());
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
 * Returns a list of directories in the Search Path.
 */
StringList Filesystem::searchPath() const noexcept
{
	StringList searchPath{std::begin(mSearchPath), std::end(mSearchPath)};
	return searchPath;
}

/**
 * Returns a list of files within a given directory.
 *
 * \param	dir		Directory to search within the searchpath.
 * \param	filter	Extension filter. Provide an empty string for no filter.
 *
 * \note	This function will also return the names of any directories in a specified search path
 */
StringList Filesystem::directoryList(const std::string& dir, const std::string& filter) const noexcept
{
	StringList result{};
	FS::path root{};

	if (dir.empty()) { root = FS::absolute(FS::path{mExePath}); }
	
	if (!FS::is_directory(root)) { return {}; }

	for (auto& f : FS::directory_iterator{root})
	{
		if (filter.empty())
		{
			if (FS::is_regular_file(f))
			{
				result.push_back(f.path().filename().string());
			}
		}
		else
		{
			if (FS::is_regular_file(f))
			{
				if (f.path().extension() == filter)
				{
					result.push_back(f.path().filename().string());
				}
			}
		}
	}

	return result;
}


/**
 * Deletes a specified file.
 *
 * \param	filename	Path of the file to delete relative to the Filesystem root directory.
 *
 * \note	This function is not named 'delete' due to
 *			language limitations.
 */
bool Filesystem::del(const std::string& filename) const noexcept
{
	std::error_code ec{};
	if (!FS::remove(FS::path{filename}, ec))
	{
		std::cerr << "Unable to delete " << filename << ".\n";
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
File Filesystem::open(const std::string& filename) const noexcept
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
	const auto p = FS::path{filename};
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
	return File(buffer, filename);
}


/**
 * Creates a new directory within the primary search path.
 *
 * \param path	Path of the directory to create.
 *
 * \return Returns \c true if successful. Otherwise, returns \c false.
 */
bool Filesystem::makeDirectory(const std::string& path) const noexcept
{
	std::error_code ec{};
	auto p = FS::path{path};
	return FS::create_directories(p, ec);
}


/**
 * Determines if a given path is a directory rather than a file.
 *
 * \param path	Path to check.
 */
bool Filesystem::isDirectory(const std::string& path) const noexcept
{
	return FS::is_directory(FS::path{path});
}


/**
 * Checks for the existence of a file.
 *
 * \param	filename	File path to check.
 *
 * Returns Returns \c true if the specified file exists. Otherwise, returns \c false.
 */
bool Filesystem::exists(const std::string& filename) const noexcept
{
	std::error_code ec{};
	bool does_not_exist = !FS::exists(filename, ec);
	if (does_not_exist)
	{
		for (const auto& cur_path_string : mSearchPath)
		{
			const auto cur_path = FS::path{cur_path_string} / filename;
			if(FS::exists(cur_path, ec))
			{
				return true;
			}
		}
		return false;
	}

	return true;
}


/**
 * Writes a file to disk.
 *
 * \param	file		A reference to a \c const \c File object.
 * \param	overwrite	Flag indicating if a file should be overwritten if it already exists. Default is true.
 *
 * \return Returns \c true if successful. Otherwise, returns \c false.
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
 * Convenience function to get the working directory of a file.
 *
 * \param filename	A file path.
 *
 * \note	File paths should not have any trailing '/' characters.
 */
std::string Filesystem::workingPath(const std::string& filename) const noexcept
{
	if (filename.empty())
	{
		#if defined(DEBUG)
		std::cerr << "Filesystem::workingPath(): empty string provided.\n";
		#endif
	
		return {};
	}

	auto p = FS::path{filename};
	return p.parent_path().make_preferred().string();
}


/**
 * Gets the extension of a given file path.
 *
 * \param	path	Path to check for an extension.
 *
 * \return	Returns a string containing the file extension. An empty string will be
 *			returned if the file has no extension or if it's a directory.
 */
std::string Filesystem::extension(const std::string& path) noexcept
{
	const auto p = FS::path{path};
	if (FS::is_directory(p))
	{
		return {};
	}
	if (p.has_extension())
	{
		return p.extension().string();
	}
	if (p.stem() == p.filename())
	{
		if (p.stem().string().find_first_of('.') != std::string::npos)
		{
			return p.stem().string();
		}
	}

	return {};

}
