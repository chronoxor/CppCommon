/*!
    \file path.cpp
    \brief Filesystem path wrapper implementation
    \author Ivan Shynkarenka
    \date 11.08.2016
    \copyright MIT License
*/

#include "filesystem/path.h"

#include <codecvt>
#include <locale>

namespace CppCommon {

Path::Path(const std::string& path) : _path(path)
{
}

Path::Path(const std::wstring& path)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    _path = converter.to_bytes(path);
}

Path& Path::operator=(const std::string& path)
{
    _path = path;
    return *this;
}

Path& Path::operator=(const std::wstring& path)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    _path = converter.to_bytes(path);
    return *this;
}

} // namespace CppCommon
