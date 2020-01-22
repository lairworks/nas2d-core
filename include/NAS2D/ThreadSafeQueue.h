#pragma once

#include <mutex>
#include <queue>

namespace NAS2D
{
	template<typename T>
	class ThreadSafeQueue
	{
	  public:
		void push(const T& t) noexcept(noexcept(mQueue.push()))
		{
			std::scoped_lock lock(_cs);
			mQueue.push();
		}
		void pop() noexcept(noexcept(mQueue.pop()))
		{
			std::scoped_lock lock(_cs);
			mQueue.pop();
		}
		bool empty() const noexcept(noexcept(mQueue.empty()))
		{
			std::scoped_lock lock(_cs);
			return mQueue.empty();
		}
		decltype(auto) size() const noexcept(noexcept(mQueue.size()))
		{
			std::scoped_lock lock(_cs);
			return mQueue.size();
		}

		T& back() const noexcept(noexcept(mQueue.back()))
		{
			std::scoped_lock lock(_cs);
			return mQueue.back();
		}
		T& back() noexcept(noexcept(mQueue.back()))
		{
			std::scoped_lock lock(_cs);
			return mQueue.back();
		}
		T& front() const noexcept(noexcept(mQueue.front()))
		{
			std::scoped_lock lock(_cs);
			return mQueue.front();
		}
		T& front() noexcept(noexcept(mQueue.front()))
		{
			std::scoped_lock lock(_cs);
			return mQueue.front();
		}
		void swap(ThreadSafeQueue<T>& b) noexcept(noexcept(mQueue.swap()))
		{
			std::scoped_lock lock(_cs, b._cs);
			mQueue.swap(b.mQueue);
		}

	  protected:
	  private:
		mutable std::mutex _cs{};
		std::queue<T> mQueue{};
	};

} // namespace NAS2D
