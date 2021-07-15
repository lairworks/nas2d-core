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
	/**
	 * Implements a virtual file system.
	 *
	 * Provides cross-platform and transparent archive Filesystem functions.
	 */
	class Filesystem
	{
	public:
		enum class WriteFlags
		{
			NoOverwrite,
			Overwrite,
		};

		Filesystem(const std::string& argv_0, const std::string& appName, const std::string& organizationName);
		Filesystem(const Filesystem&) = delete;
		Filesystem& operator=(const Filesystem&) = delete;
		Filesystem(Filesystem&&) = delete;
		Filesystem& operator=(Filesystem&&) = delete;
		~Filesystem();

		std::string basePath() const;
		std::string prefPath() const;

		int mountSoftFail(const std::string& path) const;
		void mount(const std::string& path) const;
		void mountReadWrite(const std::string& path) const;
		void unmount(const std::string& path) const;

		std::vector<std::string> searchPath() const;

		std::vector<std::string> directoryList(const std::string& dir, const std::string& filter = std::string{}) const;

		bool isDirectory(const std::string& path) const;
		void makeDirectory(const std::string& path) const;

		bool exists(const std::string& filename) const;
		void del(const std::string& path) const;

		std::string read(const std::string& filename) const;
		void write(const std::string& filename, const std::string& data, WriteFlags flags = WriteFlags::Overwrite) const;

		std::string dirSeparator() const;
		std::string parentPath(std::string_view filePath) const;
		std::string extension(std::string_view filePath) const;

	private:
		std::string mAppName;
		std::string mOrganizationName;
	};
}
