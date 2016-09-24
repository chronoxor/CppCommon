/*!
    \file memory.cpp
    \brief Memory management implementation
    \author Ivan Shynkarenka
    \date 27.07.2016
    \copyright MIT License
*/

#if defined(__CYGWIN__)
#define _WIN32_WINNT 0x601
#endif

#include "system/memory.h"

#if defined(__APPLE__)
#include <mach/mach.h>
#include <sys/sysctl.h>
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
#include <sys/sysinfo.h>
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#include <windows.h>
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
#elif defined(linux) || defined(__linux) || defined(__linux__)
    int64_t pages = sysconf(_SC_PHYS_PAGES);
    int64_t page_size = sysconf(_SC_PAGESIZE);
    if ((pages > 0) && (page_size > 0))
        return pages * page_size;

    return -1;
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
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
    kern_return_t kernReturn = host_statistics(mach_host_self(), HOST_VM_INFO, (host_info_t)&vmstat, &count);
    if (kernReturn != KERN_SUCCESS)
        return -1;

    int64_t used_mem = (vmstat.active_count + vmstat.inactive_count + vmstat.wire_count) * page_size;
    int64_t free_mem = vmstat.free_count * page_size;
    return free_mem;
#elif defined(linux) || defined(__linux) || defined(__linux__)
    int64_t pages = sysconf(_SC_AVPHYS_PAGES);
    int64_t page_size = sysconf(_SC_PAGESIZE);
    if ((pages > 0) && (page_size > 0))
        return pages * page_size;

    return -1;
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    return status.ullAvailPhys;
#else
    #error Unsupported platform
#endif
}

} // namespace CppCommon
