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

#include <vector>
#include <string>
#include <string_view>


namespace NAS2D
{
	class Filesystem
	{
	public:
		enum class WriteFlags
		{
			NoOverwrite,
			Overwrite,
		};

		Filesystem(const std::string& appName, const std::string& organizationName);
		Filesystem(const Filesystem&) = delete;
		Filesystem& operator=(const Filesystem&) = delete;
		Filesystem(Filesystem&&) = delete;
		Filesystem& operator=(Filesystem&&) = delete;
		~Filesystem() = default;

		std::string basePath() const;
		std::string prefPath() const;

		int mountSoftFail(const std::string& path);
		void mount(const std::string& path);
		void mountReadWrite(const std::string& path);
		void unmount(const std::string& path);

		std::vector<std::string> searchPath() const;

		std::vector<std::string> directoryList(const std::string& dir, const std::string& filter = std::string{}) const;

		bool isDirectory(const std::string& path) const;
		void makeDirectory(const std::string& path);

		bool exists(const std::string& path) const;
		void del(const std::string& path);

		std::string readFile(const std::string& filename) const;
		void writeFile(const std::string& filename, const std::string& data, WriteFlags flags = WriteFlags::Overwrite);

		std::string dirSeparator() const;
		std::string parentPath(std::string_view filePath) const;
		std::string extension(std::string_view filePath) const;

	private:
		std::string mBasePath;
		std::string mPrefPath;
		std::string mWritePath;
		std::vector<std::string> mSearchPaths;
	};
}
