/*!
    \file file.h
    \brief Filesystem file wrapper definition
    \author Ivan Shynkarenka
    \date 29.08.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_FILESYSTEM_FILE_H
#define CPPCOMMON_FILESYSTEM_FILE_H

#include "filesystem/path.h"

#include <memory>

namespace CppCommon {

//! Filesystem file
/*!
    Filesystem file wraps file management operations (create, open, read, write, flush).

    Not thread-safe.
*/
class File : public Path
{
public:
    //! Default file attributes (Normal)
    static const Flags<FileAttributes> DEFAULT_ATTRIBUTES;
    //! Default file permissions (IRUSR | IWUSR | IRGRP | IROTH)
    static const Flags<FilePermissions> DEFAULT_PERMISSIONS;

    //! Default constructor
    File();
    //! Initialize file path with a given string value in UTF-8 encoding (Unix)
    /*!
        \param path - Path value in UTF-8 encoding
    */
    explicit File(const std::string& path);
    //! Initialize file path with a given string value in UTF-16 encoding (Windows)
    /*!
        \param path - Path value in UTF-16 encoding
    */
    explicit File(const std::wstring& path);
    //! Initialize file path with a given path
    /*!
        \param path - Path value
    */
    explicit File(const Path& path);
    File(const File& instance);
    File(File&&) noexcept = default;
    ~File();

    File& operator=(const std::string& path)
    { Assign(path); return *this; }
    File& operator=(const std::wstring& path)
    { Assign(path); return *this; }
    File& operator=(const Path& path)
    { Assign(path); return *this; }
    File& operator=(const File&) = default;
    File& operator=(File&&) noexcept = default;

    //! Get the current read/write offset of the opened file
    uint64_t offset() const;
    //! Get the current file size
    uint64_t size() const;

    //! Is file empty?
    bool IsFileEmpty() const
    { return (size() == 0); }
    //! Is file exists?
    bool IsFileExists() const;
    //! Is file opened?
    bool IsFileOpened() const;

    //! Create a new file
    /*!
        If the file with the same name is already exist the method will raise
        a filesystem exception!

        \param read - Read mode
        \param write - Write mode
        \param truncate - Truncate file (default is false)
        \param attributes - File attributes (default is File::DEFAULT_ATTRIBUTES)
        \param permissions - File permissions (default is File::DEFAULT_PERMISSIONS)
    */
    void Create(bool read, bool write, bool truncate = false, const Flags<FileAttributes>& attributes = File::DEFAULT_ATTRIBUTES, const Flags<FilePermissions>& permissions = File::DEFAULT_PERMISSIONS);
    //! Open an existing file
    /*!
        If the file with the same name is not exist the method will raise
        a filesystem exception!

        \param read - Read mode
        \param write - Write mode
        \param truncate - Truncate file (default is false)
        \param attributes - File attributes (default is File::DEFAULT_ATTRIBUTES)
        \param permissions - File permissions (default is File::DEFAULT_PERMISSIONS)
    */
    void Open(bool read, bool write, bool truncate = false, const Flags<FileAttributes>& attributes = File::DEFAULT_ATTRIBUTES, const Flags<FilePermissions>& permissions = File::DEFAULT_PERMISSIONS);
    //! Open or create file
    /*!
        \param read - Read mode
        \param write - Write mode
        \param truncate - Truncate file (default is false)
        \param attributes - File attributes (default is File::DEFAULT_ATTRIBUTES)
        \param permissions - File permissions (default is File::DEFAULT_PERMISSIONS)
        \return The current file instance (opened or created)
    */
    void OpenOrCreate(bool read, bool write, bool truncate = false, const Flags<FileAttributes>& attributes = File::DEFAULT_ATTRIBUTES, const Flags<FilePermissions>& permissions = File::DEFAULT_PERMISSIONS);

    //! Read from the opened file
    /*!
        If the file is not opened for reading the method will raise
        a filesystem exception!

        \param buffer - Buffer to read
        \param size - Buffer size
        \return Count of read bytes
    */
    size_t Read(uint8_t* buffer, size_t size);
    //! Write into the opened file
    /*!
        If the file is not opened for writing the method will raise
        a filesystem exception!

        \param buffer - Buffer to write
        \param size - Buffer size
        \return Count of written bytes
    */
    size_t Write(const uint8_t* buffer, size_t size);

    //! Seek into the opened file
    /*!
        If the file is not opened for writing the method will raise
        a filesystem exception!

        \param offset - Seek offset
    */
    void Seek(uint64_t offset);
    //! Resize the current file
    /*!
        If the current file size is greater than required size then the file
        will be truncated with data loss. If the current file size is less
        than required size then the file will be extend.

        Works for opened and not opened files!

        \param size - File size
    */
    void Resize(uint64_t size);

    //! Flush the file
    /*!
        Flush any unwritten data of the opened file to the physical file
        on a disk. If the file is not opened for writing the method will
        raise a filesystem exception!
    */
    void Flush();

    //! Close the file
    /*!
        If the file is not opened for writing the method will raise a
        filesystem exception!
    */
    void Close();

    //! Swap two instances
    void swap(File& file) noexcept;
    friend void swap(File& file1, File& file2) noexcept;

private:
    class Impl;
    std::unique_ptr<Impl> _pimpl;
};

/*! \example filesystem_file.cpp Filesystem file wrapper example */

} // namespace CppCommon

#include "file.inl"

#endif // CPPCOMMON_FILESYSTEM_FILE_H
