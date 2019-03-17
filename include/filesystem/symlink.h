/*!
    \file symlink.h
    \brief Filesystem symlink definition
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
    //! Initialize symbolic link with an empty path
    Symlink() : Path() {}
    //! Initialize symbolic link with a given path
    /*!
        \param path - Symbolic link path
    */
    Symlink(const Path& path) : Path(path) {}
    Symlink(const Symlink&) = default;
    Symlink(Symlink&&) = default;
    ~Symlink() = default;

    Symlink& operator=(const Path& path)
    { Assign(path); return *this; }
    Symlink& operator=(const Symlink&) = default;
    Symlink& operator=(Symlink&&) = default;

    //! Check if the symlink is present
    explicit operator bool() const noexcept { return IsSymlinkExists() && IsTargetExists(); }

    //! Read symlink target path
    Path target() const;

    //! Is the symlink exists?
    bool IsSymlinkExists() const;
    //! Is the target exists?
    bool IsTargetExists() const
    { return !target().empty(); }

    //! Create a new symlink
    /*!
        \param src - Source path
        \param dst - Destination path
        \return Created symlink
    */
    static Symlink CreateSymlink(const Path& src, const Path& dst);
    //! Create a new hardlink
    /*!
        \param src - Source path
        \param dst - Destination path
        \return Created hardlink
    */
    static Path CreateHardlink(const Path& src, const Path& dst);

    //! Copy the current symlink to another destination path
    /*!
        If the source path is a symlink then a new destination symlink
        will be created based on its target. Otherwise symlink will be
        created explicitly based on the source path.

        \param src - Source path
        \param dst - Destination path
        \return Copied symlink
    */
    static Symlink CopySymlink(const Path& src, const Path& dst)
    { return CreateSymlink((src.IsSymlink() ? Symlink(src).target() : src), dst); }

    //! Swap two instances
    void swap(Symlink& symlink) noexcept;
    friend void swap(Symlink& symlink1, Symlink& symlink2) noexcept;
};

/*! \example filesystem_symlink.cpp Filesystem symlink example */

} // namespace CppCommon

#include "symlink.inl"

#endif // CPPCOMMON_FILESYSTEM_SYMLINK_H
