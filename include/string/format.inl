/*!
    \file format.inl
    \brief Format string inline implementation
    \author Ivan Shynkarenka
    \date 16.09.2016
    \copyright MIT License
*/

namespace CppCommon {

template <typename... Args>
inline std::string format(std::string_view pattern, const Args&... args)
{
    return fmt::format(pattern, args...);
}

template <typename... Args>
inline std::wstring format(std::wstring_view pattern, const Args&... args)
{
    return fmt::format(pattern, args...);
}

template <typename... Args>
inline void print(std::string_view pattern, const Args&... args)
{
    return fmt::print(pattern, args...);
}

template <typename... Args>
inline void print(std::wstring_view pattern, const Args&... args)
{
    return fmt::print(pattern, args...);
}

template <typename... Args>
inline void print(std::ostream& stream, std::string_view pattern, const Args&... args)
{
    return fmt::print(stream, pattern, args...);
}

template <typename... Args>
inline void print(std::wostream& stream, std::wstring_view pattern, const Args&... args)
{
    return fmt::print(stream, pattern, args...);
}

} // namespace CppCommon

//! @cond INTERNALS

using namespace fmt::literals;

//! @endcond
