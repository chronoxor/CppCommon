/*!
    \file path.inl
    \brief Filesystem path wrapper inline implementation
    \author Ivan Shynkarenka
    \date 11.08.2016
    \copyright MIT License
*/

namespace CppCommon {

inline Path& Path::operator/=(const std::string& path)
{
    if (_path.empty())
        _path = path;
    else
    {
        char last = _path[_path.size() - 1];
        if ((last == '\\') || (last == '/'))
            _path += path;
        else
        {
            _path += separator();
            _path += path;
        }
    }
}

inline char Path::separator()
{
#if defined(_WIN32) || defined(_WIN64)
    return '\\';
#elif defined(unix) || defined(__unix) || defined(__unix__)
    return '/';
#endif
}

inline void swap(Path& path1, Path& path2)
{
    using std::swap;
    swap(path1._path, path2._path);
}

} // namespace CppCommon
