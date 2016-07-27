/*!
    \file memory.cpp
    \brief Memory management implementation
    \author Ivan Shynkarenka
    \date 27.07.2016
    \copyright MIT License
*/

#include "system/memory.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <sys/sysinfo.h>
#endif

namespace CppCommon {

int64_t Memory::RamTotal()
{
#if defined(_WIN32) || defined(_WIN64)
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    return status.ullTotalPhys;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    struct sysinfo si;
    return (sysinfo(&si) == 0) ? si.totalram : -1;
#endif
}

int64_t Memory::RamFree()
{
#if defined(_WIN32) || defined(_WIN64)
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    return status.ullAvailPhys;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    struct sysinfo si;
    return (sysinfo(&si) == 0) ? si.freeram : -1;
#endif
}

} // namespace CppCommon
