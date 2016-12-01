/*!
    \file process.cpp
    \brief Process implementation
    \author Ivan Shynkarenka
    \date 01.12.2016
    \copyright MIT License
*/

#include "system/process.h"

#include "errors/exceptions.h"
#include "errors/fatal.h"

#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
#include <unistd.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <tlhelp32.h>
#endif

namespace CppCommon {

//! @cond INTERNALS

class Process::Impl
{
public:
    Impl()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        _process = -1;
#elif defined(_WIN32) || defined(_WIN64)
        _process = INVALID_HANDLE_VALUE;
#endif
    }

    ~Impl()
    {
    }

    static uint64_t CurrentProcessId() noexcept
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        return (uint64_t)getpid();
#elif defined(_WIN32) || defined(_WIN64)
        return (uint64_t)GetCurrentProcessId();
#endif
    }

    static uint64_t ParentProcessId() noexcept
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        return (uint64_t)getppid();
#elif defined(_WIN32) || defined(_WIN64)
        DWORD parent = 0;
        DWORD current = GetCurrentProcessId();

        // Takes a snapshot of the specified processes
        PROCESSENTRY32 pe = { 0 };
        pe.dwSize = sizeof(PROCESSENTRY32);
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnapshot == INVALID_HANDLE_VALUE)
            return 0;

        // Retrieves information about all processes encountered in a system snapshot
        if (Process32First(hSnapshot, &pe))
        {
            do
            {
                if (pe.th32ProcessID == current)
                {
                    parent = pe.th32ParentProcessID;
                    break;
                }
            } while(Process32Next(hSnapshot, &pe));
        }

        // Release the snapshot of the specified processes
        CloseHandle(hSnapshot);

        return (uint64_t)parent;
#endif
    }

private:
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    pid_t _process;
#elif defined(_WIN32) || defined(_WIN64)
    HANDLE _process;
#endif
};

//! @endcond

Process::Process() : _pimpl(new Impl())
{
}

Process::Process(Process&& process) noexcept : _pimpl(std::move(process._pimpl))
{
}

Process::~Process()
{
}

Process& Process::operator=(Process&& process) noexcept
{
    _pimpl = std::move(process._pimpl);
    return *this;
}

uint64_t Process::CurrentProcessId() noexcept
{
    return Impl::CurrentProcessId();
}

uint64_t Process::ParentProcessId() noexcept
{
    return Impl::ParentProcessId();
}

} // namespace CppCommon
