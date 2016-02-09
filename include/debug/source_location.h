/*!
    \file source_location.h
    \brief Source location wrapper definition
    \author Ivan Shynkarenka
    \date 09.02.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_SOURCE_LOCATION_H
#define CPPCOMMON_SOURCE_LOCATION_H

#include <string>

namespace CppCommon {

//! Current source location macro
/*!
    Create a new source location wrapper with a current file name and line number.
*/
#define __LOCATION__ CppCommon::SourceLocation(__FILE__, __LINE__)

//! Source location wrapper
/*!
    Source location wraps file name and line number into single object with easy-to-use interface.
*/
class SourceLocation
{
public:
    //! Default class constructor
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

    //! Get string from the current source location
    std::string to_string() const { return std::string(_filename) + ':' + std::to_string(_line); }

    //! Output source location into the given output stream
    friend std::ostream& operator<<(std::ostream& os, const SourceLocation& instance)
    { os << instance.to_string(); return os; }

private:
    const char* _filename;
    int _line;
};

} // namespace CppCommon

/*! \example debug_source_location.cpp Source location wrapper example */

#endif //CPPCOMMON_SOURCE_LOCATION_H
