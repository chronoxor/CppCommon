/*!
    \file countof.h
    \brief Static array countof definition
    \author Ivan Shynkarenka
    \date 31.08.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_UTILITY_COUNTOF_H
#define CPPCOMMON_UTILITY_COUNTOF_H

namespace CppCommon {

//! Count of elements in static array
template <typename T, size_t N>
constexpr size_t countof(const T (&)[N]) noexcept { return N; }

//! Count of elements in any other STL container
template <typename T>
size_t countof(const T& container) noexcept { return container.size(); }

} // namespace CppCommon

#endif // CPPCOMMON_UTILITY_COUNTOF_H
