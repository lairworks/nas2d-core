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
#include <vector>

#if defined(__GNUC__) || defined(__GNUG__)
#include <experimental/filesystem>
namespace FS = std::experimental::filesystem;
#else
#include <filesystem>
namespace FS = std::filesystem;
#endif


namespace NAS2D {

	using PathList = std::vector<FS::path>;

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

	Filesystem(const std::string& argv_0, const std::string& appName, const std::string& organizationName);

	void mountDefault() noexcept;
	bool mount(const FS::path& path) const noexcept;
	bool unmount(const FS::path& path) const noexcept;

	PathList directoryList(const FS::path& dir, const std::string& filter = std::string{}) const noexcept;
	PathList searchPath() const noexcept;

	File open(const FS::path& filename) const noexcept;
	bool write(const File& file, bool overwrite = true) const noexcept;
	bool del(const FS::path& path) const noexcept;
	bool makeDirectory(const FS::path& path) const noexcept;

	bool exists(FS::path filename) const noexcept;
	FS::path workingPath(const FS::path& filename) const noexcept;
	FS::path extension(const FS::path& path) noexcept;
	bool isDirectory(const FS::path& path) const noexcept;

private:
	FS::path mExePath{"./"};
	FS::path mWritePath{"./data"};
	mutable std::set<FS::path> mSearchPath{}; //This really should be std::filesystem::path
};

}
