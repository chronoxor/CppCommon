/*!
    \file directory_iterator.cpp
    \brief Filesystem directory iterator implementation
    \author Ivan Shynkarenka
    \date 30.08.2016
    \copyright MIT License
*/

#include "filesystem/directory_iterator.h"

#include "errors/fatal.h"
#include "filesystem/exceptions.h"
#include "filesystem/symlink.h"
#include "utility/countof.h"

#include <cstring>
#include <stack>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <dirent.h>
#endif

namespace CppCommon {

class DirectoryIterator::Impl
{
    friend class SimpleImpl;
    friend class RecurseImpl;

public:
    Impl(const Path& parent) : _parent(parent), _current() {}
    Impl(const Impl&) = default;
    Impl(Impl&&) noexcept = default;
    virtual ~Impl() = default;

    Impl& operator=(const Impl&) = default;
    Impl& operator=(Impl&&) noexcept = default;

    const Path& parent() const noexcept { return _parent; }
    const Path& current() const noexcept { return _current; }

    virtual Path Next() = 0;

protected:
    Path _parent;
    Path _current;
};

class DirectoryIterator::SimpleImpl : public DirectoryIterator::Impl
{
public:
    SimpleImpl(const Path& parent) : DirectoryIterator::Impl(parent), _next(false), _end(false)
    {
#if defined(_WIN32) || defined(_WIN64)
        _directory = FindFirstFileW((_parent / "*").wstring().c_str(), &_entry);
        if (_directory == INVALID_HANDLE_VALUE)
            throwex FileSystemException("Cannot open a directory!").Attach(_parent);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        _directory = opendir(_parent.native().c_str());
        if (_directory == nullptr)
            throwex FileSystemException("Cannot open a directory!").Attach(_parent);
#endif
    }

    SimpleImpl(const SimpleImpl&) = default;
    SimpleImpl(SimpleImpl&&) noexcept = default;

    ~SimpleImpl()
    {
#if defined(_WIN32) || defined(_WIN64)
        if (_directory != INVALID_HANDLE_VALUE)
        {
            if (!FindClose(_directory))
                fatality(FileSystemException("Cannot close the directory handle!").Attach(_parent));
        }
#elif defined(unix) || defined(__unix) || defined(__unix__)
        if (_directory != nullptr)
        {
            int result = closedir(_directory);
            if (result != 0)
                fatality(FileSystemException("Cannot close the directory descriptor!").Attach(_parent));
        }
#endif
    }

    SimpleImpl& operator=(const SimpleImpl&) = default;
    SimpleImpl& operator=(SimpleImpl&&) noexcept = default;

    Path Next() override
    {
        if (_end)
            return _current;
#if defined(_WIN32) || defined(_WIN64)
        do
        {
            if (_next)
            {
                _next = false;
                continue;
            }
            if (std::wcsncmp(_entry.cFileName, L".", countof(_entry.cFileName)) == 0)
                continue;
            if (std::wcsncmp(_entry.cFileName, L"..", countof(_entry.cFileName)) == 0)
                continue;
            _next = true;
            _current = _parent / _entry.cFileName;
            return _current;
        } while (FindNextFileW(_directory, &_entry) != 0);

        if (GetLastError() != ERROR_NO_MORE_FILES)
            throwex FileSystemException("Cannot read directory entries!").Attach(_parent);
#elif defined(unix) || defined(__unix) || defined(__unix__)
        struct dirent* pentry;

        int result;
        while (((result = readdir_r(_directory, &_entry, &pentry)) == 0) && (pentry != nullptr))
        {
            if (std::strncmp(pentry->d_name, ".", sizeof(pentry->d_name)) == 0)
                continue;
            if (std::strncmp(pentry->d_name, "..", sizeof(pentry->d_name)) == 0)
                continue;
            _current = _parent / pentry->d_name;
            return _current;
        }

        if (result != 0)
            throwex FileSystemException("Cannot read directory entries!").Attach(_parent);
#endif
        _end = true;
        _current = Path();
        return _current;
    }

    void Move(SimpleImpl& instance)
    {
        _parent = instance._parent;
        _current = instance._current;
        _directory = instance._directory;
        _entry = instance._entry;
        _next = instance._next;
        _end = instance._end;

#if defined(_WIN32) || defined(_WIN64)
        instance._directory = INVALID_HANDLE_VALUE;
#elif defined(unix) || defined(__unix) || defined(__unix__)
        instance._directory = nullptr;
#endif
    }

    void Swap(SimpleImpl& instance)
    {
        using std::swap;
        swap(_parent, instance._parent);
        swap(_current, instance._current);
        swap(_directory, instance._directory);
        swap(_entry, instance._entry);
        swap(_next, instance._next);
        swap(_end, instance._end);
    }

private:
#if defined(_WIN32) || defined(_WIN64)
    HANDLE _directory;
    WIN32_FIND_DATAW _entry;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    DIR* _directory;
    struct dirent _entry;
#endif
    bool _next;
    bool _end;
};

class DirectoryIterator::RecurseImpl : public DirectoryIterator::Impl
{
public:
    RecurseImpl(const Path& parent) : DirectoryIterator::Impl(parent), _current(parent) {}
    RecurseImpl(const RecurseImpl&) = default;
    RecurseImpl(RecurseImpl&&) noexcept = default;
    ~RecurseImpl() = default;

    RecurseImpl& operator=(const RecurseImpl&) = default;
    RecurseImpl& operator=(RecurseImpl&&) noexcept = default;

    Path Next() override
    {
        // Get the next path value
        Path result = _current.Next();
        if (result.empty())
        {
            // Immediately return in case of empty stack
            if (_stack.empty())
                return result;

            // If the stack has any iterators pop one and return its current path
            _current.Swap(_stack.top());
            _stack.pop();
            return _current.current();
        }

        // Special check for symbolic link
        Path target(result);
        if (result.IsSymlink())
            target = Symlink(target).target();

        // Special check for directory
        if (target.IsDirectory())
        {
            // Put the current iterator to stack
            _stack.push(_current);

            // Switch the current iterator to the target sub-directory
            SimpleImpl subdir = SimpleImpl(result);
            _current.Move(subdir);

            // Call Next() method for the new iterator
            return Next();
        }

        return result;
    }

private:
    SimpleImpl _current;
    std::stack<SimpleImpl> _stack;
};

DirectoryIterator::DirectoryIterator() : _pimpl(nullptr), _current()
{
}

DirectoryIterator::DirectoryIterator(const Path& current) : _pimpl(nullptr), _current(current)
{
}

DirectoryIterator::DirectoryIterator(const Path& parent, bool recurse) : _pimpl(recurse ? (Impl*)std::make_unique<RecurseImpl>(parent).release() : (Impl*)std::make_unique<SimpleImpl>(parent).release())
{
    _current = _pimpl->Next();
}

DirectoryIterator::DirectoryIterator(DirectoryIterator& it) : _pimpl(it._pimpl.release()), _current(it._current)
{
}

DirectoryIterator::DirectoryIterator(DirectoryIterator&& it) noexcept : _pimpl(std::move(it._pimpl)), _current(std::move(it._current))
{
}

DirectoryIterator::~DirectoryIterator()
{
}

DirectoryIterator& DirectoryIterator::operator=(DirectoryIterator& it)
{
    _pimpl.reset(it._pimpl.release());
    _current = it._current;
    return *this;
}

DirectoryIterator& DirectoryIterator::operator=(DirectoryIterator&& it) noexcept
{
    _pimpl = std::move(it._pimpl);
    _current = std::move(it._current);
    return *this;
}

DirectoryIterator& DirectoryIterator::operator++()
{
    if (_pimpl)
        _current = _pimpl->Next();
    return *this;
}

DirectoryIterator DirectoryIterator::operator++(int)
{
    DirectoryIterator result(_current);
    ++(*this);
    return result;
}

} // namespace CppCommon
