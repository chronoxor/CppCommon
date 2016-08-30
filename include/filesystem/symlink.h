/*!
    \file symlink.h
    \brief Filesystem symlink wrapper definition
    \author Ivan Shynkarenka
    \date 30.08.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_FILESYSTEM_SYMLINK_H
#define CPPCOMMON_FILESYSTEM_SYMLINK_H

#include "filesystem/path.h"

namespace CppCommon {

//! Filesystem symlink
/*!
    Filesystem symlink wraps link management operations (create symlink,
    read symlink target, copy symlink, create hardlink).

    Not thread-safe.
*/
class Symlink : public Path
{
public:
    //! Default constructor
    Symlink() : Path() {}
    //! Initialize symlink path with a given string value in UTF-8 encoding (Unix)
    /*!
        \param path - Path value in UTF-8 encoding
    */
    explicit Symlink(const std::string& path) : Path(path) {}
    //! Initialize symlink path with a given string value in UTF-16 encoding (Windows)
    /*!
        \param path - Path value in UTF-16 encoding
    */
    explicit Symlink(const std::wstring& path) : Path(path) {}
    //! Initialize symlink path with a given path
    /*!
        \param path - Path value
    */
    explicit Symlink(const Path& path) : Path(path) {}
    Symlink(const Symlink&) = default;
    Symlink(Symlink&&) noexcept = default;
    ~Symlink() = default;

    Symlink& operator=(const std::string& path)
    { Assign(path); return *this; }
    Symlink& operator=(const std::wstring& path)
    { Assign(path); return *this; }
    Symlink& operator=(const Path& path)
    { Assign(path); return *this; }
    Symlink& operator=(const Symlink&) = default;
    Symlink& operator=(Symlink&&) noexcept = default;

    //! Read symlink target path
    Path target() const;

    //! Is symlink exists?
    bool IsSymlinkExists() const
    { return type() == FileType::SYMLINK; }
    //! Is target exists?
    bool IsTargetExists() const
    { return !target().empty(); }

    //! Copy the current symlink to another destination path
    /*!
        \param dst - Destination symlink path
    */
    void CopySymlink(const Path& dst) const
    { IsSymlink() ? CreateSymlink(target(), dst) : CreateSymlink(*this, dst); }

    //! Create a new symlink
    /*!
        \param src - Source path
        \param dst - Destination symlink path
    */
    static void CreateSymlink(const Path& src, const Path& dst);
    //! Create a new hardlink
    /*!
        \param src - Source path
        \param dst - Destination hardlink path
    */
    static void CreateHardlink(const Path& src, const Path& dst);

    //! Swap two instances
    void swap(Symlink& symlink) noexcept;
    friend void swap(Symlink& symlink1, Symlink& symlink2) noexcept;
};

/*! \example filesystem_symlink.cpp Filesystem symlink wrapper example */

} // namespace CppCommon

#include "symlink.inl"

#endif // CPPCOMMON_FILESYSTEM_SYMLINK_H
