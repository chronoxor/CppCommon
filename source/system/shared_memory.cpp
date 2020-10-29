/*!
    \file shared_memory.cpp
    \brief Shared memory manager implementation
    \author Ivan Shynkarenka
    \date 19.04.2016
    \copyright MIT License
*/

#include "system/shared_memory.h"

#include "errors/fatal.h"
#include "utility/validate_aligned_storage.h"

#include <cassert>
#include <cstring>

#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

namespace CppCommon {

//! @cond INTERNALS

class SharedMemory::Impl
{
public:
    Impl(const std::string& name, size_t size)
    {
        assert(!name.empty() && "Shared memory buffer name must not be empty!");
        assert((size > 0) && "Shared memory buffer size must be greater than zero!");

        size_t total = SHARED_MEMORY_HEADER_SIZE + size;
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        _name = "/" + name;
        _owner = true;

        // Try to create a shared memory handler
        _shared = shm_open(_name.c_str(), (O_CREAT | O_EXCL | O_RDWR), (S_IRUSR | S_IWUSR));
        if (_shared == -1)
        {
            // Try to open a shared memory handler
            _shared = shm_open(_name.c_str(), (O_CREAT | O_RDWR), (S_IRUSR | S_IWUSR));
            if (_shared == -1)
                throwex SystemException("Failed to create or open a shared memory handler!");
            else
                _owner = false;
        }
        else
        {
            // Truncate a shared memory handler
            int result = ftruncate(_shared, total);
            if (result != 0)
                throwex SystemException("Failed to truncate a shared memory handler!");
        }

        // Map a shared memory buffer
        _ptr = mmap(nullptr, total, (PROT_READ | PROT_WRITE), MAP_SHARED, _shared, 0);
        if (_ptr == MAP_FAILED)
        {
            close(_shared);
            shm_unlink(_name.c_str());
            throwex SystemException("Failed to map a shared memory buffer!");
        }
#elif defined(_WIN32) || defined(_WIN64)
        _name = "Global\\" + name;
        _owner = false;

        // Try to open a shared memory handler
        _shared = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, _name.c_str());
        if (_shared == nullptr)
        {
            // Try to create a shared memory handler
            _shared = CreateFileMappingA(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, (DWORD)total, _name.c_str());
            if (_shared == nullptr)
                throwex SystemException("Failed to create or open a shared memory handler!");
            else
                _owner = true;
        }

        // Map a shared memory buffer
        _ptr = MapViewOfFile(_shared, FILE_MAP_ALL_ACCESS, 0, 0, total);
        if (_ptr == nullptr)
        {
            CloseHandle(_shared);
            throwex SystemException("Failed to map a shared memory buffer!");
        }
#endif
        static const char* SHARED_MEMORY_HEADER_PREFIX = "SHMM";

        // Owner must fill the shared memory header and user must check the shared memory header
        if (_owner)
        {
            // Fill shared memory header
            std::memcpy(((SharedMemoryHeader*)_ptr)->prefix, SHARED_MEMORY_HEADER_PREFIX, 4 );
            ((SharedMemoryHeader*)_ptr)->size = size;
        }
        else
        {
            // Check shared memory header
            bool is_valid_prefix = (std::strncmp(((SharedMemoryHeader*)_ptr)->prefix, SHARED_MEMORY_HEADER_PREFIX, 4) == 0);
            bool is_valid_size = (((SharedMemoryHeader*)_ptr)->size == size);
            if (!is_valid_prefix || !is_valid_size)
            {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
                munmap(_ptr, total);
                close(_shared);
#elif defined(_WIN32) || defined(_WIN64)
                UnmapViewOfFile(_ptr);
                CloseHandle(_shared);
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
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        // Unmap the shared memory buffer
        size_t total = ((SharedMemoryHeader*)_ptr)->size + SHARED_MEMORY_HEADER_SIZE;
        int result = munmap(_ptr, total);
        if (result != 0)
            fatality(SystemException("Failed to unmap a shared memory buffer!"));

        // Close the shared memory handler
        result = close(_shared);
        if (result != 0)
            fatality(SystemException("Failed to close a shared memory handler!"));

        // Unlink the shared memory handler (owner only)
        if (_owner)
        {
            result = shm_unlink(_name.c_str());
            if (result != 0)
                fatality(SystemException("Failed to unlink a shared memory handler!"));
        }
#elif defined(_WIN32) || defined(_WIN64)
        // Unmap the shared memory buffer
        if (!UnmapViewOfFile(_ptr))
            fatality(SystemException("Failed to unmap a shared memory buffer!"));

        // Close the shared memory handler
        if (!CloseHandle(_shared))
            fatality(SystemException("Failed to close a shared memory handler!"));
#endif
    }

    void* ptr() { return (uint8_t*)_ptr + SHARED_MEMORY_HEADER_SIZE; }
    const void* ptr() const { return _ptr; }
    bool owner() const { return _owner; }

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
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    int _shared;
#elif defined(_WIN32) || defined(_WIN64)
    HANDLE _shared;
#endif
    void* _ptr;
    bool _owner;
};

//! @endcond

SharedMemory::SharedMemory(const std::string& name, size_t size) : _name(name), _size(size)
{
    // Check implementation storage parameters
    [[maybe_unused]] ValidateAlignedStorage<sizeof(Impl), alignof(Impl), StorageSize, StorageAlign> _;
    static_assert((StorageSize >= sizeof(Impl)), "SharedMemory::StorageSize must be increased!");
    static_assert(((StorageAlign % alignof(Impl)) == 0), "SharedMemory::StorageAlign must be adjusted!");

    // Create the implementation instance
    new(&_storage)Impl(name, size);
}

SharedMemory::~SharedMemory()
{
    // Delete the implementation instance
    reinterpret_cast<Impl*>(&_storage)->~Impl();
}

void* SharedMemory::ptr() { return impl().ptr(); }
const void* SharedMemory::ptr() const { return impl().ptr(); }
bool SharedMemory::owner() const { return impl().owner(); }

} // namespace CppCommon
