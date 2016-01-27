/*!
    \file critical_section.h
    \brief Critical section synchronization primitive definition
    \author Ivan Shynkarenka
    \date 27.01.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_CRITICAL_SECTION_H
#define CPPCOMMON_CRITICAL_SECTION_H

#include <chrono>

#include "system/timestamp.h"

namespace CppCommon {

//! Critical section synchronization primitive
/*!
    Critical sections prevents code fragments from access by multiple threads simultaneously. Only one thread can
    access the code inside the critical section. Other threads must wait for the lock! Critical sections are usually
    more lightweight than mutexes and don't enter kernel mode.

    https://en.wikipedia.org/wiki/Critical_section
*/
class CriticalSection
{
public:
    CriticalSection();
    CriticalSection(const CriticalSection&) = delete;
    CriticalSection(CriticalSection&&) = default;
    ~CriticalSection();

    CriticalSection& operator=(const CriticalSection&) = delete;
    CriticalSection& operator=(CriticalSection&&) = default;

    //! Try to acquire critical section without block
    /*!
        Thread-safe. Will not block.

        \return 'true' if the critical section was successfully acquired, 'false' if the critical section is busy
    */
    bool TryLock();

    //! Try to acquire critical section for the given spin count
    /*!
        Thread-safe. Will block for the given spin count in the worst case.

        \param spin - spin count
        \return 'true' if the critical section was successfully acquired, 'false' if the critical section is busy
    */
    bool TryLockSpin(int64_t spin);

    //! Acquire critical section with block
    /*!
        Thread-safe. Will block.
    */
    void lock();

    //! Release critical section
    /*!
        Thread-safe. Will not block.
    */
    void unlock();

private:
    std::atomic<bool> _lock;
};

} // namespace CppCommon

#include "critical_section.inl"

#endif //CPPCOMMON_CRITICAL_SECTION_H
