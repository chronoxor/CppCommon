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
    explicit FileLock(const Path& path);
    FileLock(const FileLock&) = delete;
    FileLock(FileLock&&) = default;
    ~FileLock();

    FileLock& operator=(const FileLock&) = delete;
    FileLock& operator=(FileLock&&) = default;

    //! Get the file-lock path
    const Path& path() const noexcept;

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
    std::unique_ptr<Impl> _pimpl;
};

/*! \example threads_file_lock.cpp File-lock synchronization primitive example */

} // namespace CppCommon

#endif // CPPCOMMON_THREADS_FILE_LOCK_H
