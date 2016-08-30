/*!
    \file directory.cpp
    \brief Filesystem directory wrapper implementation
    \author Ivan Shynkarenka
    \date 30.08.2016
    \copyright MIT License
*/

#include "filesystem/directory.h"

#include "filesystem/exceptions.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <shlwapi.h>
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

namespace CppCommon {

bool Directory::IsDirectoryExists() const
{
#if defined(_WIN32) || defined(_WIN64)
    DWORD attributes = GetFileAttributesW(to_wstring().c_str());
    if (attributes == INVALID_FILE_ATTRIBUTES)
        return false;

    if (attributes & FILE_ATTRIBUTE_DIRECTORY)
        return true;
    else
        return false;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    struct stat st;
    int result = stat(native().c_str(), &st);
    if (result != 0)
    {
        if ((errno == ENOENT) || (errno == ENOTDIR))
            return false;
        else
            throwex FileSystemException("Cannot get the status of the directory!").Attach(*this);
    }

    if (S_ISDIR(st.st_mode))
        return true;
    else
        return false;
#endif
}

bool Directory::IsDirectoryEmpty() const
{
#if defined(_WIN32) || defined(_WIN64)
    if (!IsDirectoryExists())
        throwex FileSystemException("Directory is not exists!").Attach(*this);

    return PathIsDirectoryEmptyW(to_wstring().c_str()) ? true : false;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    DIR* dir = opendir(native().c_str());
    if (dir == nullptr)
        throwex FileSystemException("Cannot open a directory!").Attach(*this);

    struct dirent entry;
    struct dirent* pentry;

    int result;
    while (((result = readdir_r(dir, &entry, &pentry)) == 0) && (pentry != nullptr))
    {
        if (std::strncmp(pentry->d_name, ".", sizeof(pentry->d_name)) == 0)
            continue;
        if (std::strncmp(pentry->d_name, "..", sizeof(pentry->d_name)) == 0)
            continue;
        return false;
    }

    if (result != 0)
        throwex FileSystemException("Cannot read directory entries!").Attach(*this);

    return true;
#endif
}

Directory Directory::Create(const Path& path, const Flags<FileAttributes>& attributes, const Flags<FilePermissions>& permissions)
{
    Directory result(path);
    if (result.IsDirectoryExists())
        return result;
#if defined(_WIN32) || defined(_WIN64)
    if (!CreateDirectoryW(path.to_wstring().c_str(), nullptr))
        throwex FileSystemException("Cannot create directory!").Attach(path);
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

    int result = mkdir(path.native().c_str(), mode);
    if (result != 0)
        throwex FileSystemException("Cannot create directory!").Attach(path);
#endif
    return result;
}

Directory Directory::CreateTree(const Path& path, const Flags<FileAttributes>& attributes, const Flags<FilePermissions>& permissions)
{
    Directory result(path);
    if (result.IsDirectoryExists())
        return result;

    // Try to create the directory
    try
    {
        Directory::Create(result);
        return result;
    }
    catch (FileSystemException) {}

    // Failed, try to get the parent path and retry
    result = result.parent();
    if (result.empty())
        throwex FileSystemException("Cannot create directory tree!").Attach(path);
    else
        CreateTree(result);

    return result;
}

} // namespace CppCommon
