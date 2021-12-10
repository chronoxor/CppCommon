/*!
    \file format.inl
    \brief Format string inline implementation
    \author Ivan Shynkarenka
    \date 16.09.2016
    \copyright MIT License
*/

namespace CppCommon {

template <typename TString, typename... Args>
inline std::string format(const TString& pattern, Args&&... args)
{
    return fmt::format(pattern, std::forward<Args>(args)...);
}

template <typename TString, typename... Args>
inline void print(const TString& pattern, Args&&... args)
{
    return fmt::print(pattern, std::forward<Args>(args)...);
}

template <typename TOutputStream, typename TString, typename... Args>
inline void print(TOutputStream& stream, const TString& pattern, Args&&... args)
{
    return fmt::print(stream, pattern, std::forward<Args>(args)...);
}

} // namespace CppCommon

//! @cond INTERNALS

using namespace fmt::literals;

//! @endcond
