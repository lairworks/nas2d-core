#pragma once

#if defined(__clang__)
#define PLATFORM_CLANG
#elif defined(__GNUC__) || defined(__GNUG__)
#define PLATFORM_GNUC
#elif defined(__APPLE__)
#define PLATFORM_APPLE
#elif defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)
#define PLATFORM_WINDOWS
#endif
