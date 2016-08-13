/*!
    \file path.inl
    \brief Filesystem path wrapper inline implementation
    \author Ivan Shynkarenka
    \date 11.08.2016
    \copyright MIT License
*/

namespace CppCommon {

void swap(Path& path1, Path& path2)
{
    using std::swap;
    swap(path1._path, path2._path);
}

} // namespace CppCommon
