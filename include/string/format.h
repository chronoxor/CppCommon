/*!
    \file format.h
    \brief Format string definition
    \author Ivan Shynkarenka
    \date 16.09.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_STRING_FORMAT_H
#define CPPCOMMON_STRING_FORMAT_H

#include "../../modules/fmt/include/fmt/format.h"
#include "../../modules/fmt/include/fmt/ostream.h"

namespace CppCommon {

//! Format string
/*!
    Format string with the help of {fmt} library (http://fmtlib.net)

    Thread-safe.

    \param pattern - Format string pattern
    \param args - Format arguments
    \return Formatted string
*/
template <typename... Args>
std::string format(const std::string& pattern, const Args&... args);

//! Format wide string
/*!
    Format wide string with the help of {fmt} library (http://fmtlib.net)

    Thread-safe.

    \param pattern - Format wide string pattern
    \param args - Format arguments
    \return Formatted wide string
*/
template <typename... Args>
std::wstring format(const std::wstring& pattern, const Args&... args);

/*! \example string_format.cpp Format string example */

} // namespace CppCommon

#include "format.inl"

#endif // CPPCOMMON_STRING_FORMAT_H
