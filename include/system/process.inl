/*!
    \file process.h
    \brief Process inline implementation
    \author Ivan Shynkarenka
    \date 01.12.2016
    \copyright MIT License
*/

namespace CppCommon {

inline void Process::swap(Process& process) noexcept
{
    using std::swap;
    swap(_pimpl, process._pimpl);
}

inline void swap(Process& process1, Process& process2) noexcept
{
    process1.swap(process2);
}

} // namespace CppCommon
