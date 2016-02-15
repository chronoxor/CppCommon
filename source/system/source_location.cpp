/*!
    \file system_location.h
    \brief Source location wrapper implementation
    \author Ivan Shynkarenka
    \date 09.02.2016
    \copyright MIT License
*/

#include "system/source_location.h"

namespace CppCommon {

std::string SourceLocation::to_string() const
{
    if ((_filename == nullptr) || (_line == 0))
        return std::string();

    return std::string(_filename) + ':' + std::to_string(_line);
}

} // namespace CppCommon
