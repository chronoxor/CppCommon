/*!
    \file file.h
    \brief Filesystem file definition
    \author Ivan Shynkarenka
    \date 29.08.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_FILESYSTEM_FILE_H
#define CPPCOMMON_FILESYSTEM_FILE_H

#include "common/reader.h"
#include "common/writer.h"
#include "filesystem/path.h"

#include <memory>
#include <vector>

namespace CppCommon {

//! Filesystem file
/*!
    Filesystem file wraps file management operations (create, open, read, write, flush, close).

    Not thread-safe.
*/
class File : public Path, public Reader, public Writer
{
public:
    //! Default file attributes (Normal)
    static const Flags<FileAttributes> DEFAULT_ATTRIBUTES;
    //! Default file permissions (IRUSR | IWUSR | IRGRP | IROTH)
    static const Flags<FilePermissions> DEFAULT_PERMISSIONS;
    //! Default file buffer size (8192)
    static const size_t DEFAULT_BUFFER;

    //! Initialize file with an empty path
    File();
    //! Initialize file with a given path
    /*!
        \param path - File path
    */
    File(const Path& path);
    File(const File& file);
    File(File&& file) noexcept;
    virtual ~File();

    File& operator=(const Path& path)
    { Assign(path); return *this; }
    File& operator=(const File& file);
    File& operator=(File&& file) noexcept;

    //! Check if the file opened
    explicit operator bool() const noexcept { return IsFileOpened(); }

    //! Get the current read/write offset of the opened file
    uint64_t offset() const;
    //! Get the current file size
    uint64_t size() const;

    //! Is the file exists?
    bool IsFileExists() const;
    //! Is the file empty?
    bool IsFileEmpty() const
    { return (size() == 0); }
    //! Is the file opened?
    bool IsFileOpened() const;
    //! Is the file opened for reading?
    bool IsFileReadOpened() const;
    //! Is the file opened for writing?
    bool IsFileWriteOpened() const;

    //! Create a new file
    /*!
        If the file with the same name is already exist the method will raise
        a filesystem exception!

        \param read - Read mode
        \param write - Write mode
        \param attributes - File attributes (default is File::DEFAULT_ATTRIBUTES)
        \param permissions - File permissions (default is File::DEFAULT_PERMISSIONS)
        \param buffer - File buffer size (default is File::DEFAULT_BUFFER)
    */
    void Create(bool read, bool write, const Flags<FileAttributes>& attributes = File::DEFAULT_ATTRIBUTES, const Flags<FilePermissions>& permissions = File::DEFAULT_PERMISSIONS, size_t buffer = File::DEFAULT_BUFFER);
    //! Open an existing file
    /*!
        If the file with the same name is not exist the method will raise
        a filesystem exception!

        \param read - Read mode
        \param write - Write mode
        \param truncate - Truncate file (default is false)
        \param attributes - File attributes (default is File::DEFAULT_ATTRIBUTES)
        \param permissions - File permissions (default is File::DEFAULT_PERMISSIONS)
        \param buffer - File buffer size (default is File::DEFAULT_BUFFER)
    */
    void Open(bool read, bool write, bool truncate = false, const Flags<FileAttributes>& attributes = File::DEFAULT_ATTRIBUTES, const Flags<FilePermissions>& permissions = File::DEFAULT_PERMISSIONS, size_t buffer = File::DEFAULT_BUFFER);
    //! Open or create file
    /*!
        \param read - Read mode
        \param write - Write mode
        \param truncate - Truncate file (default is false)
        \param attributes - File attributes (default is File::DEFAULT_ATTRIBUTES)
        \param permissions - File permissions (default is File::DEFAULT_PERMISSIONS)
        \param buffer - File buffer size (default is File::DEFAULT_BUFFER)
    */
    void OpenOrCreate(bool read, bool write, bool truncate = false, const Flags<FileAttributes>& attributes = File::DEFAULT_ATTRIBUTES, const Flags<FilePermissions>& permissions = File::DEFAULT_PERMISSIONS, size_t buffer = File::DEFAULT_BUFFER);

    //! Read a bytes buffer from the opened file
    /*!
        If the file is not opened for reading the method will raise
        a filesystem exception!

        \param buffer - Buffer to read
        \param size - Buffer size
        \return Count of read bytes
    */
    size_t Read(void* buffer, size_t size) override;

    using Reader::ReadAllBytes;
    using Reader::ReadAllText;
    using Reader::ReadAllLines;

    //! Write a byte buffer into the opened file
    /*!
        If the file is not opened for writing the method will raise
        a filesystem exception!

        \param buffer - Buffer to write
        \param size - Buffer size
        \return Count of written bytes
    */
    size_t Write(const void* buffer, size_t size) override;

    using Writer::Write;

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
    void Flush() override;

    //! Close the file
    /*!
        If the file is not opened for writing the method will raise a
        filesystem exception!
    */
    void Close();

    //! Read all bytes from the given file
    /*!
        \param path - File path
        \return Bytes buffer
    */
    static std::vector<uint8_t> ReadAllBytes(const Path& path);
    //! Read all text from the given file
    /*!
        \param path - File path
        \return Text string
    */
    static std::string ReadAllText(const Path& path);
    //! Read all text lines from the given file
    /*!
        \param path - File path
        \return Text lines
    */
    static std::vector<std::string> ReadAllLines(const Path& path);

    //! Write a bytes buffer into the given file
    /*!
        \param path - File path
        \param buffer - Buffer to write
        \param size - Buffer size
        \return Count of written bytes
    */
    static size_t WriteAllBytes(const Path& path, const void* buffer, size_t size);
    //! Write a text string into the given file
    /*!
        \param path - File path
        \param text - Text string
        \return Count of written characters
    */
    static size_t WriteAllText(const Path& path, const std::string& text);
    //! Write text lines into the given file
    /*!
        \param path - File path
        \param lines - Text lines
        \return Count of written lines
    */
    static size_t WriteAllLines(const Path& path, const std::vector<std::string>& lines);

    //! Write an empty file
    /*!
        \param path - File path
    */
    static void WriteEmpty(const Path& path);

    //! Swap two instances
    void swap(File& file) noexcept;
    friend void swap(File& file1, File& file2) noexcept;

private:
    class Impl;

    Impl& impl() noexcept { return reinterpret_cast<Impl&>(_storage); }
    const Impl& impl() const noexcept { return reinterpret_cast<Impl const&>(_storage); }

    static const size_t StorageSize = 72;
    static const size_t StorageAlign = 8;
    std::aligned_storage<StorageSize, StorageAlign>::type _storage;
};

/*! \example filesystem_file.cpp Filesystem file example */

} // namespace CppCommon

#include "file.inl"

#endif // CPPCOMMON_FILESYSTEM_FILE_H
