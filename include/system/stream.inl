/*!
    \file stream.inl
    \brief Standard input/output/error stream inline implementation
    \author Ivan Shynkarenka
    \date 23.07.2017
    \copyright MIT License
*/

namespace CppCommon {

inline void swap(StdInput& stream1, StdInput& stream2) noexcept
{
    stream1.swap(stream2);
}

inline void swap(StdOutput& stream1, StdOutput& stream2) noexcept
{
    stream1.swap(stream2);
}

inline void swap(StdError& stream1, StdError& stream2) noexcept
{
    stream1.swap(stream2);
}

} // namespace CppCommon
