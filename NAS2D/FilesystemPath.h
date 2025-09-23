#pragma once

#include <cstddef>
#include <string_view>
#include <string>


namespace NAS2D
{
	class FilesystemPath
	{
	public:
		FilesystemPath();
		FilesystemPath(const char* path);
		FilesystemPath(std::string_view path);
		FilesystemPath(std::string path);

		explicit operator const std::string&() const;
		bool operator==(const FilesystemPath& other) const;
		bool operator<(const FilesystemPath& other) const;
		FilesystemPath operator/(const FilesystemPath& path) const;

		bool isRelative() const;
		std::size_t componentCount() const;
		FilesystemPath absolute() const;
		FilesystemPath parent() const;
		FilesystemPath stem() const;
		const std::string& string() const;
	private:
		std::string mPath;
	};


	std::string operator+(const char* string, const FilesystemPath& path);
	std::string operator+(const FilesystemPath& path, const char* string);
}
