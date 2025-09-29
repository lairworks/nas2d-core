// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================

#pragma once

#include "FilesystemPath.h"

#include <vector>
#include <string>
#include <string_view>


namespace NAS2D
{
	struct RealPath : public FilesystemPath {};
	struct VirtualPath : public FilesystemPath {};


	class Filesystem
	{
	public:
		enum class WriteFlags
		{
			NoOverwrite,
			Overwrite,
		};

		static std::string dirSeparator();
		static std::string parentPath(std::string_view filePath);
		static std::string extension(std::string_view filePath);

		Filesystem(const std::string& appName, const std::string& organizationName);
		Filesystem(const Filesystem&) = delete;
		Filesystem& operator=(const Filesystem&) = delete;
		Filesystem(Filesystem&&) = delete;
		Filesystem& operator=(Filesystem&&) = delete;
		~Filesystem() = default;

		RealPath basePath() const;
		RealPath prefPath() const;

		RealPath findInParents(const RealPath& path, const RealPath& startPath) const;
		RealPath findInParents(const RealPath& path, const RealPath& startPath, std::size_t maxLevels) const;

		int mountSoftFail(const RealPath& path);
		void mount(const RealPath& path);
		void mountReadWrite(const RealPath& path);
		void unmount(const RealPath& path);

		std::vector<RealPath> searchPath() const;

		std::vector<VirtualPath> directoryList(const VirtualPath& dir, const std::string& filter = std::string{}) const;

		bool isDirectory(const VirtualPath& path) const;
		void makeDirectory(const VirtualPath& path);

		bool exists(const VirtualPath& path) const;
		void del(const VirtualPath& path);

		std::string readFile(const VirtualPath& filename) const;
		void writeFile(const VirtualPath& filename, const std::string& data, WriteFlags flags = WriteFlags::Overwrite);

	private:
		RealPath mBasePath;
		RealPath mPrefPath;
		RealPath mWritePath;
		std::vector<RealPath> mSearchPaths;
	};
}
