#include "NAS2D/FileLogger.h"

#include "NAS2D/Filesystem.h"
#include "NAS2D/ThreadUtils.h"
#include "NAS2D/Utility.h"

#include <iostream>
#include <ostream>

using namespace NAS2D;

void NAS2D::FileLogger::LogWorker()
{
	while (IsRunning())
	{
		std::unique_lock lock(_cs);
		//Wake up when not running or queue isn't empty
		mSignal.wait(lock, [this]() -> bool { return !mIsRunning || !mQueue.empty(); });
		//Protect against spurious wakeups
		if (!mQueue.empty())
		{
			const auto str = mQueue.front();
			mQueue.pop();
			mStreamBuffer << str;
			mStreamBuffer.flush();
		}
	}
}

FileLogger::FileLogger(const std::string& logName)
{
	Initialize(logName);
}

FileLogger::~FileLogger()
{
	Shutdown();
}

bool FileLogger::IsRunning() const
{
	bool running = false;
	{
		std::scoped_lock lock(_cs);
		running = mIsRunning;
	}
	return running;
}

void FileLogger::SetIsRunning(bool value)
{
	std::scoped_lock lock(_cs);
	mIsRunning = value;
}

void FileLogger::Initialize(const std::string& logName)
{
	if (IsRunning())
	{
		std::cout << "FileLogger already running." << std::endl;
		return;
	}
	auto& fs = Utility<Filesystem>::get();
	fs.makeDirectory("data/logs");
	mLogPath = "data/logs/" + logName + ".log";
	mStreamBuffer.open(mLogPath);
	if (mStreamBuffer.fail())
	{
		std::cout << "Could not initialize FileLogger." << std::endl;
		mStreamBuffer.clear();
		mIsRunning = false;
		return;
	}
	mOldCout = std::cout.rdbuf(mStreamBuffer.rdbuf());
	mWorker = std::thread(&FileLogger::LogWorker, this);
	ThreadUtils::SetThreadDescription(mWorker, std::string{"FileLogger"});
}

void FileLogger::Shutdown()
{
	if (IsRunning())
	{
		SetIsRunning(false);
		if (mWorker.joinable())
		{
			mWorker.join();
		}
		mStreamBuffer.close();
		std::cout.rdbuf(mOldCout);
	}
}


void NAS2D::FileLogger::Log(const std::string& msg)
{
	{
		std::scoped_lock lock(_cs);
		mQueue.push(msg);
	}
	mSignal.notify_all();
}

void NAS2D::FileLogger::LogLine(const std::string& msg)
{
	Log(msg + '\n');
}
