// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================
#include "Filesystem.h"
#include "Exception.h"

#include <physfs.h>

#include <climits>
#include <cstring>
#include <string>
#include <iostream>
#include <sstream>
#include <utility>
#include <limits>


using namespace NAS2D;
using namespace NAS2D::Exception;


static bool closeFile(void* file)
{
	if (!file) { return false; }

	if (PHYSFS_close(static_cast<PHYSFS_File*>(file)) != 0) { return true; }

	throw filesystem_file_handle_still_open(PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
}



enum MountPosition
{
	MOUNT_PREPEND = 0,
	MOUNT_APPEND = 1,
};


Filesystem::Filesystem(const std::string& argv_0, const std::string& appName, const std::string& organizationName) :
	mAppName(appName),
	mOrganizationName(organizationName)
{
	if (PHYSFS_isInit()) { throw filesystem_already_initialized(); }

	if (PHYSFS_init(argv_0.c_str()) == 0)
	{
		throw filesystem_backend_init_failure(PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	}
}

Filesystem::~Filesystem()
{
	PHYSFS_deinit();
}

std::string Filesystem::basePath() const
{
	return PHYSFS_getBaseDir();
}

std::string Filesystem::prefPath() const
{
	auto prefDir = PHYSFS_getPrefDir(mOrganizationName.c_str(), mAppName.c_str());
	return (prefDir != nullptr) ? prefDir : "";
}

int Filesystem::mountSoftFail(const std::string& path) const
{
	return PHYSFS_mount(path.c_str(), "/", MountPosition::MOUNT_APPEND);
}

void Filesystem::mount(const std::string& path) const
{
	if (mountSoftFail(path) == 0)
	{
		throw std::runtime_error(std::string("Couldn't add '") + path + "' to search path: " + PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	}
}

void Filesystem::mountReadWrite(const std::string& path) const
{
	mount(path);

	if (PHYSFS_setWriteDir(path.c_str()) == 0)
	{
		throw std::runtime_error(std::string("Couldn't add write folder '") + path + "': " + PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	}
}

void Filesystem::unmount(const std::string& path) const
{
	if (PHYSFS_unmount(path.c_str()) == 0)
	{
		throw std::runtime_error(std::string("Couldn't remove '") + path + "' from search path : " + PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	}
}

std::vector<std::string> Filesystem::searchPath() const
{
	std::vector<std::string> searchPath;

	auto searchPathList = PHYSFS_getSearchPath();
	for (char **i = searchPathList; *i != nullptr; ++i)
	{
		searchPath.push_back(*i);
	}
	PHYSFS_freeList(searchPathList);

	return searchPath;
}

std::vector<std::string> Filesystem::directoryList(const std::string& dir, const std::string& filter) const
{
	char **rc = PHYSFS_enumerateFiles(dir.c_str());

	std::vector<std::string> fileList;
	if (filter.empty())
	{
		for (char **i = rc; *i != nullptr; i++)
		{
			fileList.push_back(*i);
		}
	}
	else
	{
		std::size_t filterLen = filter.size();
		for (char **i = rc; *i != nullptr; i++)
		{
			std::string tmpStr = *i;
			if (tmpStr.rfind(filter, tmpStr.length() - filterLen) != std::string::npos)
			{
				fileList.push_back(*i);
			}
		}
	}

	PHYSFS_freeList(rc);

	return fileList;
}

void Filesystem::del(const std::string& filename) const
{
	if (PHYSFS_delete(filename.c_str()) == 0)
	{
		throw std::runtime_error(std::string("Unable to delete '") + filename + "':" + PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	}
}

File Filesystem::open(const std::string& filename) const
{
	PHYSFS_file* myFile = PHYSFS_openRead(filename.c_str());
	if (!myFile)
	{
		closeFile(myFile);
		throw std::runtime_error(std::string("Unable to load '") + filename + "': " + PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	}

	auto fileLength = PHYSFS_fileLength(myFile);
	if (fileLength < 0 || static_cast<PHYSFS_uint64>(fileLength) > std::numeric_limits<std::size_t>::max())
	{
		closeFile(myFile);
		throw std::runtime_error(std::string("File '") + filename + "' is too large or size could not be determined");
	}

	const auto bufferSize = static_cast<std::size_t>(fileLength);
	std::string fileBuffer;
	fileBuffer.resize(bufferSize);

	const auto actualReadLength = PHYSFS_readBytes(myFile, fileBuffer.data(), bufferSize);
	closeFile(myFile);

	if (actualReadLength < fileLength)
	{
		throw std::runtime_error(std::string("Unable to load '") + filename + "': " + PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	}

	return File{std::move(fileBuffer), filename};
}

void Filesystem::makeDirectory(const std::string& path) const
{
	if (PHYSFS_mkdir(path.c_str()) == 0)
	{
		throw std::runtime_error(std::string("Unable to create directory '" + path + "': ") + PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	}
}

bool Filesystem::isDirectory(const std::string& path) const
{
	PHYSFS_Stat stat;
	return (PHYSFS_stat(path.c_str(), &stat) != 0) && (stat.filetype == PHYSFS_FILETYPE_DIRECTORY);
}

bool Filesystem::exists(const std::string& filename) const
{
	return PHYSFS_exists(filename.c_str()) != 0;
}

void Filesystem::write(const File& file, bool overwrite) const
{
	if (!overwrite && exists(file.filename()))
	{
		throw std::runtime_error(std::string("File exists: ") + file.filename());
	}

	PHYSFS_file* myFile = PHYSFS_openWrite(file.filename().c_str());
	if (!myFile)
	{
		throw std::runtime_error(std::string("Couldn't open '") + file.filename() + "' for writing: " + PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	}

	if (PHYSFS_writeBytes(myFile, file.bytes().c_str(), static_cast<PHYSFS_uint32>(file.size())) < static_cast<PHYSFS_sint64>(file.size()))
	{
		closeFile(myFile);
		throw std::runtime_error(std::string("Error occured while writing to file '") + file.filename() + "': " + PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	}

	closeFile(myFile);
}

std::string Filesystem::workingPath(const std::string& filename) const
{
	if (!filename.empty())
	{
		std::string tmpStr(filename);
		std::size_t pos = tmpStr.rfind("/");
		tmpStr = tmpStr.substr(0, pos + 1);
		return tmpStr;
	}
	else
	{
		return std::string();
	}
}

std::string Filesystem::extension(const std::string& path) const
{

	std::size_t pos = path.find_last_of(".");

	if (pos != std::string::npos)
	{
		return path.substr(pos);
	}
	return std::string();
}

std::string Filesystem::dirSeparator() const
{
	return PHYSFS_getDirSeparator();
}
