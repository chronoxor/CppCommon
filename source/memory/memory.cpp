/*!
    \file memory.cpp
    \brief Memory management implementation
    \author Ivan Shynkarenka
    \date 27.07.2016
    \copyright MIT License
*/

#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdlib.h>
#include <string.h>
#endif

#include "memory/memory.h"

#if defined(__APPLE__)
#include <mach/mach.h>
#include <sys/sysctl.h>
#include <fcntl.h>
#include <unistd.h>
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <sys/sysinfo.h>
#include <fcntl.h>
#include <unistd.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <wincrypt.h>
#endif

namespace CppCommon {

int64_t Memory::RamTotal()
{
#if defined(__APPLE__)
    int64_t memsize = 0;
    size_t size = sizeof(memsize);
    if (sysctlbyname("hw.memsize", &memsize, &size, nullptr, 0) == 0)
        return memsize;

    return -1;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    int64_t pages = sysconf(_SC_PHYS_PAGES);
    int64_t page_size = sysconf(_SC_PAGESIZE);
    if ((pages > 0) && (page_size > 0))
        return pages * page_size;

    return -1;
#elif defined(_WIN32) || defined(_WIN64)
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    return status.ullTotalPhys;
#else
    #error Unsupported platform
#endif
}

int64_t Memory::RamFree()
{
#if defined(__APPLE__)
    mach_port_t host_port = mach_host_self();
    if (host_port == MACH_PORT_NULL)
        return -1;

    vm_size_t page_size = 0;
    host_page_size(host_port, &page_size);

    vm_statistics_data_t vmstat;
    mach_msg_type_number_t count = HOST_VM_INFO_COUNT;
    kern_return_t kernReturn = host_statistics(host_port, HOST_VM_INFO, (host_info_t)&vmstat, &count);
    if (kernReturn != KERN_SUCCESS)
        return -1;

    [[maybe_unused]] int64_t used_mem = (vmstat.active_count + vmstat.inactive_count + vmstat.wire_count) * page_size;
    int64_t free_mem = vmstat.free_count * page_size;
    return free_mem;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    int64_t pages = sysconf(_SC_AVPHYS_PAGES);
    int64_t page_size = sysconf(_SC_PAGESIZE);
    if ((pages > 0) && (page_size > 0))
        return pages * page_size;

    return -1;
#elif defined(_WIN32) || defined(_WIN64)
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    return status.ullAvailPhys;
#else
    #error Unsupported platform
#endif
}

bool Memory::IsZero(const void* buffer, size_t size) noexcept
{
    const char* ptr = (const char*)buffer;
    for (size_t i = 0; i < size; ++i)
        if (ptr[i] != 0)
            return false;
    return true;
}

void Memory::ZeroFill(void* buffer, size_t size)
{
#ifdef __STDC_LIB_EXT1__
    memset_s(buffer, size, 0, size);
#elif defined(_WIN32) || defined(_WIN64)
    SecureZeroMemory(buffer, size);
#else
    volatile char* ptr = (volatile char*)buffer;
    while (size--)
        *ptr++ = 0;
#endif
}

void Memory::RandomFill(void* buffer, size_t size)
{
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd < 0)
        throwex SystemException("Cannot open '/dev/urandom' file for reading!");
    ssize_t count = read(fd, buffer, size);
    if (count < 0)
        throwex SystemException("Cannot read from '/dev/urandom' file!");
    int result = close(fd);
    if (result != 0)
        throwex SystemException("Cannot close '/dev/urandom' file!");
#elif defined(_WIN32) || defined(_WIN64)
    char* ptr = (char*)buffer;
    for(size_t i = 0; i < size; ++i)
        ptr[i] = rand() % 256;
#endif
}

void Memory::CryptoFill(void* buffer, size_t size)
{
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    int fd = open("/dev/random", O_RDONLY);
    if (fd < 0)
        throwex SystemException("Cannot open '/dev/random' file for reading!");
    ssize_t count = read(fd, buffer, size);
    if (count < 0)
        throwex SystemException("Cannot read from '/dev/random' file!");
    int result = close(fd);
    if (result != 0)
        throwex SystemException("Cannot close '/dev/random' file!");
#elif defined(_WIN32) || defined(_WIN64)
    HCRYPTPROV hCryptContext;
    if (!CryptAcquireContext(&hCryptContext, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
        throwex SystemException("Cannot acquire cryptographic service provider context!");
    if (!CryptGenRandom(hCryptContext, (DWORD)size, (BYTE*)buffer))
        throwex SystemException("Cannot generate random bytes using cryptographic service provider context!");
    if (!CryptReleaseContext(hCryptContext, 0))
        throwex SystemException("Cannot release cryptographic service provider context!");
#endif
}

} // namespace CppCommon
