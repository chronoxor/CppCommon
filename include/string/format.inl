/*!
    \file format.inl
    \brief Format string inline implementation
    \author Ivan Shynkarenka
    \date 16.09.2016
    \copyright MIT License
*/

namespace CppCommon {

template <typename... T>
inline std::string format(fmt::format_string<T...> pattern, T&&... args)
{
    return fmt::vformat(pattern, fmt::make_format_args(args...));
}

template <typename... T>
inline void print(fmt::format_string<T...> pattern, T&&... args)
{
    return fmt::vprint(pattern, fmt::make_format_args(args...));
}

template <typename TOutputStream, typename... T>
inline void print(TOutputStream& stream, fmt::format_string<T...> pattern, T&&... args)
{
    return fmt::vprint(stream, pattern, fmt::make_format_args(args...));
}

} // namespace CppCommon

//! @cond INTERNALS

using namespace fmt::literals;

//! @endcond
