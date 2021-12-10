/*!
    \file format.h
    \brief Format string definition
    \author Ivan Shynkarenka
    \date 16.09.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_STRING_FORMAT_H
#define CPPCOMMON_STRING_FORMAT_H

#if defined(__clang__)
#pragma clang system_header
#elif defined(__GNUC__)
#pragma GCC system_header
#elif defined(_MSC_VER)
#pragma system_header
#endif

#include <fmt/args.h>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fmt/xchar.h>

namespace CppCommon {

//! Format string
/*!
    Format string with the help of {fmt} library (http://fmtlib.net)

    Thread-safe.

    \param pattern - Format string pattern
    \param args - Format arguments
    \return Formatted string
*/
template <typename TString, typename... Args>
std::string format(const TString& pattern, Args&&... args);

//! Format string and print it into the std::cout
/*!
    Format string with the help of {fmt} library (http://fmtlib.net)

    Thread-safe.

    \param pattern - Format string pattern
    \param args - Format arguments
*/
template <typename TString, typename... Args>
void print(const TString& pattern, Args&&... args);

//! Format string and print it into the given std::ostream
/*!
    Format string with the help of {fmt} library (http://fmtlib.net)

    Thread-safe.

    \param stream - Output stream
    \param pattern - Format string pattern
    \param args - Format arguments
*/
template <typename TOutputStream, typename TString, typename... Args>
void print(TOutputStream& stream, const TString& pattern, Args&&... args);

/*! \example string_format.cpp Format string example */

} // namespace CppCommon

#include "format.inl"

#endif // CPPCOMMON_STRING_FORMAT_H
