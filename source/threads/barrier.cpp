/*!
    \file barrier.cpp
    \brief Barrier synchronization primitive implementation
    \author Ivan Shynkarenka
    \date 16.03.2016
    \copyright MIT License
*/

#include "threads/barrier.h"

#include "errors/exceptions.h"
#include "errors/fatal.h"

#include <cassert>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <pthread.h>
#endif

namespace CppCommon {

class Barrier::Impl
{
public:
    Impl(int threads)
    {
        assert((threads > 0) && "Barrier threads counter must be greater than zero!");

#if defined(_WIN32) || defined(_WIN64)
        if (!InitializeSynchronizationBarrier(&_barrier, threads, -1))
            throwex SystemException("Failed to initialize a synchronization barrier!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_barrier_init(&_barrier, nullptr, threads);
        if (result != 0)
            throwex SystemException("Failed to initialize a synchronization barrier!", result);
#endif
    }

    ~Impl()
    {
#if defined(_WIN32) || defined(_WIN64)
        DeleteSynchronizationBarrier(&_barrier);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_barrier_destroy(&_barrier);
        if (result != 0)
            fatality(SystemException("Failed to destroy a synchronization barrier!", result));
#endif
    }

    bool Wait()
    {
#if defined(_WIN32) || defined(_WIN64)
        return (EnterSynchronizationBarrier(&_barrier, 0) == TRUE);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = pthread_barrier_wait(&_barrier);
        if ((result != PTHREAD_BARRIER_SERIAL_THREAD) && (result != 0))
            throwex SystemException("Failed to wait at a synchronization barrier!", result);
        return (result == PTHREAD_BARRIER_SERIAL_THREAD);
#endif
    }

private:
#if defined(_WIN32) || defined(_WIN64)
    SYNCHRONIZATION_BARRIER _barrier;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    pthread_barrier_t _barrier;
#endif
};

Barrier::Barrier(int threads) : _pimpl(new Impl(threads)), _threads(threads)
{
}

Barrier::~Barrier()
{
}

bool Barrier::Wait()
{
    return _pimpl->Wait();
}

} // namespace CppCommon
