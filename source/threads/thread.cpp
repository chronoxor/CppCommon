/*!
    \file thread.cpp
    \brief Thread abstraction implementation
    \author Ivan Shynkarenka
    \date 27.01.2016
    \copyright MIT License
*/

#include "threads/thread.h"

#include "system/timestamp.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#undef Yield
#define STATUS_SUCCESS 0x00000000
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <errno.h>
#include <pthread.h>
#include <time.h>
#endif

namespace CppCommon {

//! @cond
namespace Internals {

#if defined(_WIN32) || defined(_WIN64)
// Helper function to set minimum resolution of the Windows Timer
uint64_t SetMinimumTimerResolution()
{
    static NTSTATUS(__stdcall *NtQueryTimerResolution)(OUT PULONG MinimumResolution, OUT PULONG MaximumResolution, OUT PULONG ActualResolution) = (NTSTATUS(__stdcall*)(PULONG, PULONG, PULONG))GetProcAddress(GetModuleHandle("ntdll.dll"), "NtQueryTimerResolution");
    static NTSTATUS(__stdcall *NtSetTimerResolution)(IN ULONG RequestedResolution, IN BOOLEAN Set, OUT PULONG ActualResolution) = (NTSTATUS(__stdcall*)(ULONG, BOOLEAN, PULONG))GetProcAddress(GetModuleHandle("ntdll.dll"), "NtSetTimerResolution");

    if ((NtQueryTimerResolution == nullptr) || (NtSetTimerResolution == nullptr))
        return 0;

    ULONG MinimumResolution, MaximumResolution, ActualResolution;
    NTSTATUS ns = NtQueryTimerResolution(&MinimumResolution, &MaximumResolution, &ActualResolution);
    if (ns == STATUS_SUCCESS)
    {
        ns = NtSetTimerResolution(min(MinimumResolution, MaximumResolution), TRUE, &ActualResolution);
        if (ns == STATUS_SUCCESS)
            return (ActualResolution * 100);
    }
    return 1000000;
}
#endif

} // namespace Internals
//! @endcond

uint64_t Thread::CurrentThreadId()
{
#if defined(_WIN32) || defined(_WIN64)
    return GetCurrentThreadId();
#elif defined(unix) || defined(__unix) || defined(__unix__)
    return pthread_self();
#endif
}

void Thread::Sleep(int64_t nanoseconds)
{
    if (nanoseconds < 0)
        return;
    if (nanoseconds == 0)
        return Yield();
#if defined(_WIN32) || defined(_WIN64)
    static NTSTATUS(__stdcall *NtDelayExecution)(IN BOOLEAN Alertable, IN PLARGE_INTEGER DelayInterval) = (NTSTATUS(__stdcall*)(BOOLEAN, PLARGE_INTEGER)) GetProcAddress(GetModuleHandle("ntdll.dll"), "NtDelayExecution");

    // Update once and get Windows Timer resolution
    static int64_t resolution = Internals::SetMinimumTimerResolution();

    int64_t sleep = nanoseconds;
    int64_t yield = nanoseconds % resolution;

    // Yield to other thread for a short time
    if (yield > 0)
    {
        int64_t current = timestamp();
        do
        {
          SwitchToThread();
          int64_t temp = timestamp() - current;
          sleep -= temp;
          yield -= temp;
        } while (yield > 0);
    }

    // Sleep if we have enough time
    if (sleep > 0)
    {
        if (NtDelayExecution != nullptr)
        {
            // Sleep with microsecond precision
            LARGE_INTEGER interval;
            interval.QuadPart = -sleep / 100;
            NtDelayExecution(FALSE, &interval);
        }
        else
        {
            // Sleep with millisecond precision
            Sleep(sleep / 1000000);
        }
    }
#elif defined(unix) || defined(__unix) || defined(__unix__)
    struct timespec req, rem;
    req.tv_sec = nanoseconds / 1000000000;
    req.tv_nsec = nanoseconds % 1000000000;

    // Call nanosleep() in loop until we have remaining time to sleep
    while (nanosleep(&req, &rem) == -1)
    {
        if (errno == EINTR)
            req = rem;
        else
            break;
    }
#endif
}

void Thread::Yield()
{
#if defined(_WIN32) || defined(_WIN64)
    SwitchToThread();
#elif defined(unix) || defined(__unix) || defined(__unix__)
    pthread_yield();
#endif
}

} // namespace CppCommon
