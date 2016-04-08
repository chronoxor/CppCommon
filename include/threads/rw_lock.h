/*!
    \file rw_lock.h
    \brief Read/Write lock synchronization primitive definition
    \author Ivan Shynkarenka
    \date 08.04.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_THREADS_RW_LOCK_H
#define CPPCOMMON_THREADS_RW_LOCK_H

#include "threads/locker.h"

#include <memory>

namespace CppCommon {

//! Read/Write lock synchronization primitive
/*!
    An read/write lock allows concurrent access for read-only operations, while write operations
    require exclusive access. This means that multiple threads can read the data in parallel but
    an exclusive lock is needed for writing or modifying data. When a writer is writing the data,
    all other writers or readers will be blocked until the writer is finished writing.

    Thread-safe.

    https://en.wikipedia.org/wiki/Readers%E2%80%93writer_lock
*/
class RWLock
{
public:
    RWLock();
    RWLock(const RWLock&) = delete;
    RWLock(RWLock&&) = default;
    ~RWLock();

    RWLock& operator=(const RWLock&) = delete;
    RWLock& operator=(RWLock&&) = default;

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

/*! \example threads_rw_lock.cpp Read/Write lock synchronization primitive example */

} // namespace CppCommon

#endif // CPPCOMMON_THREADS_RW_LOCK_H
