#include "NAS2D/ThreadUtils.h"

/**********************************************/
/* Non Windows implementation references:   *
 * https://stackoverflow.com/a/7989973/421178 *
 * https://stackoverflow.com/questions/2057960/how-to-set-a-threadname-in-macosx *
/**********************************************/

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

#if defined(__linux__)
#include <pthread.h>	//Remove whichever one does not exist!
#include <pthread_np.h> //Remove whichever one does not exist!
#endif

#if defined(__APPLE__)
#include <pthread.h>
#endif

#if defined(_WIN32)
std::wstring ConvertMultiByteToUnicode(const std::string& multi_byte_string) noexcept;
std::string ConvertUnicodeToMultiByte(const std::wstring& unicode_string) noexcept;
#endif
void SetThreadDescriptionWindows([[maybe_unused]] std::thread& thread, [[maybe_unused]] const std::string& description);
void SetThreadDescriptionWindows([[maybe_unused]] std::thread& thread, [[maybe_unused]] const std::wstring& description);
void GetThreadDescriptionWindows([[maybe_unused]] std::thread& thread, [[maybe_unused]] std::string& description);
void GetThreadDescriptionWindows([[maybe_unused]] std::thread& thread, [[maybe_unused]] std::wstring& description);

void SetThreadDescriptionApple([[maybe_unused]] std::thread& thread, [[maybe_unused]] const std::string& description);
void GetThreadDescriptionApple([[maybe_unused]] std::thread& thread, [[maybe_unused]] std::string& description);

void GetThreadDescriptionLinux([[maybe_unused]] std::thread& thread, [[maybe_unused]] std::string& description);
void SetThreadDescriptionLinux([[maybe_unused]] std::thread& thread, [[maybe_unused]] const std::string& description);

void NAS2D::ThreadUtils::GetThreadDescription(std::thread& thread, std::string& description)
{
#if defined(__APPLE__)
	GetThreadDescriptionApple(thread, description);
#elif defined(__linux__)
	GetThreadDescriptionLinux(thread, description);
#elif defined(_WIN32)
	GetThreadDescriptionWindows(thread, description);
#endif
}

#if defined(_WIN32)
void NAS2D::ThreadUtils::GetThreadDescription(std::thread& thread, std::wstring& description)
{
	GetThreadDescriptionWindows(thread, description);
}
#endif

void NAS2D::ThreadUtils::SetThreadDescription(std::thread& thread, const std::string& description)
{
#if defined(__APPLE__)
	SetThreadDescriptionApple(thread, description);
#elif defined(__linux__)
	SetThreadDescriptionLinux(thread, description);
#elif defined(_WIN32)
	SetThreadDescriptionWindows(thread, description);
#endif
}

#if defined(_WIN32)
std::wstring ConvertMultiByteToUnicode(const std::string& multi_byte_string) noexcept
{
	if (multi_byte_string.empty())
	{
		return {};
	}
	std::unique_ptr<wchar_t[]> buf = nullptr;
	auto buf_size = static_cast<std::size_t>(::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, multi_byte_string.data(), -1, buf.get(), 0));
	if (!buf_size)
	{
		return {};
	}
	buf = std::make_unique<wchar_t[]>(buf_size * sizeof(wchar_t));
	if (!buf)
	{
		return {};
	}
	buf_size = static_cast<std::size_t>(::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, multi_byte_string.data(), -1, buf.get(), static_cast<int>(buf_size)));
	if (!buf_size)
	{
		return {};
	}
	std::wstring unicode_string{};
	unicode_string.assign(buf.get(), buf_size - 1);
	return unicode_string;
}

std::string ConvertUnicodeToMultiByte(const std::wstring& unicode_string) noexcept
{
	if (unicode_string.empty())
	{
		return {};
	}
	std::unique_ptr<char[]> buf = nullptr;
	auto buf_size = static_cast<std::size_t>(::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, unicode_string.data(), -1, buf.get(), 0, nullptr, nullptr));
	if (!buf_size)
	{
		return {};
	}
	buf = std::make_unique<char[]>(buf_size * sizeof(char));
	if (!buf)
	{
		return {};
	}
	buf_size = static_cast<std::size_t>(::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, unicode_string.data(), -1, buf.get(), static_cast<int>(buf_size), nullptr, nullptr));
	if (!buf_size)
	{
		return {};
	}
	std::string mb_string{};
	mb_string.assign(buf.get(), buf_size - 1);
	return mb_string;
}
#endif

void NAS2D::ThreadUtils::SetThreadDescription(std::thread& thread, const std::wstring& description)
{
#if defined(_WIN32)
	SetThreadDescriptionWindows(thread, description);
#endif
}

void SetThreadDescriptionWindows([[maybe_unused]] std::thread& thread, [[maybe_unused]] const std::wstring& description)
{
#if defined(_WIN32)
	::SetThreadDescription(thread.native_handle(), description.c_str());
#endif
}

void SetThreadDescriptionWindows([[maybe_unused]] std::thread& thread, [[maybe_unused]] const std::string& description)
{
#if defined(_WIN32)
	const auto wide_description = ConvertMultiByteToUnicode(description);
	SetThreadDescriptionWindows(thread, wide_description);
#endif
}

void SetThreadDescriptionApple([[maybe_unused]] std::thread& thread, [[maybe_unused]] const std::string& description)
{
	/* DO NOTHING */
	//Apple doesn't allow for setting arbitrary thread descriptions. Only the calling thread.
	//As such, this code does nothing.
}

void SetThreadDescriptionLinux([[maybe_unused]] std::thread& thread, [[maybe_unused]] const std::string& description)
{
#if defined(__linux__)
	pthread_setname_np(thread.native_handle(), description.c_str());
#endif
}

void GetThreadDescriptionWindows([[maybe_unused]] std::thread& thread, [[maybe_unused]] std::string& description)
{
#if defined(_WIN32)
	std::wstring wide_description{};
	GetThreadDescriptionWindows(thread, wide_description);
	description = ConvertUnicodeToMultiByte(wide_description);
#endif
}

void GetThreadDescriptionWindows([[maybe_unused]] std::thread& thread, [[maybe_unused]] std::wstring& description)
{
#if defined(_WIN32)
	PWSTR d{};
	::GetThreadDescription(thread.native_handle(), &d);
	description.assign(d);
	::LocalFree(d);
	d = nullptr;
#endif
}

void GetThreadDescriptionLinux([[maybe_unused]] std::thread& thread, [[maybe_unused]] std::string& description)
{
#if defined(__linux__)
	//&description[0] is required because `const char*` is not `char*`
	pthread_getname_np(thread.native_handle(), &description[0], description.size());
#endif
}

void GetThreadDescriptionApple([[maybe_unused]] std::thread& thread, [[maybe_unused]] std::string& description)
{
#if defined(__APPLE__)
	//&description[0] is required because `const char*` is not `char*`
	pthread_getname_np(thread.native_handle(), &description[0], description.size());
#endif
}
