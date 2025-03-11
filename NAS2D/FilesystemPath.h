#pragma once

#include <string>
#include <filesystem>


namespace NAS2D
{
	class FilesystemPath
	{
	public:
		FilesystemPath();
		FilesystemPath(const char* path);
		FilesystemPath(std::string path);

		operator std::filesystem::path() const;
		operator std::string() const;

		bool operator==(const FilesystemPath& other) const;
		bool operator<(const FilesystemPath& other) const;
		FilesystemPath operator/(std::string path) const;

		FilesystemPath stem() const;
		std::string string() const;
	private:
		std::string mPath;
	};
}
