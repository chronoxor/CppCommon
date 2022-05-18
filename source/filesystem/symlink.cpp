/*!
    \file symlink.cpp
    \brief Filesystem symlink implementation
    \author Ivan Shynkarenka
    \date 30.08.2016
    \copyright MIT License
*/

#include "filesystem/symlink.h"

#include "utility/resource.h"

#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <unistd.h>
#include <vector>
#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <winioctl.h>
//  REPARSE_DATA_BUFFER related definitions are found in ntifs.h, which is part of the
//  Windows Device Driver Kit. Since that's inconvenient, the definitions are provided
//  here. See http://msdn.microsoft.com/en-us/library/ms791514.aspx
#if !defined(REPARSE_DATA_BUFFER_HEADER_SIZE)
#define SYMLINK_FLAG_RELATIVE 1
typedef struct _REPARSE_DATA_BUFFER
{
    ULONG ReparseTag;
    USHORT ReparseDataLength;
    USHORT Reserved;
    union
    {
        struct
        {
            USHORT SubstituteNameOffset;
            USHORT SubstituteNameLength;
            USHORT PrintNameOffset;
            USHORT PrintNameLength;
            ULONG Flags;
            WCHAR PathBuffer[1];
        } SymbolicLinkReparseBuffer;
        struct
        {
            USHORT SubstituteNameOffset;
            USHORT SubstituteNameLength;
            USHORT PrintNameOffset;
            USHORT PrintNameLength;
            WCHAR PathBuffer[1];
        } MountPointReparseBuffer;
        struct
        {
            UCHAR DataBuffer[1];
        } GenericReparseBuffer;
    };
} REPARSE_DATA_BUFFER, *PREPARSE_DATA_BUFFER;
#define REPARSE_DATA_BUFFER_HEADER_SIZE FIELD_OFFSET(REPARSE_DATA_BUFFER, GenericReparseBuffer)
#endif
#ifndef MAXIMUM_REPARSE_DATA_BUFFER_SIZE
#define MAXIMUM_REPARSE_DATA_BUFFER_SIZE (16 * 1024)
#endif
#endif

namespace CppCommon {

Path Symlink::target() const
{
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    std::vector<char> buffer(PATH_MAX);
    ssize_t size;

    while ((size = readlink(string().c_str(), buffer.data(), buffer.size())) == (ssize_t)buffer.size())
        buffer.resize(buffer.size() * 2);

    if (size < 0)
        throwex FileSystemException("Cannot read symlink target of the path!").Attach(*this);

    // Return symbolic link target path
    return Path(std::string(buffer.data(), size));
#elif defined(_WIN32) || defined(_WIN64)
    HANDLE hSymlink = CreateFileW(wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT, nullptr);
    if (hSymlink == INVALID_HANDLE_VALUE)
        throwex FileSystemException("Cannot open a symlink path for reading!").Attach(*this);

    // Smart resource cleaner pattern
    auto file = resource(hSymlink, [](HANDLE hObject) { CloseHandle(hObject); });

    union info_t
    {
        char buffer[REPARSE_DATA_BUFFER_HEADER_SIZE + MAXIMUM_REPARSE_DATA_BUFFER_SIZE];
        REPARSE_DATA_BUFFER rdb;
    };

    DWORD size;
    info_t info;
    if (!DeviceIoControl(file.get(), FSCTL_GET_REPARSE_POINT, nullptr, 0, info.buffer, sizeof(info), &size, nullptr))
        throwex FileSystemException("Cannot read symlink target of the path!").Attach(*this);

    // Release the resource manually
    if (!CloseHandle(hSymlink))
        throwex FileSystemException("Cannot close a symlink!").Attach(*this);
    file.release();

    // Get symbolic link target path
    std::wstring result((wchar_t*)info.rdb.SymbolicLinkReparseBuffer.PathBuffer +
                                 (info.rdb.SymbolicLinkReparseBuffer.PrintNameOffset / sizeof(WCHAR)),
                        (wchar_t*)info.rdb.SymbolicLinkReparseBuffer.PathBuffer +
                                 (info.rdb.SymbolicLinkReparseBuffer.PrintNameOffset / sizeof(WCHAR)) +
                                 (info.rdb.SymbolicLinkReparseBuffer.PrintNameLength / sizeof(WCHAR)));

    // Return symbolic link target path
    return Path(Encoding::ToUTF8(result));
#endif
}

bool Symlink::IsSymlinkExists() const
{
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    struct stat lstatus;
    int result = lstat(string().c_str(), &lstatus);
    if (result != 0)
    {
        if ((errno == ENOENT) || (errno == ENOTDIR))
            return false;
        else
            throwex FileSystemException("Cannot get the status of the symbolic link!").Attach(*this);
    }

    if (S_ISLNK(lstatus.st_mode))
        return true;
    else
        return false;
#elif defined(_WIN32) || defined(_WIN64)
    DWORD attributes = GetFileAttributesW(wstring().c_str());
    if (attributes == INVALID_FILE_ATTRIBUTES)
        return false;

    return (attributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0;
#endif
}

Symlink Symlink::CreateSymlink(const Path& src, const Path& dst)
{
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    int result = symlink(src.string().c_str(), dst.string().c_str());
    if (result != 0)
        throwex FileSystemException("Cannot create symbolic link!").Attach(src, dst);
#elif defined(_WIN32) || defined(_WIN64)
    std::wstring source = src.wstring();
    DWORD attributes = GetFileAttributesW(source.c_str());
    if (attributes == INVALID_FILE_ATTRIBUTES)
        throwex FileSystemException("Cannot get file attributes of the source path!").Attach(src);

    if (attributes & FILE_ATTRIBUTE_DIRECTORY)
    {
        if (!CreateSymbolicLinkW(dst.wstring().c_str(), source.c_str(), SYMBOLIC_LINK_FLAG_DIRECTORY))
            throwex FileSystemException("Cannot create symbolic link for the directory!").Attach(src, dst);
    }
    else
    {
        if (!CreateSymbolicLinkW(dst.wstring().c_str(), source.c_str(), 0))
            throwex FileSystemException("Cannot create symbolic link for the file!").Attach(src, dst);
    }
#endif
    return dst;
}

Path Symlink::CreateHardlink(const Path& src, const Path& dst)
{
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    int result = link(src.string().c_str(), dst.string().c_str());
    if (result != 0)
        throwex FileSystemException("Cannot create hard link!").Attach(src, dst);
#elif defined(_WIN32) || defined(_WIN64)
    if (!CreateHardLinkW(dst.wstring().c_str(), src.wstring().c_str(), nullptr))
        throwex FileSystemException("Cannot create hard link!").Attach(src, dst);
#endif
    return dst;
}

} // namespace CppCommon
