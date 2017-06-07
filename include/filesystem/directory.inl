/*!
    \file directory.inl
    \brief Filesystem directory inline implementation
    \author Ivan Shynkarenka
    \date 30.08.2016
    \copyright MIT License
*/

namespace CppCommon {

inline bool operator==(const DirectoryIterator& it1, const DirectoryIterator& it2)
{
    return it1._current == it2._current;
}

inline bool operator!=(const DirectoryIterator& it1, const DirectoryIterator& it2)
{
    return it1._current != it2._current;
}

inline const Path& DirectoryIterator::operator*() const
{
    return _current;
}

inline const Path* DirectoryIterator::operator->() const
{
    return &_current;
}

inline void Directory::swap(Directory& directory) noexcept
{
    using std::swap;
    Path::swap(directory);
}

inline void swap(Directory& directory1, Directory& directory2) noexcept
{
    directory1.swap(directory2);
}

} // namespace CppCommon
