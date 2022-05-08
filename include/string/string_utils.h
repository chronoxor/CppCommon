/*!
    \file string_utils.h
    \brief String utilities definition
    \author Ivan Shynkarenka
    \date 15.05.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_STRING_STRING_UTILS_H
#define CPPCOMMON_STRING_STRING_UTILS_H

#include <algorithm>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

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
    StringUtils(StringUtils&&) = delete;
    ~StringUtils() = delete;

    StringUtils& operator=(const StringUtils&) = delete;
    StringUtils& operator=(StringUtils&&) = delete;

    //! Is the given character blank (empty or contains only space characters)?
    /*!
        \param ch - Character to check
        \return 'true' if given character is blank, 'false' if given character is not blank
    */
    static bool IsBlank(char ch);
    //! Is the given C-string blank (empty or contains only space characters)?
    /*!
        \param str - C-string to check
        \return 'true' if given C-string is blank, 'false' if given C-string is not blank
    */
    static bool IsBlank(const char* str);
    //! Is the given string blank (empty or contains only space characters)?
    /*!
        \param str - String to check
        \return 'true' if given string is blank, 'false' if given string is not blank
    */
    static bool IsBlank(std::string_view str);

    //! Is the given string match to the given patterns?
    /*!
        Patterns string contains one or more regular expressions separated by ';'.
        If the regular expression has '!' prefix it treats as 'not matching'.
        Examples:
            "Demo.*;Live.*" + "DemoAccount" -> true
            "Demo.*;Live.*" + "LiveAccount" -> true
            "Demo.*;Live.*" + "UnknownAccount" -> false
            "!Demo.*;!Live.*" + "DemoAccount" -> false
            "!Demo.*;!Live.*" + "LiveAccount" -> false
            "!Demo.*;!Live.*" + "UnknownAccount" -> true

        \param patterns - Patterns to match with
        \param str - String to match
        \return 'true' if given string matches, 'false' if given string does not match
    */
    static bool IsPatternMatch(const std::string& patterns, const std::string& str);

    //! Convert the given character to lower case
    /*!
        \param ch - Character to convert
    */
    static char ToLower(char ch);
    //! Convert the given character to UPPER case
    /*!
        \param ch - Character to convert
    */
    static char ToUpper(char ch);

    //! Convert the given constant string converted to lower case
    /*!
        \param str - String to convert
        \return String converted to lower case
    */
    static std::string ToLower(std::string_view str);
    //! Convert the the given constant string converted to UPPER case
    /*!
        \param str - String to convert
        \return String converted to UPPER case
    */
    static std::string ToUpper(std::string_view str);

    //! Convert the given string to lower case
    /*!
        \param str - String to convert
        \return The same converted string
    */
    static std::string& Lower(std::string& str);
    //! Convert the given string to UPPER case
    /*!
        \param str - String to convert
        \return The same converted string
    */
    static std::string& Upper(std::string& str);

    //! Trims space characters from the start of the given constant string
    /*!
        \param str - String to trim
        \return Trimmed string
    */
    static std::string ToLTrim(std::string_view str);
    //! Trims space characters from the end of the given constant string
    /*!
        \param str - String to trim
        \return Trimmed string
    */
    static std::string ToRTrim(std::string_view str);
    //! Trims space characters from the both sides of the given constant string
    /*!
        \param str - String to trim
        \return Trimmed string
    */
    static std::string ToTrim(std::string_view str);

    //! Trims space characters from the start of the given string
    /*!
        \param str - String to trim
        \return The same trimmed string
    */
    static std::string& LTrim(std::string& str);
    //! Trims space characters from the end of the given string
    /*!
        \param str - String to trim
        \return The same trimmed string
    */
    static std::string& RTrim(std::string& str);
    //! Trims space characters from the both sides of the given string
    /*!
        \param str - String to trim
        \return The same trimmed string
    */
    static std::string& Trim(std::string& str);

    //! Compare two strings case sensitive version
    /*!
        \param str1 - First string to compare
        \param str2 - Second string to compare
        \return 'true' if two strings are equal, 'false' if two strings are different
    */
    static bool Compare(std::string_view str1, std::string_view str2);
    //! Compare two strings case insensitive version
    /*!
        \param str1 - First string to compare
        \param str2 - Second string to compare
        \return 'true' if two strings are equal, 'false' if two strings are different
    */
    static bool CompareNoCase(std::string_view str1, std::string_view str2);

    //! Is the given string contains the given character?
    /*!
        \param str - String to search in
        \param ch - Character to find
        \return 'true' if the substring was found, 'false' if the substring was not found
    */
    static bool Contains(std::string_view str, const char ch);
    //! Is the given string contains the given C-string?
    /*!
        \param str - String to search in
        \param substr - Substring to find
        \return 'true' if the substring was found, 'false' if the substring was not found
    */
    static bool Contains(std::string_view str, const char* substr);
    //! Is the given string contains the given substring?
    /*!
        \param str - String to search in
        \param substr - Substring to find
        \return 'true' if the substring was found, 'false' if the substring was not found
    */
    static bool Contains(std::string_view str, std::string_view substr);

    //! Count all occurrences of substring
    /*!
        \param str - Modifying string
        \param substr - Substring to find
        \return Count of all substring occurrences
    */
    static size_t CountAll(std::string_view str, std::string_view substr);

    //! Replace the first occurrence of substring with another substring
    /*!
        \param str - Modifying string
        \param substr - Substring to find
        \param with - Substring to replace
        \return 'true' if first occurrence of substring was found and relapsed, 'false' if the substring was not found
    */
    static bool ReplaceFirst(std::string& str, std::string_view substr, std::string_view with);
    //! Replace the last occurrence of substring with another substring
    /*!
        \param str - Modifying string
        \param substr - Substring to find
        \param with - Substring to replace
        \return 'true' if last occurrence of substring was found and relapsed, 'false' if the substring was not found
    */
    static bool ReplaceLast(std::string& str, std::string_view substr, std::string_view with);
    //! Replace all occurrences of substring with another substring
    /*!
        \param str - Modifying string
        \param substr - Substring to find
        \param with - Substring to replace
        \return 'true' if all occurrences of substring were found and relapsed, 'false' if the substring was not found
    */
    static bool ReplaceAll(std::string& str, std::string_view substr, std::string_view with);

    //! Checks the given string for specific prefix
    /*!
        \param str - String to check
        \param prefix - Prefix to check
        \return 'true' if the given string starts with the specific prefix, 'false' if the given string does not start with the specific prefix
    */
    static bool StartsWith(std::string_view str, std::string_view prefix);
    //! Checks the given string for specific suffix
    /*!
        \param str - String to check
        \param suffix - Suffix to check
        \return 'true' if the given string ends with the specific suffix, 'false' if the given string does not end with the specific suffix
    */
    static bool EndsWith(std::string_view str, std::string_view suffix);

    //! Split the string into tokens by the given delimiter character
    /*!
        \param str - String to split
        \param delimiter - Delimiter character
        \param skip_empty - Skip empty substrings flag (default is false)
        \return Vector of tokens
    */
    static std::vector<std::string> Split(std::string_view str, char delimiter, bool skip_empty = false);
    //! Split the string into tokens by the given delimiter string
    /*!
        \param str - String to split
        \param delimiter - Delimiter string
        \param skip_empty - Skip empty substrings flag (default is false)
        \return Vector of tokens
    */
    static std::vector<std::string> Split(std::string_view str, std::string_view delimiter, bool skip_empty = false);
    //! Split the string into tokens by the any character in the given delimiter string
    /*!
        \param str - String to split
        \param delimiters - Delimiters string
        \param skip_empty - Skip empty substrings flag (default is false)
        \return Vector of string tokens
    */
    static std::vector<std::string> SplitByAny(std::string_view str, std::string_view delimiters, bool skip_empty = false);

    //! Join tokens into the string
    /*!
        \param tokens - Vector of string tokens
        \param skip_empty - Skip empty tokens flag (default is false)
        \param skip_blank - Skip blank tokens flag (default is false)
        \return Joined string
    */
    static std::string Join(const std::vector<std::string>& tokens, bool skip_empty = false, bool skip_blank = false);
    //! Join tokens into the string with delimiter character
    /*!
        \param tokens - Vector of string tokens
        \param delimiter - Delimiter character
        \param skip_empty - Skip empty tokens flag (default is false)
        \param skip_blank - Skip blank tokens flag (default is false)
        \return Joined string
    */
    static std::string Join(const std::vector<std::string>& tokens, char delimiter, bool skip_empty = false, bool skip_blank = false);
    //! Join tokens into the string with delimiter C-string
    /*!
        \param tokens - Vector of string tokens
        \param delimiter - Delimiter C-string
        \param skip_empty - Skip empty tokens flag (default is false)
        \param skip_blank - Skip blank tokens flag (default is false)
        \return Joined string
    */
    static std::string Join(const std::vector<std::string>& tokens, const char* delimiter, bool skip_empty = false, bool skip_blank = false);
    //! Join tokens into the string with delimiter string
    /*!
        \param tokens - Vector of string tokens
        \param delimiter - Delimiter string
        \param skip_empty - Skip empty tokens flag (default is false)
        \param skip_blank - Skip blank tokens flag (default is false)
        \return Joined string
    */
    static std::string Join(const std::vector<std::string>& tokens, std::string_view delimiter, bool skip_empty = false, bool skip_blank = false);

    //! Converts arbitrary datatypes into string using std::ostringstream
    /*!
        \param value - Value to convert
        \return Result converted string
    */
    template <typename T>
    static std::string ToString(const T& value);
    //! Converts strings to arbitrary datatypes using std::istringstream
    /*!
        \param str - String converted into the value
        \return Result converted value
    */
    template <typename T>
    static T FromString(std::string_view str);

private:
    static char ToLowerInternal(char ch);
    static char ToUpperInternal(char ch);
};

/*! \example string_utils.cpp String utilities example */

} // namespace CppCommon

#include "string_utils.inl"

#endif // CPPCOMMON_STRING_STRING_UTILS_H
