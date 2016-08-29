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
    Filesystem file wraps file management operations (create, open, read, write, flush)
    and file manipulation methods (copy, rename, move, remove).

    Not thread-safe.
*/
class File : public Path
{
public:
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
        \param attributes - File attributes (default is FileAttributes::DEFAULT)
        \param permissions - File permissions (default is FilePermissions::DEFAULT)
    */
    void Create(bool read, bool write, bool truncate = false, const Flags<FileAttributes>& attributes = FileAttributes::DEFAULT, const Flags<FilePermissions>& permissions = FilePermissions::DEFAULT);
    //! Open an existing file
    /*!
        If the file with the same name is not exist the method will raise
        a filesystem exception!

        \param read - Read mode
        \param write - Write mode
        \param truncate - Truncate file (default is false)
        \param attributes - File attributes (default is FileAttributes::DEFAULT)
        \param permissions - File permissions (default is FilePermissions::DEFAULT)
    */
    void Open(bool read, bool write, bool truncate = false, const Flags<FileAttributes>& attributes = FileAttributes::DEFAULT, const Flags<FilePermissions>& permissions = FilePermissions::DEFAULT);
    //! Open or create file
    /*!
        \param read - Read mode
        \param write - Write mode
        \param truncate - Truncate file (default is false)
        \param attributes - File attributes (default is FileAttributes::DEFAULT)
        \param permissions - File permissions (default is FilePermissions::DEFAULT)
        \return The current file instance (opened or created)
    */
    void OpenOrCreate(bool read, bool write, bool truncate = false, const Flags<FileAttributes>& attributes = FileAttributes::DEFAULT, const Flags<FilePermissions>& permissions = FilePermissions::DEFAULT);

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
