#pragma once

#include "NAS2D/ThreadSafeQueue.h"

#include <atomic>
#include <condition_variable>
#include <fstream>
#include <iosfwd>
#include <mutex>
#include <string>
#include <thread>

namespace NAS2D
{

class FileLogger
{
public:
	FileLogger() = delete;
	explicit FileLogger(const std::string& logName);
	FileLogger(const FileLogger& other) = delete;
	FileLogger(FileLogger&& other) = delete;
	FileLogger& operator=(const FileLogger& other) = delete;
	FileLogger& operator=(FileLogger&& other) = delete;
	~FileLogger();

	void Log(const std::string& msg);
	void LogLine(const std::string& msg);

protected:
private:
	bool IsRunning() const;
	void SetIsRunning(bool value);
	void Initialize(const std::string& logName);
	void Shutdown();

	void LogWorker();

	std::streambuf* mOldCout{nullptr};
	std::ofstream mStreamBuffer{};
	std::string mLogPath{};
	mutable std::mutex _cs{};
	std::atomic_bool mIsRunning{false};
	std::condition_variable mSignal{};
	std::thread mWorker{};
	ThreadSafeQueue<std::string> mQueue{};
};

} // namespace NAS2D