/*!
    \file mutex.h
    \brief Mutex synchronization primitive definition
    \author Ivan Shynkarenka
    \date 04.04.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_THREADS_MUTEX_H
#define CPPCOMMON_THREADS_MUTEX_H

#include <memory>

namespace CppCommon {

//! Mutex synchronization primitive
/*!
    A mutex object facilitates protection against data races and allows thread-safe synchronization
    of data between threads. A thread obtains ownership of a mutex object by calling one of the lock
    functions and relinquishes ownership by calling the corresponding unlock function.

    Thread-safe.

    https://en.wikipedia.org/wiki/Mutual_exclusion
*/
class Mutex
{
public:
    Mutex();
    Mutex(const Mutex&) = delete;
    Mutex(Mutex&&) = default;
    ~Mutex();

    Mutex& operator=(const Mutex&) = delete;
    Mutex& operator=(Mutex&&) = default;

    //! Try to acquire mutex without block
    /*!
        Will not block.

        \return 'true' if the mutex was successfully acquired, 'false' if the mutex is busy
    */
    bool TryLock();

    //! Try to acquire mutex for the given spin count
    /*!
        Will block for the given spin count in the worst case.

        \param spin - Spin count
        \return 'true' if the mutex was successfully acquired, 'false' if the mutex is busy
    */
    bool TryLockSpin(int64_t spin);

    //! Acquire mutex with block
    /*!
        Will block.
    */
    void lock();

    //! Release mutex
    /*!
        Will not block.
    */
    void unlock();

private:
    class Impl;
    std::unique_ptr<Impl> _pimpl;
};

/*! \example threads_mutex.cpp Mutex synchronization primitive example */

} // namespace CppCommon

#include "mutex.inl"

#endif // CPPCOMMON_THREADS_MUTEX_H
