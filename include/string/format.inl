/*!
    \file format.inl
    \brief Format string inline implementation
    \author Ivan Shynkarenka
    \date 16.09.2016
    \copyright MIT License
*/

namespace CppCommon {

template <typename... Args>
inline std::string format(const std::string_view pattern, Args&&... args)
{
    return fmt::format(pattern, std::forward<Args>(args)...);
}

template <typename... Args>
inline std::wstring format(const std::wstring_view pattern, Args&&... args)
{
    return fmt::format(pattern, std::forward<Args>(args)...);
}

template <typename... Args>
inline void print(const std::string_view pattern, Args&&... args)
{
    return fmt::print(pattern, std::forward<Args>(args)...);
}

template <typename... Args>
inline void print(const std::wstring_view pattern, Args&&... args)
{
    return fmt::print(pattern, std::forward<Args>(args)...);
}

template <typename... Args>
inline void print(std::ostream& stream, const std::string_view pattern, Args&&... args)
{
    return fmt::print(stream, pattern, std::forward<Args>(args)...);
}

template <typename... Args>
inline void print(std::wostream& stream, const std::wstring_view pattern, Args&&... args)
{
    return fmt::print(stream, pattern, std::forward<Args>(args)...);
}

} // namespace CppCommon

//! @cond INTERNALS

using namespace fmt::literals;

//! @endcond
