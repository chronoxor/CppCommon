/*!
    \file path.h
    \brief Filesystem path wrapper definition
    \author Ivan Shynkarenka
    \date 11.08.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_FILESYSTEM_PATH_H
#define CPPCOMMON_FILESYSTEM_PATH_H

#include "string/encoding.h"

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
    explicit Path(const std::string& path) : _path(path) {}
    //! Initialize path with a given string value in UTF-16 encoding (Windows)
    /*!
        \param path - Path value in UTF-16 encoding
    */
    explicit Path(const std::wstring& path) : _path(Encoding::ToUTF8(path)) {}
    Path(const Path&) = default;
    Path(Path&&) noexcept = default;
    ~Path() = default;

    Path& operator=(const std::string& path)
    { return Assign(path); }
    Path& operator=(const std::wstring& path)
    { return Assign(path); }
    Path& operator=(const Path&) = default;
    Path& operator=(Path&&) noexcept = default;

    // Append the given path with a path separator
    Path& operator/=(const std::string& path)
    { return Append(path); }
    Path& operator/=(const std::wstring& path)
    { return Append(path); }
    Path& operator/=(const Path& path)
    { return Append(path); }
    friend Path operator/(const Path& path1, const std::string& path2)
    { return Path(path1).Append(path2); }
    friend Path operator/(const std::string& path1, const Path& path2)
    { return Path(path1).Append(path2); }
    friend Path operator/(const Path& path1, const std::wstring& path2)
    { return Path(path1).Append(path2); }
    friend Path operator/(const std::wstring& path1, const Path& path2)
    { return Path(path1).Append(path2); }
    friend Path operator/(const Path& path1, const Path& path2)
    { return Path(path1).Append(path2); }

    // Concatenate the given path without a path separator
    Path& operator+=(const std::string& path)
    { return Concat(path); }
    Path& operator+=(const std::wstring& path)
    { return Concat(path); }
    Path& operator+=(const Path& path)
    { return Concat(path); }
    friend Path operator+(const Path& path1, const std::string& path2)
    { return Path(path1).Concat(path2); }
    friend Path operator+(const std::string& path1, const Path& path2)
    { return Path(path1).Concat(path2); }
    friend Path operator+(const Path& path1, const std::wstring& path2)
    { return Path(path1).Concat(path2); }
    friend Path operator+(const std::wstring& path1, const Path& path2)
    { return Path(path1).Concat(path2); }
    friend Path operator+(const Path& path1, const Path& path2)
    { return Path(path1).Concat(path2); }

    // Path comparison
    friend bool operator==(const Path& path1, const std::string& path2)
    { return path1 == Path(path2); }
    friend bool operator==(const std::string& path1, const Path& path2)
    { return Path(path1) == path2; }
    friend bool operator==(const Path& path1, const std::wstring& path2)
    { return path1 == Path(path2); }
    friend bool operator==(const std::wstring& path1, const Path& path2)
    { return Path(path1) == path2; }
    friend bool operator==(const Path& path1, const Path& path2)
    { return path1._path == path2._path; }
    friend bool operator!=(const Path& path1, const std::string& path2)
    { return path1 != Path(path2); }
    friend bool operator!=(const std::string& path1, const Path& path2)
    { return Path(path1) != path2; }
    friend bool operator!=(const Path& path1, const std::wstring& path2)
    { return path1 != Path(path2); }
    friend bool operator!=(const std::wstring& path1, const Path& path2)
    { return Path(path1) != path2; }
    friend bool operator!=(const Path& path1, const Path& path2)
    { return path1._path != path2._path; }
    friend bool operator<(const Path& path1, const std::string& path2)
    { return path1 < Path(path2); }
    friend bool operator<(const std::string& path1, const Path& path2)
    { return Path(path1) < path2; }
    friend bool operator<(const Path& path1, const std::wstring& path2)
    { return path1 < Path(path2); }
    friend bool operator<(const std::wstring& path1, const Path& path2)
    { return Path(path1) < path2; }
    friend bool operator<(const Path& path1, const Path& path2)
    { return path1._path < path2._path; }
    friend bool operator>(const Path& path1, const std::string& path2)
    { return path1 > Path(path2); }
    friend bool operator>(const std::string& path1, const Path& path2)
    { return Path(path1) > path2; }
    friend bool operator>(const Path& path1, const std::wstring& path2)
    { return path1 > Path(path2); }
    friend bool operator>(const std::wstring& path1, const Path& path2)
    { return Path(path1) > path2; }
    friend bool operator>(const Path& path1, const Path& path2)
    { return path1._path > path2._path; }
    friend bool operator<=(const Path& path1, const std::string& path2)
    { return path1 <= Path(path2); }
    friend bool operator<=(const std::string& path1, const Path& path2)
    { return Path(path1) <= path2; }
    friend bool operator<=(const Path& path1, const std::wstring& path2)
    { return path1 <= Path(path2); }
    friend bool operator<=(const std::wstring& path1, const Path& path2)
    { return Path(path1) <= path2; }
    friend bool operator<=(const Path& path1, const Path& path2)
    { return path1._path <= path2._path; }
    friend bool operator>=(const Path& path1, const std::string& path2)
    { return path1 >= Path(path2); }
    friend bool operator>=(const std::string& path1, const Path& path2)
    { return Path(path1) >= path2; }
    friend bool operator>=(const Path& path1, const std::wstring& path2)
    { return path1 >= Path(path2); }
    friend bool operator>=(const std::wstring& path1, const Path& path2)
    { return Path(path1) >= path2; }
    friend bool operator>=(const Path& path1, const Path& path2)
    { return path1._path >= path2._path; }

    //! Input instance from the given input stream
    friend std::istream& operator>>(std::istream& is, Path& instance)
    { is >> instance._path; return is; }
    //! Output instance into the given output stream
    friend std::ostream& operator<<(std::ostream& os, const Path& instance)
    { os << instance._path; return os; }

    //! Swap two instances
    friend void swap(Path& path1, Path& path2);

    //! Get the native path value
    const std::string& native() const noexcept { return _path; }

    //! Get the path value in UTF-8 format
    std::string string() const { return std::string(_path); }
    //! Get the path value in UTF-16 format
    std::wstring wstring() const { return Encoding::FromUTF8(_path); }

    //! Is the path empty?
    bool empty() const noexcept { return _path.empty(); }

    //! Clear path content
    void clear() noexcept { return _path.clear(); }

    //! Assign the given path to the current one
    Path& Assign(const std::string& path)
    { return Assign(Path(path)); }
    Path& Assign(const std::wstring& path)
    { return Assign(Path(path)); }
    Path& Assign(const Path& path);

    //! Append the given path to the current one
    Path& Append(const std::string& path)
    { return Append(Path(path)); }
    Path& Append(const std::wstring& path)
    { return Append(Path(path)); }
    Path& Append(const Path& path);

    //! Concatenate the given path to the current one
    Path& Concat(const std::string& path)
    { return Concat(Path(path)); }
    Path& Concat(const std::wstring& path)
    { return Concat(Path(path)); }
    Path& Concat(const Path& path);

    //! Convert all path separators to system ones ('\' for Windows or '/' for Unix)
    Path& MakePreferred();

    //! Replace the current path filename with a given one
    Path& ReplaceFilename(const std::string& filename)
    { return ReplaceFilename(Path(filename)); }
    Path& ReplaceFilename(const std::wstring& filename)
    { return ReplaceFilename(Path(filename)); }
    Path& ReplaceFilename(const Path& filename);

    //! Replace the current path extension with a given one
    Path& ReplaceExtension(const std::string& extension)
    { return ReplaceExtension(Path(extension)); }
    Path& ReplaceExtension(const std::wstring& extension)
    { return ReplaceExtension(Path(extension)); }
    Path& ReplaceExtension(const Path& extension);

    //! Remove the current path filename
    Path& RemoveFilename() { return ReplaceFilename(""); }
    //! Remove the current path extension
    Path& RemoveExtension() { return ReplaceExtension(""); }
    //! Remove all trailing separators form the current path
    Path& RemoveTrailingSeparators();

    //! Get the system path separator character ('\' for Windows or '/' for Unix)
    static char separator();

protected:
    std::string _path;
};

/*! \example filesystem_path.cpp Filesystem path wrapper example */

} // namespace CppCommon

#include "path.inl"

#endif // CPPCOMMON_FILESYSTEM_PATH_H
