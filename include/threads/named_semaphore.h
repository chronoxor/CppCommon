/*!
    \file named_semaphore.h
    \brief Named semaphore synchronization primitive definition
    \author Ivan Shynkarenka
    \date 15.04.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_THREADS_NAMED_SEMAPHORE_H
#define CPPCOMMON_THREADS_NAMED_SEMAPHORE_H

#include "threads/locker.h"
#include "time/timestamp.h"

#include <memory>
#include <string>

namespace CppCommon {

//! Named semaphore synchronization primitive
/*!
    Named semaphore behaves as a simple semaphore but could be shared between processes
    on the same machine.

    Thread-safe.

    \see Semaphore
*/
class NamedSemaphore
{
public:
    //! Default class constructor
    /*!
        \param name - Semaphore name
        \param resources - Semaphore resources counter
    */
    explicit NamedSemaphore(const std::string& name, int resources);
    NamedSemaphore(const NamedSemaphore&) = delete;
    NamedSemaphore(NamedSemaphore&& semaphore) = delete;
    ~NamedSemaphore();

    NamedSemaphore& operator=(const NamedSemaphore&) = delete;
    NamedSemaphore& operator=(NamedSemaphore&& semaphore) = delete;

    //! Get the semaphore name
    const std::string& name() const;
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

    static const size_t StorageSize = 56;
    static const size_t StorageAlign = 8;
    std::aligned_storage<StorageSize, StorageAlign>::type _storage;
};

/*! \example threads_named_semaphore.cpp Named semaphore synchronization primitive example */

} // namespace CppCommon

#endif // CPPCOMMON_THREADS_NAMED_SEMAPHORE_H
