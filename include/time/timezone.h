/*!
    \file timezone.h
    \brief Time zone definition
    \author Ivan Shynkarenka
    \date 18.07.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_TIME_TIMEZONE_H
#define CPPCOMMON_TIME_TIMEZONE_H

#include "time/time.h"

#include <string>

namespace CppCommon {

//! Time zone
/*!
    Time zone wraps the current time zone and dynamic daylight saving time settings.

    Not thread-safe.
*/
class Timezone
{
public:
    //! Initialize timezone with a current local time zone
    Timezone();
    //! Initialize time zone with a given local time offset and daylight saving time offset
    /*!
        \param name - Time zone name
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

    //! Get time zone name
    const std::string& name() const noexcept { return _name; }
    //! Get time zone offset
    const Timespan& offset() const noexcept { return _offset; }
    //! Get time zone daylight saving time offset
    const Timespan& daylight() const noexcept { return _dstoffset; }

    //! Get time zone total offset
    Timespan total() const noexcept { return _offset + _dstoffset; }

    //! Convert UTC time to local time using the current time zone
    /*!
        \param utctime - UTC time
        \return Local time
    */
    LocalTime Convert(const UtcTime& utctime)
    { return LocalTime(utctime + total()); }
    //! Convert local time to UTC time using the current time zone
    /*!
        \param localtime - Local time
        \return Local time
    */
    UtcTime Convert(const LocalTime& localtime)
    { return UtcTime(localtime - total()); }

    //! Get the UTC time zone (Greenwich Mean Time)
    /*!
        Thread-safe.

        \return UTC time zone
    */
    static Timezone utc()
    { return Timezone("GMT", Timespan::zero()); }

    //! Get the local time zone
    /*!
        Thread-safe.

        \return Local time zone
    */
    static Timezone local()
    { return Timezone(); }

    //! Swap two instances
    void swap(Timezone& timezone) noexcept;
    friend void swap(Timezone& timezone1, Timezone& timezone2) noexcept;

protected:
    //! Time zone name
    std::string _name;
    //! Time zone offset
    Timespan _offset;
    //! Time zone daylight saving time offset
    Timespan _dstoffset;
};

/*! \example time_timezone.cpp Time zone example */

} // namespace CppCommon

#include "timezone.inl"

#endif // CPPCOMMON_TIME_TIMEZONE_H
