/*!
    \file path.cpp
    \brief Filesystem path wrapper implementation
    \author Ivan Shynkarenka
    \date 11.08.2016
    \copyright MIT License
*/

#include "filesystem/path.h"

#include "filesystem/exceptions.h"
#include "system/environment.h"
#include "system/uuid.h"

#include <algorithm>
#include <memory>
#include <vector>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <userenv.h>
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <sys/stat.h>
#include <sys/time.h>
#include <limits.h>
#include <pwd.h>
#include <stdlib.h>
#include <unistd.h>
#endif

namespace CppCommon {

//! @cond
namespace Internals {

Path initial = Path::current();

std::pair<Path, size_t> root(const std::string& path)
{
    bool root_found = false;
    size_t root_length = 0;

    // Unix case 1: "/" or "/foo"
    if (((path.size() == 1) && ((path[0] == '\\') || (path[0] == '/'))) || ((path.size() > 1) && ((path[0] == '\\') || (path[0] == '/')) && ((path[1] != '\\') && (path[1] != '/'))))
    {
        root_found = true;
        root_length = 1;

        return std::make_pair(Path("/"), root_length);
    }

    // Unix case 2: "///foo"
    if ((path.size() > 2) && ((path[0] == '\\') || (path[0] == '/')) && ((path[1] == '\\') || (path[1] == '/')) && ((path[2] == '\\') || (path[2] == '/')))
    {
        root_found = true;
        root_length = 3;

        // Find root position
        while (root_length < path.size())
        {
            if ((path[root_length] != '\\') && (path[root_length] != '/'))
                break;
            ++root_length;
        }

        return std::make_pair(Path("/"), root_length);
    }

    // Windows case 1: "\\net" or "//net"
    if ((path.size() > 2) && ((path[0] == '\\') || (path[0] == '/')) && ((path[1] == '\\') || (path[1] == '/')) && ((path[2] != '\\') && (path[2] != '/') && (path[2] != '?')))
    {
        root_found = true;
        root_length = 3;

        // Find root position
        while (root_length < path.size())
        {
            if ((path[root_length] == '\\') || (path[root_length] == '/'))
            {
                ++root_length;
                break;
            }
            ++root_length;
        }

        return std::make_pair(Path(path.substr(0, root_length)), root_length);
    }

    // Windows case 2: "\\?\"
    if ((path.size() > 3) && ((path[0] == '\\') && (path[1] == '\\') && (path[2] == '?') && (path[3] == '\\')))
    {
        root_found = true;
        root_length = 4;
    }

    // Windows case 3: "C:" or "C:\"
    while (root_length < path.size())
    {
        if (path[root_length] == ':')
        {
            root_found = true;

            ++root_length;

            while (root_length < path.size())
            {
                if ((path[root_length] != '\\') && (path[root_length] != '/'))
                    break;
                ++root_length;
            }

            break;
        }

        ++root_length;
    }

    return (root_found && (root_length > 0)) ? std::make_pair(Path(path.substr(0, root_length)), root_length) : std::make_pair<Path, size_t>(Path(), 0);
}

} // namespace Internals
//! @endcond

Path Path::root() const
{
    return Internals::root(_path).first;
}

Path Path::relative() const
{
    size_t root_length = Internals::root(_path).second;
    size_t relative_length = _path.size() - root_length;
    return Path(_path.substr(root_length, relative_length));
}

Path Path::parent() const
{
    bool parent_found = false;
    size_t parent_length = _path.size();

    // Find parent path position
    while (parent_length > 0)
    {
        --parent_length;
        if ((_path[parent_length] == '\\') || (_path[parent_length] == '/'))
        {
            parent_found = true;

            // Windows case 1: "\\net" or "//net"
            if ((parent_length == 1) && ((_path[parent_length - 1] == '\\') || (_path[parent_length - 1] == '/')))
                parent_found = false;
            // Windows case 2: "\\?\"
            if ((parent_length > 0) && (_path[parent_length - 1] == '?'))
                parent_found = false;

            // Skip multiple path separators
            while (parent_length > 0)
            {
                --parent_length;
                if ((_path[parent_length] != '\\') && (_path[parent_length] != '/'))
                {
                    ++parent_length;
                    break;
                }
            }

            // Unix case 1: "/foo" -> "/", but "/" -> ""
            if ((parent_length == 0) && (_path.size() > 1))
                ++parent_length;

            break;
        }
    }

    return (parent_found && (parent_length > 0)) ? Path(_path.substr(0, parent_length)) : Path();
}

Path Path::filename() const
{
    bool filename_found = false;
    size_t filename_begin = _path.size();
    size_t filename_end = _path.size();

    // Find filename position
    while (filename_begin > 0)
    {
        --filename_begin;
        if ((_path[filename_begin] == '\\') || (_path[filename_begin] == '/') || (_path[filename_begin] == ':'))
        {
            filename_found = ((_path[filename_begin] == '\\') || (_path[filename_begin] == '/'));
            ++filename_begin;
            break;
        }
    }

    size_t filename_length = (filename_end - filename_begin);

    return (filename_length > 0) ? Path(_path.substr(filename_begin, filename_length)) : (filename_found ? Path(".") : Path());
}

Path Path::stem() const
{
    bool ext_found = false;
    size_t ext_begin = _path.size();
    size_t ext_end = _path.size();

    // Find extension position
    while (ext_begin > 0)
    {
        --ext_begin;
        if (_path[ext_begin] == '.')
        {
            ext_found = true;
            if ((ext_begin > 0) && (_path[ext_begin - 1] == '.'))
                ext_end = ext_begin;
            break;
        }
        if ((_path[ext_begin] == '\\') || (_path[ext_begin] == '/') || (_path[ext_begin] == ':'))
        {
            ++ext_begin;
            ext_end = ext_begin;
            break;
        }
    }

    size_t ext_length = ext_end - ext_begin;

    bool stem_found = false;
    size_t stem_begin = ext_begin;
    size_t stem_end = (ext_found && (ext_length > 1)) ? ext_begin : _path.size();

    // Find stem position
    while (stem_begin > 0)
    {
        --stem_begin;
        if ((_path[stem_begin] == '\\') || (_path[stem_begin] == '/') || (_path[stem_begin] == ':'))
        {
            stem_found = ((_path[stem_begin] == '\\') || (_path[stem_begin] == '/'));
            ++stem_begin;
            break;
        }
    }

    size_t stem_length = (stem_end - stem_begin);

    return (stem_length > 0) ? Path(_path.substr(stem_begin, stem_length)) : (stem_found ? Path(".") : Path());
}

Path Path::extension() const
{
    bool ext_found = false;
    size_t ext_begin = _path.size();
    size_t ext_end = _path.size();

    // Find extension position
    while (ext_begin > 0)
    {
        --ext_begin;
        if (_path[ext_begin] == '.')
        {
            ext_found = true;
            if ((ext_begin > 0) && (_path[ext_begin - 1] == '.'))
                ext_end = ext_begin;
            break;
        }
        if ((_path[ext_begin] == '\\') || (_path[ext_begin] == '/') || (_path[ext_begin] == ':'))
        {
            ++ext_begin;
            ext_end = ext_begin;
            break;
        }
    }

    size_t ext_length = ext_end - ext_begin;

    return (ext_found && (ext_length > 1)) ? Path(_path.substr(ext_begin, ext_length)) : Path();
}

FileType Path::type() const
{
#if defined(_WIN32) || defined(_WIN64)
    DWORD attributes = GetFileAttributesW(to_wstring().c_str());
    if (attributes == INVALID_FILE_ATTRIBUTES)
        return FileType::NONE;

    if (attributes & FILE_ATTRIBUTE_REPARSE_POINT)
        return FileType::SYMLINK;
    else if (attributes & FILE_ATTRIBUTE_DIRECTORY)
        return FileType::DIRECTORY;
    else
        return FileType::REGULAR;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    struct stat st;
    int result = stat(native().c_str(), &st);
    if (result != 0)
    {
        if ((errno == ENOENT) || (errno == ENOTDIR))
            return FileType::NONE;
        else
            throwex FileSystemException("Cannot get the status of the path!").Attach(*this);
    }

    if (S_ISLNK(st.st_mode))
        return FileType::SYMLINK;
    else if (S_ISDIR(st.st_mode))
        return FileType::DIRECTORY;
    else if (S_ISREG(st.st_mode))
        return FileType::REGULAR;
    else if (S_ISBLK(st.st_mode))
        return FileType::BLOCK;
    else if (S_ISCHR(st.st_mode))
        return FileType::CHARACTER;
    else if (S_ISFIFO(st.st_mode))
        return FileType::FIFO;
    else if (S_ISSOCK(st.st_mode))
        return FileType::SOCKET;
    else
        return FileType::UNKNOWN;
#endif
}

Flags<FileAttributes> Path::attributes() const
{
    Flags<FileAttributes> result;
#if defined(_WIN32) || defined(_WIN64)
    DWORD attributes = GetFileAttributesW(to_wstring().c_str());
    if (attributes == INVALID_FILE_ATTRIBUTES)
        return FileAttributes::NONE;
    if (attributes & FILE_ATTRIBUTE_NORMAL)
        result |= FileAttributes::NORMAL;
    if (attributes & FILE_ATTRIBUTE_ARCHIVE)
        result |= FileAttributes::ARCHIVED;
    if (attributes & FILE_ATTRIBUTE_HIDDEN)
        result |= FileAttributes::HIDDEN;
    if (attributes & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)
        result |= FileAttributes::INDEXED;
    if (attributes & FILE_ATTRIBUTE_OFFLINE)
        result |= FileAttributes::OFFLINE;
    if (attributes & FILE_ATTRIBUTE_READONLY)
        result |= FileAttributes::READONLY;
    if (attributes & FILE_ATTRIBUTE_SYSTEM)
        result |= FileAttributes::SYSTEM;
    if (attributes & FILE_ATTRIBUTE_TEMPORARY)
        result |= FileAttributes::TEMPORARY;
#endif
    return result;
}

Flags<FilePermissions> Path::permissions() const
{
    Flags<FilePermissions> permissions;
#if defined(unix) || defined(__unix) || defined(__unix__)
    struct stat st;
    int result = stat(native().c_str(), &st);
    if (result != 0)
    {
        if ((errno == ENOENT) || (errno == ENOTDIR))
            return FilePermissions::NONE;
        else
            throwex FileSystemException("Cannot get file permissions of the path!").Attach(*this);
    }

    if (st.st_mode & S_IRUSR)
        permissions |= FilePermissions::IRUSR;
    if (st.st_mode & S_IWUSR)
        permissions |= FilePermissions::IWUSR;
    if (st.st_mode & S_IXUSR)
        permissions |= FilePermissions::IXUSR;
    if (st.st_mode & S_IRGRP)
        permissions |= FilePermissions::IRGRP;
    if (st.st_mode & S_IWGRP)
        permissions |= FilePermissions::IWGRP;
    if (st.st_mode & S_IXGRP)
        permissions |= FilePermissions::IXGRP;
    if (st.st_mode & S_IROTH)
        permissions |= FilePermissions::IROTH;
    if (st.st_mode & S_IWOTH)
        permissions |= FilePermissions::IWOTH;
    if (st.st_mode & S_IXOTH)
        permissions |= FilePermissions::IXOTH;
    if (st.st_mode & S_ISUID)
        permissions |= FilePermissions::ISUID;
    if (st.st_mode & S_ISGID)
        permissions |= FilePermissions::ISGID;
    if (st.st_mode & S_ISVTX)
        permissions |= FilePermissions::ISVTX;
#endif
    return permissions;
}

UtcTimestamp Path::created() const
{
#if defined(_WIN32) || defined(_WIN64)
    HANDLE hFile = CreateFileW(to_wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hFile == INVALID_HANDLE_VALUE)
        throwex FileSystemException("Cannot open the path for reading!").Attach(*this);

    // Smart resource deleter pattern
    auto clear = [](HANDLE hFile) { CloseHandle(hFile); };
    auto file = std::unique_ptr<std::remove_pointer<HANDLE>::type, decltype(clear)>(hFile, clear);

    FILETIME created;
    if (!GetFileTime(file.get(), &created, nullptr, nullptr))
        throwex FileSystemException("Cannot get file created time of the path!").Attach(*this);

    ULARGE_INTEGER result;
    result.LowPart = created.dwLowDateTime;
    result.HighPart = created.dwHighDateTime;
    return UtcTimestamp(Timestamp((result.QuadPart - 116444736000000000ull) * 100));
#elif defined(unix) || defined(__unix) || defined(__unix__)
    struct stat st;
    int result = stat(native().c_str(), &st);
    if (result != 0)
        throwex FileSystemException("Cannot get the status of the path!").Attach(*this);

    return UtcTimestamp(Timestamp((st.st_mtim.tv_sec * 1000 * 1000 * 1000) + st.st_mtim.tv_nsec));
#endif
}

UtcTimestamp Path::modified() const
{
#if defined(_WIN32) || defined(_WIN64)
    HANDLE hFile = CreateFileW(to_wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hFile == INVALID_HANDLE_VALUE)
        throwex FileSystemException("Cannot open the path for reading!").Attach(*this);

    // Smart resource deleter pattern
    auto clear = [](HANDLE hFile) { CloseHandle(hFile); };
    auto file = std::unique_ptr<std::remove_pointer<HANDLE>::type, decltype(clear)>(hFile, clear);

    FILETIME write;
    if (!GetFileTime(file.get(), nullptr, nullptr, &write))
        throwex FileSystemException("Cannot get file modified time of the path!").Attach(*this);

    ULARGE_INTEGER result;
    result.LowPart = write.dwLowDateTime;
    result.HighPart = write.dwHighDateTime;
    return UtcTimestamp(Timestamp((result.QuadPart - 116444736000000000ull) * 100));
#elif defined(unix) || defined(__unix) || defined(__unix__)
    struct stat st;
    int result = stat(native().c_str(), &st);
    if (result != 0)
        throwex FileSystemException("Cannot get the status of the path!").Attach(*this);

    return UtcTimestamp(Timestamp((st.st_mtim.tv_sec * 1000 * 1000 * 1000) + st.st_mtim.tv_nsec));
#endif
}

size_t Path::hardlinks() const
{
#if defined(_WIN32) || defined(_WIN64)
    HANDLE hFile = CreateFileW(to_wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hFile == INVALID_HANDLE_VALUE)
        throwex FileSystemException("Cannot open the path for reading!").Attach(*this);

    // Smart resource deleter pattern
    auto clear = [](HANDLE hFile) { CloseHandle(hFile); };
    auto file = std::unique_ptr<std::remove_pointer<HANDLE>::type, decltype(clear)>(hFile, clear);

    BY_HANDLE_FILE_INFORMATION bhfi;
    if (!GetFileInformationByHandle(file.get(), &bhfi))
        throwex FileSystemException("Cannot get file information of the path!").Attach(*this);

    return (size_t)bhfi.nNumberOfLinks;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    struct stat st;
    int result = stat(native().c_str(), &st);
    if (result != 0)
        throwex FileSystemException("Cannot get the status of the path!").Attach(*this);

    return (size_t)st.st_nlink;
#endif
}

Path& Path::Append(const Path& path)
{
    if (_path.empty())
        _path = path._path;
    else
    {
        char last = _path[_path.size() - 1];
        if ((last == '\\') || (last == '/'))
            _path += path._path;
        else
        {
            _path += separator();
            _path += path._path;
        }
    }

    return *this;
}

Path& Path::MakePreferred()
{
#if defined(_WIN32) || defined(_WIN64)
    std::replace(_path.begin(), _path.end(), '/', '\\');
#elif defined(unix) || defined(__unix) || defined(__unix__)
    std::replace(_path.begin(), _path.end(), '\\', '/');
#endif
    return *this;
}

Path& Path::ReplaceFilename(const Path& filename)
{
    if (_path.empty())
        _path.append(filename._path);
    else
    {
        size_t index = _path.size();

        // Find filename position
        while (index > 0)
        {
            --index;
            if ((_path[index] == '\\') || (_path[index] == '/') || (_path[index] == ':'))
            {
                if (!filename.empty())
                    ++index;
                break;
            }
        }

        _path.resize(index);
        _path.append(filename._path);
    }
    return *this;
}

Path& Path::ReplaceExtension(const Path& extension)
{
    bool dot_required = (!extension._path.empty() && (extension._path[0] != '.'));

    if (_path.empty())
    {
        if (dot_required)
            _path.append(".");
        _path.append(extension._path);
    }
    else
    {
        size_t dot = _path.size();
        size_t index = _path.size();

        // Find extension position
        while (index > 0)
        {
            --index;
            if (_path[index] == '.')
            {
                if ((index > 0) && (_path[index - 1] == '.'))
                    dot = index - 1;
                else
                    dot = index;
                break;
            }
            if ((_path[index] == '\\') || (_path[index] == '/') || (_path[index] == ':'))
            {
                if (!extension.empty())
                    ++index;
                break;
            }
        }

        _path.resize(dot);
        if (dot_required)
            _path.append(".");
        _path.append(extension._path);
    }
    return *this;
}

Path& Path::RemoveTrailingSeparators()
{
    size_t index = _path.size();

    while (index > 0)
    {
        --index;
        if (((_path[index] != '\\') && (_path[index] != '/')) || ((index > 0) && (_path[index - 1] == ':')))
        {
            ++index;
            break;
        }
    }

    _path.resize(index);
    return *this;
}

Path Path::initial()
{
    return Internals::initial;
}

Path Path::current()
{
#if defined(_WIN32) || defined(_WIN64)
    std::vector<wchar_t> buffer(MAX_PATH);

    DWORD size = GetCurrentDirectoryW((DWORD)buffer.size(), buffer.data());
    if (size > buffer.size())
    {
        buffer.resize(size);
        size = GetCurrentDirectoryW((DWORD)buffer.size(), buffer.data());
    }

    if (size == 0)
        throwex FileSystemException("Cannot get the current path of the current process!");

    return Path(std::wstring(buffer.data(), size));
#elif defined(unix) || defined(__unix) || defined(__unix__)
    std::vector<char> buffer(PATH_MAX);
    char* result;

    while (((result = getcwd(buffer.data(), buffer.size())) == nullptr) && (errno == ERANGE))
        buffer.resize(buffer.size() * 2);

    if (result == nullptr)
        throwex FileSystemException("Cannot get the current path of the current process!");

    return Path(std::string(buffer.data()));
#endif
}

Path Path::executable()
{
#if defined(_WIN32) || defined(_WIN64)
    std::vector<wchar_t> buffer(MAX_PATH);
    DWORD size;

    while ((size = GetModuleFileNameW(nullptr, buffer.data(), (DWORD)buffer.size())) == buffer.size())
        buffer.resize(buffer.size() * 2);

    if (size == 0)
        throwex FileSystemException("Cannot get the executable path of the current process!");

    return Path(std::wstring(buffer.data(), size));
#elif defined(unix) || defined(__unix) || defined(__unix__)
    std::vector<char> buffer(PATH_MAX);
    ssize_t size;

    while ((size = readlink("/proc/self/exe", buffer.data(), buffer.size())) == (ssize_t)buffer.size())
        buffer.resize(buffer.size() * 2);

    if (size < 0)
        throwex FileSystemException("Cannot get the executable path of the current process!");

    return Path(std::string(buffer.data(), size));
#endif
}

Path Path::home()
{
#if defined(_WIN32) || defined(_WIN64)
    std::vector<wchar_t> buffer(MAX_PATH);

    HANDLE hToken;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_READ, &hToken))
        throwex FileSystemException("Cannot open the current process token!");

    DWORD size = (DWORD)buffer.size();
    if (!GetUserProfileDirectoryW(hToken, buffer.data(), &size))
    {
        buffer.resize(size);
        if (!GetUserProfileDirectoryW(hToken, buffer.data(), &size))
        {
            CloseHandle(hToken);
            throwex FileSystemException("Cannot get the home path of the current process!");
        }
    }

    if (!CloseHandle(hToken))
        throwex FileSystemException("Cannot close the current process token!");

    return Path(std::wstring(buffer.data(), size));
#elif defined(unix) || defined(__unix) || defined(__unix__)
    std::vector<char> buffer(PATH_MAX);
    uid_t uid = getuid();
    int result;

    struct passwd pwd;
    struct passwd* ppwd;

    while (((result = getpwuid_r(uid, &pwd, buffer.data(), buffer.size(), &ppwd)) != 0) && (result == ERANGE))
        buffer.resize(buffer.size() * 2);

    if ((result != 0) || (ppwd == nullptr))
        throwex FileSystemException("Cannot get the home path of the current process!");

    return Path(std::string(pwd.pw_dir));
#endif
}

Path Path::temp()
{
#if defined(_WIN32) || defined(_WIN64)
    std::vector<wchar_t> buffer(MAX_PATH);

    DWORD size = GetTempPathW((DWORD)buffer.size(), buffer.data());
    if (size > buffer.size())
    {
        buffer.resize(size);
        size = GetTempPathW((DWORD)buffer.size(), buffer.data());
    }

    if (size == 0)
        throwex FileSystemException("Cannot get the temporary path of the current process!");

    return Path(std::wstring(buffer.data(), size));
#elif defined(unix) || defined(__unix) || defined(__unix__)
    char* temp;
    if (((temp = getenv("TMPDIR")) != nullptr) || ((temp = getenv("TMP")) != nullptr) || ((temp = getenv("TEMP")) != nullptr) || ((temp = getenv("TEMPDIR")) != nullptr))
        return Path(std::string(temp));
    else
        return Path(std::string("/tmp"));
#endif
}

Path Path::unique()
{
    return Path(UUID::Generate().to_string());
}

Path Path::Rename(const Path& src, const Path& dst)
{
#if defined(_WIN32) || defined(_WIN64)
    if (!MoveFileW(src.to_wstring().c_str(), dst.to_wstring().c_str()))
        throwex FileSystemException("Cannot move the path!").Attach(src, dst);
#elif defined(unix) || defined(__unix) || defined(__unix__)
    int result = rename(src.native().c_str(), dst.native().c_str());
    if (result != 0)
        throwex FileSystemException("Cannot rename the path!").Attach(src, dst);
#endif
    return dst;
}

Path Path::Remove(const Path& path)
{
#if defined(_WIN32) || defined(_WIN64)
    std::wstring wpath = path.to_wstring();
    DWORD attributes = GetFileAttributesW(wpath.c_str());
    if (attributes == INVALID_FILE_ATTRIBUTES)
        throwex FileSystemException("Cannot get file attributes of the deleted path!").Attach(path);

    if (attributes & FILE_ATTRIBUTE_DIRECTORY)
    {
        if (!RemoveDirectoryW(wpath.c_str()))
            throwex FileSystemException("Cannot remove the path directory!").Attach(path);
    }
    else
    {
        if (attributes & FILE_ATTRIBUTE_READONLY)
            attributes &= ~FILE_ATTRIBUTE_READONLY;
        if (!SetFileAttributesW(wpath.c_str(), attributes))
            throwex FileSystemException("Cannot set file attributes of the deleted path!").Attach(path);
        if (!DeleteFileW(wpath.c_str()))
            throwex FileSystemException("Cannot delete the path file!").Attach(path);
    }
#elif defined(unix) || defined(__unix) || defined(__unix__)
    struct stat st;
    int result = stat(path.native().c_str(), &st);
    if (result != 0)
        throwex FileSystemException("Cannot get the status of the path!").Attach(path);

    if (S_ISDIR(st.st_mode))
    {
        int result = rmdir(path.native().c_str());
        if (result != 0)
            throwex FileSystemException("Cannot remove the path directory!").Attach(path);
    }
    else
    {
        int result = unlink(path.native().c_str());
        if (result != 0)
            throwex FileSystemException("Cannot unlink the path file!").Attach(path);
    }
#endif
    return path.parent();
}

void Path::SetAttributes(const Path& path, const Flags<FileAttributes>& attributes)
{
#if defined(_WIN32) || defined(_WIN64)
    std::wstring wpath = path.to_wstring();
    DWORD result = GetFileAttributesW(wpath.c_str());
    if (result == INVALID_FILE_ATTRIBUTES)
        throwex FileSystemException("Cannot get file attributes of the path!").Attach(path);

    if (attributes & FileAttributes::NORMAL)
        result |= FILE_ATTRIBUTE_NORMAL;
    else
        result &= ~FILE_ATTRIBUTE_NORMAL;
    if (attributes & FileAttributes::ARCHIVED)
        result |= FILE_ATTRIBUTE_ARCHIVE;
    else
        result &= ~FILE_ATTRIBUTE_ARCHIVE;
    if (attributes & FileAttributes::HIDDEN)
        result |= FILE_ATTRIBUTE_HIDDEN;
    else
        result &= ~FILE_ATTRIBUTE_HIDDEN;
    if (attributes & FileAttributes::INDEXED)
        result |= FILE_ATTRIBUTE_NOT_CONTENT_INDEXED;
    else
        result &= ~FILE_ATTRIBUTE_NOT_CONTENT_INDEXED;
    if (attributes & FileAttributes::OFFLINE)
        result |= FILE_ATTRIBUTE_OFFLINE;
    else
        result &= ~FILE_ATTRIBUTE_OFFLINE;
    if (attributes & FileAttributes::READONLY)
        result |= FILE_ATTRIBUTE_READONLY;
    else
        result &= ~FILE_ATTRIBUTE_READONLY;
    if (attributes & FileAttributes::SYSTEM)
        result |= FILE_ATTRIBUTE_SYSTEM;
    else
        result &= ~FILE_ATTRIBUTE_SYSTEM;
    if (attributes & FileAttributes::TEMPORARY)
        result |= FILE_ATTRIBUTE_TEMPORARY;
    else
        result &= ~FILE_ATTRIBUTE_TEMPORARY;

    if (!SetFileAttributesW(wpath.c_str(), result))
        throwex FileSystemException("Cannot set file attributes of the path!").Attach(path);
#endif
}

void Path::SetPermissions(const Path& path, const Flags<FilePermissions>& permissions)
{
#if defined(unix) || defined(__unix) || defined(__unix__)
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

    int result = chmod(path.native().c_str(), mode);
    if (result != 0)
        throwex FileSystemException("Cannot set file permissions of the path!").Attach(path);
#endif
}

void Path::SetCreated(const Path& path, const UtcTimestamp& timestamp)
{
#if defined(_WIN32) || defined(_WIN64)
    HANDLE hFile = CreateFileW(path.to_wstring().c_str(), GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hFile == INVALID_HANDLE_VALUE)
        throwex FileSystemException("Cannot open the path for writing!").Attach(path);

    // Smart resource deleter pattern
    auto clear = [](HANDLE hFile) { CloseHandle(hFile); };
    auto file = std::unique_ptr<std::remove_pointer<HANDLE>::type, decltype(clear)>(hFile, clear);

    ULARGE_INTEGER result;
    result.QuadPart = (timestamp.total() / 100) + 116444736000000000ull;

    FILETIME created;
    created.dwLowDateTime = result.LowPart;
    created.dwHighDateTime = result.HighPart;
    if (!SetFileTime(file.get(), &created, nullptr, nullptr))
        throwex FileSystemException("Cannot set file created time of the path!").Attach(path);
#elif defined(unix) || defined(__unix) || defined(__unix__)
    struct stat st;
    int result = stat(path.native().c_str(), &st);
    if (result != 0)
        throwex FileSystemException("Cannot get the status of the path!").Attach(path);

    struct timeval times[2];
    TIMESPEC_TO_TIMEVAL(&times[0], &st.st_atim);
    times[1].tv_sec = timestamp.seconds();
    times[1].tv_usec = timestamp.microseconds() % 1000000;

    result = utimes(path.native().c_str(), times);
    if (result != 0)
        throwex FileSystemException("Cannot set file created time of the path!").Attach(path);
#endif
}

void Path::SetModified(const Path& path, const UtcTimestamp& timestamp)
{
#if defined(_WIN32) || defined(_WIN64)
    HANDLE hFile = CreateFileW(path.to_wstring().c_str(), GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hFile == INVALID_HANDLE_VALUE)
        throwex FileSystemException("Cannot open the path for writing!").Attach(path);

    // Smart resource deleter pattern
    auto clear = [](HANDLE hFile) { CloseHandle(hFile); };
    auto file = std::unique_ptr<std::remove_pointer<HANDLE>::type, decltype(clear)>(hFile, clear);

    ULARGE_INTEGER result;
    result.QuadPart = (timestamp.total() / 100) + 116444736000000000ull;

    FILETIME write;
    write.dwLowDateTime = result.LowPart;
    write.dwHighDateTime = result.HighPart;
    if (!SetFileTime(file.get(), nullptr, nullptr, &write))
        throwex FileSystemException("Cannot set file modified time of the path!").Attach(path);
#elif defined(unix) || defined(__unix) || defined(__unix__)
    struct stat st;
    int result = stat(path.native().c_str(), &st);
    if (result != 0)
        throwex FileSystemException("Cannot get the status of the path!").Attach(path);

    struct timeval times[2];
    TIMESPEC_TO_TIMEVAL(&times[0], &st.st_atim);
    times[1].tv_sec = timestamp.seconds();
    times[1].tv_usec = timestamp.microseconds() % 1000000;

    result = utimes(path.native().c_str(), times);
    if (result != 0)
        throwex FileSystemException("Cannot set file modified time of the path!").Attach(path);
#endif
}

void Path::SetCurrent(const Path& path)
{
#if defined(_WIN32) || defined(_WIN64)
    Path temp(path / "");
    if (!SetCurrentDirectoryW(temp.to_wstring().c_str()))
        throwex FileSystemException("Cannot set the current path of the current process!").Attach(temp);
#elif defined(unix) || defined(__unix) || defined(__unix__)
    int result = chdir(path.native().c_str());
    if (result != 0)
        throwex FileSystemException("Cannot set the current path of the current process!").Attach(path);
#endif
}

} // namespace CppCommon
