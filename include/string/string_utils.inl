/*!
    \file string_utils.inl
    \brief String utilities inline implementation
    \author Ivan Shynkarenka
    \date 15.05.2016
    \copyright MIT License
*/

namespace CppCommon {

inline bool StringUtils::IsBlank(char ch)
{
    return std::isspace(ch);
}

inline char StringUtils::ToLowerInternal(char ch)
{
    return (char)std::tolower(ch);
}

inline char StringUtils::ToUpperInternal(char ch)
{
    return (char)std::toupper(ch);
}

inline char StringUtils::ToLower(char ch)
{
    return ToLowerInternal(ch);
}

inline char StringUtils::ToUpper(char ch)
{
    return ToUpperInternal(ch);
}

inline std::string StringUtils::ToLower(std::string_view str)
{
    std::string result(str);
    Lower(result);
    return result;
}

inline std::string StringUtils::ToUpper(std::string_view str)
{
    std::string result(str);
    Upper(result);
    return result;
}

inline std::string& StringUtils::Lower(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), ToLowerInternal);
    return str;
}

inline std::string& StringUtils::Upper(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), ToUpperInternal);
    return str;
}

inline std::string& StringUtils::Trim(std::string& str)
{
    return LTrim(RTrim(str));
}

inline bool StringUtils::Contains(std::string_view str, const char ch)
{
    return (str.find(ch) != std::string::npos);
}

inline bool StringUtils::Contains(std::string_view str, const char* substr)
{
    return (str.find(substr) != std::string::npos);
}

inline bool StringUtils::Contains(std::string_view str, std::string_view substr)
{
    return (str.find(substr) != std::string::npos);
}

inline bool StringUtils::StartsWith(std::string_view str, std::string_view prefix)
{
    return (str.size() >= prefix.size()) && (str.compare(0, prefix.size(), prefix) == 0);
}

inline bool StringUtils::EndsWith(std::string_view str, std::string_view suffix)
{
    return (str.size() >= suffix.size()) && (str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0);
}

template <typename T>
inline std::string StringUtils::ToString(const T& value)
{
    std::ostringstream ss;
    ss << value;
    return ss.str();
}

template <typename T>
inline T StringUtils::FromString(std::string_view str)
{
    T result;
    std::istringstream(std::string(str)) >> result;
    return result;
}

template <>
inline std::string StringUtils::FromString(std::string_view str)
{
    return std::string(str);
}

template <>
inline std::string_view StringUtils::FromString(std::string_view str)
{
    return str;
}

template <>
inline bool StringUtils::FromString(std::string_view str)
{
    std::string value = ToLower(str);
    if ((value == "true") || (value == "yes") || (value == "on") || (value == "1"))
        return true;
    if ((value == "false") || (value == "no") || (value == "off") || (value == "0"))
        return false;

    assert("Invalid boolean value represented in string!");
    return false;
}

} // namespace CppCommon
