/*!
    \file timestamp.cpp
    \brief Timestamp implementation
    \author Ivan Shynkarenka
    \date 26.01.2016
    \copyright MIT License
*/

#include "time/timestamp.h"

#include "math/math.h"

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
uint64_t PrepareTimebaseInfo(mach_timebase_info_data_t& tb)
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

    return 0;
}

#endif

} // namespace Internals
//! @endcond

uint64_t Timestamp::utc()
{
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    struct timespec timestamp;
    if (clock_gettime(CLOCK_REALTIME, &timestamp) != 0)
        throwex SystemException("Cannot get value of CLOCK_REALTIME timer!");
    return (timestamp.tv_sec * 1000000000) + timestamp.tv_nsec;
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
    time_t seconds = timestamp / (1000000000);
    if (localtime_r(&seconds, &local) != &local)
        throwex SystemException("Cannot convert CLOCK_REALTIME time to local date & time structure!");
    return timestamp + (local.tm_gmtoff * 1000000000);
#elif defined(_WIN32) || defined(_WIN64)
    FILETIME ft;
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN8)
    GetSystemTimePreciseAsFileTime(&ft);
#else
    GetSystemTimeAsFileTime(&ft);
#endif

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
    static uint64_t bias = Internals::PrepareTimebaseInfo(info);
    return ((mach_absolute_time() - bias) * info.numer) / info.denom;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    struct timespec timestamp = { 0 };
    if (clock_gettime(CLOCK_MONOTONIC, &timestamp) != 0)
        throwex SystemException("Cannot get value of CLOCK_MONOTONIC timer!");
    return (timestamp.tv_sec * 1000000000) + timestamp.tv_nsec;
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
        return offset + Math::MulDiv64(timestamp.QuadPart, 1000000000, frequency.QuadPart);
    }
    else
        return offset;
#else
    #error Unsupported platform
#endif
}

uint64_t Timestamp::rdts()
{
#if defined(__APPLE__)
    // This goes at the top because we need ALL Macs, regardless of
    // architecture, to return the number of "mach time units" that
    // have passed since startup.  See sysinfo.cc where
    // InitializeSystemInfo() sets the supposed cpu clock frequency of
    // macs to the number of mach time units per second, not actual
    // CPU clock frequency (which can change in the face of CPU
    // frequency scaling).  Also note that when the Mac sleeps, this
    // counter pauses; it does not continue counting, nor does it
    // reset to zero.
    return mach_absolute_time();
#elif defined(_MSC_VER)
    return __rdtsc();
#elif defined(__i386__)
    int64_t ret;
    __asm__ volatile("rdtsc" : "=A"(ret));
    return ret;
#elif defined(__x86_64__) || defined(__amd64__)
    uint64_t low, high;
    __asm__ volatile("rdtsc" : "=a"(low), "=d"(high));
    return (high << 32) | low;
#elif defined(__powerpc__) || defined(__ppc__)
    // This returns a time-base, which is not always precisely a cycle-count.
    int64_t tbl, tbu0, tbu1;
    asm("mftbu %0" : "=r"(tbu0));
    asm("mftb  %0" : "=r"(tbl));
    asm("mftbu %0" : "=r"(tbu1));
    tbl &= -static_cast<int64>(tbu0 == tbu1);
    // High 32 bits in tbu1;
    // How 32 bits in tbl (tbu0 is garbage)
    return (tbu1 << 32) | tbl;
#elif defined(__sparc__)
    int64_t tick;
    asm(".byte 0x83, 0x41, 0x00, 0x00");
    asm("mov   %%g1, %0" : "=r"(tick));
    return tick;
#elif defined(__ia64__)
    int64_t itc;
    asm("mov %0 = ar.itc" : "=r"(itc));
    return itc;
#elif defined(COMPILER_MSVC) && defined(_M_IX86)
    // Older MSVC compilers (like 7.x) don't seem to support the
    // __rdtsc intrinsic properly, so I prefer to use _asm instead
    // when I know it will work.  Otherwise, I'll use __rdtsc and hope
    // the code is being compiled with a non-ancient compiler.
    _asm rdtsc
#elif defined(__aarch64__)
    // System timer of ARMv8 runs at a different frequency than the CPU's.
    // The frequency is fixed, typically in the range 1-50MHz.  It can be
    // read at CNTFRQ special register.  We assume the OS has set up
    // the virtual timer properly.
    int64_t virtual_timer_value;
    asm volatile("mrs %0, cntvct_el0" : "=r"(virtual_timer_value));
    return virtual_timer_value;
#elif defined(__ARM_ARCH)

    // V6 is the earliest arch that has a standard cyclecount
    #if (__ARM_ARCH >= 6)
    uint32_t pmccntr;
    uint32_t pmuseren;
    uint32_t pmcntenset;
    // Read the user mode perf monitor counter access permissions.
    asm volatile("mrc p15, 0, %0, c9, c14, 0" : "=r"(pmuseren));
    if (pmuseren & 1)
    {
        // Allows reading perfmon counters for user mode code.
        asm volatile("mrc p15, 0, %0, c9, c12, 1" : "=r"(pmcntenset));
        if (pmcntenset & 0x80000000ul)
        {
            // Is it counting?
            asm volatile("mrc p15, 0, %0, c9, c13, 0" : "=r"(pmccntr));
            // The counter is set up to count every 64th cycle
            return static_cast<int64_t>(pmccntr) * 64;
        }
    }
    #endif

    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return static_cast<int64_t>(tv.tv_sec) * 1000000 + tv.tv_usec;
#elif defined(__mips__)
    // mips apparently only allows rdtsc for superusers, so we fall back to gettimeofday.
    // It's possible clock_gettime would be better.
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return static_cast<int64_t>(tv.tv_sec) * 1000000 + tv.tv_usec;
#else
    #error Unsupported platform
#endif
}

} // namespace CppCommon
