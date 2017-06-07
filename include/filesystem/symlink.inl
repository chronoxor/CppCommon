/*!
    \file symlink.inl
    \brief Filesystem symlink inline implementation
    \author Ivan Shynkarenka
    \date 30.08.2016
    \copyright MIT License
*/

namespace CppCommon {

inline void Symlink::swap(Symlink& symlink) noexcept
{
    using std::swap;
    Path::swap(symlink);
}

inline void swap(Symlink& symlink1, Symlink& symlink2) noexcept
{
    symlink1.swap(symlink2);
}

} // namespace CppCommon
