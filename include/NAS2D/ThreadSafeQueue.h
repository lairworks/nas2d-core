#pragma once

#include <mutex>
#include <queue>

namespace NAS2D
{
	template<typename T>
	class ThreadSafeQueue
	{
	  public:
		void push(const T& t)
		{
			std::scoped_lock lock(_cs);
			mQueue.push(t);
		}
		void pop()
		{
			std::scoped_lock lock(_cs);
			mQueue.pop();
		}
		bool empty() const noexcept
		{
			std::scoped_lock lock(_cs);
			return mQueue.empty();
		}
		decltype(auto) size() const noexcept
		{
			std::scoped_lock lock(_cs);
			return mQueue.size();
		}

		T& back() const noexcept
		{
			std::scoped_lock lock(_cs);
			return mQueue.back();
		}
		T& back() noexcept
		{
			std::scoped_lock lock(_cs);
			return mQueue.back();
		}
		T& front() const noexcept
		{
			std::scoped_lock lock(_cs);
			return mQueue.front();
		}
		T& front() noexcept
		{
			std::scoped_lock lock(_cs);
			return mQueue.front();
		}
		void swap(ThreadSafeQueue<T>& b) noexcept
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
