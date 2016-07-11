/*!
    \file rdts.h
    \brief Read Time Stamp Counter definition
    \author Ivan Shynkarenka
    \date 22.01.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_TIME_RDTS_H
#define CPPCOMMON_TIME_RDTS_H

#include <cstdint>

namespace CppCommon {

//! Read Time Stamp Counter
/*!
    Counts the number of CPU cycles since reset. The Time Stamp Counter (TSC) is a 64-bit register present on all x86
    processors since the Pentium.

    Thread-safe.

    https://en.wikipedia.org/wiki/Time_Stamp_Counter

    \return Number of CPU cycles since the last system reset
*/
uint64_t rdts() noexcept;

/*! \example time_rdts.cpp Read Time Stamp Counter example */

} // namespace CppCommon

#endif // CPPCOMMON_TIME_RDTS_H
