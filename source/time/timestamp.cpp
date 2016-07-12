/*!
    \file timestamp.cpp
    \brief Timestamp implementation
    \author Ivan Shynkarenka
    \date 26.01.2016
    \copyright MIT License
*/

#include "time/timestamp.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <time.h>
#endif

namespace CppCommon {

uint64_t Timestamp::now() noexcept
{
#if defined(_WIN32) || defined(_WIN64)
    static uint64_t offset = 0;
    static LARGE_INTEGER first{0};
    static LARGE_INTEGER frequency{0};
    static bool initialized = false;
    static bool qpc = true;

    if (!initialized)
    {
        // Calculate timestamp offset
        FILETIME timestamp;
        GetSystemTimePreciseAsFileTime(&timestamp);

        ULARGE_INTEGER result;
        result.LowPart = timestamp.dwLowDateTime;
        result.HighPart = timestamp.dwHighDateTime;

        // Convert 01.01.1601 to 01.01.1970
        result.QuadPart -= 116444736000000000ll;
        offset = result.QuadPart * 100;

        // Setup performance counter
        qpc = QueryPerformanceFrequency(&frequency) && QueryPerformanceCounter(&first);

        initialized = true;
    }

    if (qpc)
    {
        LARGE_INTEGER timestamp{0};
        QueryPerformanceCounter(&timestamp);
        timestamp.QuadPart -= first.QuadPart;
        return offset + ((timestamp.QuadPart * 1000 * 1000 * 1000) / frequency.QuadPart);
    }
    else
        return offset;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    struct timespec timestamp;
    clock_gettime(CLOCK_REALTIME, &timestamp);
    return (timestamp.tv_sec * 1000 * 1000 * 1000) + timestamp.tv_nsec;
#endif
}

uint64_t Timestamp::rdts() noexcept
{
#if defined(_MSC_VER)
    return __rdtsc();
#elif defined(__i386__)
    uint64_t x;
    __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
    return x;
#elif defined(__x86_64__)
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)lo) | (((uint64_t)hi) << 32);
#endif
}

} // namespace CppCommon
