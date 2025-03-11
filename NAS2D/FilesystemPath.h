#pragma once

#include <string>


namespace NAS2D
{
	class FilesystemPath
	{
	public:
		FilesystemPath();
		FilesystemPath(const char* path);
		FilesystemPath(std::string path);

		bool operator==(const FilesystemPath& other) const;
		bool operator<(const FilesystemPath& other) const;
		FilesystemPath operator/(const FilesystemPath& path) const;

		FilesystemPath stem() const;
		std::string string() const;
	private:
		std::string mPath;
	};
}
