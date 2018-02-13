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

inline fmt::internal::udl_arg<char> operator""_a(const char* str, std::size_t) { return { str }; }
inline fmt::internal::udl_arg<wchar_t> operator""_a(const wchar_t* str, std::size_t) { return { str }; }
#if defined(_MSC_VER)
inline fmt::internal::udl_formatter<char> operator""_format(const char* str, std::size_t) { return { str }; }
inline fmt::internal::udl_formatter<wchar_t> operator""_format(const wchar_t* str, std::size_t) { return { str }; }
#else
template <typename TChar, TChar... CHARS>
FMT_CONSTEXPR fmt::internal::udl_formatter<TChar, CHARS...> operator""_format() { return {}; }
#endif

//! @endcond
