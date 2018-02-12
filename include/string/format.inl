/*!
    \file format.inl
    \brief Format string inline implementation
    \author Ivan Shynkarenka
    \date 16.09.2016
    \copyright MIT License
*/

namespace CppCommon {

template <typename... Args>
std::string format(const std::string& pattern, const Args&... args)
{
    return fmt::format(pattern, args...);
}

template <typename... Args>
std::wstring format(const std::wstring& pattern, const Args&... args)
{
    return fmt::format(pattern, args...);
}

} // namespace CppCommon

//! @cond INTERNALS

inline fmt::internal::UdlArg<char> operator"" _a(const char* str, std::size_t) { return { str }; }
inline fmt::internal::UdlArg<wchar_t> operator"" _a(const wchar_t* str, std::size_t) { return { str }; }
inline fmt::internal::UdlFormat<char> operator"" _format(const char* str, std::size_t) { return { str }; }
inline fmt::internal::UdlFormat<wchar_t> operator"" _format(const wchar_t* str, std::size_t) { return { str }; }

//! @endcond
