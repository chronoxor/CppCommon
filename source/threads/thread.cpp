/*!
    \file thread.cpp
    \brief Thread abstraction implementation
    \author Ivan Shynkarenka
    \date 27.01.2016
    \copyright MIT License
*/

#include "threads/thread.h"

#include "errors/exceptions.h"
#include "time/timestamp.h"

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

uint64_t Thread::CurrentThreadId() noexcept
{
#if defined(_WIN32) || defined(_WIN64)
    return GetCurrentThreadId();
#elif defined(unix) || defined(__unix) || defined(__unix__)
    return pthread_self();
#endif
}

void Thread::SleepFor(const Timespan& timespan) noexcept
{
    if (timespan < 0)
        return;
    if (timespan == 0)
        return Yield();
#if defined(_WIN32) || defined(_WIN64)
    static NTSTATUS(__stdcall *NtDelayExecution)(IN BOOLEAN Alertable, IN PLARGE_INTEGER DelayInterval) = (NTSTATUS(__stdcall*)(BOOLEAN, PLARGE_INTEGER)) GetProcAddress(GetModuleHandle("ntdll.dll"), "NtDelayExecution");

    // Update once and get Windows Timer resolution
    static int64_t resolution = Internals::SetMinimumTimerResolution();

    int64_t sleep = timespan.nanoseconds();
    int64_t yield = timespan.nanoseconds() % resolution;

    // Yield to other thread for a short time
    if (yield > 0)
    {
        int64_t current = Timestamp::nano();
        do
        {
          SwitchToThread();
          int64_t temp = Timestamp::nano() - current;
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
    req.tv_sec = timespan.seconds();
    req.tv_nsec = timespan.nanoseconds() % 1000000000;

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

void Thread::Yield() noexcept
{
#if defined(_WIN32) || defined(_WIN64)
    SwitchToThread();
#elif defined(unix) || defined(__unix) || defined(__unix__)
    pthread_yield();
#endif
}

ThreadPriority Thread::GetPriority()
{
#if defined(_WIN32) || defined(_WIN64)
    HANDLE hThread = GetCurrentThread();
    int priority = GetThreadPriority(hThread);
    if (priority == THREAD_PRIORITY_ERROR_RETURN)
        throwex SystemException("Failed to get the current thread priority!");
    if (priority < THREAD_PRIORITY_LOWEST)
        return ThreadPriority::IDLE;
    else if (priority < THREAD_PRIORITY_BELOW_NORMAL)
        return ThreadPriority::LOWEST;
    else if (priority < THREAD_PRIORITY_NORMAL)
        return ThreadPriority::LOW;
    else if (priority < THREAD_PRIORITY_ABOVE_NORMAL)
        return ThreadPriority::NORMAL;
    else if (priority < THREAD_PRIORITY_HIGHEST)
        return ThreadPriority::HIGH;
    else if (priority < THREAD_PRIORITY_TIME_CRITICAL)
        return ThreadPriority::HIGHEST;
    else
        return ThreadPriority::REALTIME;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    int policy;
    struct sched_param sched;
    int result = pthread_getschedparam(pthread_self(), &policy, &sched);
    if (result != 0)
        throwex SystemException("Failed to get the current thread priority!");
    if ((policy == SCHED_FIFO) || (policy == SCHED_RR))
    {
        if (sched.sched_priority < 15)
            return ThreadPriority::IDLE;
        else if (sched.sched_priority < 30)
            return ThreadPriority::LOWEST;
        else if (sched.sched_priority < 50)
            return ThreadPriority::LOW;
        else if (sched.sched_priority < 70)
            return ThreadPriority::NORMAL;
        else if (sched.sched_priority < 85)
            return ThreadPriority::HIGH;
        else if (sched.sched_priority < 99)
            return ThreadPriority::HIGHEST;
        else
            return ThreadPriority::REALTIME;
    }
    else
        return ThreadPriority::NORMAL;
#endif
}

ThreadPriority Thread::GetPriority(std::thread& thread)
{
#if defined(_WIN32) || defined(_WIN64)
    HANDLE hThread = thread.native_handle();
    int priority = GetThreadPriority(hThread);
    if (priority == THREAD_PRIORITY_ERROR_RETURN)
        throwex SystemException("Failed to get the given thread priority!");
    if (priority < THREAD_PRIORITY_LOWEST)
        return ThreadPriority::IDLE;
    else if (priority < THREAD_PRIORITY_BELOW_NORMAL)
        return ThreadPriority::LOWEST;
    else if (priority < THREAD_PRIORITY_NORMAL)
        return ThreadPriority::LOW;
    else if (priority < THREAD_PRIORITY_ABOVE_NORMAL)
        return ThreadPriority::NORMAL;
    else if (priority < THREAD_PRIORITY_HIGHEST)
        return ThreadPriority::HIGH;
    else if (priority < THREAD_PRIORITY_TIME_CRITICAL)
        return ThreadPriority::HIGHEST;
    else
        return ThreadPriority::REALTIME;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    int policy;
    struct sched_param sched;
    int result = pthread_getschedparam(thread.native_handle(), &policy, &sched);
    if (result != 0)
        throwex SystemException("Failed to get the given thread priority!");
    if ((policy == SCHED_FIFO) || (policy == SCHED_RR))
    {
        if (sched.sched_priority < 15)
            return ThreadPriority::IDLE;
        else if (sched.sched_priority < 30)
            return ThreadPriority::LOWEST;
        else if (sched.sched_priority < 50)
            return ThreadPriority::LOW;
        else if (sched.sched_priority < 70)
            return ThreadPriority::NORMAL;
        else if (sched.sched_priority < 85)
            return ThreadPriority::HIGH;
        else if (sched.sched_priority < 99)
            return ThreadPriority::HIGHEST;
        else
            return ThreadPriority::REALTIME;
    }
    else
        return ThreadPriority::NORMAL;
#endif
}

void Thread::SetPriority(ThreadPriority priority)
{
#if defined(_WIN32) || defined(_WIN64)
    int nPriority = THREAD_PRIORITY_NORMAL;
    switch (priority)
    {
        case ThreadPriority::IDLE:
            nPriority = THREAD_PRIORITY_IDLE;
            break;
        case ThreadPriority::LOWEST:
            nPriority = THREAD_PRIORITY_LOWEST;
            break;
        case ThreadPriority::LOW:
            nPriority = THREAD_PRIORITY_BELOW_NORMAL;
            break;
        case ThreadPriority::NORMAL:
            nPriority = THREAD_PRIORITY_NORMAL;
            break;
        case ThreadPriority::HIGH:
            nPriority = THREAD_PRIORITY_ABOVE_NORMAL;
            break;
        case ThreadPriority::HIGHEST:
            nPriority = THREAD_PRIORITY_HIGHEST;
            break;
        case ThreadPriority::REALTIME:
            nPriority = THREAD_PRIORITY_TIME_CRITICAL;
            break;
    }

    HANDLE hThread = GetCurrentThread();
    if (!SetThreadPriority(hThread, nPriority))
        throwex SystemException("Failed to set the current thread priority!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
    int policy = SCHED_RR;
    struct sched_param sched;
    sched.sched_priority = 50;
    switch (priority)
    {
        case ThreadPriority::IDLE:
            nPriority = 1;
            break;
        case ThreadPriority::LOWEST:
            nPriority = 15;
            break;
        case ThreadPriority::LOW:
            nPriority = 30;
            break;
        case ThreadPriority::NORMAL:
            nPriority = 50;
            break;
        case ThreadPriority::HIGH:
            nPriority = 70;
            break;
        case ThreadPriority::HIGHEST:
            nPriority = 85;
            break;
        case ThreadPriority::REALTIME:
            nPriority = 99;
            break;
    }

    int result = pthread_setschedparam(pthread_self(), policy, &sched);
    if (result != 0)
        throwex SystemException("Failed to set the current thread priority!");
#endif
}

void Thread::SetPriority(std::thread& thread, ThreadPriority priority)
{
#if defined(_WIN32) || defined(_WIN64)
    int nPriority = THREAD_PRIORITY_NORMAL;
    switch (priority)
    {
        case ThreadPriority::IDLE:
            nPriority = THREAD_PRIORITY_IDLE;
            break;
        case ThreadPriority::LOWEST:
            nPriority = THREAD_PRIORITY_LOWEST;
            break;
        case ThreadPriority::LOW:
            nPriority = THREAD_PRIORITY_BELOW_NORMAL;
            break;
        case ThreadPriority::NORMAL:
            nPriority = THREAD_PRIORITY_NORMAL;
            break;
        case ThreadPriority::HIGH:
            nPriority = THREAD_PRIORITY_ABOVE_NORMAL;
            break;
        case ThreadPriority::HIGHEST:
            nPriority = THREAD_PRIORITY_HIGHEST;
            break;
        case ThreadPriority::REALTIME:
            nPriority = THREAD_PRIORITY_TIME_CRITICAL;
            break;
    }

    HANDLE hThread = thread.native_handle();
    if (!SetThreadPriority(hThread, nPriority))
        throwex SystemException("Failed to set the given thread priority!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
    int policy = SCHED_RR;
    struct sched_param sched;
    sched.sched_priority = 50;
    switch (priority)
    {
        case ThreadPriority::IDLE:
            nPriority = 1;
            break;
        case ThreadPriority::LOWEST:
            nPriority = 15;
            break;
        case ThreadPriority::LOW:
            nPriority = 30;
            break;
        case ThreadPriority::NORMAL:
            nPriority = 50;
            break;
        case ThreadPriority::HIGH:
            nPriority = 70;
            break;
        case ThreadPriority::HIGHEST:
            nPriority = 85;
            break;
        case ThreadPriority::REALTIME:
            nPriority = 99;
            break;
    }

    int result = pthread_setschedparam(thread.native_handle(), policy, &sched);
    if (result != 0)
        throwex SystemException("Failed to set the given thread priority!");
#endif
}

} // namespace CppCommon
