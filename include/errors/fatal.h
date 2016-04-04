/*!
    \file fatal.h
    \brief Fatal abort execution definition
    \author Ivan Shynkarenka
    \date 04.04.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_ERRORS_FATAL_H
#define CPPCOMMON_ERRORS_FATAL_H

#include "system/source_location.h"

#include <string>

//! Fatal abort execution extended macro
/*!
    Fatal abort execution with the current location.
*/
#define fatality(message) fatal(__LOCATION__, (message))

namespace CppCommon {

//! Fatal abort execution
/*!
    Fatal aborts execution. It will print fatal message into the 'stderr' stream,
    get and print the last system error code and message, and finally call the
    abort() function.

    Thread-safe.

    \param location - Source location wrapper
    \param message - Fatal message
*/
void fatal(const SourceLocation& location, const std::string& message) noexcept;

/*! \example errors_fatal.cpp Fatal abort execution example */

} // namespace CppCommon

#endif // CPPCOMMON_ERRORS_FATAL_H
