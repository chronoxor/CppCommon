/*!
    \file directory.cpp
    \brief Filesystem directory implementation
    \author Ivan Shynkarenka
    \date 30.08.2016
    \copyright MIT License
*/

#include "filesystem/directory.h"

#include "utility/countof.h"
#include "utility/resource.h"

#include <cstring>
#include <regex>

#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

namespace CppCommon {

const Flags<FileAttributes> Directory::DEFAULT_ATTRIBUTES = FileAttributes::NORMAL;
const Flags<FilePermissions> Directory::DEFAULT_PERMISSIONS = FilePermissions::IRUSR | FilePermissions::IWUSR | FilePermissions::IXUSR | FilePermissions::IRGRP | FilePermissions::IXGRP | FilePermissions::IROTH | FilePermissions::IXOTH;

bool Directory::IsDirectoryExists() const
{
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    struct stat status;
    int result = stat(string().c_str(), &status);
    if (result != 0)
    {
        if ((errno == ENOENT) || (errno == ENOTDIR))
            return false;
        else
            throwex FileSystemException("Cannot get the status of the directory!").Attach(*this);
    }

    if (S_ISDIR(status.st_mode))
        return true;
    else
        return false;
#elif defined(_WIN32) || defined(_WIN64)
    DWORD attributes = GetFileAttributesW(wstring().c_str());
    if (attributes == INVALID_FILE_ATTRIBUTES)
        return false;

    if (attributes & FILE_ATTRIBUTE_DIRECTORY)
        return true;
    else
        return false;
#endif
}

bool Directory::IsDirectoryEmpty() const
{
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    DIR* dir = opendir(string().c_str());
    if (dir == nullptr)
        throwex FileSystemException("Cannot open a directory!").Attach(*this);

    // Smart resource cleaner pattern
    auto directory = resource(dir, [](DIR* dirp) { closedir(dirp); });

    struct dirent* pentry;
    while ((pentry = readdir(dir)) != nullptr)
    {
        if (std::strncmp(pentry->d_name, ".", sizeof(pentry->d_name)) == 0)
            continue;
        if (std::strncmp(pentry->d_name, "..", sizeof(pentry->d_name)) == 0)
            continue;
        return false;
    }

    // Release the resource manually
    int result = closedir(dir);
    if (result != 0)
        throwex FileSystemException("Cannot close a directory!").Attach(*this);
    directory.release();

    return true;
#elif defined(_WIN32) || defined(_WIN64)
    WIN32_FIND_DATAW fd;
    HANDLE hDirectory = FindFirstFileW((*this / "*").wstring().c_str(), &fd);
    if (hDirectory == INVALID_HANDLE_VALUE)
        throwex FileSystemException("Cannot open a directory!").Attach(*this);

    // Smart resource cleaner pattern
    auto directory = resource(hDirectory, [](HANDLE hFindFile) { FindClose(hFindFile); });

    do
    {
        if (std::wcsncmp(fd.cFileName, L".", countof(fd.cFileName)) == 0)
            continue;
        if (std::wcsncmp(fd.cFileName, L"..", countof(fd.cFileName)) == 0)
            continue;
        return false;
    } while (FindNextFileW(hDirectory, &fd) != 0);

    if (GetLastError() != ERROR_NO_MORE_FILES)
        throwex FileSystemException("Cannot read directory entries!").Attach(*this);

    // Release the resource manually
    if (!FindClose(hDirectory))
        throwex FileSystemException("Cannot close a directory!").Attach(*this);
    directory.release();

    return true;
#endif
}

DirectoryIterator Directory::begin() const
{
    return DirectoryIterator(*this, false);
}

DirectoryIterator Directory::end() const
{
    return DirectoryIterator();
}

DirectoryIterator Directory::rbegin() const
{
    return DirectoryIterator(*this, true);
}

DirectoryIterator Directory::rend() const
{
    return DirectoryIterator();
}

std::vector<Path> Directory::GetEntries(const std::string& pattern)
{
    std::vector<Path> result;
    std::regex matcher(pattern);
    for (auto it = begin(); it != end(); ++it)
        if (pattern.empty() || std::regex_match(it->filename().string(), matcher))
            result.push_back(*it);
    return result;
}

std::vector<Path> Directory::GetEntriesRecursive(const std::string& pattern)
{
    std::vector<Path> result;
    std::regex matcher(pattern);
    for (auto it = rbegin(); it != rend(); ++it)
        if (pattern.empty() || std::regex_match(it->filename().string(), matcher))
            result.push_back(Directory(*it));
    return result;
}

std::vector<Directory> Directory::GetDirectories(const std::string& pattern)
{
    std::vector<Directory> result;
    std::regex matcher(pattern);
    for (auto it = begin(); it != end(); ++it)
    {
        // Special check for symbolic link
        Directory target(*it);
        if (it->IsSymlink())
            target = Symlink(target).target();

        // Special check for directory
        if (target.IsDirectory())
            if (pattern.empty() || std::regex_match(it->filename().string(), matcher))
                result.emplace_back(*it);
    }
    return result;
}

std::vector<Directory> Directory::GetDirectoriesRecursive(const std::string& pattern)
{
    std::vector<Directory> result;
    std::regex matcher(pattern);
    for (auto it = rbegin(); it != rend(); ++it)
    {
        // Special check for symbolic link
        Directory target(*it);
        if (it->IsSymlink())
            target = Symlink(target).target();

        // Special check for directory
        if (target.IsDirectory())
            if (pattern.empty() || std::regex_match(it->filename().string(), matcher))
                result.emplace_back(*it);
    }
    return result;
}

std::vector<File> Directory::GetFiles(const std::string& pattern)
{
    std::vector<File> result;
    std::regex matcher(pattern);
    for (auto it = begin(); it != end(); ++it)
    {
        // Special check for symbolic link
        File target(*it);
        if (it->IsSymlink())
            target = Symlink(target).target();

        // Special check for directory
        if (!target.IsDirectory())
            if (pattern.empty() || std::regex_match(it->filename().string(), matcher))
                result.emplace_back(*it);
    }
    return result;
}

std::vector<File> Directory::GetFilesRecursive(const std::string& pattern)
{
    std::vector<File> result;
    std::regex matcher(pattern);
    for (auto it = rbegin(); it != rend(); ++it)
    {
        // Special check for symbolic link
        File target(*it);
        if (it->IsSymlink())
            target = Symlink(target).target();

        // Special check for directory
        if (!target.IsDirectory())
            if (pattern.empty() || std::regex_match(it->filename().string(), matcher))
                result.emplace_back(*it);
    }
    return result;
}

std::vector<Symlink> Directory::GetSymlinks(const std::string& pattern)
{
    std::vector<Symlink> result;
    std::regex matcher(pattern);
    for (auto it = begin(); it != end(); ++it)
    {
        // Special check for symbolic link
        if (it->IsSymlink())
            if (pattern.empty() || std::regex_match(it->filename().string(), matcher))
                result.emplace_back(*it);
    }
    return result;
}

std::vector<Symlink> Directory::GetSymlinksRecursive(const std::string& pattern)
{
    std::vector<Symlink> result;
    std::regex matcher(pattern);
    for (auto it = rbegin(); it != rend(); ++it)
    {
        // Special check for symbolic link
        if (it->IsSymlink())
            if (pattern.empty() || std::regex_match(it->filename().string(), matcher))
                result.emplace_back(*it);
    }
    return result;
}

Directory Directory::Create(const Path& path, const Flags<FileAttributes>& attributes, const Flags<FilePermissions>& permissions)
{
    Directory directory(path);
    if (directory.IsDirectoryExists())
        return directory;
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

    int result = mkdir(path.string().c_str(), mode);
    if (result != 0)
        throwex FileSystemException("Cannot create directory!").Attach(path);
#elif defined(_WIN32) || defined(_WIN64)
    if (!CreateDirectoryW(path.wstring().c_str(), nullptr))
        throwex FileSystemException("Cannot create directory!").Attach(path);
#endif
    return directory;
}

Directory Directory::CreateTree(const Path& path, const Flags<FileAttributes>& attributes, const Flags<FilePermissions>& permissions)
{
    Directory tree(path);
    if (tree.IsDirectoryExists())
        return tree;

    // Try to create the last directory
    try
    {
        Create(tree);
        return tree;
    }
    catch (const FileSystemException&) {}

    // Failed, try to get the parent path and retry
    Directory parent = tree.parent();
    if (parent.empty())
        throwex FileSystemException("Cannot create directory tree!").Attach(path);
    else
        CreateTree(parent);

    // Retry to create the last directory
    Create(tree);

    return tree;
}

} // namespace CppCommon
