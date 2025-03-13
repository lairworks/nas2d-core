#include "FilesystemPath.h"

#include <filesystem>
#include <utility>


using namespace NAS2D;


FilesystemPath::FilesystemPath() :
	mPath{}
{
}


FilesystemPath::FilesystemPath(const char* path) :
	mPath{path}
{
}


FilesystemPath::FilesystemPath(std::string path) :
	mPath{std::move(path)}
{
}


FilesystemPath::operator const std::string&() const
{
	return mPath;
}


bool FilesystemPath::operator==(const FilesystemPath& other) const
{
	return mPath == other.mPath;
}


bool FilesystemPath::operator<(const FilesystemPath& other) const
{
	return mPath < other.mPath;
}


FilesystemPath FilesystemPath::operator/(const FilesystemPath& path) const
{
	return (std::filesystem::path{mPath} / std::filesystem::path{std::string{path}}).string();
}


FilesystemPath FilesystemPath::parent() const
{
	// Keep the trailing "/" as part of the folder name
	// This is contrary to how <filesystem> behaves, which strips the trailing "/"
	return ((std::filesystem::path{mPath} / "").parent_path().remove_filename()).string();
}


FilesystemPath FilesystemPath::stem() const
{
	return std::filesystem::path{mPath}.stem().string();
}


const std::string& FilesystemPath::string() const
{
	return mPath;
}


std::string NAS2D::operator+(const char* string, const FilesystemPath& path)
{
	return string + std::string{path};
}


std::string NAS2D::operator+(const FilesystemPath& path, const char* string)
{
	return std::string{path} + string;
}
