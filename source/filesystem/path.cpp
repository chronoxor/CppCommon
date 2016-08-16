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

Path Path::root() const
{
    return Path();
}

Path Path::relative() const
{
    return Path();
}

Path Path::parent() const
{
    bool parent_found = false;
    size_t parent_length = _path.size();

    // Find parent path position
    while (parent_length > 0)
    {
        --parent_length;
        if ((_path[parent_length] == '\\') || (_path[parent_length] == '/'))
        {
            parent_found = true;

            // Windows case 2: "\\?\"
            if ((parent_length > 0) && (_path[parent_length - 1] == '?'))
                parent_found = false;
            // Windows case 3: "\\net" or "//net"
            if ((parent_length == 1) && ((_path[parent_length - 1] == '\\') || (_path[parent_length - 1] == '/')))
                parent_found = false;

            // Unix case 1: "/foo" -> "/", but "/" -> ""
            if ((parent_length == 0) && (_path.size() > 1))
                ++parent_length;

            while (parent_length > 0)
            {
                --parent_length;
                if ((_path[parent_length] != '\\') && (_path[parent_length] != '/'))
                {
                    ++parent_length;
                    break;
                }
            }
            break;
        }
    }

    return (parent_found && (parent_length > 0)) ? Path(_path.substr(0, parent_length)) : Path();
}

Path Path::filename() const
{
    bool filename_found = false;
    size_t filename_begin = _path.size();
    size_t filename_end = _path.size();

    // Find filename position
    while (filename_begin > 0)
    {
        --filename_begin;
        if ((_path[filename_begin] == '\\') || (_path[filename_begin] == '/') || (_path[filename_begin] == ':'))
        {
            filename_found = ((_path[filename_begin] == '\\') || (_path[filename_begin] == '/'));
            ++filename_begin;
            break;
        }
    }

    size_t filename_length = (filename_end - filename_begin);

    return (filename_length > 0) ? Path(_path.substr(filename_begin, filename_length)) : (filename_found ? Path(".") : Path());
}

Path Path::stem() const
{
    bool ext_found = false;
    size_t ext_begin = _path.size();
    size_t ext_end = _path.size();

    // Find extension position
    while (ext_begin > 0)
    {
        --ext_begin;
        if (_path[ext_begin] == '.')
        {
            ext_found = true;
            if ((ext_begin > 0) && (_path[ext_begin - 1] == '.'))
                ext_end = ext_begin;
            break;
        }
        if ((_path[ext_begin] == '\\') || (_path[ext_begin] == '/') || (_path[ext_begin] == ':'))
        {
            ++ext_begin;
            ext_end = ext_begin;
            break;
        }
    }

    size_t ext_length = ext_end - ext_begin;

    bool stem_found = false;
    size_t stem_begin = ext_begin;
    size_t stem_end = (ext_found && (ext_length > 1)) ? ext_begin : _path.size();

    // Find stem position
    while (stem_begin > 0)
    {
        --stem_begin;
        if ((_path[stem_begin] == '\\') || (_path[stem_begin] == '/') || (_path[stem_begin] == ':'))
        {
            stem_found = ((_path[stem_begin] == '\\') || (_path[stem_begin] == '/'));
            ++stem_begin;
            break;
        }
    }

    size_t stem_length = (stem_end - stem_begin);

    return (stem_length > 0) ? Path(_path.substr(stem_begin, stem_length)) : (stem_found ? Path(".") : Path());
}

Path Path::extension() const
{
    bool ext_found = false;
    size_t ext_begin = _path.size();
    size_t ext_end = _path.size();

    // Find extension position
    while (ext_begin > 0)
    {
        --ext_begin;
        if (_path[ext_begin] == '.')
        {
            ext_found = true;
            if ((ext_begin > 0) && (_path[ext_begin - 1] == '.'))
                ext_end = ext_begin;
            break;
        }
        if ((_path[ext_begin] == '\\') || (_path[ext_begin] == '/') || (_path[ext_begin] == ':'))
        {
            ++ext_begin;
            ext_end = ext_begin;
            break;
        }
    }

    size_t ext_length = ext_end - ext_begin;

    return (ext_found && (ext_length > 1)) ? Path(_path.substr(ext_begin, ext_length)) : Path();
}

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

        // Find filename position
        while (index > 0)
        {
            --index;
            if ((_path[index] == '\\') || (_path[index] == '/') || (_path[index] == ':'))
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

        // Find extension position
        while (index > 0)
        {
            --index;
            if (_path[index] == '.')
            {
                if ((index > 0) && (_path[index - 1] == '.'))
                    dot = index - 1;
                else
                    dot = index;
                break;
            }
            if ((_path[index] == '\\') || (_path[index] == '/') || (_path[index] == ':'))
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
        if (((_path[index] != '\\') && (_path[index] != '/')) || ((index > 0) && (_path[index - 1] == ':')))
        {
            ++index;
            break;
        }
    }

    _path.resize(index);
    return *this;
}

} // namespace CppCommon
