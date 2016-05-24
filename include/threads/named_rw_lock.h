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
    NamedRWLock(NamedRWLock&&) = default;
    ~NamedRWLock();

    NamedRWLock& operator=(const NamedRWLock&) = delete;
    NamedRWLock& operator=(NamedRWLock&&) = default;

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

/*! \example threads_named_rw_lock.cpp Named read/write lock synchronization primitive example */

} // namespace CppCommon

#endif // CPPCOMMON_THREADS_NAMED_RW_LOCK_H
