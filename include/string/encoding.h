/*!
    \file encoding.h
    \brief Encoding utilities definition
    \author Ivan Shynkarenka
    \date 12.08.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_STRING_ENCODING_H
#define CPPCOMMON_STRING_ENCODING_H

#include <string>

namespace CppCommon {

//! Encoding utilities
/*!
    Encoding utilities contains methods for UTF-8, UTF-16, UTF-32 encoding conversions.

    Thread-safe.
*/
class Encoding
{
public:
    Encoding() = delete;
    Encoding(const Encoding&) = delete;
    Encoding(Encoding&&) = delete;
    ~Encoding() = delete;

    Encoding& operator=(const Encoding&) = delete;
    Encoding& operator=(Encoding&&) = delete;

    //! Convert system wide-string to UTF-8 encoded string
    /*!
        System wide-string could be UTF-16 (Windows) or UTF-32 (Unix).

        \param wstr - System wide-string to convert
        \return UTF-8 encoded string
    */
    static std::string ToUTF8(std::wstring_view wstr);

    //! Convert UTF-8 encoded string to system wide-string
    /*!
        System wide-string could be UTF-16 (Windows) or UTF-32 (Unix).

        \param str - UTF-8 encoded string to convert
        \return System wide-string
    */
    static std::wstring FromUTF8(std::string_view str);

    //! Convert UTF-8 encoded string to UTF-16 encoded string
    /*!
        \param str - UTF-8 encoded string to convert
        \return UTF-16 encoded string
    */
    static std::u16string UTF8toUTF16(std::string_view str);
    //! Convert UTF-8 encoded string to UTF-32 encoded string
    /*!
        \param str - UTF-8 encoded string to convert
        \return UTF-32 encoded string
    */
    static std::u32string UTF8toUTF32(std::string_view str);

    //! Convert UTF-16 encoded string to UTF-8 encoded string
    /*!
        \param str - UTF-16 encoded string to convert
        \return UTF-8 encoded string
    */
    static std::string UTF16toUTF8(std::u16string_view str);
    //! Convert UTF-16 encoded string to UTF-32 encoded string
    /*!
        \param str - UTF-16 encoded string to convert
        \return UTF-32 encoded string
    */
    static std::u32string UTF16toUTF32(std::u16string_view str);

    //! Convert UTF-32 encoded string to UTF-8 encoded string
    /*!
        \param str - UTF-32 encoded string to convert
        \return UTF-8 encoded string
    */
    static std::string UTF32toUTF8(std::u32string_view str);
    //! Convert UTF-32 encoded string to UTF-16 encoded string
    /*!
        \param str - UTF-32 encoded string to convert
        \return UTF-16 encoded string
    */
    static std::u16string UTF32toUTF16(std::u32string_view str);

    //! Base16 encode string
    /*!
        \param str - String to encode
        \return Base16 encoded string
    */
    static std::string Base16Encode(std::string_view str);
    //! Base16 decode string
    /*!
        \param str - Base16 encoded string
        \return Decoded string
    */
    static std::string Base16Decode(std::string_view str);

    //! Base32 encode string
    /*!
        \param str - String to encode
        \return Base32 encoded string
    */
    static std::string Base32Encode(std::string_view str);
    //! Base32 decode string
    /*!
        \param str - Base32 encoded string
        \return Decoded string
    */
    static std::string Base32Decode(std::string_view str);

    //! Base64 encode string
    /*!
        \param str - String to encode
        \return Base64 encoded string
    */
    static std::string Base64Encode(std::string_view str);
    //! Base64 decode string
    /*!
        \param str - Base64 encoded string
        \return Decoded string
    */
    static std::string Base64Decode(std::string_view str);

    //! URL encode string
    /*!
        \param str - String to encode
        \return URL encoded string
    */
    static std::string URLEncode(std::string_view str);
    //! URL decode string
    /*!
        \param str - URL encoded string
        \return Decoded string
    */
    static std::string URLDecode(std::string_view str);
};

/*! \example string_encoding.cpp Encoding utilities example */

} // namespace CppCommon

#endif // CPPCOMMON_STRING_ENCODING_H
