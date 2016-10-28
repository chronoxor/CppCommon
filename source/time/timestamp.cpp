/*!
    \file timestamp.cpp
    \brief Timestamp implementation
    \author Ivan Shynkarenka
    \date 26.01.2016
    \copyright MIT License
*/

#include "time/timestamp.h"

#include "errors/exceptions.h"

#if defined(__APPLE__)
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <sys/time.h>
#include <math.h>
#include <time.h>
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <time.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

namespace CppCommon {

//! @cond INTERNALS
namespace Internals {

#if defined(__APPLE__)

uint32_t CeilLog2(uint32_t x)
{
    uint32_t result = 0;

    --x;
    while (x > 0)
    {
        ++result;
        x >>= 1;
    }

    return result;
}

// This function returns the rational number inside the given interval with
// the smallest denominator (and smallest numerator breaks ties; correctness
// proof neglects floating-point errors).
mach_timebase_info_data_t BestFrac(double a, double b)
{
    if (floor(a) < floor(b))
    {
        mach_timebase_info_data_t rv = { (uint32_t)ceil(a), 1 };
        return rv;
    }

    double m = floor(a);
    mach_timebase_info_data_t next = BestFrac(1 / (b - m), 1 / (a - m));
    mach_timebase_info_data_t rv = { (int)m * next.numer + next.denom, next.numer };
    return rv;
}

// This is just less than the smallest thing we can multiply numer by without
// overflowing. CeilLog2(numer) = 64 - number of leading zeros of numer
uint64_t GetExpressibleSpan(uint32_t numer, uint32_t denom)
{
  uint64_t maxDiffWithoutOverflow = ((uint64_t)1 << (64 - CeilLog2(numer))) - 1;
  return maxDiffWithoutOverflow * numer / denom;
}

// The clock may run up to 0.1% faster or slower than the "exact" tick count.
// However, although the bound on the error is the same as for the pragmatic
// answer, the error is actually minimized over the given accuracy bound.
uint64_t PrepareTimebaseInfo(mach_timebase_info_data_t& tb, uint64_t base)
{
    tb.numer = 0;
    tb.denom = 1;

    kern_return_t mtiStatus = mach_timebase_info(&tb);
    if (mtiStatus != KERN_SUCCESS)
        return 0;

    double frac = (double)tb.numer / tb.denom;
    uint64_t spanTarget = 315360000000000000llu;
    if (GetExpressibleSpan(tb.numer, tb.denom) >= spanTarget)
        return 0;

    for (double errorTarget = 1 / 1024.0; errorTarget > 0.000001;)
    {
        mach_timebase_info_data_t newFrac = BestFrac((1 - errorTarget) * frac, (1 + errorTarget) * frac);
        if (GetExpressibleSpan(newFrac.numer, newFrac.denom) < spanTarget)
            break;
        tb = newFrac;
        errorTarget = fabs((double)tb.numer / tb.denom - frac) / frac / 8;
    }

    return base + mach_absolute_time();
}

#endif

} // namespace Internals
//! @endcond

uint64_t Timestamp::utc()
{
#if defined(__APPLE__)
    struct timeval timestamp;
    if (gettimeofday(&timestamp, NULL) != 0)
        throwex SystemException("Cannot get time of day!");
    return (timestamp.tv_sec * 1000 * 1000 * 1000) + (timestamp.tv_usec * 1000);
#elif defined(unix) || defined(__unix) || defined(__unix__)
    struct timespec timestamp;
    if (clock_gettime(CLOCK_REALTIME, &timestamp) != 0)
        throwex SystemException("Cannot get value of CLOCK_REALTIME timer!");
    return (timestamp.tv_sec * 1000 * 1000 * 1000) + timestamp.tv_nsec;
#elif defined(_WIN32) || defined(_WIN64)
    FILETIME ft;
    GetSystemTimePreciseAsFileTime(&ft);

    ULARGE_INTEGER result;
    result.LowPart = ft.dwLowDateTime;
    result.HighPart = ft.dwHighDateTime;
    return (result.QuadPart - 116444736000000000ull) * 100;
#endif
}

uint64_t Timestamp::local()
{
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    uint64_t timestamp = utc();

    // Adjust UTC time with local timezone offset
    struct tm local;
    time_t seconds = timestamp / (1000 * 1000 * 1000);
    if (localtime_r(&seconds, &local) != &local)
        throwex SystemException("Cannot convert CLOCK_REALTIME time to local date & time structure!");
    return timestamp + (local.tm_gmtoff * 1000 * 1000 * 1000);
#elif defined(_WIN32) || defined(_WIN64)
    FILETIME ft;
    GetSystemTimePreciseAsFileTime(&ft);

    FILETIME ft_local;
    if (!FileTimeToLocalFileTime(&ft, &ft_local))
        throwex SystemException("Cannot convert UTC file time to local file time structure!");

    ULARGE_INTEGER result;
    result.LowPart = ft_local.dwLowDateTime;
    result.HighPart = ft_local.dwHighDateTime;
    return (result.QuadPart - 116444736000000000ull) * 100;
#endif
}

uint64_t Timestamp::nano()
{
#if defined(__APPLE__)
    static mach_timebase_info_data_t info;
    static uint64_t bias = Internals::PrepareTimebaseInfo(info, 86400000000000ull);
    return (uint64_t)((double)(mach_absolute_time() - bias) * (double)info.numer / (double)info.denom);
#elif defined(unix) || defined(__unix) || defined(__unix__)
    struct timespec timestamp = { 0 };
    if (clock_gettime(CLOCK_MONOTONIC, &timestamp) != 0)
        throwex SystemException("Cannot get value of CLOCK_MONOTONIC timer!");
    return (timestamp.tv_sec * 1000 * 1000 * 1000) + timestamp.tv_nsec;
#elif defined(_WIN32) || defined(_WIN64)
    static uint64_t offset = 0;
    static LARGE_INTEGER first = { 0 };
    static LARGE_INTEGER frequency = { 0 };
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
        LARGE_INTEGER timestamp = { 0 };
        QueryPerformanceCounter(&timestamp);
        timestamp.QuadPart -= first.QuadPart;
        return offset + ((timestamp.QuadPart * 1000 * 1000 * 1000) / frequency.QuadPart);
    }
    else
        return offset;
#else
    #error Unsupported platform
#endif
}

uint64_t Timestamp::rdts()
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
