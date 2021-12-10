/*!
    \file thread.cpp
    \brief Thread implementation
    \author Ivan Shynkarenka
    \date 27.01.2016
    \copyright MIT License
*/

#include "threads/thread.h"

#include "system/cpu.h"
#include "time/timestamp.h"

#include <algorithm>

#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
#include <errno.h>
#include <pthread.h>
#include <sched.h>
#include <time.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <winternl.h>
#undef Yield
#undef max
#undef min
#define STATUS_SUCCESS 0x00000000
#endif

namespace CppCommon {

//! @cond INTERNALS
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
        ns = NtSetTimerResolution(std::min(MinimumResolution, MaximumResolution), TRUE, &ActualResolution);
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
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    return (uint64_t)pthread_self();
#elif defined(_WIN32) || defined(_WIN64)
    return GetCurrentThreadId();
#else
    #error Unsupported platform
#endif
}

uint32_t Thread::CurrentThreadAffinity() noexcept
{
#if defined(__APPLE__) || defined(__CYGWIN__)
    return 0;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    int affinity = sched_getcpu();
    return (affinity < 0) ? 0 : affinity;
#elif defined(_WIN32) || defined(_WIN64)
    return GetCurrentProcessorNumber();
#endif
}

void Thread::SleepFor(const Timespan& timespan) noexcept
{
    if (timespan < 0)
        return;
    if (timespan == 0)
        return Yield();
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    struct timespec req, rem;
    req.tv_sec = timespan.seconds();
    req.tv_nsec = timespan.nanoseconds() % 1000000000;

    // Call nanosleep() in loop until we have remaining time to sleep
    while (nanosleep(&req, &rem) != 0)
    {
        if (errno == EINTR)
            req = rem;
        else
            break;
    }
#elif defined(_WIN32) || defined(_WIN64)
    static NTSTATUS(__stdcall *NtDelayExecution)(IN BOOLEAN Alertable, IN PLARGE_INTEGER DelayInterval) = (NTSTATUS(__stdcall*)(BOOLEAN, PLARGE_INTEGER))GetProcAddress(GetModuleHandle("ntdll.dll"), "NtDelayExecution");

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
#endif
}

void Thread::Yield() noexcept
{
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    sched_yield();
#elif defined(_WIN32) || defined(_WIN64)
    SwitchToThread();
#endif
}

std::bitset<64> Thread::GetAffinity()
{
#if defined(__APPLE__) || defined(__CYGWIN__)
    return std::bitset<64>(0xFFFFFFFFFFFFFFFFull >> (64 - CPU::Affinity()));
#elif defined(unix) || defined(__unix) || defined(__unix__)
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    int result = pthread_getaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
    if (result != 0)
        throwex SystemException("Failed to get the current thread CPU affinity!");
    std::bitset<64> affinity;
    for (int i = 0; i < std::min(CPU_SETSIZE, 64); ++i)
        if (CPU_ISSET(i, &cpuset))
            affinity.set(i);
    return affinity;
#elif defined(_WIN32) || defined(_WIN64)
    typedef LONG KPRIORITY;

    typedef struct _NT_CLIENT_ID
    {
        HANDLE UniqueProcess;
        HANDLE UniqueThread;
    } NT_CLIENT_ID;

    typedef struct _THREAD_BASIC_INFORMATION
    {
        NTSTATUS ExitStatus;
        PTEB TebBaseAddress;
        NT_CLIENT_ID ClientId;
        ULONG_PTR AffinityMask;
        KPRIORITY Priority;
        LONG BasePriority;
    } THREAD_BASIC_INFORMATION;

    static NTSTATUS(__stdcall *NtQueryInformationThread)(IN HANDLE ThreadHandle, IN THREADINFOCLASS ThreadInformationClass, OUT PVOID ThreadInformation, IN ULONG ThreadInformationLength, OUT PULONG ReturnLength) = (NTSTATUS(__stdcall*)(HANDLE, THREADINFOCLASS, PVOID, ULONG, PULONG))GetProcAddress(GetModuleHandle("ntdll.dll"), "NtQueryInformationThread");

    if (NtQueryInformationThread != nullptr)
    {
        THREAD_BASIC_INFORMATION tbi;
        ZeroMemory(&tbi, sizeof(tbi));
        NTSTATUS ns = NtQueryInformationThread(GetCurrentThread(), (THREADINFOCLASS)0, &tbi, sizeof(tbi), nullptr);
        if (ns == STATUS_SUCCESS)
            return std::bitset<64>(tbi.AffinityMask);
        else
            throwex SystemException("Failed to get the current thread CPU affinity!");
    }

    return std::bitset<64>(0xFFFFFFFFFFFFFFFFull >> (64 - CPU::Affinity()));
#endif
}

std::bitset<64> Thread::GetAffinity(std::thread& thread)
{
#if defined(__APPLE__) || defined(__CYGWIN__)
    return std::bitset<64>(0xFFFFFFFFFFFFFFFFull >> (64 - CPU::Affinity()));
#elif defined(unix) || defined(__unix) || defined(__unix__)
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    int result = pthread_getaffinity_np(thread.native_handle(), sizeof(cpu_set_t), &cpuset);
    if (result != 0)
        throwex SystemException("Failed to get the given thread CPU affinity!");
    std::bitset<64> affinity;
    for (int i = 0; i < std::min(CPU_SETSIZE, 64); ++i)
        if (CPU_ISSET(i, &cpuset))
            affinity.set(i);
    return affinity;
#elif defined(_WIN32) || defined(_WIN64)
    typedef LONG KPRIORITY;

    typedef struct _NT_CLIENT_ID
    {
        HANDLE UniqueProcess;
        HANDLE UniqueThread;
    } NT_CLIENT_ID;

    typedef struct _THREAD_BASIC_INFORMATION
    {
        NTSTATUS ExitStatus;
        PTEB TebBaseAddress;
        NT_CLIENT_ID ClientId;
        ULONG_PTR AffinityMask;
        KPRIORITY Priority;
        LONG BasePriority;
    } THREAD_BASIC_INFORMATION;

    static NTSTATUS(__stdcall *NtQueryInformationThread)(IN HANDLE ThreadHandle, IN THREADINFOCLASS ThreadInformationClass, OUT PVOID ThreadInformation, IN ULONG ThreadInformationLength, OUT PULONG ReturnLength) = (NTSTATUS(__stdcall*)(HANDLE, THREADINFOCLASS, PVOID, ULONG, PULONG))GetProcAddress(GetModuleHandle("ntdll.dll"), "NtQueryInformationThread");

    if (NtQueryInformationThread != nullptr)
    {
        THREAD_BASIC_INFORMATION tbi;
        ZeroMemory(&tbi, sizeof(tbi));
        NTSTATUS ns = NtQueryInformationThread((HANDLE)thread.native_handle(), (THREADINFOCLASS)0, &tbi, sizeof(tbi), nullptr);
        if (ns == STATUS_SUCCESS)
            return std::bitset<64>(tbi.AffinityMask);
        else
            throwex SystemException("Failed to get the given thread CPU affinity!");
    }

    return std::bitset<64>(0xFFFFFFFFFFFFFFFFull >> (64 - CPU::Affinity()));
#endif
}

void Thread::SetAffinity(const std::bitset<64>& affinity)
{
#if defined(__APPLE__)
    throwex SystemException("Apple platform does not allow to set the current thread CPU affinity!");
#elif defined(__CYGWIN__)
    throwex SystemException("Cygwin platform does not allow to set the current thread CPU affinity!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    for (int i = 0; i < std::min(CPU_SETSIZE, 64); ++i)
        if (affinity[i])
            CPU_SET(i, &cpuset);
    int result = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
    if (result != 0)
        throwex SystemException("Failed to set the current thread CPU affinity!");
#elif defined(_WIN32) || defined(_WIN64)
    DWORD_PTR dwThreadAffinityMask = (DWORD_PTR)affinity.to_ullong();
    if (!SetThreadAffinityMask(GetCurrentThread(), dwThreadAffinityMask))
        throwex SystemException("Failed to set the current thread CPU affinity!");
#endif
}

void Thread::SetAffinity(std::thread& thread, const std::bitset<64>& affinity)
{
#if defined(__APPLE__)
    throwex SystemException("Apple platform does not allow to set the given thread CPU affinity!");
#elif defined(__CYGWIN__)
    throwex SystemException("Cygwin platform does not allow to set the given thread CPU affinity!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    for (int i = 0; i < std::min(CPU_SETSIZE, 64); ++i)
        if (affinity[i])
            CPU_SET(i, &cpuset);
    int result = pthread_setaffinity_np(thread.native_handle(), sizeof(cpu_set_t), &cpuset);
    if (result != 0)
        throwex SystemException("Failed to set the given thread CPU affinity!");
#elif defined(_WIN32) || defined(_WIN64)
    DWORD_PTR dwThreadAffinityMask = (DWORD_PTR)affinity.to_ullong();
    if (!SetThreadAffinityMask((HANDLE)thread.native_handle(), dwThreadAffinityMask))
        throwex SystemException("Failed to set the given thread CPU affinity!");
#endif
}

ThreadPriority Thread::GetPriority()
{
#if defined(__CYGWIN__)
    return ThreadPriority::NORMAL;
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
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
#elif defined(_WIN32) || defined(_WIN64)
    int priority = GetThreadPriority(GetCurrentThread());
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
#endif
}

ThreadPriority Thread::GetPriority(std::thread& thread)
{
#if defined(__CYGWIN__)
    return ThreadPriority::NORMAL;
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
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
#elif defined(_WIN32) || defined(_WIN64)
    int priority = GetThreadPriority((HANDLE)thread.native_handle());
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
#endif
}

void Thread::SetPriority(ThreadPriority priority)
{
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    int policy = SCHED_RR;
    struct sched_param sched;
    sched.sched_priority = 50;
    switch (priority)
    {
        case ThreadPriority::IDLE:
            sched.sched_priority = 1;
            break;
        case ThreadPriority::LOWEST:
            sched.sched_priority = 15;
            break;
        case ThreadPriority::LOW:
            sched.sched_priority = 30;
            break;
        case ThreadPriority::NORMAL:
            sched.sched_priority = 50;
            break;
        case ThreadPriority::HIGH:
            sched.sched_priority = 70;
            break;
        case ThreadPriority::HIGHEST:
            sched.sched_priority = 85;
            break;
        case ThreadPriority::REALTIME:
            sched.sched_priority = 99;
            break;
    }

    int result = pthread_setschedparam(pthread_self(), policy, &sched);
    if (result != 0)
        throwex SystemException("Failed to set the current thread priority!");
#elif defined(_WIN32) || defined(_WIN64)
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

    if (!SetThreadPriority(GetCurrentThread(), nPriority))
        throwex SystemException("Failed to set the current thread priority!");
#endif
}

void Thread::SetPriority(std::thread& thread, ThreadPriority priority)
{
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    int policy = SCHED_RR;
    struct sched_param sched;
    sched.sched_priority = 50;
    switch (priority)
    {
        case ThreadPriority::IDLE:
            sched.sched_priority = 1;
            break;
        case ThreadPriority::LOWEST:
            sched.sched_priority = 15;
            break;
        case ThreadPriority::LOW:
            sched.sched_priority = 30;
            break;
        case ThreadPriority::NORMAL:
            sched.sched_priority = 50;
            break;
        case ThreadPriority::HIGH:
            sched.sched_priority = 70;
            break;
        case ThreadPriority::HIGHEST:
            sched.sched_priority = 85;
            break;
        case ThreadPriority::REALTIME:
            sched.sched_priority = 99;
            break;
    }

    int result = pthread_setschedparam(thread.native_handle(), policy, &sched);
    if (result != 0)
        throwex SystemException("Failed to set the given thread priority!");
#elif defined(_WIN32) || defined(_WIN64)
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

    if (!SetThreadPriority((HANDLE)thread.native_handle(), nPriority))
        throwex SystemException("Failed to set the given thread priority!");
#endif
}

} // namespace CppCommon
