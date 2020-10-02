/*!
    \file named_rw_lock.h
    \brief Named read/write lock synchronization primitive definition
    \author Ivan Shynkarenka
    \date 24.05.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_THREADS_NAMED_RW_LOCK_H
#define CPPCOMMON_THREADS_NAMED_RW_LOCK_H

#include "threads/locker.h"
#include "time/timestamp.h"

#include <memory>
#include <string>

namespace CppCommon {

//! Named read/write lock synchronization primitive
/*!
    Named read/write lock behaves as a simple read/write lock but could be shared between processes
    on the same machine.

    The fast lock is around 7% faster than the critical section when there is no contention, when
    used solely for mutual exclusion. It is also much smaller than the critical section.

    Thread-safe.

    \see RWLock
*/
class NamedRWLock
{
public:
    //! Default class constructor
    /*!
        \param name - Read/Write lock name
    */
    explicit NamedRWLock(const std::string& name);
    NamedRWLock(const NamedRWLock&) = delete;
    NamedRWLock(NamedRWLock&& lock) = delete;
    ~NamedRWLock();

    NamedRWLock& operator=(const NamedRWLock&) = delete;
    NamedRWLock& operator=(NamedRWLock&& lock) = delete;

    //! Get the read/write lock name
    const std::string& name() const;

    //! Try to acquire read lock without block
    /*!
        Will not block.

        \return 'true' if the read lock was successfully acquired, 'false' if the read lock is busy
    */
    bool TryLockRead();
    //! Try to acquire write lock without block
    /*!
        Will not block.

        \return 'true' if the write lock was successfully acquired, 'false' if the write lock is busy
    */
    bool TryLockWrite();
    //! Try to convert write lock to read lock without block
    /*!
        Will not block.

        \return 'true' if the write lock was successfully converted to the read one, 'false' if the write lock is busy
    */
    bool TryConvertWriteToRead();

    //! Try to acquire read lock for the given timespan
    /*!
        Will block for the given timespan in the worst case.

        \param timespan - Timespan to wait for the read lock
        \return 'true' if the read lock was successfully acquired, 'false' if the read lock is busy
    */
    bool TryLockReadFor(const Timespan& timespan);
    //! Try to acquire write lock for the given timespan
    /*!
        Will block for the given timespan in the worst case.

        \param timespan - Timespan to wait for the write lock
        \return 'true' if the write lock was successfully acquired, 'false' if the write lock is busy
    */
    bool TryLockWriteFor(const Timespan& timespan);
    //! Try to convert write lock to read lock for the given timespan
    /*!
        Will block for the given timespan in the worst case.

        \param timespan - Timespan to wait for the write lock to read lock conversion
        \return 'true' if the write lock was successfully converted to the read one, 'false' if the write lock is busy
    */
    bool TryConvertWriteToReadFor(const Timespan& timespan);
    //! Try to acquire read lock until the given timestamp
    /*!
        Will block until the given timestamp in the worst case.

        \param timestamp - Timestamp to stop wait for the read lock
        \return 'true' if the read lock was successfully acquired, 'false' if the read lock is busy
    */
    bool TryLockReadUntil(const UtcTimestamp& timestamp)
    { return TryLockReadFor(timestamp - UtcTimestamp()); }
    //! Try to acquire write lock until the given timestamp
    /*!
        Will block until the given timestamp in the worst case.

        \param timestamp - Timestamp to stop wait for the write lock
        \return 'true' if the write lock was successfully acquired, 'false' if the write lock is busy
    */
    bool TryLockWriteUntil(const UtcTimestamp& timestamp)
    { return TryLockWriteFor(timestamp - UtcTimestamp()); }
    //! Try to convert write lock to read lock until the given timestamp
    /*!
        Will block until the given timestamp in the worst case.

        \param timestamp - Timestamp to stop wait for the write lock to read lock conversion
        \return 'true' if the write lock was successfully converted to the read one, 'false' if the write lock is busy
    */
    bool TryConvertWriteToReadUntil(const UtcTimestamp& timestamp)
    { return TryConvertWriteToReadFor(timestamp - UtcTimestamp()); }

    //! Acquire read lock with block
    /*!
        Will block.
    */
    void LockRead();
    //! Acquire write lock with block
    /*!
        Will block.
    */
    void LockWrite();

    //! Release read lock
    /*!
        Will not block.
    */
    void UnlockRead();
    //! Release write lock
    /*!
        Will not block.
    */
    void UnlockWrite();

    //! Convert write lock to read lock without block
    /*!
        Exclusive acquires are not given a chance to acquire
        the lock before this function does - as a result,
        this function will never block.

        Will not block.
    */
    void ConvertWriteToRead();

private:
    class Impl;

    Impl& impl() noexcept { return reinterpret_cast<Impl&>(_storage); }
    const Impl& impl() const noexcept { return reinterpret_cast<Impl const&>(_storage); }

    static const size_t StorageSize = 184;
    static const size_t StorageAlign = 8;
    std::aligned_storage<StorageSize, StorageAlign>::type _storage;
};

/*! \example threads_named_rw_lock.cpp Named read/write lock synchronization primitive example */

} // namespace CppCommon

#endif // CPPCOMMON_THREADS_NAMED_RW_LOCK_H
