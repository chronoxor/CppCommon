/*!
    \file path.h
    \brief Filesystem path wrapper definition
    \author Ivan Shynkarenka
    \date 11.08.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_FILESYSTEM_PATH_H
#define CPPCOMMON_FILESYSTEM_PATH_H

#include <string>

namespace CppCommon {

//! Filesystem path
/*!
    Filesystem path wraps string directory, filename, symlink and other path types
    in a class and provide utility methods to check type of the path, status and
    permissions in a file system. Additionally path wrapper contains operators and
    methods for path manipulation (concatenation, canonization, absolute path).

    Path is managed in UTF-8 encoding!

    Not thread-safe.
*/
class Path
{
public:
    //! Default constructor
    Path() : _path() {}
    //! Initialize path with a given string value in UTF-8 encoding (Unix)
    /*!
        \param path - Path value in UTF-8 encoding
    */
    explicit Path(const std::string& path);
    //! Initialize path with a given string value in UTF-16 encoding (Windows)
    /*!
        \param path - Path value in UTF-16 encoding
    */
    explicit Path(const std::wstring& path);
    Path(const Path&) = default;
    Path(Path&&) noexcept = default;
    ~Path() = default;

    Path& operator=(const std::string& path);
    Path& operator=(const std::wstring& path);
    Path& operator=(const Path&) = default;
    Path& operator=(Path&&) noexcept = default;

protected:
    std::string _path;
};

/*! \example filesystem_path.cpp Filesystem path wrapper example */

} // namespace CppCommon

#endif // CPPCOMMON_FILESYSTEM_PATH_H
