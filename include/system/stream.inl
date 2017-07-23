/*!
    \file stream.inl
    \brief Standard input/output/error stream inline implementation
    \author Ivan Shynkarenka
    \date 23.07.2017
    \copyright MIT License
*/

namespace CppCommon {

inline void StdInput::swap(StdInput& stream) noexcept
{
    using std::swap;
    swap(_pimpl, stream._pimpl);
}

inline void swap(StdInput& stream1, StdInput& stream2) noexcept
{
    stream1.swap(stream2);
}

inline void StdOutput::swap(StdOutput& stream) noexcept
{
    using std::swap;
    swap(_pimpl, stream._pimpl);
}

inline void swap(StdOutput& stream1, StdOutput& stream2) noexcept
{
    stream1.swap(stream2);
}

inline void StdError::swap(StdError& stream) noexcept
{
    using std::swap;
    swap(_pimpl, stream._pimpl);
}

inline void swap(StdError& stream1, StdError& stream2) noexcept
{
    stream1.swap(stream2);
}

} // namespace CppCommon
