/*!
    \file thread.cpp
    \brief Thread abstraction implementation
    \author Ivan Shynkarenka
    \date 27.01.2016
    \copyright MIT License
*/

#if defined(_MSC_VER)
#include <windows.h>
#undef Yield
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <pthread.h>
#include <time.h>
#endif

#include "threads/thread.h"

namespace CppCommon {

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
#if defined(_WIN32) || defined(_WIN64)

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
