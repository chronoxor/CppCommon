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
template <typename T, std::size_t N>
constexpr std::size_t countof(T const (&)[N]) noexcept
{
    return N;
}

} // namespace CppCommon

#endif // CPPCOMMON_UTILITY_COUNTOF_H
