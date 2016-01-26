/*!
    \file timestamp.h
    \brief Nanoseconds timestamp accessor definition
    \author Ivan Shynkarenka
    \date 26.01.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_TIMESTAMP_H
#define CPPCOMMON_TIMESTAMP_H

#include <cstdint>

namespace CppCommon {

//! Nanoseconds timestamp accessor
/*!
    \return Timestamp in nanoseconds resolution
*/
uint64_t timestamp();

} // namespace CppCommon

#endif //CPPCOMMON_TIMESTAMP_H
