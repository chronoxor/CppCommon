/*!
    \file timestamp.cpp
    \brief Nanoseconds timestamp accessor implementation
    \author Ivan Shynkarenka
    \date 26.01.2016
    \copyright MIT License
*/

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <time.h>
#endif

#include "system/timestamp.h"

namespace CppCommon {

uint64_t timestamp()
{
#if defined(_WIN32) || defined(_WIN64)
    static LARGE_INTEGER frequency{0};
    static BOOL initialized = QueryPerformanceFrequency(&frequency);
    if (initialized)
    {
        LARGE_INTEGER timestamp{0};
        QueryPerformanceCounter(&timestamp);
        return (timestamp.QuadPart * 1000 * 1000 * 1000) / frequency.QuadPart;
    }
    else
        return GetTickCount() * 1000 * 1000;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    struct timespec timestamp;
    clock_gettime(CLOCK_REALTIME, &timestamp);
    return (timestamp.tv_sec * 1000 * 1000 * 1000) + timestamp.tv_nsec;
#endif
}

} // namespace CppCommon
