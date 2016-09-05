/*!
    \file file.cpp
    \brief Filesystem file wrapper implementation
    \author Ivan Shynkarenka
    \date 29.08.2016
    \copyright MIT License
*/

#include "filesystem/file.h"

#include "errors/fatal.h"
#include "filesystem/exceptions.h"
#include "system/environment.h"

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
            if (!CloseHandle(_file))
                fatality(FileSystemException("Cannot close the file handle!").Attach(_path));
#elif defined(unix) || defined(__unix) || defined(__unix__)
            int result = close(_file);
            if (result != 0)
                fatality(FileSystemException("Cannot close the file descriptor!").Attach(_path));
#endif
        }
    }

    uint64_t offset() const
    {
        assert(IsFileOpened() && "File is not opened!");
        if (!IsFileOpened())
            throwex FileSystemException("File is not opened!").Attach(_path);
#if defined(_WIN32) || defined(_WIN64)
        LARGE_INTEGER seek;
        LARGE_INTEGER result;
        seek.QuadPart = 0;
        if (!SetFilePointerEx(_file, seek, &result, FILE_CURRENT))
            throwex FileSystemException("Cannot seek the file!").Attach(_path);
        return (uint64_t)result.QuadPart;
#elif defined(unix) || defined(__unix) || defined(__unix__)
        off_t result = lseek(_file, 0, SEEK_CUR);
        if (result == (off_t)-1)
            throwex FileSystemException("Cannot seek the file!").Attach(_path);
        return (uint64_t)result;
#endif
    }

    uint64_t size() const
    {
#if defined(_WIN32) || defined(_WIN64)
        if (IsFileOpened())
        {
            LARGE_INTEGER result;
            if (!GetFileSizeEx(_file, &result))
                throwex FileSystemException("Cannot get the current file size!").Attach(_path);
            return (uint64_t)result.QuadPart;
        }
        else
        {
            WIN32_FILE_ATTRIBUTE_DATA fad;
            if (!GetFileAttributesExW(_path.wstring().c_str(), GetFileExInfoStandard, &fad))
                throwex FileSystemException("Cannot get the current file size!").Attach(_path);

            LARGE_INTEGER result;
            result.HighPart = fad.nFileSizeHigh;
            result.LowPart = fad.nFileSizeLow;
            return (uint64_t)result.QuadPart;
        }
#elif defined(unix) || defined(__unix) || defined(__unix__)
        if (IsFileOpened())
        {
            struct stat st;
            int result = fstat(_file, &st);
            if (result != 0)
                throwex FileSystemException("Cannot get the current file size!").Attach(_path);
            return (uint64_t)st.st_size;
        }
        else
        {
            struct stat st;
            int result = stat(_path.native().c_str(), &st);
            if (result != 0)
                throwex FileSystemException("Cannot get the current file size!").Attach(_path);
            return (uint64_t)st.st_size;
        }
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

    void Create(bool read, bool write, bool truncate = false, const Flags<FileAttributes>& attributes = File::DEFAULT_ATTRIBUTES, const Flags<FilePermissions>& permissions = File::DEFAULT_PERMISSIONS)
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

        _file = CreateFileW(_path.wstring().c_str(), (read ? GENERIC_READ : 0) | (write ? GENERIC_WRITE : 0), FILE_SHARE_READ, nullptr, CREATE_NEW, dwFlagsAndAttributes, nullptr);
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

        _file = open(_path.native().c_str(), O_CREAT | O_EXCL | ((read && write) ? O_RDWR : (read ? O_RDONLY : (write ? O_WRONLY : 0))) | (truncate ? O_TRUNC : 0), mode);
        if (_file < 0)
            throwex FileSystemException("Cannot create a new file!").Attach(_path);
#endif
    }

    void Open(bool read, bool write, bool truncate = false, const Flags<FileAttributes>& attributes = File::DEFAULT_ATTRIBUTES, const Flags<FilePermissions>& permissions = File::DEFAULT_PERMISSIONS)
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

        _file = CreateFileW(_path.wstring().c_str(), (read ? GENERIC_READ : 0) | (write ? GENERIC_WRITE : 0), FILE_SHARE_READ, nullptr, (truncate ? TRUNCATE_EXISTING : OPEN_EXISTING), dwFlagsAndAttributes, nullptr);
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

        _file = open(_path.native().c_str(), ((read && write) ? O_RDWR : (read ? O_RDONLY : (write ? O_WRONLY : 0))) | (truncate ? O_TRUNC : 0), mode);
        if (_file < 0)
            throwex FileSystemException("Cannot create a new file!").Attach(_path);
#endif
    }

    void OpenOrCreate(bool read, bool write, bool truncate = false, const Flags<FileAttributes>& attributes = File::DEFAULT_ATTRIBUTES, const Flags<FilePermissions>& permissions = File::DEFAULT_PERMISSIONS)
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

        _file = CreateFileW(_path.wstring().c_str(), (read ? GENERIC_READ : 0) | (write ? GENERIC_WRITE : 0), FILE_SHARE_READ, nullptr, (truncate ? CREATE_ALWAYS : OPEN_ALWAYS), dwFlagsAndAttributes, nullptr);
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
        LARGE_INTEGER seek;
        LARGE_INTEGER result;
        seek.QuadPart = offset;
        if (!SetFilePointerEx(_file, seek, &result, FILE_BEGIN))
            throwex FileSystemException("Cannot seek the file!").Attach(_path);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        off_t result = lseek(_file, (off_t)offset, SEEK_SET);
        if (result == (off_t)-1)
            throwex FileSystemException("Cannot seek the file!").Attach(_path);
#endif
    }

    void Resize(uint64_t size)
    {
#if defined(_WIN32) || defined(_WIN64)
        if (IsFileOpened())
        {
            uint64_t current = offset();
            Seek(size);
            if (!SetEndOfFile(_file))
                throwex FileSystemException("Cannot resize the current file!").Attach(_path);
            Seek((current < size) ? current : size);
        }
        else
        {
            OpenOrCreate(false, true);
            Resize(size);
            Close();
        }
#elif defined(unix) || defined(__unix) || defined(__unix__)
        if (IsFileOpened())
        {
            int result = ftruncate(_file, (off_t)size);
            if (result != 0)
                throwex FileSystemException("Cannot resize the current file!").Attach(_path);
        }
        else
        {
            int result = truncate(_path.native().c_str(), (off_t)size);
            if (result != 0)
                throwex FileSystemException("Cannot resize the current file!").Attach(_path);
        }
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

File::File(const Path& path) : Path(path), _pimpl(std::make_unique<Impl>(*this))
{
}

File::File(const File& instance) : File((const Path&)instance)
{
}

File::~File()
{
}

uint64_t File::offset() const
{
    return _pimpl->offset();
}

uint64_t File::size() const
{
    return _pimpl->size();
}

bool File::IsFileExists() const
{
#if defined(_WIN32) || defined(_WIN64)
    DWORD attributes = GetFileAttributesW(wstring().c_str());
    if (attributes == INVALID_FILE_ATTRIBUTES)
        return false;

    if (attributes & FILE_ATTRIBUTE_DIRECTORY)
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

    if (S_ISDIR(st.st_mode))
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

void File::Resize(uint64_t size)
{
    return _pimpl->Resize(size);
}

void File::Flush()
{
    _pimpl->Flush();
}

void File::Close()
{
    _pimpl->Close();
}

std::vector<uint8_t> File::ReadAllBytes(const Path& path)
{
    File temp(path);
    temp.Open(true, false);
    std::vector<uint8_t> result(temp.size());
    result.resize(temp.Read(result.data(), result.size()));
    temp.Close();
    return result;
}

std::vector<std::string> File::ReadAllLines(const Path& path)
{
    std::string temp;
    std::vector<std::string> result;
    std::vector<uint8_t> bytes = ReadAllBytes(path);

    for (auto ch : bytes)
    {
        if ((ch == '\r') || (ch == '\n'))
        {
            if (!temp.empty())
            {
                result.push_back(temp);
                temp.clear();
            }
        }
        else
            temp += ch;
    }

    return result;
}

std::string File::ReadAllText(const Path& path)
{
    std::vector<uint8_t> bytes = ReadAllBytes(path);
    return std::string(bytes.begin(), bytes.end());
}

size_t File::WriteAllBytes(const Path& path, const uint8_t* buffer, size_t size)
{
    File temp(path);
    temp.OpenOrCreate(false, true, true);
    size_t result = temp.Write(buffer, size);
    temp.Close();
    return result;
}

size_t File::WriteAllLines(const Path& path, const std::vector<std::string>& lines)
{
    static std::string endline = Environment::EndLine();

    File temp(path);
    temp.OpenOrCreate(false, true, true);
    size_t result = 0;
    for (auto& line : lines)
    {
        if (temp.Write((const uint8_t*)line.data(), line.size()) != line.size())
            break;
        if (temp.Write((const uint8_t*)endline.data(), endline.size()) != endline.size())
            break;
        ++result;
    }
    temp.Close();
    return result;
}

size_t File::WriteAllText(const Path& path, const std::string& text)
{
    return WriteAllBytes(path, (const uint8_t*)text.data(), text.size());
}

} // namespace CppCommon
