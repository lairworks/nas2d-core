#include "FilesystemPath.h"


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
	mPath{path}
{
}


FilesystemPath::operator std::filesystem::path() const
{
	return mPath;
}


FilesystemPath::operator std::string() const
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


FilesystemPath FilesystemPath::operator/(std::string path) const
{
	return (std::filesystem::path{mPath} / std::filesystem::path{path}).string();
}


FilesystemPath FilesystemPath::stem() const
{
	return std::filesystem::path{mPath}.stem().string();
}


std::string FilesystemPath::string() const
{
	return mPath;
}
