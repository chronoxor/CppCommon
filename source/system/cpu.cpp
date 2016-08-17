/*!
    \file cpu.cpp
    \brief CPU management implementation
    \author Ivan Shynkarenka
    \date 27.07.2016
    \copyright MIT License
*/

#include "system/cpu.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <unistd.h>
#include <fstream>
#include <regex>
#endif

namespace CppCommon {

//! @cond
namespace Internals {

#if defined(_WIN32) || defined(_WIN64)
// Helper function to count set bits in the processor mask
DWORD CountSetBits(ULONG_PTR pBitMask)
{
    DWORD dwLeftShift = sizeof(ULONG_PTR) * 8 - 1;
    DWORD dwBitSetCount = 0;
    ULONG_PTR pBitTest = (ULONG_PTR)1 << dwLeftShift;

    for (DWORD i = 0; i <= dwLeftShift; ++i)
    {
        dwBitSetCount += ((pBitMask & pBitTest) ? 1 : 0);
        pBitTest /= 2;
    }

    return dwBitSetCount;
}
#endif

} // namespace Internals
//! @endcond

std::string CPU::Architecture()
{
#if defined(_WIN32) || defined(_WIN64)
    HKEY hKey;
    LONG lError = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKey);
    if (lError != ERROR_SUCCESS)
        return "<unknown>";

    // Smart resource deleter pattern
    auto clear = [](HKEY hKey) { RegCloseKey(hKey); };
    auto key = std::unique_ptr<std::remove_pointer<HKEY>::type, decltype(clear)>(hKey, clear);

    CHAR pBuffer[_MAX_PATH] = { 0 };
    DWORD dwBufferSize = _MAX_PATH;
    lError = RegQueryValueExA(key.get(), "ProcessorNameString", nullptr, nullptr, (LPBYTE)pBuffer, &dwBufferSize);
    if (lError != ERROR_SUCCESS)
        return "<unknown>";

    return std::string(pBuffer);
#elif defined(unix) || defined(__unix) || defined(__unix__)
    static std::regex pattern("model name(.*): (.*)");

    std::string line;
    std::ifstream stream("/proc/cpuinfo");
    while (getline(stream, line))
    {
        std::smatch matches;
        if (std::regex_match(line, matches, pattern))
            return matches[2];
    }

    return "<unknown>";
#endif
}

int CPU::Affinity()
{
#if defined(_WIN32) || defined(_WIN64)
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    return si.dwNumberOfProcessors;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    return sysconf(_SC_NPROCESSORS_ONLN);
#endif
}

int CPU::LogicalCores()
{
    return TotalCores().first;
}

int CPU::PhysicalCores()
{
    return TotalCores().second;
}

std::pair<int, int> CPU::TotalCores()
{
#if defined(_WIN32) || defined(_WIN64)
    BOOL allocated = FALSE;
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION pBuffer = nullptr;
    DWORD dwLength = 0;

    while (!allocated)
    {
        DWORD dwResult = GetLogicalProcessorInformation(pBuffer, &dwLength);
        if (dwResult == FALSE)
        {
            if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
            {
                if (pBuffer != nullptr)
                    free(pBuffer);
                pBuffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc(dwLength);
                if (pBuffer == nullptr)
                    return std::make_pair(-1, -1);
            }
            else
                return std::make_pair(-1, -1);
        }
        else
            allocated = TRUE;
    }

    std::pair<int, int> result(0, 0);
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION pCurrent = pBuffer;
    DWORD dwOffset = 0;

    while (dwOffset + sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION) <= dwLength)
    {
        switch (pCurrent->Relationship)
        {
            case RelationProcessorCore:
                result.first += Internals::CountSetBits(pCurrent->ProcessorMask);
                result.second += 1;
                break;
            case RelationNumaNode:
            case RelationCache:
            case RelationProcessorPackage:
                break;
            default:
                return std::make_pair(-1, -1);
        }
        dwOffset += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
        pCurrent++;
    }

    free(pBuffer);

    return result;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    long processors = sysconf(_SC_NPROCESSORS_ONLN);
    std::pair<int, int> result(processors, processors);
    return result;
#endif
}

int64_t CPU::ClockSpeed()
{
#if defined(_WIN32) || defined(_WIN64)
    HKEY hKey;
    long lError = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKey);
    if (lError != ERROR_SUCCESS)
        return -1;

    // Smart resource deleter pattern
    auto clear = [](HKEY hKey) { RegCloseKey(hKey); };
    auto key = std::unique_ptr<std::remove_pointer<HKEY>::type, decltype(clear)>(hKey, clear);

    DWORD dwMHz = 0;
    DWORD dwBufferSize = sizeof(DWORD);
    lError = RegQueryValueExA(key.get(), "~MHz", nullptr, nullptr, (LPBYTE)&dwMHz, &dwBufferSize);
    if (lError != ERROR_SUCCESS)
        return -1;

    return dwMHz * 1000 * 1000;
#elif defined(unix) || defined(__unix) || defined(__unix__)
    static std::regex pattern("cpu MHz(.*): (.*)");

    std::string line;
    std::ifstream stream("/proc/cpuinfo");
    while (getline(stream, line))
    {
        std::smatch matches;
        if (std::regex_match(line, matches, pattern))
            return (int64_t)(atof(matches[2].str().c_str()) * 1000 * 1000);
    }

    return -1;
#endif
}

bool CPU::HyperThreading()
{
    std::pair<int, int> cores = TotalCores();
    return (cores.first != cores.second);
}

} // namespace CppCommon
