/*!
    \file directory_iterator.inl
    \brief Filesystem directory iterator inline implementation
    \author Ivan Shynkarenka
    \date 04.09.2016
    \copyright MIT License
*/

namespace CppCommon {

inline bool operator==(const DirectoryIterator& it1, const DirectoryIterator& it2) noexcept
{
    return it1._current == it2._current;
}

inline bool operator!=(const DirectoryIterator& it1, const DirectoryIterator& it2) noexcept
{
    return it1._current != it2._current;
}

inline const Path& DirectoryIterator::operator*() const noexcept
{
    return _current;
}

inline const Path* DirectoryIterator::operator->() const noexcept
{
    return &_current;
}

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
