/*!
    \file file.cpp
    \brief Filesystem file wrapper implementation
    \author Ivan Shynkarenka
    \date 29.08.2016
    \copyright MIT License
*/

#include "filesystem/file.h"

#include "filesystem/exceptions.h"

#include <cassert>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

namespace CppCommon {

class File::Impl
{
public:
    Impl(Path& path) : _path(path)
    {
#if defined(_WIN32) || defined(_WIN64)
        _file = INVALID_HANDLE_VALUE;
#elif defined(unix) || defined(__unix) || defined(__unix__)
        _file = -1;
#endif
    }

    ~Impl()
    {
        if (IsFileOpened())
        {
#if defined(_WIN32) || defined(_WIN64)
            CloseHandle(_file);
            _file = INVALID_HANDLE_VALUE;
#elif defined(unix) || defined(__unix) || defined(__unix__)
            close(_file);
            _file = -1;
#endif
        }
    }

    uint64_t offset() const
    {
        assert(IsFileOpened() && "File is not opened!");
        if (!IsFileOpened())
            throwex FileSystemException("File is not opened!").Attach(_path);
#if defined(_WIN32) || defined(_WIN64)
        LARGE_INTEGER liOffset;
        LARGE_INTEGER liResult;
        liOffset.QuadPart = 0;
        if (!SetFilePointerEx(_file, liOffset, &liResult, FILE_CURRENT))
            throwex FileSystemException("Cannot seek the file!").Attach(_path);
        return (uint64_t)liResult.QuadPart;
#elif defined(unix) || defined(__unix) || defined(__unix__)
        off_t result = lseek64(_file, 0, SEEK_CUR);
        if (result == (off_t)-1)
            throwex FileSystemException("Cannot seek the file!").Attach(_path);
        return (uint64_t)result;
#endif
    }

    bool IsFileOpened() const
    {
#if defined(_WIN32) || defined(_WIN64)
        return (_file != INVALID_HANDLE_VALUE);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        return (_file >= 0);
#endif
    }

    void Create(bool read, bool write, bool truncate, const Flags<FileAttributes>& attributes, const Flags<FilePermissions>& permissions)
    {
        // Close previously opened file
        assert(!IsFileOpened() && "File is already opened!");
        if (IsFileOpened())
            Close();
#if defined(_WIN32) || defined(_WIN64)
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
        _file = CreateFileW(_path.to_wstring().c_str(), (read ? GENERIC_READ : 0) | (write ? GENERIC_WRITE : 0), 0, nullptr, CREATE_NEW | (truncate ? TRUNCATE_EXISTING : 0), dwFlagsAndAttributes, nullptr);
        if (_file == INVALID_HANDLE_VALUE)
            throwex FileSystemException("Cannot create a new file!").Attach(_path);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        mode_t mode = 0;
        if (permissions & FilePermissions::IRUSR)
            mode |= S_IRUSR;
        if (permissions & FilePermissions::IWUSR)
            mode |= S_IWUSR;
        if (permissions & FilePermissions::IXUSR)
            mode |= S_IXUSR;
        if (permissions & FilePermissions::IRWXU)
            mode |= S_IRWXU;
        if (permissions & FilePermissions::IRGRP)
            mode |= S_IRGRP;
        if (permissions & FilePermissions::IWGRP)
            mode |= S_IWGRP;
        if (permissions & FilePermissions::IXGRP)
            mode |= S_IXGRP;
        if (permissions & FilePermissions::IRWXG)
            mode |= S_IRWXG;
        if (permissions & FilePermissions::IROTH)
            mode |= S_IROTH;
        if (permissions & FilePermissions::IWOTH)
            mode |= S_IWOTH;
        if (permissions & FilePermissions::IXOTH)
            mode |= S_IXOTH;
        if (permissions & FilePermissions::IRWXO)
            mode |= S_IRWXO;
        if (permissions & FilePermissions::ISUID)
            mode |= S_ISUID;
        if (permissions & FilePermissions::ISGID)
            mode |= S_ISGID;
        if (permissions & FilePermissions::ISVTX)
            mode |= S_ISVTX;
        _file = open(_path.native().c_str(), O_CREAT | O_EXCL | ((read && write) ? O_RDWR : (read ? O_RDONLY : (write ? O_WRONLY : 0))) | (truncate ? O_TRUNC : 0), mode);
        if (_file < 0)
            throwex FileSystemException("Cannot create a new file!").Attach(_path);
#endif
    }

    void Open(bool read, bool write, bool truncate, const Flags<FileAttributes>& attributes, const Flags<FilePermissions>& permissions)
    {
        // Close previously opened file
        assert(!IsFileOpened() && "File is already opened!");
        if (IsFileOpened())
            Close();
#if defined(_WIN32) || defined(_WIN64)
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
        _file = CreateFileW(_path.to_wstring().c_str(), (read ? GENERIC_READ : 0) | (write ? GENERIC_WRITE : 0), 0, nullptr, OPEN_EXISTING | (truncate ? TRUNCATE_EXISTING : 0), dwFlagsAndAttributes, nullptr);
        if (_file == INVALID_HANDLE_VALUE)
            throwex FileSystemException("Cannot open existing file!").Attach(_path);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        mode_t mode = 0;
        if (permissions & FilePermissions::IRUSR)
            mode |= S_IRUSR;
        if (permissions & FilePermissions::IWUSR)
            mode |= S_IWUSR;
        if (permissions & FilePermissions::IXUSR)
            mode |= S_IXUSR;
        if (permissions & FilePermissions::IRWXU)
            mode |= S_IRWXU;
        if (permissions & FilePermissions::IRGRP)
            mode |= S_IRGRP;
        if (permissions & FilePermissions::IWGRP)
            mode |= S_IWGRP;
        if (permissions & FilePermissions::IXGRP)
            mode |= S_IXGRP;
        if (permissions & FilePermissions::IRWXG)
            mode |= S_IRWXG;
        if (permissions & FilePermissions::IROTH)
            mode |= S_IROTH;
        if (permissions & FilePermissions::IWOTH)
            mode |= S_IWOTH;
        if (permissions & FilePermissions::IXOTH)
            mode |= S_IXOTH;
        if (permissions & FilePermissions::IRWXO)
            mode |= S_IRWXO;
        if (permissions & FilePermissions::ISUID)
            mode |= S_ISUID;
        if (permissions & FilePermissions::ISGID)
            mode |= S_ISGID;
        if (permissions & FilePermissions::ISVTX)
            mode |= S_ISVTX;
        _file = open(_path.native().c_str(), ((read && write) ? O_RDWR : (read ? O_RDONLY : (write ? O_WRONLY : 0))) | (truncate ? O_TRUNC : 0), mode);
        if (_file < 0)
            throwex FileSystemException("Cannot create a new file!").Attach(_path);
#endif
    }

    void OpenOrCreate(bool read, bool write, bool truncate, const Flags<FileAttributes>& attributes, const Flags<FilePermissions>& permissions)
    {
        // Close previously opened file
        assert(!IsFileOpened() && "File is already opened!");
        if (IsFileOpened())
            Close();
#if defined(_WIN32) || defined(_WIN64)
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
        _file = CreateFileW(_path.to_wstring().c_str(), (read ? GENERIC_READ : 0) | (write ? GENERIC_WRITE : 0), 0, nullptr, OPEN_ALWAYS | (truncate ? TRUNCATE_EXISTING : 0), dwFlagsAndAttributes, nullptr);
        if (_file == INVALID_HANDLE_VALUE)
            throwex FileSystemException("Cannot open existing file!").Attach(_path);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        mode_t mode = 0;
        if (permissions & FilePermissions::IRUSR)
            mode |= S_IRUSR;
        if (permissions & FilePermissions::IWUSR)
            mode |= S_IWUSR;
        if (permissions & FilePermissions::IXUSR)
            mode |= S_IXUSR;
        if (permissions & FilePermissions::IRWXU)
            mode |= S_IRWXU;
        if (permissions & FilePermissions::IRGRP)
            mode |= S_IRGRP;
        if (permissions & FilePermissions::IWGRP)
            mode |= S_IWGRP;
        if (permissions & FilePermissions::IXGRP)
            mode |= S_IXGRP;
        if (permissions & FilePermissions::IRWXG)
            mode |= S_IRWXG;
        if (permissions & FilePermissions::IROTH)
            mode |= S_IROTH;
        if (permissions & FilePermissions::IWOTH)
            mode |= S_IWOTH;
        if (permissions & FilePermissions::IXOTH)
            mode |= S_IXOTH;
        if (permissions & FilePermissions::IRWXO)
            mode |= S_IRWXO;
        if (permissions & FilePermissions::ISUID)
            mode |= S_ISUID;
        if (permissions & FilePermissions::ISGID)
            mode |= S_ISGID;
        if (permissions & FilePermissions::ISVTX)
            mode |= S_ISVTX;
        _file = open(_path.native().c_str(), O_CREAT | ((read && write) ? O_RDWR : (read ? O_RDONLY : (write ? O_WRONLY : 0))) | (truncate ? O_TRUNC : 0), mode);
        if (_file < 0)
            throwex FileSystemException("Cannot create a new file!").Attach(_path);
#endif
    }

    size_t Read(uint8_t* buffer, size_t size)
    {
        assert(IsFileOpened() && "File is not opened!");
        if (!IsFileOpened())
            throwex FileSystemException("File is not opened!").Attach(_path);
#if defined(_WIN32) || defined(_WIN64)
        DWORD result;
        if (!ReadFile(_file, buffer, (DWORD)size, &result, nullptr))
            throwex FileSystemException("Cannot read from the file!").Attach(_path);
        return (size_t)result;
#elif defined(unix) || defined(__unix) || defined(__unix__)
        ssize_t result = read(_file, buffer, size);
        if (result < 0)
            throwex FileSystemException("Cannot read from the file!").Attach(_path);
        return (size_t)result;
#endif
    }

    size_t Write(const uint8_t* buffer, size_t size)
    {
        assert(IsFileOpened() && "File is not opened!");
        if (!IsFileOpened())
            throwex FileSystemException("File is not opened!").Attach(_path);
#if defined(_WIN32) || defined(_WIN64)
        DWORD result;
        if (!WriteFile(_file, buffer, (DWORD)size, &result, nullptr))
            throwex FileSystemException("Cannot write into the file!").Attach(_path);
        return (size_t)result;
#elif defined(unix) || defined(__unix) || defined(__unix__)
        ssize_t result = write(_file, buffer, size);
        if (result < 0)
            throwex FileSystemException("Cannot write into the file!").Attach(_path);
        return (size_t)result;
#endif
    }

    void Seek(uint64_t offset)
    {
        assert(IsFileOpened() && "File is not opened!");
        if (!IsFileOpened())
            throwex FileSystemException("File is not opened!").Attach(_path);
#if defined(_WIN32) || defined(_WIN64)
        LARGE_INTEGER liOffset;
        LARGE_INTEGER liResult;
        liOffset.QuadPart = offset;
        if (!SetFilePointerEx(_file, liOffset, &liResult, FILE_BEGIN))
            throwex FileSystemException("Cannot seek the file!").Attach(_path);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        off_t result = lseek64(_file, offset, SEEK_SET);
        if (result == (off_t)-1)
            throwex FileSystemException("Cannot seek the file!").Attach(_path);
#endif
    }

    void Flush()
    {
        assert(IsFileOpened() && "File is not opened!");
        if (!IsFileOpened())
            throwex FileSystemException("File is not opened!").Attach(_path);
#if defined(_WIN32) || defined(_WIN64)
        if (!FlushFileBuffers(_file))
            throwex FileSystemException("Cannot flush the file buffers!").Attach(_path);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = fsync(_file);
        if (result != 0)
            throwex FileSystemException("Cannot flush the file buffers!").Attach(_path);
#endif
    }

    void Close()
    {
        assert(IsFileOpened() && "File is not opened!");
        if (!IsFileOpened())
            throwex FileSystemException("File is not opened!").Attach(_path);
#if defined(_WIN32) || defined(_WIN64)
        if (!CloseHandle(_file))
            throwex FileSystemException("Cannot close the file handle!").Attach(_path);
        _file = INVALID_HANDLE_VALUE;
#elif defined(unix) || defined(__unix) || defined(__unix__)
        int result = close(_file);
        if (result != 0)
            throwex FileSystemException("Cannot close the file descriptor!").Attach(_path);
        _file = -1;
#endif
    }

private:
    Path& _path;
#if defined(_WIN32) || defined(_WIN64)
    HANDLE _file;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    int _file;
#endif
};

const Flags<FileAttributes> File::DEFAULT_ATTRIBUTES = FileAttributes::NORMAL;
const Flags<FilePermissions> File::DEFAULT_PERMISSIONS = FilePermissions::IRUSR | FilePermissions::IWUSR | FilePermissions::IRGRP | FilePermissions::IROTH;

File::File() : Path(), _pimpl(new Impl(*this))
{
}

File::File(const std::string& path) : Path(path), _pimpl(new Impl(*this))
{
}

File::File(const std::wstring& path) : Path(path), _pimpl(new Impl(*this))
{
}

File::File(const Path& path) : Path(path), _pimpl(new Impl(*this))
{
}

File::File(const File& instance) : Path(instance), _pimpl(new Impl(*this))
{
}

File::~File()
{
}

uint64_t File::offset() const
{
    return _pimpl->offset();
}

bool File::IsFileExists() const
{
#if defined(_WIN32) || defined(_WIN64)
    DWORD attributes = GetFileAttributesW(to_wstring().c_str());
    if (attributes == INVALID_FILE_ATTRIBUTES)
        return false;
    else if (attributes & FILE_ATTRIBUTE_DIRECTORY)
        return false;
    else
        return true;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    struct stat st;
    int result = stat(native().c_str(), &st);
    if (result != 0)
    {
        if ((errno == ENOENT) || (errno == ENOTDIR))
            return false;
        else
            throwex FileSystemException("Cannot get the status of the file!").Attach(*this);
    }

    else if (S_ISDIR(st.st_mode))
        return false;
    else if (S_ISREG(st.st_mode))
        return true;
    else if (S_ISBLK(st.st_mode))
        return true;
    else if (S_ISCHR(st.st_mode))
        return true;
    else if (S_ISFIFO(st.st_mode))
        return true;
    else if (S_ISSOCK(st.st_mode))
        return true;
    else
        return true;
#endif
}

bool File::IsFileOpened() const
{
    return _pimpl->IsFileOpened();
}

void File::Create(bool read, bool write, bool truncate, const Flags<FileAttributes>& attributes, const Flags<FilePermissions>& permissions)
{
    return _pimpl->Create(read, write, truncate, attributes, permissions);
}

void File::Open(bool read, bool write, bool truncate, const Flags<FileAttributes>& attributes, const Flags<FilePermissions>& permissions)
{
    _pimpl->Open(read, write, truncate, attributes, permissions);
}

void File::OpenOrCreate(bool read, bool write, bool truncate, const Flags<FileAttributes>& attributes, const Flags<FilePermissions>& permissions)
{
    _pimpl->OpenOrCreate(read, write, truncate, attributes, permissions);
}

size_t File::Read(uint8_t* buffer, size_t size)
{
    return _pimpl->Read(buffer, size);
}

size_t File::Write(const uint8_t* buffer, size_t size)
{
    return _pimpl->Write(buffer, size);
}

void File::Seek(uint64_t offset)
{
    return _pimpl->Seek(offset);
}

void File::Flush()
{
    _pimpl->Flush();
}

void File::Close()
{
    _pimpl->Close();
}

} // namespace CppCommon
