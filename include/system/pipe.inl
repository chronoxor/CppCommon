/*!
    \file pipe.h
    \brief Pipe inline implementation
    \author Ivan Shynkarenka
    \date 30.11.2016
    \copyright MIT License
*/

namespace CppCommon {

inline void Pipe::swap(Pipe& pipe) noexcept
{
    using std::swap;
    swap(_pimpl, pipe._pimpl);
}

inline void swap(Pipe& pipe1, Pipe& pipe2) noexcept
{
    pipe1.swap(pipe2);
}

} // namespace CppCommon
