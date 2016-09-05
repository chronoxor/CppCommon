/*!
    \file fatal.cpp
    \brief Fatal abort execution implementation
    \author Ivan Shynkarenka
    \date 04.04.2016
    \copyright MIT License
*/

#include "errors/fatal.h"

#include <cstdlib>
#include <iostream>

namespace CppCommon {

void fatal(const SourceLocation& location, const StackTrace& trace, const std::string& message, int error) noexcept
{
    std::cerr << "Fatal error: " << message << std::endl;
    std::cerr << "System error: " << error << std::endl;
    std::cerr << "System message: " << SystemError::Description(error) << std::endl;
    std::cerr << "Source location: " << location.string() << std::endl;
    std::cerr << "Stack trace: " << std::endl << trace.string() << std::endl;
    std::abort();
}

void fatal(const SourceLocation& location, const StackTrace& trace, const std::exception& fatal) noexcept
{
    std::cerr << fatal.what() << std::endl;
    std::abort();
}

} // namespace CppCommon
