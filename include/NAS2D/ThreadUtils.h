#pragma once

#include <string>
#include <thread>

namespace NAS2D
{
	//RAII thread that automatically joins when it goes out of scope and cannot be in a detached state.
	//This is a proposed class for C++20 and the implementation here can be safely removed once implemented there.
	class joinable_thread : public std::thread
	{
		~joinable_thread()
		{
			if (joinable())
			{
				join();
			}
		}
		void detach() = delete;
	};

	//This is an alias for when C++20 adds the class named jthread.
	//Using this instead of the class named joinable_thread directly will allow code to not break after the switchover.
	//Just be weary of namespace collisions
	using jthread = joinable_thread;

	namespace ThreadUtils
	{
		//Set a thread's description
		//NOTE: The Set* functions do nothing on Apple machines.
		//They do not allow setting arbitrary thread descriptions except for the calling thread.
		void SetThreadDescription(std::thread& thread, const std::string& description);
		
#if defined(_WIN32)
		//Set a thread's description as wide string
		void SetThreadDescription(std::thread& thread, const std::wstring& description);
#endif

		//Get a thread's description
		void GetThreadDescription(std::thread& thread, std::string& description);
#if defined(_WIN32)
		//Get a thread's description as a wide string
		void GetThreadDescription(std::thread& thread, std::wstring& description);
#endif
	}
} // namespace NAS2D