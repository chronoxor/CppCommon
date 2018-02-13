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

using namespace fmt::literals;

//! @endcond
