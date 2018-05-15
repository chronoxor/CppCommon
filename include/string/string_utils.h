/*!
    \file string_utils.h
    \brief String utilities definition
    \author Ivan Shynkarenka
    \date 15.05.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_STRING_STRING_UTILS_H
#define CPPCOMMON_STRING_STRING_UTILS_H

#include <cctype>
#include <string>

namespace CppCommon {

//! String utilities
/*!
    String utilities contains methods for UPPER/lower case conversions, join/split strings
    and other useful string manipulation methods.

    Thread-safe.
*/
class StringUtils
{
public:
    StringUtils() = delete;
    StringUtils(const StringUtils&) = delete;
    StringUtils(StringUtils&&) noexcept = delete;
    ~StringUtils() = delete;

    StringUtils& operator=(const StringUtils&) = delete;
    StringUtils& operator=(StringUtils&&) noexcept = delete;

    //! Convert the given string to lower case
    /*!
        \param str - String to convert
        \return String converted to lower case
    */
    static std::string ToLower(const std::string& str);
    //! Convert the given string to UPPER case
    /*!
        \param str - String to convert
        \return String converted to UPPER case
    */
    static std::string ToUpper(const std::string& str);

private:
    static char ToLowerInternal(char ch);
    static char ToUpperInternal(char ch);
};

/*! \example string_utils.cpp String utilities example */

} // namespace CppCommon

#include "string_utils.inl"

#endif // CPPCOMMON_STRING_STRING_UTILS_H
