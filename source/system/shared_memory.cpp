/*!
    \file shared_memory.cpp
    \brief Shared memory manager implementation
    \author Ivan Shynkarenka
    \date 19.04.2016
    \copyright MIT License
*/

#include "system/shared_memory.h"

#include "errors/exceptions.h"
#include "errors/fatal.h"

#include <cassert>
#include <cstring>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <sys/mman.h>
#include <fcntl.h>
#endif

namespace CppCommon {

class SharedMemory::Impl
{
public:
    Impl(const std::string& name, size_t size)
    {
        assert(!name.empty() && "Shared memory buffer name must not be empty!");
        assert((size > 0) && "Shared memory buffer size must be greater than zero!");

        size_t total = SHARED_MEMORY_HEADER_SIZE + size;
#if defined(_WIN32) || defined(_WIN64)
        _name = "Global\\" + name;
        _owner = false;
        _shared = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, _name.c_str());
        if (_shared == nullptr)
        {
            _shared = CreateFileMappingA(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, (DWORD)total, _name.c_str());
            if (_shared == nullptr)
                throwex SystemException("Failed to create or open a shared memory handler!");
            else
                _owner = true;
        }
        _ptr = MapViewOfFile(_shared, FILE_MAP_ALL_ACCESS, 0, 0, total);
        if (_ptr == nullptr)
        {
            CloseHandle(_shared);
            throwex SystemException("Failed to map a shared memory buffer!");
        }
#elif defined(unix) || defined(__unix) || defined(__unix__)
        _name = "/" + name;
        _owner = true;
        _shared = shm_open(_name.c_str(), (O_CREAT | O_EXCL | O_RDWR), (S_IRUSR | S_IWUSR));
        if (_shared == -1)
        {
            _shared = shm_open(_name.c_str(), (O_CREAT | O_RDWR), (S_IRUSR | S_IWUSR));
            if (_shared == -1)
                throwex SystemException("Failed to create or open a shared memory handler!");
            else
                _owner = false;
        }
        int result = ftruncate(_shared, total);
        if (result != 0)
            throwex SystemException(result, "Failed to truncate a shared memory handler!");
        _ptr = mmap(nullptr, total, (PROT_READ | PROT_WRITE), MAP_SHARED, _shared, 0);
        if (_ptr == MAP_FAILED)
        {
            close(_shared);
            shm_unlink(_name.c_str());
            throwex SystemException("Failed to map a shared memory buffer!");
        }
#endif
        static const char* SHARED_MEMORY_HEADER_PREFIX = "SHMM";

        // Fill or check shared memory header
        if (_owner)
        {
            std::strncpy(((SharedMemoryHeader*)_ptr)->prefix, SHARED_MEMORY_HEADER_PREFIX, 4 );
            ((SharedMemoryHeader*)_ptr)->size = size;
        }
        else
        {
            bool is_valid_prefix = (std::strncmp(((SharedMemoryHeader*)_ptr)->prefix, SHARED_MEMORY_HEADER_PREFIX, 4) == 0);
            bool is_valid_size = (((SharedMemoryHeader*)_ptr)->size == size);
            if (!is_valid_prefix || !is_valid_size)
            {
#if defined(_WIN32) || defined(_WIN64)
                UnmapViewOfFile(_ptr);
                CloseHandle(_shared);
#elif defined(unix) || defined(__unix) || defined(__unix__)
                munmap(_ptr, total);
                close(_shared);
                shm_unlink(_name.c_str());
#endif
                if (!is_valid_prefix)
                    throwex SystemException("Invalid shared memory buffer prefix!");
                if (!is_valid_size)
                    throwex SystemException("Invalid shared memory buffer size!");
            }
        }
    }

    ~Impl()
    {
#if defined(_WIN32) || defined(_WIN64)
        if (!UnmapViewOfFile(_ptr))
            fatality("Failed to unmap a shared memory buffer!");
        if (!CloseHandle(_shared))
            fatality("Failed to close a shared memory handler!");
#elif defined(unix) || defined(__unix) || defined(__unix__)
        size_t total = ((SharedMemoryHeader*)_ptr)->size + SHARED_MEMORY_HEADER_SIZE;
        int result = munmap(_ptr, total);
        if (result != 0)
            fatality("Failed to unmap a shared memory buffer!", result);
        result = close(_shared);
        if (result != 0)
            fatality("Failed to close a shared memory handler!");
        result = shm_unlink(_name.c_str());
        if (result != 0)
            fatality("Failed to unlink a shared memory handler!");
#endif
    }

    void* ptr() { return (char*)_ptr + SHARED_MEMORY_HEADER_SIZE; }
    const void* ptr() const { return ptr(); }
    bool owner() { return _owner; }

private:
    // Shared memory header size
    static const int SHARED_MEMORY_HEADER_SIZE = 64;

    // Shared memory header structure
    struct SharedMemoryHeader
    {
        // Prefix must always be "SHMM"
        char prefix[4];
        // Padding
        char padding[4];
        // Block size
        size_t size;
    };

    std::string _name;
#if defined(_WIN32) || defined(_WIN64)
    HANDLE _shared;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    int _shared;
#endif
    void* _ptr;
    bool _owner;
};

SharedMemory::SharedMemory(const std::string& name, size_t size) : _pimpl(new Impl(name, size)), _name(name), _size(size)
{
}

SharedMemory::~SharedMemory()
{
}

void* SharedMemory::ptr()
{
    return _pimpl->ptr();
}

const void* SharedMemory::ptr() const
{
    return _pimpl->ptr();
}

bool SharedMemory::owner() const
{
    return _pimpl->owner();
}

} // namespace CppCommon
