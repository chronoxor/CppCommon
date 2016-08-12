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

    // Append the given path with a path separator
    Path& operator/=(const std::string& path);
    Path& operator/=(const std::wstring& path);
    Path& operator/=(const Path&);
    friend Path operator/(const Path& path1, const std::string& path2);
    friend Path operator/(const std::string& path1, const Path& path2);
    friend Path operator/(const Path& path1, const std::wstring& path2);
    friend Path operator/(const std::wstring& path1, const Path& path2);
    friend Path operator/(const Path& path1, const Path& path2);

    // Concatenate the given path without a path separator
    Path& operator+=(const std::string& path);
    Path& operator+=(const std::wstring& path);
    Path& operator+=(const Path&);
    friend Path operator+(const Path& path1, const std::string& path2);
    friend Path operator+(const std::string& path1, const Path& path2);
    friend Path operator+(const Path& path1, const std::wstring& path2);
    friend Path operator+(const std::wstring& path1, const Path& path2);
    friend Path operator+(const Path& path1, const Path& path2);

    // Path comparison
    friend bool operator==(const Path& path1, const std::string& path2);
    friend bool operator==(const std::string& path1, const Path& path2);
    friend bool operator==(const Path& path1, const std::wstring& path2);
    friend bool operator==(const std::wstring& path1, const Path& path2);
    friend bool operator==(const Path& path1, const Path& path2);
    friend bool operator!=(const Path& path1, const std::string& path2);
    friend bool operator!=(const std::string& path1, const Path& path2);
    friend bool operator!=(const Path& path1, const std::wstring& path2);
    friend bool operator!=(const std::wstring& path1, const Path& path2);
    friend bool operator!=(const Path& path1, const Path& path2);
    friend bool operator<(const Path& path1, const std::string& path2);
    friend bool operator<(const std::string& path1, const Path& path2);
    friend bool operator<(const Path& path1, const std::wstring& path2);
    friend bool operator<(const std::wstring& path1, const Path& path2);
    friend bool operator<(const Path& path1, const Path& path2);
    friend bool operator>(const Path& path1, const std::string& path2);
    friend bool operator>(const std::string& path1, const Path& path2);
    friend bool operator>(const Path& path1, const std::wstring& path2);
    friend bool operator>(const std::wstring& path1, const Path& path2);
    friend bool operator>(const Path& path1, const Path& path2);
    friend bool operator<=(const Path& path1, const std::string& path2);
    friend bool operator<=(const std::string& path1, const Path& path2);
    friend bool operator<=(const Path& path1, const std::wstring& path2);
    friend bool operator<=(const std::wstring& path1, const Path& path2);
    friend bool operator<=(const Path& path1, const Path& path2);
    friend bool operator>=(const Path& path1, const std::string& path2);
    friend bool operator>=(const std::string& path1, const Path& path2);
    friend bool operator>=(const Path& path1, const std::wstring& path2);
    friend bool operator>=(const std::wstring& path1, const Path& path2);
    friend bool operator>=(const Path& path1, const Path& path2);

    //! Get the native path value
    const std::string& native() const noexcept { return _path; }

    //! Get the path value in UTF-8 format
    std::string string() const;
    //! Get the path value in UTF-16 format
    std::wstring wstring() const;

    //! Is the path empty?
    bool empty() const noexcept { return _path.empty(); }

    //! Clear path content
    void clear() noexcept { return _path.clear(); }

    //! Swap two path wrappers
    /*!
         \param path1 - Path wrapper
         \param path2 - Path wrapper
    */
    friend void swap(Path& path1, Path& path2);

protected:
    std::string _path;
};

/*! \example filesystem_path.cpp Filesystem path wrapper example */

} // namespace CppCommon

#include "path.inl"

#endif // CPPCOMMON_FILESYSTEM_PATH_H
