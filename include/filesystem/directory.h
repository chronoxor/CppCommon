/*!
    \file directory.h
    \brief Filesystem directory wrapper definition
    \author Ivan Shynkarenka
    \date 30.08.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_FILESYSTEM_DIRECTORY_H
#define CPPCOMMON_FILESYSTEM_DIRECTORY_H

#include "filesystem/path.h"

namespace CppCommon {

//! Filesystem directory
/*!
    Filesystem directory wraps directory management operations (create,
    remove, iterate).

    Not thread-safe.
*/
class Directory : public Path
{
public:
    //! Default directory attributes (Normal)
    static const Flags<FileAttributes> DEFAULT_ATTRIBUTES;
    //! Default directory permissions (IRUSR | IWUSR | IXUSR | IRGRP | IXGRP | IROTH | IXOTH)
    static const Flags<FilePermissions> DEFAULT_PERMISSIONS;

    Directory() : Path() {}
    Directory(const Path& path) : Path(path) {}
    Directory(const Directory&) = default;
    Directory(Directory&&) noexcept = default;
    ~Directory() = default;

    Directory& operator=(const Path& path)
    { Assign(path); return *this; }
    Directory& operator=(const Directory&) = default;
    Directory& operator=(Directory&&) noexcept = default;

    //! Is directory exists?
    bool IsDirectoryExists() const;
    //! Is directory empty?
    bool IsDirectoryEmpty() const;

    //! Create directory from the given path
    /*!
        \param path - Directory path
        \param attributes - Directory attributes (default is Directory::DEFAULT_ATTRIBUTES)
        \param permissions - Directory permissions (default is Directory::DEFAULT_PERMISSIONS)
        \return Created directory
    */
    static Directory Create(const Path& path, const Flags<FileAttributes>& attributes = Directory::DEFAULT_ATTRIBUTES, const Flags<FilePermissions>& permissions = Directory::DEFAULT_PERMISSIONS);
    //! Create full directory tree of the given path
    /*!
        \param path - Directory path
        \param attributes - Directory attributes (default is Directory::DEFAULT_ATTRIBUTES)
        \param permissions - Directory permissions (default is Directory::DEFAULT_PERMISSIONS)
        \return Created full directory tree
    */
    static Directory CreateTree(const Path& path, const Flags<FileAttributes>& attributes = Directory::DEFAULT_ATTRIBUTES, const Flags<FilePermissions>& permissions = Directory::DEFAULT_PERMISSIONS);

    //! Swap two instances
    void swap(Directory& directory) noexcept;
    friend void swap(Directory& directory1, Directory& directory2) noexcept;
};

/*! \example filesystem_symlink.cpp Filesystem symlink wrapper example */

} // namespace CppCommon

#include "directory.inl"

#endif // CPPCOMMON_FILESYSTEM_DIRECTORY_H
