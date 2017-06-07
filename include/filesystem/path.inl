/*!
    \file path.inl
    \brief Filesystem path inline implementation
    \author Ivan Shynkarenka
    \date 11.08.2016
    \copyright MIT License
*/

ENUM_FLAGS(CppCommon::FileAttributes)
ENUM_FLAGS(CppCommon::FilePermissions)

namespace CppCommon {

inline bool Path::IsOther() const
{
    FileType t = type();
    return ((t != FileType::NONE) && (t != FileType::REGULAR) && (t != FileType::DIRECTORY) && (t != FileType::SYMLINK));
}

inline Path& Path::Assign(const Path& path)
{
    _path = path._path;
    return *this;
}

inline Path& Path::Concat(const Path& path)
{
    _path.append(path._path);
    return *this;
}

inline void Path::swap(Path& path) noexcept
{
    using std::swap;
    swap(_path, path._path);
}

inline void swap(Path& path1, Path& path2) noexcept
{
    path1.swap(path2);
}

} // namespace CppCommon
