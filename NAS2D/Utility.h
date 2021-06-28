// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================
#pragma once

#include <utility>
#include <type_traits>
#include <stdexcept>


namespace NAS2D {

template<typename T>
class Utility
{
public:
	Utility<T>() = delete;
	~Utility() = delete;
	Utility<T>(const Utility& s) = delete;
	Utility<T>& operator=(const Utility& s) = delete;

	template<typename Type = T>
	static
	std::enable_if_t<std::is_default_constructible<Type>::value, T&>
	get()
	{
		if (!mInstance)
		{
			mInstance = new T();
		}

		return *mInstance;
	}

	template<typename Type = T>
	static
	std::enable_if_t<!std::is_default_constructible<Type>::value, T&>
	get()
	{
		if (!mInstance)
		{
			throw std::runtime_error("Type must be default constructible or initialized with `init`");
		}

		return *mInstance;
	}

	template<typename Type = T, typename... Args>
	static Type& init(Args&&... args)
	{
		auto newInstance = new Type(std::forward<Args>(args)...);

		delete mInstance;

		mInstance = newInstance;
		return *newInstance;
	}

	static void clear()
	{
		delete mInstance;
		mInstance = nullptr;
	}

private:
	//TODO: static inline std::unique_ptr<T> mInstance{};
	static T* mInstance;
};

//TODO: Remove in favor of static inline declaration
template<typename T> T* Utility<T>::mInstance = nullptr;

} // namespace
