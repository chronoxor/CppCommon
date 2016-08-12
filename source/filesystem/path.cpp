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

Path::Path(const std::wstring& path)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    _path = converter.to_bytes(path);
}

Path& Path::operator=(const std::wstring& path)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    _path = converter.to_bytes(path);
    return *this;
}

std::string Path::string() const
{
    return std::string(_path);
}

std::wstring Path::wstring() const
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(_path);
}

} // namespace CppCommon
