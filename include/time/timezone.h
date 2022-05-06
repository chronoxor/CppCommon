/*!
    \file timezone.h
    \brief Timezone definition
    \author Ivan Shynkarenka
    \date 18.07.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_TIME_TIMEZONE_H
#define CPPCOMMON_TIME_TIMEZONE_H

#include "time/time.h"

#include <string>

namespace CppCommon {

//! Timezone
/*!
    Timezone wraps the current timezone and dynamic daylight saving time settings.

    Not thread-safe.
*/
class Timezone
{
public:
    //! Initialize timezone with a current local timezone
    Timezone();
    //! Initialize timezone with a given local time offset and daylight saving time offset
    /*!
        \param name - Timezone name
        \param offset - Local time offset
        \param dstoffset - Daylight saving time offset (default is Timespan::zero())
    */
    explicit Timezone(const std::string& name, const Timespan& offset, const Timespan& dstoffset = Timespan::zero())
        : _name(name), _offset(offset), _dstoffset(dstoffset)
    {}
    Timezone(const Timezone&) = default;
    Timezone(Timezone&&) = default;
    ~Timezone() = default;

    Timezone& operator=(const Timezone&) = default;
    Timezone& operator=(Timezone&&) = default;

    // Timezone comparison
    friend bool operator==(const Timezone& timezone1, const Timezone& timezone2) noexcept
    { return timezone1.total() == timezone2.total(); }
    friend bool operator!=(const Timezone& timezone1, const Timezone& timezone2) noexcept
    { return timezone1.total() != timezone2.total(); }
    friend bool operator>(const Timezone& timezone1, const Timezone& timezone2) noexcept
    { return timezone1.total() > timezone2.total(); }
    friend bool operator<(const Timezone& timezone1, const Timezone& timezone2) noexcept
    { return timezone1.total() < timezone2.total(); }
    friend bool operator>=(const Timezone& timezone1, const Timezone& timezone2) noexcept
    { return timezone1.total() >= timezone2.total(); }
    friend bool operator<=(const Timezone& timezone1, const Timezone& timezone2) noexcept
    { return timezone1.total() <= timezone2.total(); }

    //! Get timezone name
    const std::string& name() const noexcept { return _name; }
    //! Get timezone offset
    const Timespan& offset() const noexcept { return _offset; }
    //! Get timezone daylight saving time offset
    const Timespan& daylight() const noexcept { return _dstoffset; }

    //! Get timezone total offset
    Timespan total() const noexcept { return _offset + _dstoffset; }

    //! Convert UTC time to local time using the current timezone
    /*!
        \param utctime - UTC time
        \return Local time
    */
    LocalTime Convert(const UtcTime& utctime) const
    { return LocalTime(utctime + total()); }
    //! Convert local time to UTC time using the current timezone
    /*!
        \param localtime - Local time
        \return Local time
    */
    UtcTime Convert(const LocalTime& localtime) const
    { return UtcTime(localtime - total()); }

    //! Get the UTC timezone (Greenwich Mean Time)
    /*!
        Thread-safe.

        \return UTC timezone
    */
    static Timezone utc()
    { return Timezone("GMT", Timespan::zero()); }

    //! Get the local timezone
    /*!
        Thread-safe.

        \return Local timezone
    */
    static Timezone local()
    { return Timezone(); }

    //! Swap two instances
    void swap(Timezone& timezone) noexcept;
    friend void swap(Timezone& timezone1, Timezone& timezone2) noexcept;

protected:
    //! Timezone name
    std::string _name;
    //! Timezone offset
    Timespan _offset;
    //! Timezone daylight saving time offset
    Timespan _dstoffset;
};

/*! \example time_timezone.cpp Timezone example */

} // namespace CppCommon

#include "timezone.inl"

#endif // CPPCOMMON_TIME_TIMEZONE_H
