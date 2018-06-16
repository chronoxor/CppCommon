/*!
    \file format.inl
    \brief Format string inline implementation
    \author Ivan Shynkarenka
    \date 16.09.2016
    \copyright MIT License
*/

namespace CppCommon {

template <typename... Args>
inline std::string format(const std::string& pattern, const Args&... args)
{
    return fmt::format(pattern, args...);
}

template <typename... Args>
inline std::wstring format(const std::wstring& pattern, const Args&... args)
{
    return fmt::format(pattern, args...);
}

template <typename... Args>
inline void print(const std::string& pattern, const Args&... args)
{
    return fmt::print(pattern, args...);
}

template <typename... Args>
inline void print(const std::wstring& pattern, const Args&... args)
{
    return fmt::print(pattern, args...);
}

template <typename... Args>
inline void print(std::ostream& stream, const std::string& pattern, const Args&... args)
{
    return fmt::print(stream, pattern, args...);
}

template <typename... Args>
inline void print(std::wostream& stream, const std::wstring& pattern, const Args&... args)
{
    return fmt::print(stream, pattern, args...);
}

} // namespace CppCommon

//! @cond INTERNALS

using namespace fmt::literals;

//! @endcond
