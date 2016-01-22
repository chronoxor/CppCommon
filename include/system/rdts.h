/*!
    \file rdts.h
    \brief Read Time Stamp Counter definition
    \author Ivan Shynkarenka
    \date 22.01.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_RDTS_H
#define CPPCOMMON_RDTS_H

#include <cstdint>

namespace CppCommon {

//! Read Time Stamp Counter
/*!
    Counts the number of CPU cycles since reset. The Time Stamp Counter (TSC) is a 64-bit register present on all x86
    processors since the Pentium.

    https://en.wikipedia.org/wiki/Time_Stamp_Counter
*/
class RDTS
{
public:
    RDTS() = delete;
    RDTS(const RDTS&) = delete;
    RDTS(RDTS&&) = delete;
    ~RDTS() = delete;

    RDTS& operator=(const RDTS&) = delete;
    RDTS& operator=(RDTS&&) = delete;

    //! Get the current number of CPU cycles since reset.
    static uint64_t current();
};

} // namespace CppCommon

#endif //CPPCOMMON_RDTS_H
