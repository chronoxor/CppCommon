/*!
    \file environment.cpp
    \brief Environment management implementation
    \author Ivan Shynkarenka
    \date 27.07.2016
    \copyright MIT License
*/

#include "system/environment.h"

#include "string/encoding.h"
#include "utility/resource.h"

#include <codecvt>
#include <cstring>
#include <locale>
#include <sstream>

#if defined(__APPLE__)
#include <sys/sysctl.h>
extern char **environ;
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <sys/stat.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include <cstring>
#include <fstream>
#include <regex>
extern char **environ;
#endif
#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#include <windows.h>
#include <winternl.h>
#define STATUS_SUCCESS 0x00000000
#include <map>
#include <vector>
#endif

namespace CppCommon {

bool Environment::Is32BitOS()
{
    return !Is64BitOS();
}

bool Environment::Is64BitOS()
{
#if defined(__APPLE__)
    return true;
#elif defined(linux) || defined(__linux) || defined(__linux__)
    struct stat buffer;
    return (stat("/lib64/ld-linux-x86-64.so.2", &buffer) == 0);
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#if defined(_WIN64)
    return true;
#elif defined(_WIN32) || defined(__CYGWIN__)
    BOOL bWow64Process = FALSE;
    return IsWow64Process(GetCurrentProcess(), &bWow64Process) && bWow64Process;
#endif
#else
    #error Unsupported platform
#endif
}

bool Environment::Is32BitProcess()
{
    return !Is64BitProcess();
}

bool Environment::Is64BitProcess()
{
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
#if defined(__x86_64__) || defined(__amd64__) || defined(__aarch64__) || defined(__ia64__) || defined(__ppc64__)
    return true;
#else
    return false;
#endif
#elif defined(_WIN32) || defined(_WIN64)
#if defined(_WIN64)
    return true;
#elif defined(_WIN32)
    return false;
#endif
#else
    #error Unsupported platform
#endif
}

bool Environment::IsDebug()
{
    return !IsRelease();
}

bool Environment::IsRelease()
{
#if defined(NDEBUG)
    return true;
#else
    return false;
#endif
}

bool Environment::IsBigEndian()
{
    char16_t test = 0x0102;
    return ((char*)&test)[0] == 0x01;
}

bool Environment::IsLittleEndian()
{
    char16_t test = 0x0102;
    return ((char*)&test)[0] == 0x02;
}

std::string Environment::OSVersion()
{
#if defined(__APPLE__)
    char result[1024];
    size_t size = sizeof(result);
    if (sysctlbyname("kern.osrelease", result, &size, nullptr, 0) == 0)
        return result;

    return "<apple>";
#elif defined(__CYGWIN__)
    struct utsname name;
    if (uname(&name) == 0)
    {
        std::string result(name.sysname);
        result.append(" ");
        result.append(name.release);
        result.append(" ");
        result.append(name.version);
        return result;
    }

    return "<cygwin>";
#elif defined(linux) || defined(__linux) || defined(__linux__)
    static std::regex pattern("DISTRIB_DESCRIPTION=\"(.*)\"");

    std::string line;
    std::ifstream stream("/etc/lsb-release");
    while (getline(stream, line))
    {
        std::smatch matches;
        if (std::regex_match(line, matches, pattern))
            return matches[1];
    }

    return "<linux>";
#elif defined(_WIN32) || defined(_WIN64)
    static NTSTATUS(__stdcall *RtlGetVersion)(OUT PRTL_OSVERSIONINFOEXW lpVersionInformation) = (NTSTATUS(__stdcall*)(PRTL_OSVERSIONINFOEXW))GetProcAddress(GetModuleHandle("ntdll.dll"), "RtlGetVersion");
    static void(__stdcall *GetNativeSystemInfo)(OUT LPSYSTEM_INFO lpSystemInfo) = (void(__stdcall*)(LPSYSTEM_INFO))GetProcAddress(GetModuleHandle("kernel32.dll"), "GetNativeSystemInfo");
    static BOOL(__stdcall *GetProductInfo)(IN DWORD dwOSMajorVersion, IN DWORD dwOSMinorVersion, IN DWORD dwSpMajorVersion, IN DWORD dwSpMinorVersion, OUT PDWORD pdwReturnedProductType) = (BOOL(__stdcall*)(DWORD, DWORD, DWORD, DWORD, PDWORD))GetProcAddress(GetModuleHandle("kernel32.dll"), "GetProductInfo");

    OSVERSIONINFOEXW osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEXW));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXW);

    if (RtlGetVersion != nullptr)
    {
        NTSTATUS ntRtlGetVersionStatus = RtlGetVersion(&osvi);
        if (ntRtlGetVersionStatus != STATUS_SUCCESS)
            return "<windows>";
    }
    else
    {
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4996) // C4996: 'function': was declared deprecated
#endif
        BOOL bOsVersionInfoEx = GetVersionExW((OSVERSIONINFOW*)&osvi);
        if (bOsVersionInfoEx == 0)
            return "<windows>";
#if defined(_MSC_VER)
#pragma warning(pop)
#endif
    }

    SYSTEM_INFO si;
    ZeroMemory(&si, sizeof(SYSTEM_INFO));

    if (GetNativeSystemInfo != nullptr)
        GetNativeSystemInfo(&si);
    else
        GetSystemInfo(&si);

    if ((osvi.dwPlatformId != VER_PLATFORM_WIN32_NT) || (osvi.dwMajorVersion <= 4))
    {
        return "<windows>";
    }

    std::stringstream os;

    // Windows version
    os << "Microsoft ";
    if (osvi.dwMajorVersion >= 6)
    {
        if (osvi.dwMajorVersion == 10)
        {
            if (osvi.dwMinorVersion == 0)
            {
                if (osvi.wProductType == VER_NT_WORKSTATION)
                {
                    if (osvi.dwBuildNumber >= 22000)
                        os << "Windows 11 ";
                    else
                        os << "Windows 10 ";
                }
                else
                {
                    if (osvi.dwBuildNumber >= 20348)
                        os << "Windows Server 2022 ";
                    else if (osvi.dwBuildNumber >= 17763)
                        os << "Windows Server 2019 ";
                    else
                        os << "Windows Server 2016 ";
                }
            }
        }
        else if (osvi.dwMajorVersion == 6)
        {
            if (osvi.dwMinorVersion == 3)
            {
                if (osvi.wProductType == VER_NT_WORKSTATION)
                    os << "Windows 8.1 ";
                else
                    os << "Windows Server 2012 R2 ";
            }
            else if (osvi.dwMinorVersion == 2)
            {
                if (osvi.wProductType == VER_NT_WORKSTATION)
                    os << "Windows 8 ";
                else
                    os << "Windows Server 2012 ";
            }
            else if (osvi.dwMinorVersion == 1)
            {
                if (osvi.wProductType == VER_NT_WORKSTATION)
                    os << "Windows 7 ";
                else
                    os << "Windows Server 2008 R2 ";
            }
            else if (osvi.dwMinorVersion == 0)
            {
                if (osvi.wProductType == VER_NT_WORKSTATION)
                    os << "Windows Vista ";
                else
                    os << "Windows Server 2008 ";
            }
        }

        DWORD dwType;
        if ((GetProductInfo != nullptr) && GetProductInfo(osvi.dwMajorVersion, osvi.dwMinorVersion, 0, 0, &dwType))
        {
            switch (dwType)
            {
                case PRODUCT_ULTIMATE:
                    os << "Ultimate Edition";
                    break;
                case PRODUCT_PROFESSIONAL:
                    os << "Professional";
                    break;
                case PRODUCT_HOME_PREMIUM:
                    os << "Home Premium Edition";
                    break;
                case PRODUCT_HOME_BASIC:
                    os << "Home Basic Edition";
                    break;
                case PRODUCT_ENTERPRISE:
                    os << "Enterprise Edition";
                    break;
                case PRODUCT_BUSINESS:
                    os << "Business Edition";
                    break;
                case PRODUCT_STARTER:
                    os << "Starter Edition";
                    break;
                case PRODUCT_CLUSTER_SERVER:
                    os << "Cluster Server Edition";
                    break;
                case PRODUCT_DATACENTER_SERVER:
                    os << "Datacenter Edition";
                    break;
                case PRODUCT_DATACENTER_SERVER_CORE:
                    os << "Datacenter Edition (core installation)";
                    break;
                case PRODUCT_ENTERPRISE_SERVER:
                    os << "Enterprise Edition";
                    break;
                case PRODUCT_ENTERPRISE_SERVER_CORE:
                    os << "Enterprise Edition (core installation)";
                    break;
                case PRODUCT_ENTERPRISE_SERVER_IA64:
                    os << "Enterprise Edition for Itanium-based Systems";
                    break;
                case PRODUCT_SMALLBUSINESS_SERVER:
                    os << "Small Business Server";
                    break;
                case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:
                    os << "Small Business Server Premium Edition";
                    break;
                case PRODUCT_STANDARD_SERVER:
                    os << "Standard Edition";
                    break;
                case PRODUCT_STANDARD_SERVER_CORE:
                    os << "Standard Edition (core installation)";
                    break;
                case PRODUCT_WEB_SERVER:
                    os << "Web Server Edition";
                    break;
            }
        }
    }
    else if ((osvi.dwMajorVersion == 5) && (osvi.dwMinorVersion == 2))
    {
        if (GetSystemMetrics(SM_SERVERR2))
            os << "Windows Server 2003 R2, ";
        else if (osvi.wSuiteMask & VER_SUITE_STORAGE_SERVER)
            os << "Windows Storage Server 2003";
        else if (osvi.wSuiteMask & VER_SUITE_WH_SERVER)
            os << "Windows Home Server";
        else if ((osvi.wProductType == VER_NT_WORKSTATION) && (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64))
            os << "Windows XP Professional x64 Edition";
        else
            os << "Windows Server 2003, ";
        if (osvi.wProductType != VER_NT_WORKSTATION)
        {
            if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
            {
                if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
                    os << "Datacenter Edition for Itanium-based Systems";
                else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
                    os << "Enterprise Edition for Itanium-based Systems";
            }
            else if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
            {
                if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
                    os << "Datacenter x64 Edition";
                else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
                    os << "Enterprise x64 Edition";
                else
                    os << "Standard x64 Edition";
            }
            else
            {
                if (osvi.wSuiteMask & VER_SUITE_COMPUTE_SERVER)
                    os << "Compute Cluster Edition";
                else if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
                    os << "Datacenter Edition";
                else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
                    os << "Enterprise Edition";
                else if (osvi.wSuiteMask & VER_SUITE_BLADE)
                    os << "Web Edition";
                else
                    os << "Standard Edition";
            }
        }
    }
    else if ((osvi.dwMajorVersion == 5) && (osvi.dwMinorVersion == 1))
    {
        os << "Windows XP ";
        if (osvi.wSuiteMask & VER_SUITE_PERSONAL)
            os << "Home Edition";
        else
            os << "Professional";
    }
    else if ((osvi.dwMajorVersion == 5) && (osvi.dwMinorVersion == 0))
    {
        os << "Windows 2000 ";
        if (osvi.wProductType == VER_NT_WORKSTATION)
            os <<  "Professional";
        else
        {
            if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
                os << "Datacenter Server";
            else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
                os << "Advanced Server";
            else
                os << "Server";
        }
    }

    // Windows Service Pack version
    std::wstring sp_version(osvi.szCSDVersion);
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
    if (std::wcslen(osvi.szCSDVersion) > 0)
        os << " " << convert.to_bytes(sp_version.data(), sp_version.data() + sp_version.size());

    // Windows build
    os << " (build " << osvi.dwBuildNumber << ")";

    // Windows architecture
    if (osvi.dwMajorVersion >= 6)
    {
        if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)
            os << ", 32-bit";
        else if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
            os << ", 64-bit";
        else if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
            os << ", Intel Itanium";
        else if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_ARM)
            os << ", ARM";
#if !defined(__MINGW32__) && !defined(__MINGW64__)
        else if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_ARM64)
            os << ", ARM64";
#endif
    }

    return os.str();
#else
    #error Unsupported platform
#endif
}

std::string Environment::EndLine()
{
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    return "\n";
#elif defined(_WIN32) || defined(_WIN64)
    return "\r\n";
#else
    #error Unsupported platform
#endif
}

std::string Environment::UnixEndLine()
{
    return "\n";
}

std::string Environment::WindowsEndLine()
{
    return "\r\n";
}

std::map<std::string, std::string> Environment::envars()
{
    std::map<std::string, std::string> result;
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    for (char** envars = environ; *envars; ++envars)
    {
        char* envar = *envars;
        int offset = (envar[0] == '=') ? 1 : 0;
        char* separator = std::strchr(envar + offset, '=');
        std::string key(envar, separator - envar);

        char* pvalue = separator + 1;
        std::string value(pvalue);

        result[key] = value;
    }
#elif defined(_WIN32) || defined(_WIN64)
    // Smart resource cleaner pattern
    auto envars = resource(GetEnvironmentStringsW(), [](wchar_t* penv) { FreeEnvironmentStringsW(penv); });

    for (const wchar_t* envar = envars.get(); *envar != L'\0'; )
    {
        int offset = (envar[0] == '=') ? 1 : 0;
        const wchar_t* separator = std::wcschr(envar + offset, L'=');
        std::wstring key(envar, separator - envar);

        const wchar_t* pvalue = separator + 1;
        std::wstring value(pvalue);

        result[Encoding::ToUTF8(key)] = Encoding::ToUTF8(value);

        envar = pvalue + value.size() + 1;
    }
#endif
    return result;
}

std::string Environment::GetEnvar(const std::string name)
{
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    char* envar = getenv(name.c_str());
    return (envar != nullptr) ? std::string(envar) : std::string();
#elif defined(_WIN32) || defined(_WIN64)
    std::wstring wname = Encoding::FromUTF8(name);
    std::vector<wchar_t> buffer(MAX_PATH);

    DWORD size = GetEnvironmentVariableW(wname.c_str(), buffer.data(), (DWORD)buffer.size());
    if (size > buffer.size())
    {
        buffer.resize(size);
        size = GetEnvironmentVariableW(wname.c_str(), buffer.data(), (DWORD)buffer.size());
    }

    return (size > 0) ? Encoding::ToUTF8(std::wstring(buffer.data(), size)) : std::string();
#endif
}

void Environment::SetEnvar(const std::string name, const std::string value)
{
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    if (setenv(name.c_str(), value.c_str(), 1) != 0)
        throwex SystemException("Cannot set environment variable - " + name);
#elif defined(_WIN32) || defined(_WIN64)
    if (!SetEnvironmentVariableW(Encoding::FromUTF8(name).c_str(), Encoding::FromUTF8(value).c_str()))
        throwex SystemException("Cannot set environment variable - " + name);
#endif
}

void Environment::ClearEnvar(const std::string name)
{
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    if (unsetenv(name.c_str()) != 0)
        throwex SystemException("Cannot clear environment variable - " + name);
#elif defined(_WIN32) || defined(_WIN64)
    if (!SetEnvironmentVariableW(Encoding::FromUTF8(name).c_str(), nullptr))
        throwex SystemException("Cannot clear environment variable - " + name);
#endif
}

} // namespace CppCommon
