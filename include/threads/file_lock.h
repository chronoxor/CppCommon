/*!
    \file file_lock.h
    \brief File-lock synchronization primitive definition
    \author Ivan Shynkarenka
    \date 01.09.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_THREADS_FILE_LOCK_H
#define CPPCOMMON_THREADS_FILE_LOCK_H

#include "filesystem/file.h"
#include "threads/locker.h"
#include "time/timestamp.h"

#include <memory>

namespace CppCommon {

//! File-lock synchronization primitive
/*!
    A file-lock provide shared and exclusive access to some resource based
    on the file locking functionality provided by OS. This means that multiple
    threads or processes can read the data in parallel but an exclusive lock
    is needed for writing or modifying data. When a writer is writing the data,
    all other writers or readers will be blocked until the writer is finished
    writing.

    Thread-safe.

    https://en.wikipedia.org/wiki/File_locking
*/
class FileLock
{
public:
    FileLock();
    explicit FileLock(const Path& path);
    FileLock(const FileLock&) = delete;
    FileLock(FileLock&& lock) = delete;
    ~FileLock();

    FileLock& operator=(const Path& path);
    FileLock& operator=(const FileLock&) = delete;
    FileLock& operator=(FileLock&& lock) = delete;

    //! Get the file-lock path
    const Path& path() const noexcept;

    //! Assign a new file-lock path
    /*!
        \param path - File-lock path
    */
    void Assign(const Path& path);

    //! Reset file-lock
    void Reset();

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

private:
    class Impl;

    Impl& impl() noexcept { return reinterpret_cast<Impl&>(_storage); }
    const Impl& impl() const noexcept { return reinterpret_cast<Impl const&>(_storage); }

    static const size_t StorageSize = 48;
    static const size_t StorageAlign = 8;
    std::aligned_storage<StorageSize, StorageAlign>::type _storage;
};

/*! \example threads_file_lock.cpp File-lock synchronization primitive example */

} // namespace CppCommon

#endif // CPPCOMMON_THREADS_FILE_LOCK_H
