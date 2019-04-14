#pragma once

#if defined(__clang__)
#define PLATFORM_CLANG
#define PLATFORM_LINUX
#elif defined(__GNUC__) || defined(__GNUG__)
#define PLATFORM_GNUC
#define PLATFORM_LINUX
#endif

#if defined(__APPLE__)
#undef PLATFORM_LINUX
#define PLATFORM_APPLE
#elif defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)
#define PLATFORM_WINDOWS
#endif
