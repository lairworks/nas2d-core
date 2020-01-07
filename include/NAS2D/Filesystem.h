// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#pragma once

#include "StringUtils.h"
#include "File.h"

#include <set>
#include <string>


#if defined(__GNUC__) || defined(__GNUG__)
#include <experimental/filesystem>
namespace FS = std::experimental::filesystem;
#else
#include <filesystem>
namespace FS = std::filesystem;
#endif


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
	Filesystem() = delete;
	Filesystem(const Filesystem&) = delete;
	Filesystem& operator=(const Filesystem&) = delete;
	Filesystem(Filesystem&&) = delete;
	Filesystem& operator=(Filesystem&&) = delete;
	~Filesystem() = default;

public:
	void init(const std::string& argv_0, const std::string& appName, const std::string& organizationName, const std::string& dataPath) noexcept;

	const std::string& dataPath() const noexcept;
	std::string workingPath(const std::string& filename) const noexcept;
	StringList searchPath() const noexcept;
	bool mount(const std::string& path) const noexcept;

	StringList directoryList(const std::string& dir, const std::string& filter = std::string{}) const noexcept;

	File open(const std::string& filename) const noexcept;
	bool write(const File& file, bool overwrite = true) const noexcept;
	bool del(const std::string& path) const noexcept;
	bool exists(const std::string& filename) const noexcept;

	std::string extension(const std::string& path) noexcept;

	bool isDirectory(const std::string& path) const noexcept;
	bool makeDirectory(const std::string& path) const noexcept;

private:
	std::string			mDataPath;			/**< Data path string. This will typically be 'data/'. */
	mutable std::set<std::string> mSearchPath{}; //This really should be std::filesystem::path
};

}
