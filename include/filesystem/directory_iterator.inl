/*!
    \file directory_iterator.inl
    \brief Filesystem directory iterator inline implementation
    \author Ivan Shynkarenka
    \date 04.09.2016
    \copyright MIT License
*/

namespace CppCommon {

inline void DirectoryIterator::swap(DirectoryIterator& it) noexcept
{
    using std::swap;
    _pimpl.swap(it._pimpl);
}

inline void swap(DirectoryIterator& it1, DirectoryIterator& it2) noexcept
{
    it1.swap(it2);
}

} // namespace CppCommon
