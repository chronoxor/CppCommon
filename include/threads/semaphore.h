/*!
    \file semaphore.h
    \brief Semaphore synchronization primitive definition
    \author Ivan Shynkarenka
    \date 12.04.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_THREADS_SEMAPHORE_H
#define CPPCOMMON_THREADS_SEMAPHORE_H

#include "threads/locker.h"
#include "time/timestamp.h"

#include <memory>

namespace CppCommon {

//! Semaphore synchronization primitive
/*!
    Semaphore synchronization primitive allows fixed count of threads to access some resource
    while other threads are waiting for it. When some thread unlocks the semaphore then one of
    waiting threads will lock it.

    Thread-safe.

    https://en.wikipedia.org/wiki/Semaphore_(programming)
*/
class Semaphore
{
public:
    //! Default class constructor
    /*!
        \param resources - Semaphore resources counter
    */
    explicit Semaphore(int resources);
    Semaphore(const Semaphore&) = delete;
    Semaphore(Semaphore&& semaphore) = delete;
    ~Semaphore();

    Semaphore& operator=(const Semaphore&) = delete;
    Semaphore& operator=(Semaphore&& semaphore) = delete;

    //! Get the semaphore resources counter
    int resources() const noexcept;

    //! Try to acquire semaphore without block
    /*!
        Will not block.

        \return 'true' if the semaphore was successfully acquired, 'false' if the semaphore is busy
    */
    bool TryLock();

    //! Try to acquire semaphore for the given timespan
    /*!
        Will block for the given timespan in the worst case.

        \param timespan - Timespan to wait for the semaphore
        \return 'true' if the semaphore was successfully acquired, 'false' if the semaphore is busy
    */
    bool TryLockFor(const Timespan& timespan);
    //! Try to acquire semaphore until the given timestamp
    /*!
        Will block until the given timestamp in the worst case.

        \param timestamp - Timestamp to stop wait for the semaphore
        \return 'true' if the semaphore was successfully acquired, 'false' if the semaphore is busy
    */
    bool TryLockUntil(const UtcTimestamp& timestamp)
    { return TryLockFor(timestamp - UtcTimestamp()); }

    //! Acquire semaphore with block
    /*!
        Will block.
    */
    void Lock();

    //! Release semaphore
    /*!
        Will not block.
    */
    void Unlock();

private:
    class Impl;

    Impl& impl() noexcept { return reinterpret_cast<Impl&>(_storage); }
    const Impl& impl() const noexcept { return reinterpret_cast<Impl const&>(_storage); }

    static const size_t StorageSize = 40;
    static const size_t StorageAlign = 8;
    std::aligned_storage<StorageSize, StorageAlign>::type _storage;
};

/*! \example threads_semaphore.cpp Semaphore synchronization primitive example */

} // namespace CppCommon

#endif // CPPCOMMON_THREADS_SEMAPHORE_H
