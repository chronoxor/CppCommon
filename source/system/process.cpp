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
#include "string/format.h"
#include "utility/resource.h"

#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
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
        _pid = (pid_t)-1;
#elif defined(_WIN32) || defined(_WIN64)
        _pid = (DWORD)-1;
#endif
    }

    Impl(uint64_t id)
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        _pid = (pid_t)id;
#elif defined(_WIN32) || defined(_WIN64)
        _pid = (DWORD)id;
#endif
    }

    ~Impl()
    {
    }

    uint64_t id() noexcept
    {
        return (uint64_t)_pid;
    }

    void Kill()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        int result = kill(_pid, SIGKILL);
        if (result != 0)
            throwex SystemException("Failed to kill a process with Id {}!"_format(id()));
#elif defined(_WIN32) || defined(_WIN64)
        HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, _pid);
        if (hProcess == nullptr)
            throwex SystemException("Failed to open a process with Id {}!"_format(id()));

        // Smart resource cleaner pattern
        auto process = resource(hProcess, [](HANDLE hObject) { CloseHandle(hObject); });

        if (!TerminateProcess(process.get(), 1))
            throwex SystemException("Failed to kill a process with Id {}!"_format(id()));
#endif
    }

    int Wait()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        int status;
        pid_t result = waitpid(_pid, &status, 0);
        if (result == -1)
            throwex SystemException("Failed to wait for a process with Id {}!"_format(id()));

        if (WIFEXITED(status))
            return WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            throwex SystemException("Process with Id {} was killed by signal {}!"_format(id(), WTERMSIG(status)));
        else if (WIFSTOPPED(status))
            throwex SystemException("Process with Id {} was stopped by signal {}!"_format(id(), WSTOPSIG(status)));
        else if (WIFCONTINUED(status))
            throwex SystemException("Process with Id {} was continued by signal SIGCONT!"_format(id()));
        else
            throwex SystemException("Process with Id {} has unknown wait status!"_format(id()));
#elif defined(_WIN32) || defined(_WIN64)
        HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, _pid);
        if (hProcess == nullptr)
            throwex SystemException("Failed to open a process with Id {}!"_format(id()));

        // Smart resource cleaner pattern
        auto process = resource(hProcess, [](HANDLE hObject) { CloseHandle(hObject); });

        DWORD result = WaitForSingleObject(process.get(), INFINITE);
        if (result != WAIT_OBJECT_0)
            throwex SystemException("Failed to wait for a process with Id {}!"_format(id()));

        DWORD dwExitCode;
        if (!GetExitCodeProcess(process.get(), &dwExitCode))
            throwex SystemException("Failed to get exit code for a process with Id {}!"_format(id()));

        return (int)dwExitCode;
#endif
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
        DWORD current = GetCurrentProcessId();

        // Takes a snapshot of the specified processes
        PROCESSENTRY32 pe = { 0 };
        pe.dwSize = sizeof(PROCESSENTRY32);
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnapshot == INVALID_HANDLE_VALUE)
            return (uint64_t)-1;

        // Smart resource cleaner pattern
        auto snapshot = resource(hSnapshot, [](HANDLE hObject) { CloseHandle(hObject); });

        // Retrieves information about all processes encountered in a system snapshot
        if (Process32First(snapshot.get(), &pe))
        {
            do
            {
                if (pe.th32ProcessID == current)
                    return (uint64_t)pe.th32ParentProcessID;
            } while(Process32Next(snapshot.get(), &pe));
        }

        return (uint64_t)-1;
#endif
    }

    static void Exit(int result)
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        exit(result);
#elif defined(_WIN32) || defined(_WIN64)
        ExitProcess((UINT)result);
#endif
    }

private:
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    pid_t _pid;
#elif defined(_WIN32) || defined(_WIN64)
    DWORD _pid;
#endif
};

//! @endcond

Process::Process() : _pimpl(new Impl())
{
}

Process::Process(uint64_t id) : _pimpl(new Impl(id))
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

uint64_t Process::id() noexcept
{
    return _pimpl->id();
}

void Process::Kill()
{
    return _pimpl->Kill();
}

int Process::Wait()
{
    return _pimpl->Wait();
}

uint64_t Process::CurrentProcessId() noexcept
{
    return Impl::CurrentProcessId();
}

uint64_t Process::ParentProcessId() noexcept
{
    return Impl::ParentProcessId();
}

void Process::Exit(int result)
{
    return Impl::Exit(result);
}

} // namespace CppCommon
