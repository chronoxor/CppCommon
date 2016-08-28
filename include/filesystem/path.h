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

#include "time/timestamp.h"
#include "types/flags.h"

#include <string>

namespace CppCommon {

//! File types
enum class FileType
{
    NONE      = 0,      //!< None (file not found)
    REGULAR   = 1,      //!< Regular file
    DIRECTORY = 2,      //!< Directory
    SYMLINK   = 3,      //!< Symbolic link
    BLOCK     = 4,      //!< Block device
    CHARACTER = 5,      //!< Character device
    FIFO      = 6,      //!< FIFO (named pipe)
    SOCKET    = 7,      //!< Socket
    UNKNOWN   = 8       //!< Unknown
};

//! File attributes (Windows specific)
enum class FileAttributes
{
    NONE      = 0x00,   //!< None
    NORMAL    = 0x01,   //!< Normal
    ARCHIVED  = 0x02,   //!< Archived
    HIDDEN    = 0x04,   //!< Hidden
    INDEXED   = 0x08,   //!< Indexed
    OFFLINE   = 0x10,   //!< Offline
    READONLY  = 0x20,   //!< Readonly
    SYSTEM    = 0x40,   //!< System
    TEMPORARY = 0x80    //!< Temporary
};

//! File permissions (Unix specific)
enum class FilePermissions
{
    NONE      = 00000,  //!< None
    IRUSR     = 00400,  //!< Read permission bit for the owner of the file
    IWUSR     = 00200,  //!< Write permission bit for the owner of the file
    IXUSR     = 00100,  //!< Execute (for ordinary files) or search (for directories) permission bit for the owner of the file
    IRWXU     = 00700,  //!< This is equivalent to IRUSR | IWUSR | IXUSR
    IRGRP     = 00040,  //!< Read permission bit for the group owner of the file
    IWGRP     = 00020,  //!< Write permission bit for the group owner of the file
    IXGRP     = 00010,  //!< Execute or search permission bit for the group owner of the file
    IRWXG     = 00070,  //!< This is equivalent to IRGRP | IWGRP | IXGRP
    IROTH     = 00004,  //!< Read permission bit for other users
    IWOTH     = 00002,  //!< Write permission bit for other users
    IXOTH     = 00001,  //!< Execute or search permission bit for other users
    IRWXO     = 00007,  //!< This is equivalent to IROTH | IWOTH | IXOTH
    ISUID     = 04000,  //!< This is the set-user-ID on execute bit
    ISGID     = 02000,  //!< This is the set-group-ID on execute bit
    ISVTX     = 01000,  //!< This is the sticky bit
};

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
    virtual ~Path() = default;

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

    //! Get the native path value
    const std::string& native() const noexcept { return _path; }

    //! Get the pah value in UTF-8 format
    std::string to_string() const { return std::string(_path); }
    //! Get the path value in UTF-16 format
    std::wstring to_wstring() const { return Encoding::FromUTF8(_path); }

    //! Decompose root path from the current path
    Path root() const;
    //! Decompose relative path from the current path
    Path relative() const;
    //! Decompose parent path from the current path
    Path parent() const;
    //! Decompose filename from the current path
    Path filename() const;
    //! Decompose stem from the current path
    Path stem() const;
    //! Decompose extension from the current path
    Path extension() const;

    //! Get the path file type
    FileType type() const;
    //! Get the path file attributes
    Flags<FileAttributes> attributes() const;
    //! Get the path file permissions
    Flags<FilePermissions> permissions() const;
    //! Get the path created UTC timestamp
    UtcTimestamp created() const;
    //! Get the path modified UTC timestamp
    UtcTimestamp modified() const;
    //! Get the path count of hardlinks
    int hardlinks() const;

    //! Is path empty?
    bool empty() const noexcept { return _path.empty(); }

    //! Has root path?
    bool HasRoot() const { return !root().empty(); }
    //! Has relative path?
    bool HasRelative() const { return !relative().empty(); }
    //! Has parent path?
    bool HasParent() const { return !parent().empty(); }
    //! Has filename?
    bool HasFilename() const { return !filename().empty(); }
    //! Has stem?
    bool HasStem() const { return !stem().empty(); }
    //! Has extension?
    bool HasExtension() const { return !extension().empty(); }

    //! Is absolute path?
    bool IsAbsolute() const { return HasRoot(); }
    //! Is relative path?
    bool IsRelative() const { return !HasRoot(); }

    //! Is path exists?
    virtual bool IsExists() const { return type() != FileType::NONE; }

    //! Is path points to regular file?
    bool IsRegularFile() const { return type() == FileType::REGULAR; }
    //! Is path points to directory?
    bool IsDirectory() const { return type() == FileType::DIRECTORY; }
    //! Is path points to symbolic link?
    bool IsSymlink() const { return type() == FileType::SYMLINK; }
    //! Is path points to special file (block, character, FIFO, socket)?
    bool IsOther() const;

    //! Clear path content
    void Clear() noexcept { return _path.clear(); }

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

    //! Set file attributes of the current path
    void SetAttributes(const Flags<FileAttributes>& attributes);
    //! Set file permissions of the current path
    void SetPermissions(const Flags<FilePermissions>& permissions);
    //! Set created UTC timestamp of the current path
    void SetCreated(const UtcTimestamp& timestamp);
    //! Set modified UTC timestamp of the current path
    void SetModified(const UtcTimestamp& timestamp);
    //! Touch the current path and set its modified UTC timestamp to the current value
    void Touch() { SetModified(UtcTimestamp()); }

    //! Get the system path separator character ('\' for Windows or '/' for Unix)
    static char separator() noexcept;

    //! Get the initial path of the current process
    static Path initial();
    //! Get the current path of the current process
    static Path current();
    //! Get the executable path of the current process
    static Path executable();
    //! Get the home path of the current process
    static Path home();
    //! Get the temporary path of the current process
    static Path temp();
    //! Get the unique filename in UUID format "00000000-0000-0000-0000-000000000000"
    static Path unique();

    //! Set the current path of the current process
    static void SetCurrent(const std::string& path)
    { SetCurrent(Path(path)); }
    static void SetCurrent(const std::wstring& path)
    { SetCurrent(Path(path)); }
    static void SetCurrent(const Path& path);

    //! Input instance from the given input stream
    friend std::istream& operator>>(std::istream& is, Path& instance)
    { is >> instance._path; return is; }
    //! Output instance into the given output stream
    friend std::ostream& operator<<(std::ostream& os, const Path& instance)
    { os << instance._path; return os; }

    //! Swap two instances
    friend void swap(Path& path1, Path& path2) noexcept;

protected:
    std::string _path;
};

/*! \example filesystem_path.cpp Filesystem path wrapper example */

} // namespace CppCommon

#include "path.inl"

#endif // CPPCOMMON_FILESYSTEM_PATH_H
