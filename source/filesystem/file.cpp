/*!
    \file file.cpp
    \brief Filesystem file implementation
    \author Ivan Shynkarenka
    \date 29.08.2016
    \copyright MIT License
*/

#include "filesystem/file.h"

#include "errors/fatal.h"
#include "utility/validate_aligned_storage.h"

#include <cassert>
#include <cstring>

#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

namespace CppCommon {

//! @cond INTERNALS

class File::Impl
{
    friend class File;

public:
    explicit Impl(const Path* path) : _path(path), _read(false), _write(false), _index(0), _size(0), _buffer()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        _file = -1;
#elif defined(_WIN32) || defined(_WIN64)
        _file = INVALID_HANDLE_VALUE;
#endif
    }

    ~Impl()
    {
        try
        {
            if (IsFileOpened())
                Close();
        }
        catch (const FileSystemException& ex)
        {
            fatality(FileSystemException(ex.string()).Attach(path()));
        }
    }

    const Path& path() const { return *_path; }

    uint64_t offset() const
    {
        assert(IsFileOpened() && "File is not opened!");
        if (!IsFileOpened())
            throwex FileSystemException("File is not opened!").Attach(path());
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        off_t result = lseek(_file, 0, SEEK_CUR);
        if (result == (off_t)-1)
            throwex FileSystemException("Cannot seek the file!").Attach(path());
        return (uint64_t)result;
#elif defined(_WIN32) || defined(_WIN64)
        LARGE_INTEGER seek;
        LARGE_INTEGER result;
        seek.QuadPart = 0;
        if (!SetFilePointerEx(_file, seek, &result, FILE_CURRENT))
            throwex FileSystemException("Cannot seek the file!").Attach(path());
        return (uint64_t)result.QuadPart;
#endif
    }

    uint64_t size() const
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        if (IsFileOpened())
        {
            struct stat status;
            int result = fstat(_file, &status);
            if (result != 0)
                throwex FileSystemException("Cannot get the current file size!").Attach(path());
            return (uint64_t)status.st_size;
        }
        else
        {
            struct stat status;
            int result = stat(path().string().c_str(), &status);
            if (result != 0)
                throwex FileSystemException("Cannot get the current file size!").Attach(path());
            return (uint64_t)status.st_size;
        }
#elif defined(_WIN32) || defined(_WIN64)
        if (IsFileOpened())
        {
            LARGE_INTEGER result;
            if (!GetFileSizeEx(_file, &result))
                throwex FileSystemException("Cannot get the current file size!").Attach(path());
            return (uint64_t)result.QuadPart;
        }
        else
        {
            WIN32_FILE_ATTRIBUTE_DATA fad;
            if (!GetFileAttributesExW(path().wstring().c_str(), GetFileExInfoStandard, &fad))
                throwex FileSystemException("Cannot get the current file size!").Attach(path());

            LARGE_INTEGER result;
            result.HighPart = fad.nFileSizeHigh;
            result.LowPart = fad.nFileSizeLow;
            return (uint64_t)result.QuadPart;
        }
#endif
    }

    bool IsFileOpened() const
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        return (_file >= 0);
#elif defined(_WIN32) || defined(_WIN64)
        return (_file != INVALID_HANDLE_VALUE);
#endif
    }

    bool IsFileReadOpened() const
    {
        return _read;
    }

    bool IsFileWriteOpened() const
    {
        return _write;
    }

    void Create(bool read, bool write, const Flags<FileAttributes>& attributes = File::DEFAULT_ATTRIBUTES, const Flags<FilePermissions>& permissions = File::DEFAULT_PERMISSIONS, size_t buffer = File::DEFAULT_BUFFER)
    {
        // Close previously opened file
        assert(!IsFileOpened() && "File is already opened!");
        if (IsFileOpened())
            Close();
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        mode_t mode = 0;
        if (permissions & FilePermissions::IRUSR)
            mode |= S_IRUSR;
        if (permissions & FilePermissions::IWUSR)
            mode |= S_IWUSR;
        if (permissions & FilePermissions::IXUSR)
            mode |= S_IXUSR;
        if (permissions & FilePermissions::IRGRP)
            mode |= S_IRGRP;
        if (permissions & FilePermissions::IWGRP)
            mode |= S_IWGRP;
        if (permissions & FilePermissions::IXGRP)
            mode |= S_IXGRP;
        if (permissions & FilePermissions::IROTH)
            mode |= S_IROTH;
        if (permissions & FilePermissions::IWOTH)
            mode |= S_IWOTH;
        if (permissions & FilePermissions::IXOTH)
            mode |= S_IXOTH;
        if (permissions & FilePermissions::ISUID)
            mode |= S_ISUID;
        if (permissions & FilePermissions::ISGID)
            mode |= S_ISGID;
        if (permissions & FilePermissions::ISVTX)
            mode |= S_ISVTX;

        _file = open(path().string().c_str(), O_CREAT | O_EXCL | ((read && write) ? O_RDWR : (read ? O_RDONLY : (write ? O_WRONLY : 0))), mode);
        if (_file < 0)
            throwex FileSystemException("Cannot create a new file!").Attach(path());
#elif defined(_WIN32) || defined(_WIN64)
        DWORD dwFlagsAndAttributes = 0;
        if (attributes & FileAttributes::NORMAL)
            dwFlagsAndAttributes |= FILE_ATTRIBUTE_NORMAL;
        if (attributes & FileAttributes::ARCHIVED)
            dwFlagsAndAttributes |= FILE_ATTRIBUTE_ARCHIVE;
        if (attributes & FileAttributes::HIDDEN)
            dwFlagsAndAttributes |= FILE_ATTRIBUTE_HIDDEN;
        if (attributes & FileAttributes::INDEXED)
            dwFlagsAndAttributes |= FILE_ATTRIBUTE_NOT_CONTENT_INDEXED;
        if (attributes & FileAttributes::OFFLINE)
            dwFlagsAndAttributes |= FILE_ATTRIBUTE_OFFLINE;
        if (attributes & FileAttributes::READONLY)
            dwFlagsAndAttributes |= FILE_ATTRIBUTE_READONLY;
        if (attributes & FileAttributes::SYSTEM)
            dwFlagsAndAttributes |= FILE_ATTRIBUTE_SYSTEM;
        if (attributes & FileAttributes::TEMPORARY)
            dwFlagsAndAttributes |= FILE_ATTRIBUTE_TEMPORARY;

        _file = CreateFileW(path().wstring().c_str(), (read ? GENERIC_READ : 0) | (write ? GENERIC_WRITE : 0), FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, CREATE_NEW, dwFlagsAndAttributes, nullptr);
        if (_file == INVALID_HANDLE_VALUE)
            throwex FileSystemException("Cannot create a new file!").Attach(path());
#endif
        _read = read;
        _write = write;
        _index = 0;
        _size = 0;
        _buffer.resize(buffer);
    }

    void Open(bool read, bool write, bool truncate = false, const Flags<FileAttributes>& attributes = File::DEFAULT_ATTRIBUTES, const Flags<FilePermissions>& permissions = File::DEFAULT_PERMISSIONS, size_t buffer = File::DEFAULT_BUFFER)
    {
        // Close previously opened file
        assert(!IsFileOpened() && "File is already opened!");
        if (IsFileOpened())
            Close();
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        mode_t mode = 0;
        if (permissions & FilePermissions::IRUSR)
            mode |= S_IRUSR;
        if (permissions & FilePermissions::IWUSR)
            mode |= S_IWUSR;
        if (permissions & FilePermissions::IXUSR)
            mode |= S_IXUSR;
        if (permissions & FilePermissions::IRGRP)
            mode |= S_IRGRP;
        if (permissions & FilePermissions::IWGRP)
            mode |= S_IWGRP;
        if (permissions & FilePermissions::IXGRP)
            mode |= S_IXGRP;
        if (permissions & FilePermissions::IROTH)
            mode |= S_IROTH;
        if (permissions & FilePermissions::IWOTH)
            mode |= S_IWOTH;
        if (permissions & FilePermissions::IXOTH)
            mode |= S_IXOTH;
        if (permissions & FilePermissions::ISUID)
            mode |= S_ISUID;
        if (permissions & FilePermissions::ISGID)
            mode |= S_ISGID;
        if (permissions & FilePermissions::ISVTX)
            mode |= S_ISVTX;

        _file = open(path().string().c_str(), ((read && write) ? O_RDWR : (read ? O_RDONLY : (write ? O_WRONLY : 0))) | (truncate ? O_TRUNC : 0), mode);
        if (_file < 0)
            throwex FileSystemException("Cannot create a new file!").Attach(path());
#elif defined(_WIN32) || defined(_WIN64)
        DWORD dwFlagsAndAttributes = 0;
        if (attributes & FileAttributes::NORMAL)
            dwFlagsAndAttributes |= FILE_ATTRIBUTE_NORMAL;
        if (attributes & FileAttributes::ARCHIVED)
            dwFlagsAndAttributes |= FILE_ATTRIBUTE_ARCHIVE;
        if (attributes & FileAttributes::HIDDEN)
            dwFlagsAndAttributes |= FILE_ATTRIBUTE_HIDDEN;
        if (attributes & FileAttributes::INDEXED)
            dwFlagsAndAttributes |= FILE_ATTRIBUTE_NOT_CONTENT_INDEXED;
        if (attributes & FileAttributes::OFFLINE)
            dwFlagsAndAttributes |= FILE_ATTRIBUTE_OFFLINE;
        if (attributes & FileAttributes::READONLY)
            dwFlagsAndAttributes |= FILE_ATTRIBUTE_READONLY;
        if (attributes & FileAttributes::SYSTEM)
            dwFlagsAndAttributes |= FILE_ATTRIBUTE_SYSTEM;
        if (attributes & FileAttributes::TEMPORARY)
            dwFlagsAndAttributes |= FILE_ATTRIBUTE_TEMPORARY;

        _file = CreateFileW(path().wstring().c_str(), (read ? GENERIC_READ : 0) | (write ? GENERIC_WRITE : 0), FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, (truncate ? TRUNCATE_EXISTING : OPEN_EXISTING), dwFlagsAndAttributes, nullptr);
        if (_file == INVALID_HANDLE_VALUE)
            throwex FileSystemException("Cannot open existing file!").Attach(path());
#endif
        _read = read;
        _write = write;
        _index = 0;
        _size = 0;
        _buffer.resize(buffer);
    }

    void OpenOrCreate(bool read, bool write, bool truncate = false, const Flags<FileAttributes>& attributes = File::DEFAULT_ATTRIBUTES, const Flags<FilePermissions>& permissions = File::DEFAULT_PERMISSIONS, size_t buffer = File::DEFAULT_BUFFER)
    {
        // Close previously opened file
        assert(!IsFileOpened() && "File is already opened!");
        if (IsFileOpened())
            Close();
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        mode_t mode = 0;
        if (permissions & FilePermissions::IRUSR)
            mode |= S_IRUSR;
        if (permissions & FilePermissions::IWUSR)
            mode |= S_IWUSR;
        if (permissions & FilePermissions::IXUSR)
            mode |= S_IXUSR;
        if (permissions & FilePermissions::IRGRP)
            mode |= S_IRGRP;
        if (permissions & FilePermissions::IWGRP)
            mode |= S_IWGRP;
        if (permissions & FilePermissions::IXGRP)
            mode |= S_IXGRP;
        if (permissions & FilePermissions::IROTH)
            mode |= S_IROTH;
        if (permissions & FilePermissions::IWOTH)
            mode |= S_IWOTH;
        if (permissions & FilePermissions::IXOTH)
            mode |= S_IXOTH;
        if (permissions & FilePermissions::ISUID)
            mode |= S_ISUID;
        if (permissions & FilePermissions::ISGID)
            mode |= S_ISGID;
        if (permissions & FilePermissions::ISVTX)
            mode |= S_ISVTX;

        _file = open(path().string().c_str(), O_CREAT | ((read && write) ? O_RDWR : (read ? O_RDONLY : (write ? O_WRONLY : 0))) | (truncate ? O_TRUNC : 0), mode);
        if (_file < 0)
            throwex FileSystemException("Cannot create a new file!").Attach(path());
#elif defined(_WIN32) || defined(_WIN64)
        DWORD dwFlagsAndAttributes = 0;
        if (attributes & FileAttributes::NORMAL)
            dwFlagsAndAttributes |= FILE_ATTRIBUTE_NORMAL;
        if (attributes & FileAttributes::ARCHIVED)
            dwFlagsAndAttributes |= FILE_ATTRIBUTE_ARCHIVE;
        if (attributes & FileAttributes::HIDDEN)
            dwFlagsAndAttributes |= FILE_ATTRIBUTE_HIDDEN;
        if (attributes & FileAttributes::INDEXED)
            dwFlagsAndAttributes |= FILE_ATTRIBUTE_NOT_CONTENT_INDEXED;
        if (attributes & FileAttributes::OFFLINE)
            dwFlagsAndAttributes |= FILE_ATTRIBUTE_OFFLINE;
        if (attributes & FileAttributes::READONLY)
            dwFlagsAndAttributes |= FILE_ATTRIBUTE_READONLY;
        if (attributes & FileAttributes::SYSTEM)
            dwFlagsAndAttributes |= FILE_ATTRIBUTE_SYSTEM;
        if (attributes & FileAttributes::TEMPORARY)
            dwFlagsAndAttributes |= FILE_ATTRIBUTE_TEMPORARY;

        _file = CreateFileW(path().wstring().c_str(), (read ? GENERIC_READ : 0) | (write ? GENERIC_WRITE : 0), FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, (truncate ? CREATE_ALWAYS : OPEN_ALWAYS), dwFlagsAndAttributes, nullptr);
        if (_file == INVALID_HANDLE_VALUE)
            throwex FileSystemException("Cannot open existing file!").Attach(path());
#endif
        _read = read;
        _write = write;
        _index = 0;
        _size = 0;
        _buffer.resize(buffer);
    }

    size_t Read(void* buffer, size_t size)
    {
        if ((buffer == nullptr) || (size == 0))
            return 0;

        assert(IsFileReadOpened() && "File is not opened for reading!");
        if (!IsFileReadOpened())
            throwex FileSystemException("File is not opened for reading!").Attach(path());

        // Read file with zero buffer
        if (_buffer.empty())
        {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
            ssize_t result = read(_file, buffer, size);
            if (result < 0)
                throwex FileSystemException("Cannot read from the file!").Attach(path());
            return (size_t)result;
#elif defined(_WIN32) || defined(_WIN64)
            DWORD result;
            if (!ReadFile(_file, buffer, (DWORD)size, &result, nullptr))
                throwex FileSystemException("Cannot read from the file!").Attach(path());
            return (size_t)result;
#endif
        }

        uint8_t* bytes = (uint8_t*)buffer;
        size_t counter = 0;

        while (size > 0)
        {
            // Update the local read buffer from the file
            if (_index == _size)
            {
                _index = 0;
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
                ssize_t result = read(_file, _buffer.data(), _buffer.size());
                if (result < 0)
                    throwex FileSystemException("Cannot read from the file!").Attach(path());
                _size = (size_t)result;
#elif defined(_WIN32) || defined(_WIN64)
                DWORD result;
                if (!ReadFile(_file, _buffer.data(), (DWORD)_buffer.size(), &result, nullptr))
                    throwex FileSystemException("Cannot read from the file!").Attach(path());
                _size = (size_t)result;
#endif
                // Stop if the end of file was met
                if (_size == 0)
                    break;
            }

            // Read remaining data form the local read buffer
            size_t remain = _size - _index;
            size_t num = (size < remain) ? size : remain;
            std::memcpy(bytes, _buffer.data() + _index, num);
            counter += num;
            _index += num;
            bytes += num;
            size -= num;
        }

        return counter;
    }

    size_t Write(const void* buffer, size_t size)
    {
        if ((buffer == nullptr) || (size == 0))
            return 0;

        assert(IsFileWriteOpened() && "File is not opened for writing!");
        if (!IsFileWriteOpened())
            throwex FileSystemException("File is not opened for writing!").Attach(path());

        // Write file with zero buffer
        if (_buffer.empty())
        {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
            ssize_t result = write(_file, buffer, size);
            if (result < 0)
                throwex FileSystemException("Cannot write into the file!").Attach(path());
            return (size_t)result;
#elif defined(_WIN32) || defined(_WIN64)
            DWORD result;
            if (!WriteFile(_file, buffer, (DWORD)size, &result, nullptr))
                throwex FileSystemException("Cannot write into the file!").Attach(path());
            return (size_t)result;
#endif
        }

        const uint8_t* bytes = (const uint8_t*)buffer;
        size_t counter = 0;

        while (size > 0)
        {
            // Update the local read buffer from the file
            if (_size == _buffer.size())
            {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
                ssize_t result = write(_file, _buffer.data() + _index, (_size - _index));
                if (result < 0)
                    throwex FileSystemException("Cannot write into the file!").Attach(path());
                _index += (size_t)result;
#elif defined(_WIN32) || defined(_WIN64)
                DWORD result;
                if (!WriteFile(_file, _buffer.data() + _index, (DWORD)(_size - _index), &result, nullptr))
                    throwex FileSystemException("Cannot write into the file!").Attach(path());
                _index += (size_t)result;
#endif
                // Stop if the buffer was not written completely
                if (_index != _size)
                    break;

                // Reset the buffer cursor
                _index = 0;
                _size = 0;
            }

            // Write remaining data into the local write buffer
            size_t remain = _buffer.size() - _size;
            size_t num = (size < remain) ? size : remain;
            std::memcpy(_buffer.data() + _size, bytes, num);
            counter += num;
            _size += num;
            bytes += num;
            size -= num;
        }

        return counter;
    }

    void Seek(uint64_t offset)
    {
        assert(IsFileOpened() && "File is not opened!");
        if (!IsFileOpened())
            throwex FileSystemException("File is not opened!").Attach(path());
        // Flush write buffers
        if (IsFileWriteOpened())
            FlushBuffer();
        // Reset the buffer cursor
        _index = 0;
        _size = 0;
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        off_t result = lseek(_file, (off_t)offset, SEEK_SET);
        if (result == (off_t)-1)
            throwex FileSystemException("Cannot seek the file!").Attach(path());
#elif defined(_WIN32) || defined(_WIN64)
        LARGE_INTEGER seek;
        LARGE_INTEGER result;
        seek.QuadPart = offset;
        if (!SetFilePointerEx(_file, seek, &result, FILE_BEGIN))
            throwex FileSystemException("Cannot seek the file!").Attach(path());
#endif
    }

    void Resize(uint64_t size)
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        if (IsFileOpened())
        {
            int result = ftruncate(_file, (off_t)size);
            if (result != 0)
                throwex FileSystemException("Cannot resize the current file!").Attach(path());
        }
        else
        {
            int result = truncate(path().string().c_str(), (off_t)size);
            if (result != 0)
                throwex FileSystemException("Cannot resize the current file!").Attach(path());
        }
#elif defined(_WIN32) || defined(_WIN64)
        if (IsFileOpened())
        {
            uint64_t current = offset();
            Seek(size);
            if (!SetEndOfFile(_file))
                throwex FileSystemException("Cannot resize the current file!").Attach(path());
            Seek((current < size) ? current : size);
        }
        else
        {
            OpenOrCreate(false, true);
            Resize(size);
            Close();
        }
#endif
    }

    void FlushBuffer()
    {
        assert(IsFileWriteOpened() && "File is not opened for writing!");
        if (!IsFileWriteOpened())
            throwex FileSystemException("File is not opened for writing!").Attach(path());
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        // Force to write all buffered data
        size_t remain = _size - _index;
        if (remain > 0)
        {
            ssize_t result = write(_file, _buffer.data() + _index, (_size - _index));
            if (result < 0)
                throwex FileSystemException("Cannot write into the file during the flush operation!").Attach(path());
            _index += (size_t)result;
            if (_index != _size)
                throwex FileSystemException("Cannot write all remaining data into the file during the flush operation!").Attach(path());

            // Reset the buffer cursor
            _index = 0;
            _size = 0;
        }
#elif defined(_WIN32) || defined(_WIN64)
        // Force to write all buffered data
        size_t remain = _size - _index;
        if (remain > 0)
        {
            DWORD result;
            if (!WriteFile(_file, _buffer.data() + _index, (DWORD)(_size - _index), &result, nullptr))
                throwex FileSystemException("Cannot write into the file during the flush operation!").Attach(path());
            _index += (size_t)result;
            if (_index != _size)
                throwex FileSystemException("Cannot write all remaining data into the file during the flush operation!").Attach(path());

            // Reset the buffer cursor
            _index = 0;
            _size = 0;
        }
#endif
    }

    void Flush()
    {
        FlushBuffer();
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        int result = fsync(_file);
        if (result != 0)
            throwex FileSystemException("Cannot flush the file buffers!").Attach(path());
#elif defined(_WIN32) || defined(_WIN64)
        if (!FlushFileBuffers(_file))
            throwex FileSystemException("Cannot flush the file buffers!").Attach(path());
#endif
    }

    void Close()
    {
        assert(IsFileOpened() && "File is not opened!");
        if (!IsFileOpened())
            throwex FileSystemException("File is not opened!").Attach(path());
        // Flush the file buffer if the file is opened for writing
        if (IsFileWriteOpened())
            FlushBuffer();
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        int result = close(_file);
        if (result != 0)
            throwex FileSystemException("Cannot close the file descriptor!").Attach(path());
        _file = -1;
#elif defined(_WIN32) || defined(_WIN64)
        if (!CloseHandle(_file))
            throwex FileSystemException("Cannot close the file handle!").Attach(path());
        _file = INVALID_HANDLE_VALUE;
#endif
        _read = false;
        _write = false;
        _index = 0;
        _size = 0;
        _buffer.clear();
    }

private:
    const Path* _path;
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    int _file;
#elif defined(_WIN32) || defined(_WIN64)
    HANDLE _file;
#endif
    bool _read;
    bool _write;
    size_t _index;
    size_t _size;
    std::vector<uint8_t> _buffer;
};

//! @endcond

const Flags<FileAttributes> File::DEFAULT_ATTRIBUTES = FileAttributes::NORMAL;
const Flags<FilePermissions> File::DEFAULT_PERMISSIONS = FilePermissions::IRUSR | FilePermissions::IWUSR | FilePermissions::IRGRP | FilePermissions::IROTH;
const size_t File::DEFAULT_BUFFER = 8192;

File::File() : Path()
{
    // Check implementation storage parameters
    [[maybe_unused]] ValidateAlignedStorage<sizeof(Impl), alignof(Impl), StorageSize, StorageAlign> _;
    static_assert((StorageSize >= sizeof(Impl)), "File::StorageSize must be increased!");
    static_assert(((StorageAlign % alignof(Impl)) == 0), "File::StorageAlign must be adjusted!");

    // Create the implementation instance
    new(&_storage)Impl(this);
}

File::File(const Path& path) : Path(path)
{
    // Check implementation storage parameters
    [[maybe_unused]] ValidateAlignedStorage<sizeof(Impl), alignof(Impl), StorageSize, StorageAlign> _;
    static_assert((StorageSize >= sizeof(Impl)), "File::StorageSize must be increased!");
    static_assert(((StorageAlign % alignof(Impl)) == 0), "File::StorageAlign must be adjusted!");

    // Create the implementation instance
    new(&_storage)Impl(this);
}

File::File(const File& file) : Path(file)
{
    // Check implementation storage parameters
    [[maybe_unused]] ValidateAlignedStorage<sizeof(Impl), alignof(Impl), StorageSize, StorageAlign> _;
    static_assert((StorageSize >= sizeof(Impl)), "File::StorageSize must be increased!");
    static_assert(((StorageAlign % alignof(Impl)) == 0), "File::StorageAlign must be adjusted!");

    // Create the implementation instance
    new(&_storage)Impl(this);
}

File::File(File&& file) noexcept : File()
{
    file.swap(*this);
}

File::~File()
{
    // Delete the implementation instance
    reinterpret_cast<Impl*>(&_storage)->~Impl();
}

File& File::operator=(const File& file)
{
    File(file).swap(*this);
    return *this;
}

File& File::operator=(File&& file) noexcept
{
    File(std::move(file)).swap(*this);
    return *this;
}

uint64_t File::offset() const { return impl().offset(); }
uint64_t File::size() const { return impl().size(); }

bool File::IsFileExists() const
{
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    struct stat status;
    int result = stat(string().c_str(), &status);
    if (result != 0)
    {
        if ((errno == ENOENT) || (errno == ENOTDIR))
            return false;
        else
            throwex FileSystemException("Cannot get the status of the file!").Attach(*this);
    }

    if (S_ISDIR(status.st_mode))
        return false;
    else if (S_ISREG(status.st_mode))
        return true;
    else if (S_ISBLK(status.st_mode))
        return true;
    else if (S_ISCHR(status.st_mode))
        return true;
    else if (S_ISFIFO(status.st_mode))
        return true;
    else if (S_ISSOCK(status.st_mode))
        return true;
    else
        return true;
#elif defined(_WIN32) || defined(_WIN64)
    DWORD attributes = GetFileAttributesW(wstring().c_str());
    if (attributes == INVALID_FILE_ATTRIBUTES)
        return false;

    return (attributes & FILE_ATTRIBUTE_DIRECTORY) == 0;
#endif
}

bool File::IsFileOpened() const { return impl().IsFileOpened(); }
bool File::IsFileReadOpened() const { return impl().IsFileReadOpened(); }
bool File::IsFileWriteOpened() const { return impl().IsFileWriteOpened(); }

void File::Create(bool read, bool write, const Flags<FileAttributes>& attributes, const Flags<FilePermissions>& permissions, size_t buffer) { return impl().Create(read, write, attributes, permissions, buffer); }
void File::Open(bool read, bool write, bool truncate, const Flags<FileAttributes>& attributes, const Flags<FilePermissions>& permissions, size_t buffer) { impl().Open(read, write, truncate, attributes, permissions, buffer); }
void File::OpenOrCreate(bool read, bool write, bool truncate, const Flags<FileAttributes>& attributes, const Flags<FilePermissions>& permissions, size_t buffer) { impl().OpenOrCreate(read, write, truncate, attributes, permissions, buffer); }

size_t File::Read(void* buffer, size_t size) { return impl().Read(buffer, size); }
size_t File::Write(const void* buffer, size_t size) { return impl().Write(buffer, size); }

void File::Seek(uint64_t offset) { return impl().Seek(offset); }
void File::Resize(uint64_t size) { return impl().Resize(size); }
void File::Flush() { impl().Flush(); }
void File::Close() { impl().Close(); }

std::vector<uint8_t> File::ReadAllBytes(const Path& path)
{
    File temp(path);
    temp.Open(true, false);
    std::vector<uint8_t> result = temp.ReadAllBytes();
    temp.Close();
    return result;
}

std::string File::ReadAllText(const Path& path)
{
    File temp(path);
    temp.Open(true, false);
    std::string result = temp.ReadAllText();
    temp.Close();
    return result;
}

std::vector<std::string> File::ReadAllLines(const Path& path)
{
    File temp(path);
    temp.Open(true, false);
    std::vector<std::string> result = temp.ReadAllLines();
    temp.Close();
    return result;
}

size_t File::WriteAllBytes(const Path& path, const void* buffer, size_t size)
{
    File temp(path);
    temp.OpenOrCreate(false, true, true);
    size_t result = temp.Write(buffer, size);
    temp.Close();
    return result;
}

size_t File::WriteAllText(const Path& path, const std::string& text)
{
    File temp(path);
    temp.OpenOrCreate(false, true, true);
    size_t result = temp.Write(text);
    temp.Close();
    return result;
}

size_t File::WriteAllLines(const Path& path, const std::vector<std::string>& lines)
{
    File temp(path);
    temp.OpenOrCreate(false, true, true);
    size_t result = temp.Write(lines);
    temp.Close();
    return result;
}

void File::WriteEmpty(const Path& path)
{
    File temp(path);
    temp.OpenOrCreate(false, true, true);
    temp.Close();
}

void File::swap(File& file) noexcept
{
    using std::swap;
    Path::swap(file);
    swap(_storage, file._storage);
    swap(impl()._path, file.impl()._path);
}

} // namespace CppCommon
