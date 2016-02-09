/*!
    \file call_stack.cpp
    \brief Call stack snapshot provider implementation
    \author Ivan Shynkarenka
    \date 09.02.2016
    \copyright MIT License
*/

#include "debug/call_stack.h"

#include <sstream>

namespace CppCommon {

std::string CallStack::Frame::to_string() const
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

CallStack::CallStack(int skip)
{

}

std::string CallStack::to_string() const
{
    std::stringstream stream;
    for (auto& frame : _frames)
        stream << frame.to_string() << std::endl;
    return stream.str(); 
}

} // namespace CppCommon
