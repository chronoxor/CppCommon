/*!
    \file stack_trace.cpp
    \brief Stack trace snapshot provider implementation
    \author Ivan Shynkarenka
    \date 09.02.2016
    \copyright MIT License
*/

#include "debug/stack_trace.h"

#include <sstream>

namespace CppCommon {

std::string StackTrace::Frame::to_string() const
{
    std::stringstream stream;
    if (address != nullptr)
        stream << address << ": ";
    if (!module.empty())
        stream << module << '!';
    if (!function.empty())
        stream << function << ' ';
    if (!filename.empty())
        stream << filename;
    if (line > 0)
        stream << '(' << line << ')';
    return stream.str();
}

StackTrace::StackTrace(int skip)
{

}

std::string StackTrace::to_string() const
{
    std::stringstream stream;
    for (auto& frame : _frames)
        stream << frame.to_string() << std::endl;
    return stream.str();
}

} // namespace CppCommon
