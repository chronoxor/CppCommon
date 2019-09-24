/*!
    \file pipe.inl
    \brief Pipe inline implementation
    \author Ivan Shynkarenka
    \date 30.11.2016
    \copyright MIT License
*/

namespace CppCommon {

inline void swap(Pipe& pipe1, Pipe& pipe2) noexcept
{
    pipe1.swap(pipe2);
}

} // namespace CppCommon
