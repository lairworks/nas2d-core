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
#include <filesystem>


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

		static std::string dirSeparator();
		static std::string parentPath(std::string_view filePath);
		static std::string extension(std::string_view filePath);

		Filesystem(const std::string& appName, const std::string& organizationName);
		Filesystem(const Filesystem&) = delete;
		Filesystem& operator=(const Filesystem&) = delete;
		Filesystem(Filesystem&&) = delete;
		Filesystem& operator=(Filesystem&&) = delete;
		~Filesystem() = default;

		std::filesystem::path basePath() const;
		std::filesystem::path prefPath() const;

		int mountSoftFail(const std::filesystem::path& path);
		void mount(const std::filesystem::path& path);
		void mountReadWrite(const std::filesystem::path& path);
		void unmount(const std::filesystem::path& path);

		std::vector<std::filesystem::path> searchPath() const;

		std::vector<std::string> directoryList(const std::string& dir, const std::string& filter = std::string{}) const;

		bool isDirectory(const std::string& path) const;
		void makeDirectory(const std::string& path);

		bool exists(const std::string& path) const;
		void del(const std::string& path);

		std::string readFile(const std::string& filename) const;
		void writeFile(const std::string& filename, const std::string& data, WriteFlags flags = WriteFlags::Overwrite);

	private:
		std::filesystem::path mBasePath;
		std::filesystem::path mPrefPath;
		std::filesystem::path mWritePath;
		std::vector<std::filesystem::path> mSearchPaths;
	};
}
