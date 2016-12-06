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
        _process = nullptr;
#endif
    }

    Impl(uint64_t id)
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        _pid = (pid_t)id;
#elif defined(_WIN32) || defined(_WIN64)
        _pid = (DWORD)id;
        _process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_TERMINATE | SYNCHRONIZE, FALSE, _pid);
        if (_process == nullptr)
            throwex SystemException("Failed to open a process with Id {}!"_format(id));
#endif
    }

    ~Impl()
    {
#if defined(_WIN32) || defined(_WIN64)
        if (_process != nullptr)
        {
            if (!CloseHandle(_process))
                fatality(SystemException("Failed to close a process with Id {}!"_format(id())));
            _process = nullptr;
        }
#endif
    }

    uint64_t id() const noexcept
    {
        return (uint64_t)_pid;
    }

    bool IsRunning() const
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        return (kill(_pid, 0) == 0);
#elif defined(_WIN32) || defined(_WIN64)
        if (_process == nullptr)
            throwex SystemException("Failed to get exit code for a process with Id {}!"_format(id()));

        DWORD dwExitCode;
        if (!GetExitCodeProcess(_process, &dwExitCode))
            throwex SystemException("Failed to get exit code for a process with Id {}!"_format(id()));

        return (dwExitCode == STILL_ACTIVE);
#endif
    }

    void Kill()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        int result = kill(_pid, SIGKILL);
        if (result != 0)
            throwex SystemException("Failed to kill a process with Id {}!"_format(id()));
#elif defined(_WIN32) || defined(_WIN64)
        if (_process == nullptr)
            throwex SystemException("Failed to kill a process with Id {}!"_format(id()));

        if (!TerminateProcess(_process, 1))
            throwex SystemException("Failed to kill a process with Id {}!"_format(id()));
#endif
    }

    int Wait()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        int status;
        pid_t result;

        do
        {
            result = waitpid(_pid, &status, 0);
        }
        while ((result < 0) && (errno == EINTR));

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
        if (_process == nullptr)
            throwex SystemException("Failed to wait for a process with Id {}!"_format(id()));

        DWORD result = WaitForSingleObject(_process, INFINITE);
        if (result != WAIT_OBJECT_0)
            throwex SystemException("Failed to wait for a process with Id {}!"_format(id()));

        DWORD dwExitCode;
        if (!GetExitCodeProcess(_process, &dwExitCode))
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
    HANDLE _process;
#endif
};

//! @endcond

Process::Process() : _pimpl(std::make_unique<Impl>())
{
}

Process::Process(uint64_t id) : _pimpl(std::make_unique<Impl>(id))
{
}

Process::Process(const Process& process) : _pimpl(std::make_unique<Impl>(process._pimpl->id()))
{
}

Process::Process(Process&& process) noexcept : _pimpl(std::move(process._pimpl))
{
}

Process::~Process()
{
}

Process& Process::operator=(const Process& process)
{
    _pimpl = std::make_unique<Impl>(process._pimpl->id());
    return *this;
}

Process& Process::operator=(Process&& process) noexcept
{
    _pimpl = std::move(process._pimpl);
    return *this;
}

uint64_t Process::id() const noexcept
{
    return _pimpl->id();
}

bool Process::IsRunning() const
{
    return _pimpl->IsRunning();
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
