/*!
    \file timespan.h
    \brief Time span definition
    \author Ivan Shynkarenka
    \date 11.07.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_SYSTEM_TIMESPAN_H
#define CPPCOMMON_SYSTEM_TIMESPAN_H

#include <chrono>

namespace CppCommon {

//! Time span
/*!
    Time span wraps time duration in nanoseconds or difference between two timestamps
    and allows to get delta and total values of days, hours, minutes, seconds and
    milli/micro/nano-seconds.

    Not thread-safe.
*/
class TimeSpan
{
public:
    //! Initialize time span with a time duration value in nanoseconds
    /*!
        \param duration - Time duration value in nanoseconds
    */
    explicit TimeSpan(int64_t duration) noexcept;
    //! Initialize time span with a time duration chrono value
    /*!
        \param duration - Time duration chrono value
    */
    template <class Rep, class Period>
    explicit TimeSpan(const std::chrono::duration<Rep, Period>& duration) noexcept;
    TimeSpan(const TimeSpan&) noexcept = default;
    TimeSpan(TimeSpan&&) noexcept = default;
    ~TimeSpan() noexcept = default;

    TimeSpan& operator=(const TimeSpan&) noexcept = default;
    TimeSpan& operator=(TimeSpan&&) noexcept = default;

    //!
    operator std::chrono::duration<int64_t, std::nanoseconds>() const { return (ptr() != nullptr); }

    //! Get the shared memory block name
    const std::string& name() const noexcept { return _name; }
    //! Get the shared memory block size
    size_t size() const noexcept { return _size; }

    //! Get the shared memory block pointer
    void* ptr();
    //! Get the constant shared memory block pointer
    const void* ptr() const;

    //! Get the shared memory owner flag (true if the new one was created, false if the existing one was opened)
    bool owner() const;

private:
    class Impl;
    std::unique_ptr<Impl> _pimpl;
    std::string _name;
    size_t _size;
};

/*! \example system_timespan.cpp Time span example */

} // namespace CppCommon

#endif // CPPCOMMON_SYSTEM_TIMESTAMP_H
