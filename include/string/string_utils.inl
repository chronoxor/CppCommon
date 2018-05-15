/*!
    \file string_utils.inl
    \brief String utilities inline implementation
    \author Ivan Shynkarenka
    \date 15.05.2016
    \copyright MIT License
*/

namespace CppCommon {

inline char StringUtils::ToLowerInternal(char ch)
{
    return (char)std::tolower(ch);
}

inline char StringUtils::ToUpperInternal(char ch)
{
    return (char)std::toupper(ch);
}

} // namespace CppCommon
