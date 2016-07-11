/*!
    \file timestamp.h
    \brief Nanoseconds timestamp accessor definition
    \author Ivan Shynkarenka
    \date 26.01.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_TIME_TIMESTAMP_H
#define CPPCOMMON_TIME_TIMESTAMP_H

#include <cstdint>

namespace CppCommon {

//! Nanoseconds timestamp accessor
/*!
    Thread-safe.

    \return Timestamp in nanoseconds resolution
*/
uint64_t timestamp() noexcept;

/*! \example time_timestamp.cpp Nanoseconds timestamp accessor example */

} // namespace CppCommon

#endif // CPPCOMMON_TIME_TIMESTAMP_H
