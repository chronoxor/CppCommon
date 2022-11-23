/*!
    \file path.h
    \brief Filesystem path definition
    \author Ivan Shynkarenka
    \date 11.08.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_FILESYSTEM_PATH_H
#define CPPCOMMON_FILESYSTEM_PATH_H

#include "common/flags.h"
#include "string/encoding.h"
#include "string/format.h"
#include "time/timestamp.h"

#include <string>

namespace CppCommon {

//! File types
enum class FileType
{
    NONE,               //!< None (file not found)
    REGULAR,            //!< Regular file
    DIRECTORY,          //!< Directory
    SYMLINK,            //!< Symbolic link
    BLOCK,              //!< Block device
    CHARACTER,          //!< Character device
    FIFO,               //!< FIFO (named pipe)
    SOCKET,             //!< Socket
    UNKNOWN             //!< Unknown
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
    ISVTX     = 01000   //!< This is the sticky bit
};

//! Filesystem space information
struct SpaceInfo
{
    uint64_t capacity;  //!< Total size of the filesystem, in bytes
    uint64_t free;      //!< Free space on the filesystem, in bytes
    uint64_t available; //!< Free space available to a non-privileged process (may be equal or less than free)
};

//! Filesystem path
/*!
    Filesystem path wraps string directory, filename, symlink and other path types
    in a class and provide utility methods to check type of the path, status and
    permissions in a file system. Additionally path contains operators and methods
    for path manipulation (concatenation, canonization, absolute path).

    Path is managed in UTF-8 encoding!

    Not thread-safe.
*/
class Path
{
public:
    //! Initialize path with an empty value
    Path() : _path() {}
    //! Initialize path with a given C-string value
    /*!
        \param path - Path value as C-string
    */
    Path(const char* path) : _path(path) {}
    //! Initialize path with a given string value
    /*!
        \param path - Path value as string
    */
    Path(const std::string& path) : _path(path) {}
    //! Initialize path with a given wide C-string value
    /*!
        \param path - Path value as wide C-string
    */
    Path(const wchar_t* path) : _path(Encoding::ToUTF8(path)) {}
    //! Initialize path with a given wide string value
    /*!
        \param path - Path value as wide string
    */
    Path(const std::wstring& path) : _path(Encoding::ToUTF8(path)) {}
    Path(const Path&) = default;
    Path(Path&&) = default;
    ~Path() = default;

    Path& operator=(const Path&) = default;
    Path& operator=(Path&&) = default;

    //! Check if the path is not empty
    explicit operator bool() const noexcept { return !empty(); }

    // Append the given path with a path separator
    Path& operator/=(const Path& path)
    { return Append(path); }
    friend Path operator/(const Path& path1, const Path& path2)
    { return Path(path1).Append(path2); }

    // Concatenate the given path without a path separator
    Path& operator+=(const Path& path)
    { return Concat(path); }
    friend Path operator+(const Path& path1, const Path& path2)
    { return Path(path1).Concat(path2); }

    // Path comparison
    friend bool operator==(const Path& path1, const Path& path2)
    { return path1._path == path2._path; }
    friend bool operator!=(const Path& path1, const Path& path2)
    { return path1._path != path2._path; }
    friend bool operator<(const Path& path1, const Path& path2)
    { return path1._path < path2._path; }
    friend bool operator>(const Path& path1, const Path& path2)
    { return path1._path > path2._path; }
    friend bool operator<=(const Path& path1, const Path& path2)
    { return path1._path <= path2._path; }
    friend bool operator>=(const Path& path1, const Path& path2)
    { return path1._path >= path2._path; }

    //! Get the path value as UTF-8 string
    const std::string& string() const noexcept { return _path; }
    //! Get the path value as a wide string
    std::wstring wstring() const { return Encoding::FromUTF8(_path); }

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

    //! Transform the current path to the real path on a filesystem
    Path absolute() const;
    //! Transform the current path and replace all '.' and '..' properly
    Path canonical() const;
    //! Transform the current path and replace all deprecated characters with a given placeholder (default is '_')
    Path validate(char placeholder = '_') const;

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
    size_t hardlinks() const;
    //! Get the path space information
    SpaceInfo space() const;

    //! Is the path empty?
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

    //! Is the path exists?
    bool IsExists() const { return type() != FileType::NONE; }

    //! Is the path points to regular file?
    bool IsRegularFile() const { return type() == FileType::REGULAR; }
    //! Is the path points to directory?
    bool IsDirectory() const { return type() == FileType::DIRECTORY; }
    //! Is the path points to symbolic link?
    bool IsSymlink() const { return type() == FileType::SYMLINK; }
    //! Is the path points to special file (block, character, FIFO, socket)?
    bool IsOther() const;

    //! Is the current path is equivalent to the given one (points to the same node on a filesystem)?
    bool IsEquivalent(const Path& path) const;

    //! Clear the path content
    void Clear() noexcept { return _path.clear(); }

    //! Assign the given path to the current one
    Path& Assign(const Path& path);
    //! Append the given path to the current one
    Path& Append(const Path& path);
    //! Concatenate the given path to the current one
    Path& Concat(const Path& path);
    //! Convert all path separators to system ones ('\' for Windows or '/' for Unix)
    Path& MakePreferred();
    //! Replace the current path filename with a given one
    Path& ReplaceFilename(const Path& filename);
    //! Replace the current path extension with a given one
    Path& ReplaceExtension(const Path& extension);

    //! Remove the current path filename
    Path& RemoveFilename() { return ReplaceFilename(""); }
    //! Remove the current path extension
    Path& RemoveExtension() { return ReplaceExtension(""); }
    //! Remove all trailing separators form the current path
    Path& RemoveTrailingSeparators();

    //! Is the given character is deprecated by the filesystem?
    static bool deprecated(char character) noexcept;
    //! Is the given wide character is deprecated by the filesystem?
    static bool deprecated(wchar_t character) noexcept;
    //! Get filesystem deprecated characters ('\', '/', '?', '%', '*', ':', '|', '"', '<', '>')
    static std::string deprecated();

    //! Get the system path separator character ('\' for Windows or '/' for Unix)
    static char separator() noexcept;

    //! Get the initial path of the process
    static Path initial();
    //! Get the current path of the process
    static Path current();
    //! Get the executable path of the process
    static Path executable();
    //! Get the home path of the process
    static Path home();
    //! Get the temporary path of the process
    static Path temp();
    //! Get the unique filename in UUID format "00000000-0000-0000-0000-000000000000"
    static Path unique();

    //! Copy the given source path to destination path (file, empty directory, symlink, etc)
    /*!
        \param src - Source path
        \param dst - Destination path
        \param overwrite - Overwrite destination path (default is false)
        \return Copied path
    */
    static Path Copy(const Path& src, const Path& dst, bool overwrite = false);
    //! Copy all matched files from the the given source path to destination path (files, directories, symlinks, etc)
    /*!
        \param src - Source path
        \param dst - Destination path
        \param pattern - Regular expression pattern (default is "")
        \param overwrite - Overwrite destination path (default is false)
        \return Copied path
    */
    static Path CopyIf(const Path& src, const Path& dst, const std::string& pattern = "", bool overwrite = false);
    //! Recursively copy the given source path to destination path (files, directories, symlinks, etc)
    /*!
        \param src - Source path
        \param dst - Destination path
        \param overwrite - Overwrite destination path (default is false)
        \return Copied path
    */
    static Path CopyAll(const Path& src, const Path& dst, bool overwrite = false);
    //! Rename the given source path to destination path (file, empty directory, symlink, etc)
    /*!
        \param src - Source path
        \param dst - Destination path
        \return Renamed path
    */
    static Path Rename(const Path& src, const Path& dst);
    //! Remove the given path (file, empty directory, symlink, etc) from the filesystem
    /*!
        \param path - Path to remove
        \return Parent path
    */
    static Path Remove(const Path& path);
    //! Recursively remove the given path (file, empty directory, symlink, etc) from the filesystem
    /*!
        \param path - Path to remove
        \return Parent path
    */
    static Path RemoveAll(const Path& path);
    //! Recursively remove the given path matched to the given pattern (file, empty directory, symlink, etc) from the filesystem
    /*!
        All files/symlinks will be matched to the given pattern!

        \param path - Path to remove
        \param pattern - Regular expression pattern (default is "")
        \return Parent path
    */
    static Path RemoveIf(const Path& path, const std::string& pattern = "");

    //! Set file attributes for the given path
    /*!
        \param path - Path to modify
        \param attributes - Path attributes
    */
    static void SetAttributes(const Path& path, const Flags<FileAttributes>& attributes);
    //! Set file permissions for the given path
    /*!
        \param path - Path to modify
        \param permissions - Path permissions
    */
    static void SetPermissions(const Path& path, const Flags<FilePermissions>& permissions);
    //! Set created UTC timestamp for the given path
    /*!
        \param path - Path to modify
        \param timestamp - Created timestamp
    */
    static void SetCreated(const Path& path, const UtcTimestamp& timestamp);
    //! Set modified UTC timestamp for the given path
    /*!
        \param path - Path to modify
        \param timestamp - Modified timestamp
    */
    static void SetModified(const Path& path, const UtcTimestamp& timestamp);

    //! Set the given path of the process as a current one
    /*!
        \param path - Current path
    */
    static void SetCurrent(const Path& path);

    //! Touch the given path and set its modified UTC timestamp to the current value
    /*!
        \param path - Path to touch
    */
    static void Touch(const Path& path);

    //! Input instance from the given input stream
    friend std::istream& operator>>(std::istream& is, Path& path)
    { is >> path._path; return is; }
    //! Output instance into the given output stream
    friend std::ostream& operator<<(std::ostream& os, const Path& path)
    { os << path._path; return os; }

    //! Swap two instances
    void swap(Path& path) noexcept;
    friend void swap(Path& path1, Path& path2) noexcept;

protected:
    //! Path string
    std::string _path;
};

/*! \example filesystem_path.cpp Filesystem path example */

} // namespace CppCommon

#include "path.inl"

// Resolve forward declaration issue
#include "filesystem/exceptions.h"

#endif // CPPCOMMON_FILESYSTEM_PATH_H
