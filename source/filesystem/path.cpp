/*!
    \file path.cpp
    \brief Filesystem path wrapper implementation
    \author Ivan Shynkarenka
    \date 11.08.2016
    \copyright MIT License
*/

#include "filesystem/path.h"

#include <algorithm>

namespace CppCommon {

Path& Path::Append(const Path& path)
{
    if (_path.empty())
        _path = path._path;
    else
    {
        char last = _path[_path.size() - 1];
        if ((last == '\\') || (last == '/'))
            _path += path._path;
        else
        {
            _path += separator();
            _path += path._path;
        }
    }

    return *this;
}

Path& Path::MakePreferred()
{
#if defined(_WIN32) || defined(_WIN64)
    std::replace(_path.begin(), _path.end(), '/', '\\');
#elif defined(unix) || defined(__unix) || defined(__unix__)
    std::replace(_path.begin(), _path.end(), '\\', '/');
#endif
    return *this;
}

Path& Path::ReplaceFilename(const Path& filename)
{
    if (_path.empty())
        _path.append(filename._path);
    else
    {
        size_t index = _path.size();
        while (index > 0)
        {
            --index;
            if ((_path[index] == '\\') || (_path[index] == '/'))
            {
                if (!filename.empty())
                    ++index;
                break;
            }
        }

        _path.resize(index);
        _path.append(filename._path);
    }
    return *this;
}

Path& Path::ReplaceExtension(const Path& extension)
{
    bool dot_required = (!extension._path.empty() && (extension._path[0] != '.'));

    if (_path.empty())
    {
        if (dot_required)
            _path.append(".");
        _path.append(extension._path);
    }
    else
    {
        size_t dot = _path.size();
        size_t index = _path.size();
        while (index > 0)
        {
            --index;
            if (_path[index] == '.')
            {
                dot = index;
                break;
            }
            if ((_path[index] == '\\') || (_path[index] == '/'))
            {
                if (!extension.empty())
                    ++index;
                break;
            }
        }

        _path.resize(dot);
        if (dot_required)
            _path.append(".");
        _path.append(extension._path);
    }
    return *this;
}

Path& Path::RemoveTrailingSeparators()
{
    size_t index = _path.size();
    while (index > 0)
    {
        --index;
        if ((_path[index] != '\\') && (_path[index] != '/'))
        {
            ++index;
            break;
        }
    }

    _path.resize(index);
    return *this;
}

} // namespace CppCommon
