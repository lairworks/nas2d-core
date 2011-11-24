// ==================================================================================
// = NAS2D
// = Copyright © 2008 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software as long under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Exception.h"
#include "NAS2D/Filesystem.h"
#include "NAS2D/Timer.h"

#include <iostream>
#include <sstream>
#include <limits.h>


#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif

using namespace std;

const unsigned int	CLOSE_MAX_ATTEMPTS			= 3;
const unsigned int	CLOSE_ATTEMPT_TIMEOUT		= 5;	// Seconds


/**
 * Initializes the Filesystem class, sets up PhysicsFS and sets any necessary
 * platform directory strings.
 */
Filesystem::Filesystem(const string& argv_0, const string& startPath):	mVerbose(false)
{

	init(argv_0, startPath);
}


Filesystem::Filesystem(): mVerbose(false)
{
}


/**
 * Shuts down PhysFS and cleans up.
 */
Filesystem::~Filesystem()
{
	PHYSFS_deinit();
	cout << "Filesystem Terminated." << endl;
}


/**
 * Shuts down PhysFS and cleans up.
 */
void Filesystem::init(const std::string& argv_0, const std::string& startPath)
{
	// Throw an exception if PhysFS couldn't start properly.
	if(PHYSFS_init(argv_0.c_str()) == 0)
		throw Exception(602, "Filesystem::Filesystem(): Unable to initialize PhysFS!", PHYSFS_getLastError());

	mStartPath = startPath;

#if defined(WIN32) || defined(__APPLE__)
	std::string basePath = PHYSFS_getBaseDir();
	mDirSeparator = PHYSFS_getDirSeparator();

	if(mStartPath.size() < 1)
		mDataPath = basePath;
	else
		mDataPath = basePath + mStartPath + mDirSeparator;

	if(PHYSFS_addToSearchPath(mDataPath.c_str(), 0) == 0)
		cout << "Couldn't find data path '" << mDataPath << "'. " << PHYSFS_getLastError() << "." << endl;

	PHYSFS_setWriteDir(mDataPath.c_str());

#elif defined(__linux__)
	std::string mTempWritePath = PHYSFS_getUserDir();
	std::string mDirName = ".lom/data/";
	mDataPath = mTempWritePath + mDirName;

	PHYSFS_setWriteDir(mTempWritePath.c_str());

	if(PHYSFS_exists(mDataPath.c_str()) == 0)
	{
		makeDirectory(mDirName.c_str());
	}

	PHYSFS_setWriteDir(mDataPath.c_str());

	if(PHYSFS_addToSearchPath(mDataPath.c_str(), 0) == 0)
	{
		mErrorMessages.push_back(PHYSFS_getLastError());
		Logger::log << "(FSYS) Couldn't find data path '" << mDataPath << "'. " << PHYSFS_getLastError() << "." << endl;
	}

#else
	#error Filesystem support for this platform has not been developed.
#endif
}


/**
 * Adds a directory or supported archive to the Search Path.
 *
* \return Returns \c true if successful. Otherwise, returns \c false.
 */
bool Filesystem::addToSearchPath(const string& pathName) const
{
	if(mVerbose) cout << "Adding '" << pathName << "' to search path." << endl;

	if(PHYSFS_exists(pathName.c_str()) == 0)
	{
		mErrorMessages.push_back(PHYSFS_getLastError());
		return false;
	}

#ifdef WIN32
	string searchPath = mDataPath + pathName;

	if(PHYSFS_addToSearchPath(searchPath.c_str(), 1) == 0)
	{
		mErrorMessages.push_back(PHYSFS_getLastError());
		cout << "Couldn't add '" << pathName << "' to search path. " << mErrorMessages.back() << "." << endl;
		return false;
	}
#elif __APPLE__
	string searchPath(mDataPath + pathName);

	if(PHYSFS_addToSearchPath(searchPath.c_str(), 1) == 0)
	{
		mErrorMessages.push_back(PHYSFS_getLastError());
		cout << "Couldn't add '" << pathName << "' to search path. " << mErrorMessages.back() << "." << endl;
		return false;
	}
#elif __linux__
    string searchPath = mDataPath + pathName;

    if(PHYSFS_addToSearchPath(searchPath.c_str(), 1) == 0)
    {
        mErrorMessages.push_back(PHYSFS_getLastError());
        Logger::log << Logger::warning() << "Couldn't add '" << pathName << "' to search path. " << mErrorMessages.back() << "." << endl;
        return false;
    }
#else
	#error Filesystem support for this platform has not been developed.
#endif

	if(mVerbose) cout << "Added '" << pathName << "' to search path." << endl;

	return true;
}


/**
 * Returns a list of directories in the Search Path.
 */
StringList Filesystem::searchPath() const
{
	StringList searchPath;

	for(char **i = PHYSFS_getSearchPath(); *i != NULL; i++)
		searchPath.push_back(*i);

	return searchPath;
}


/**
 * Returns a list of files within a given directory.
 *
 * \param	searchDir	Directory to search within the searchpath.
 *
 * \note	This function will also return the names of any directories in a specified search path
 */
StringList Filesystem::directoryList(const std::string& dir) const
{
	return directoryList(dir, string(""));
}


/**
 * Returns a list of files within a given directory.
 *
 * \param	searchDir	Directory to search within the searchpath.
 * \param	filter		Optional extension filter. Only use the extension without a wildcard (*) character or period (e.g., 'png' vs '*.png' or '.png').
 *
 * \note	This function will also return the names of any directories in a specified search path
 */
StringList Filesystem::directoryList(const string& dir, const string& filter) const
{
	char **rc = PHYSFS_enumerateFiles(dir.c_str());

	StringList fileList;
	if(filter.empty())
	{
		for(char **i = rc; *i != NULL; i++)
			fileList.push_back(*i);

		//Logger::log << fileList.back() << endl;
	}
	else
	{
		int filterLen = filter.size();
		for(char **i = rc; *i != NULL; i++)
		{
			string tmpStr = *i;
			if(tmpStr.rfind(filter, strlen(*i) - filterLen) != std::string::npos)
				fileList.push_back(*i);
		}
	}

	PHYSFS_freeList(rc);

	return fileList;
}


/**
 * Deletes a specified file.
 * 
 * \note	This function is not named 'delete' due to
 *			language limitations.
 */
bool Filesystem::del(const string& filename) const
{
	if(PHYSFS_delete(filename.c_str()) == 0)
	{
		mErrorMessages.push_back(lastError());
		cout << "Unable to delete '" << filename << "':" << lastError() << endl;
		return false;
	}

	return true;
}


File Filesystem::open(const std::string& fileName) const
{
	if(mVerbose) cout << "Attempting to load '" << fileName << endl;

	PHYSFS_file* myFile = PHYSFS_openRead(fileName.c_str());

	if(!myFile)
	{
		mErrorMessages.push_back(PHYSFS_getLastError());
		cout << "Unable to load '" << fileName << "'. " << mErrorMessages.back() << "." << endl;
		closeFile(myFile);
		return File();
	}


	// Ensure that the file size is greater than zero and can fit in a 32-bit integer.
	PHYSFS_sint64 len = PHYSFS_fileLength(myFile);
	if(len < 0 || len > UINT_MAX)
	{
		mErrorMessages.push_back("Filesystem::open(): Attempting to load a file that's too big.");
		cout << mErrorMessages.back() << endl;
		closeFile(myFile);
		return File();
	}


	// Create a char* buffer large enough to hold the entire file.
	PHYSFS_uint32 fileLength = static_cast<PHYSFS_uint32>(len);
	char *fileBuffer = new char[fileLength + 1];


	// If we read less then the file length, return an empty File object, log a message and free any used memory.
	if(PHYSFS_read(myFile, fileBuffer, sizeof(char), fileLength) < fileLength)
	{
		mErrorMessages.push_back(PHYSFS_getLastError());
		cout << "Unable to load '" << fileName << "'. " << mErrorMessages.back() << "." << endl;

		delete [] fileBuffer;
		// Close the file as we no longer need it.
		closeFile(myFile);
		return File();
	}

	// Instantiate a new File object and clean up any remaining used memory.
	std::string returnStr(fileBuffer, fileLength);
	File file(string(fileBuffer, fileLength), fileName);
	closeFile(myFile);
	delete [] fileBuffer;

	if(mVerbose) cout << "Loaded '" << fileName << "' successfully." << endl;

	return file;
}


/**
 * Creates a new directory within the primary search path.
 *
 * \return Returns \c true if successful. Otherwise, returns \c false.
 */
bool Filesystem::makeDirectory(const std::string& dirPath) const
{
	return (PHYSFS_mkdir(dirPath.c_str()) == 0) ? false : true;
}


/**
 * Determines if a given path is a directory rather than a file.
 */
bool Filesystem::isDirectory(const std::string& path) const
{
	return PHYSFS_isDirectory(path.c_str()) != 0;
}


/**
 * Checks for the existence of a file.
 *
 * Returns Returns \c true if the specified file exists. Otherwise, returns \c false.
 */
bool Filesystem::exists(const std::string& fileName) const
{
	return PHYSFS_exists(fileName.c_str()) != 0;
}


/**
 * Returns the last error that occurred.
 */
string Filesystem::lastError() const
{
	return mErrorMessages.back();
}


/*
 * Toggles Verbose Mode.
 *
 * When Verbose mode is off, only critical messages are displayed.
 * Verbose Mode is generally useful for debugging purposes.
 */
void Filesystem::toggleVerbose() const
{
	mVerbose ? mVerbose = false : mVerbose = true;
}


/**
 * Writes basic Filesystem debug information to the global Log File.
 */
void Filesystem::debug()
{
	cout << endl;
	cout << "=== Filesystem Debug Info ===" << endl;
	cout << "Working Directory: " << PHYSFS_getBaseDir() << endl;

#ifdef __APPLE__
	cout << "Bundle Directory: " << mBundlePath << endl;
#endif

	cout << "Base Data Path: " << mDataPath << endl;
	cout << "Search Path:"  << endl;

	for(char **i = PHYSFS_getSearchPath(); *i != NULL; i++)
		cout << "\t - " << *i << endl;

	if(!mErrorMessages.empty())
	{
		cout << endl;

		for(size_t i = 0; i < mErrorMessages.size(); i++)
			cout << mErrorMessages[i] << endl;
	}

	cout << endl;
}


/**
 * Closes a file handle.
 *
 * \param	file	A handle to a PHYSFS_file.
 *
 * \return	True on success, false otherwise.
 *
 * \note	This function sets its paramter to NULL after the file is closed. This
 *			effectively invalidates the file handle and so it should not be used again
 *			unless properly initialized. This is an intentional design feature to ensure
 *			that the use of a file that's been closed cannot be used again.
 */
bool Filesystem::closeFile(PHYSFS_File *file) const
{
	if(!file)
	{
		mErrorMessages.push_back("Filesystem::closeFile(): attempted to pass a NULL parameter.");
		return false;
	}


	// Attempt to close the file. If the first attempt fails, try to close the
	// file again every CLOSE_ATTEMPT_TIMEOUT seconds at least CLOSE_MAX_ATTEMPTS
	// times before giving up.
	Timer t;
	int currentTick = t.s();
	int lastAttempt = currentTick;
	int attempts = 0;

	while((attempts < CLOSE_MAX_ATTEMPTS))
	{
		if(PHYSFS_close(file) > 0)
		{
			file = NULL;
			return true;
		}

		while(currentTick - lastAttempt < CLOSE_ATTEMPT_TIMEOUT)
			currentTick = t.s();

		if(mVerbose) cout << "Unable to close file handle: " << PHYSFS_getLastError() << ". Will attempt to close " << CLOSE_MAX_ATTEMPTS - attempts << " time(s)." << endl;

		lastAttempt = currentTick;
		attempts++;
	}


	cout << "Unable to close file handle: " << PHYSFS_getLastError() << ". File handle is still open." << endl;

	return false;
}


/**
 * Writes a file to disk.
 *
 * \param	file	A reference to a \c const \c File object.
 *
 * \return Returns \c true if successful. Otherwise, returns \c false.
 */
bool Filesystem::write(const File& file, bool overwrite) const
{
	if(file.empty())
	{
		mErrorMessages.push_back(string("Attempted to write empty file '") + file.filename() + "'");
		return false;
	}

	if(!overwrite && exists(file.filename()))
	{
		if(mVerbose) cout << "Attempted to overwrite a file '" << file.filename() << "' that exists." << endl;
		mErrorMessages.push_back(string("Attempted to overwrite a file '") + file.filename() + "' that exists.");
		return false;
	}

	PHYSFS_file* myFile = PHYSFS_openWrite(file.filename().c_str());
	if(!myFile)
	{
		std::stringstream tmpString;
		tmpString << PHYSFS_getLastError() << ": '" << file.filename() << "'";
		mErrorMessages.push_back(tmpString.str());
		return false;
	}

	if(PHYSFS_write(myFile, file.bytes().c_str(), sizeof(char), file.size()) < file.size())
	{
		std::stringstream tmpString;
		tmpString << PHYSFS_getLastError() << ": '" << file.filename() << "'";
		mErrorMessages.push_back(tmpString.str());
		closeFile(myFile);
		return false;
	}
	else
	{
		closeFile(myFile);
		if(mVerbose) cout << "Wrote '" << file.size() << "' bytes to file '" << file.filename() << "'." << endl;
	}

	return true;
}


/**
 * Gets the current User path.
 */
string Filesystem::userPath() const
{
	return PHYSFS_getUserDir();
}


/**
 * Gets the base data path.
 */
string Filesystem::dataPath() const
{
	return mDataPath;
}


/*
 * Convenience function to get the working directory of a file.
 *
 * \path	fileName	A file path.
 *
 * \note	File paths should not have any trailing '/' characters.
 */
string Filesystem::workingPath(const string& fileName) const
{

	if(!fileName.empty())
	{
		string tmpStr(fileName);
		int pos = tmpStr.rfind("/");
		tmpStr = tmpStr.substr(0, pos + 1);
		return tmpStr;
	}
	else
	{
		mErrorMessages.push_back("Filesystem::getworkingPath() was given an empty string.");
		cout << mErrorMessages.back();
		return string();
	}
}

std::string Filesystem::extension(const std::string path)
{
	int num = path.find_last_of(".");
	if (num >= 0)
		return path.substr(num + 1);
	else
	{
		mErrorMessages.push_back("Filesystem::getFileExtension() was given a file path without a valid file extension.");
		cout << mErrorMessages.back();
		return string();
	}
}
