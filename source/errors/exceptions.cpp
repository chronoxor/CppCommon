/*!
    \file exceptions.cpp
    \brief Exceptions implementation
    \author Ivan Shynkarenka
    \date 09.02.2016
    \copyright MIT License
*/

#include "errors/exceptions.h"

#include <sstream>

namespace CppCommon {

const char* Exception::what() const noexcept
{
    try
    {
        if (_cache.empty())
            to_string();
        return _cache.c_str();
    }
    catch (...)
    {
        return "Out of memory!";
    }
}

std::string Exception::to_string() const
{
    if (_cache.empty())
    {
        std::stringstream stream;
        stream << "Exception: " << _message << std::endl;
        std::string location = _location.to_string();
        if (!location.empty())
            stream << "Source location: " << location << std::endl;
        _cache = stream.str();
    }
    return _cache;
}

std::string SystemException::to_string() const
{
    if (_cache.empty())
    {
        std::stringstream stream;
        stream << "System exception: " << _message << std::endl;
        stream << "System error: " << _system_error << std::endl;
        stream << "System message: " << _system_message << std::endl;
        std::string location = _location.to_string();
        if (!location.empty())
            stream << "Source location: " << location << std::endl;
        _cache = stream.str();
    }
    return _cache;
}

} // namespace CppCommon
