/*!
    \file symlink.cpp
    \brief Filesystem symlink wrapper implementation
    \author Ivan Shynkarenka
    \date 30.08.2016
    \copyright MIT License
*/

#include "filesystem/symlink.h"

#include "filesystem/exceptions.h"

#include <memory>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
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
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

namespace CppCommon {

Path Symlink::target() const
{
#if defined(_WIN32) || defined(_WIN64)
    HANDLE hFile = CreateFileW(to_wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT, nullptr);
    if (hFile == INVALID_HANDLE_VALUE)
        throwex FileSystemException("Cannot open the symlink path for reading!").Attach(*this);

    // Smart resource deleter pattern
    auto clear = [](HANDLE hFile) { CloseHandle(hFile); };
    auto file = std::unique_ptr<std::remove_pointer<HANDLE>::type, decltype(clear)>(hFile, clear);

    union info_t
    {
        char buffer[REPARSE_DATA_BUFFER_HEADER_SIZE + MAXIMUM_REPARSE_DATA_BUFFER_SIZE];
        REPARSE_DATA_BUFFER rdb;
    };

    DWORD size;
    info_t info;
    if (!DeviceIoControl(file.get(), FSCTL_GET_REPARSE_POINT, 0, 0, info.buffer, sizeof(info), &size, 0))
        throwex FileSystemException("Cannot read symlink target of the path!").Attach(*this);

    return Path(std::string((wchar_t*)info.rdb.SymbolicLinkReparseBuffer.PathBuffer +
                                     (info.rdb.SymbolicLinkReparseBuffer.PrintNameOffset / sizeof(WCHAR)),
                            (wchar_t*)info.rdb.SymbolicLinkReparseBuffer.PathBuffer +
                                     (info.rdb.SymbolicLinkReparseBuffer.PrintNameOffset / sizeof(WCHAR)) +
                                     (info.rdb.SymbolicLinkReparseBuffer.PrintNameLength / sizeof(WCHAR))));
#elif defined(unix) || defined(__unix) || defined(__unix__)
    std::vector<char> buffer(PATH_MAX);
    ssize_t size;

    while ((size = readlink(native.c_str(), buffer.data(), buffer.size())) == (ssize_t)buffer.size())
        buffer.resize(buffer.size() * 2);

    if (size < 0)
        throwex FileSystemException("Cannot read symlink target of the path!").Attach(*this);

    return Path(std::string(buffer.data(), size));
#endif
}

bool Symlink::IsSymlinkExists() const
{
#if defined(_WIN32) || defined(_WIN64)
    DWORD attributes = GetFileAttributesW(to_wstring().c_str());
    if (attributes == INVALID_FILE_ATTRIBUTES)
        return false;

    if (attributes & FILE_ATTRIBUTE_REPARSE_POINT)
        return true;
    else
        return false;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    struct stat st;
    int result = lstat(native().c_str(), &st);
    if (result != 0)
    {
        if ((errno == ENOENT) || (errno == ENOTDIR))
            return false;
        else
            throwex FileSystemException("Cannot get the status of the symbolic link!").Attach(*this);
    }

    if (S_ISLNK(st.st_mode))
        return true;
    else
        return false;
#endif
}

Symlink Symlink::CreateSymlink(const Path& src, const Path& dst)
{
#if defined(_WIN32) || defined(_WIN64)
    std::wstring source = src.to_wstring();
    DWORD attributes = GetFileAttributesW(source.c_str());
    if (attributes == INVALID_FILE_ATTRIBUTES)
        throwex FileSystemException("Cannot get file attributes of the source path!").Attach(src);

    if (attributes & FILE_ATTRIBUTE_DIRECTORY)
    {
        if (!CreateSymbolicLinkW(source.c_str(), dst.to_wstring().c_str(), SYMBOLIC_LINK_FLAG_DIRECTORY))
            throwex FileSystemException("Cannot create symbolic link for the directory!").Attach(src, dst);
    }
    else
    {
        if (!CreateSymbolicLinkW(source.c_str(), dst.to_wstring().c_str(), 0))
            throwex FileSystemException("Cannot create symbolic link for the file!").Attach(src, dst);
    }
#elif defined(unix) || defined(__unix) || defined(__unix__)
    int result = symlink(src.native().c_str(), dst.native().c_str());
    if (result != 0)
        throwex FileSystemException("Cannot create symbolic link!").Attach(src, dst);
#endif
    return dst;
}

Path Symlink::CreateHardlink(const Path& src, const Path& dst)
{
#if defined(_WIN32) || defined(_WIN64)
    if (!CreateHardLinkW(src.to_wstring().c_str(), dst.to_wstring().c_str(), nullptr))
        throwex FileSystemException("Cannot create hard link!").Attach(src, dst);
#elif defined(unix) || defined(__unix) || defined(__unix__)
    int result = link(src.native().c_str(), dst.native().c_str());
    if (result != 0)
        throwex FileSystemException("Cannot create hard link!").Attach(src, dst);
#endif
    return dst;
}

} // namespace CppCommon
