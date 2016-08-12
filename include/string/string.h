/*!
    \file string.h
    \brief String utilities definition
    \author Ivan Shynkarenka
    \date 12.08.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_STRING_STRING_H
#define CPPCOMMON_STRING_STRING_H

#include <string>

namespace CppCommon {

//! String utilities
/*!
    String utilities contains different kinds of string manipulation functionality
    such UTF-8 conversion for wide strings.

    Thread-safe.
*/
class String
{
public:
    String() = delete;
    String(const String&) = delete;
    String(String&&) = delete;
    ~String() = delete;

    String& operator=(const String&) = delete;
    String& operator=(String&&) = delete;

    //! Convert system wide-string to UTF-8 encoded string
    /*!
        System wide-string could be UTF-16 (Windows) or UTF-32 (Unix).

        \param wstr - System wide-string to convert
        \return UTF-8 encoded string
    */
    static std::string ToUTF8(const std::wstring& wstr);

    //! Convert UTF-8 encoded string to system wide-string
    /*!
        System wide-string could be UTF-16 (Windows) or UTF-32 (Unix).

        \param str - UTF-8 encoded string to convert
        \return System wide-string
    */
    static std::wstring FromUTF8(const std::string& str);

    //! Convert UTF-8 encoded string to UTF-16 encoded string
    /*!
        \param str - UTF-8 encoded string to convert
        \return UTF-16 encoded string
    */
    static std::u16string UTF8toUTF16(const std::string& str);
    //! Convert UTF-8 encoded string to UTF-32 encoded string
    /*!
        \param str - UTF-8 encoded string to convert
        \return UTF-32 encoded string
    */
    static std::u32string UTF8toUTF32(const std::string& str);
    //! Convert UTF-8 encoded string to USC-2 encoded string
    /*!
        \param str - UTF-8 encoded string to convert
        \return USC-2 encoded string
    */
    static std::u16string UTF8toUSC2(const std::string& str);
    //! Convert UTF-8 encoded string to USC-4 encoded string
    /*!
        \param str - UTF-8 encoded string to convert
        \return USC-4 encoded string
    */
    static std::u32string UTF8toUSC4(const std::string& str);

    //! Convert UTF-16 encoded string to UTF-8 encoded string
    /*!
        \param str - UTF-16 encoded string to convert
        \return UTF-8 encoded string
    */
    static std::string UTF16toUTF8(const std::u16string& str);
    //! Convert UTF-16 encoded string to UTF-32 encoded string
    /*!
        \param str - UTF-16 encoded string to convert
        \return UTF-32 encoded string
    */
    static std::u32string UTF16toUTF32(const std::u16string& str);
    //! Convert UTF-16 encoded string to USC-2 encoded string
    /*!
        \param str - UTF-16 encoded string to convert
        \return USC-2 encoded string
    */
    static std::u16string UTF16toUSC2(const std::u16string& str);
    //! Convert UTF-16 encoded string to USC-4 encoded string
    /*!
        \param str - UTF-16 encoded string to convert
        \return USC-4 encoded string
    */
    static std::u32string UTF16toUSC4(const std::u16string& str);
};

/*! \example string_string.cpp String utilities example */

} // namespace CppCommon

#endif // CPPCOMMON_STRING_STRING_H
