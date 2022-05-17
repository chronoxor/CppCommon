/*!
    \file source_location.h
    \brief Source location definition
    \author Ivan Shynkarenka
    \date 09.02.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_SYSTEM_SOURCE_LOCATION_H
#define CPPCOMMON_SYSTEM_SOURCE_LOCATION_H

#include "string/format.h"

#include <sstream>
#include <string>

namespace CppCommon {

//! Current source location macro
/*!
    Create a new source location with a current file name and line number.
*/
#define __LOCATION__ CppCommon::SourceLocation(__FILE__, __LINE__)

//! Source location
/*!
    Source location wraps file name and line number into single object with easy-to-use interface.

    Thread-safe.
*/
class SourceLocation
{
    friend class Exception;

public:
    //! Create a new source location with the given file name and line number
    /*!
        \param filename - File name
        \param line - Line number
    */
    explicit SourceLocation(const char* filename, int line) noexcept : _filename(filename), _line(line) {}
    SourceLocation(const SourceLocation&) noexcept = default;
    SourceLocation(SourceLocation&&) noexcept = default;
    ~SourceLocation() noexcept = default;

    SourceLocation& operator=(const SourceLocation&) noexcept = default;
    SourceLocation& operator=(SourceLocation&&) noexcept = default;

    //! Get file name
    const char* filename() const noexcept { return _filename; }
    //! Get line number
    int line() const noexcept { return _line; }

    //! Get the string from the current source location
    std::string string() const
    { std::stringstream ss; ss << *this; return ss.str(); }

    //! Output source location into the given output stream
    friend std::ostream& operator<<(std::ostream& os, const SourceLocation& source_location);

private:
    const char* _filename;
    int _line;

    SourceLocation() noexcept : SourceLocation(nullptr, 0) {}
};

/*! \example system_source_location.cpp Source location example */

} // namespace CppCommon

#include "source_location.inl"

#endif // CPPCOMMON_SYSTEM_SOURCE_LOCATION_H
