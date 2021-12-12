/*!
    \file process.cpp
    \brief Process implementation
    \author Ivan Shynkarenka
    \date 01.12.2016
    \copyright MIT License
*/

#include "system/process.h"

#include "errors/fatal.h"
#include "string/encoding.h"
#include "string/format.h"
#include "threads/thread.h"
#include "utility/resource.h"
#include "utility/validate_aligned_storage.h"

#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <tlhelp32.h>
#undef max
#undef min
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

    Impl(uint64_t pid)
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        _pid = (pid_t)pid;
#elif defined(_WIN32) || defined(_WIN64)
        _pid = (DWORD)pid;
        _process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_TERMINATE | SYNCHRONIZE, FALSE, _pid);
        if (_process == nullptr)
            throwex SystemException(format("Failed to open a process with Id {}!", pid));
#endif
    }

    ~Impl()
    {
#if defined(_WIN32) || defined(_WIN64)
        if (_process != nullptr)
        {
            if (!CloseHandle(_process))
                fatality(SystemException(format("Failed to close a process with Id {}!", pid())));
            _process = nullptr;
        }
#endif
    }

    uint64_t pid() const noexcept
    {
        return (uint64_t)_pid;
    }

    bool IsRunning() const
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        return (kill(_pid, 0) == 0);
#elif defined(_WIN32) || defined(_WIN64)
        if (_process == nullptr)
            throwex SystemException(format("Failed to get exit code for a process with Id {}!", pid()));

        DWORD dwExitCode;
        if (!GetExitCodeProcess(_process, &dwExitCode))
            throwex SystemException(format("Failed to get exit code for a process with Id {}!", pid()));

        return (dwExitCode == STILL_ACTIVE);
#endif
    }

    void Kill()
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        int result = kill(_pid, SIGKILL);
        if (result != 0)
            throwex SystemException(format("Failed to kill a process with Id {}!", pid()));
#elif defined(_WIN32) || defined(_WIN64)
        if (_process == nullptr)
            throwex SystemException(format("Failed to kill a process with Id {}!", pid()));

        if (!TerminateProcess(_process, 666))
            throwex SystemException(format("Failed to kill a process with Id {}!", pid()));
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
            throwex SystemException(format("Failed to wait for a process with Id {}!", pid()));

        if (WIFEXITED(status))
            return WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            throwex SystemException(format("Process with Id {} was killed by signal {}!", pid(), WTERMSIG(status)));
        else if (WIFSTOPPED(status))
            throwex SystemException(format("Process with Id {} was stopped by signal {}!", pid(), WSTOPSIG(status)));
        else if (WIFCONTINUED(status))
            throwex SystemException(format("Process with Id {} was continued by signal SIGCONT!", pid()));
        else
            throwex SystemException(format("Process with Id {} has unknown wait status!", pid()));
#elif defined(_WIN32) || defined(_WIN64)
        if (_process == nullptr)
            throwex SystemException(format("Failed to wait for a process with Id {}!", pid()));

        DWORD result = WaitForSingleObject(_process, INFINITE);
        if (result != WAIT_OBJECT_0)
            throwex SystemException(format("Failed to wait for a process with Id {}!", pid()));

        DWORD dwExitCode;
        if (!GetExitCodeProcess(_process, &dwExitCode))
            throwex SystemException(format("Failed to get exit code for a process with Id {}!", pid()));

        return (int)dwExitCode;
#endif
    }

    int WaitFor(const Timespan& timespan)
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        int status;
        pid_t result;

        do
        {
            result = waitpid(_pid, &status, WNOHANG);
            if (result == 0)
                Thread::Yield();
        }
        while ((result == 0) || ((result < 0) && (errno == EINTR)));

        if (result == -1)
            throwex SystemException(format("Failed to wait for a process with Id {}!", pid()));

        if (WIFEXITED(status))
            return WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            throwex SystemException(format("Process with Id {} was killed by signal {}!", pid(), WTERMSIG(status)));
        else if (WIFSTOPPED(status))
            throwex SystemException(format("Process with Id {} was stopped by signal {}!", pid(), WSTOPSIG(status)));
        else if (WIFCONTINUED(status))
            throwex SystemException(format("Process with Id {} was continued by signal SIGCONT!", pid()));
        else
            throwex SystemException(format("Process with Id {} has unknown wait status!", pid()));
#elif defined(_WIN32) || defined(_WIN64)
        if (_process == nullptr)
            throwex SystemException(format("Failed to wait for a process with Id {}!", pid()));

        DWORD result = WaitForSingleObject(_process, std::max((DWORD)1, (DWORD)timespan.milliseconds()));
        if (result != WAIT_OBJECT_0)
            return std::numeric_limits<int>::min();

        DWORD dwExitCode;
        if (!GetExitCodeProcess(_process, &dwExitCode))
            throwex SystemException(format("Failed to get exit code for a process with Id {}!", pid()));

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

    static Process Execute(const std::string& command, const std::vector<std::string>* arguments, const std::map<std::string, std::string>* envars, const std::string* directory, Pipe* input, Pipe* output, Pipe* error)
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        // Prepare arguments
        size_t index = 0;
        std::vector<char*> argv(1 + ((arguments != nullptr) ? arguments->size() : 0) + 1);
        argv[index++] = (char*)command.c_str();
        if (arguments != nullptr)
        {
            for (const auto& argument : *arguments)
            {
                argv[index++] = (char*)argument.c_str();
            }
        }
        argv[index++] = nullptr;

        // Prepare environment variables
        std::vector<char> environment = PrepareEnvars(envars);

        // Fork the current process
        pid_t pid = fork();
        if (pid < 0)
            throwex SystemException("Failed to fork the current process!");
        else if (pid == 0)
        {
            // Set environment variables of the new process
            if (!environment.empty())
            {
                char* envar = environment.data();
                while (*envar != '\0')
                {
                    putenv(envar);
                    while (*envar != '\0')
                        ++envar;
                    ++envar;
                }
            }

            // Change the current directory of the new process
            if (directory != nullptr)
            {
                int result = chdir(directory->c_str());
                if (result != 0)
                    _exit(666);
            }

            // Prepare input communication pipe
            if (input != nullptr)
                dup2((int)(size_t)input->reader(), STDIN_FILENO);

            // Prepare output communication pipe
            if (output != nullptr)
                dup2((int)(size_t)output->writer(), STDOUT_FILENO);

            // Prepare error communication pipe
            if (error != nullptr)
                dup2((int)(size_t)error->writer(), STDERR_FILENO);

            // Close pipes endpoints
            if (input != nullptr)
                input->Close();
            if (output != nullptr)
                output->Close();
            if (error != nullptr)
                error->Close();

            // Close all open file descriptors other than stdin, stdout, stderr
            for (int i = 3; i < sysconf(_SC_OPEN_MAX); ++i)
                close(i);

            // Execute a new process image
            execvp(argv[0], argv.data());

            // Get here only if error occurred during image execution
            _exit(666);
        }

        // Close pipes endpoints
        if (input != nullptr)
            input->CloseRead();
        if (output != nullptr)
            output->CloseWrite();
        if (error != nullptr)
            error->CloseWrite();

        // Return result process
        Process result;
        result.impl()._pid = pid;
        return result;
#elif defined(_WIN32) || defined(_WIN64)
        BOOL bInheritHandles = FALSE;

        // Prepare command line
        std::wstring command_line = Encoding::FromUTF8(command);
        if (arguments != nullptr)
        {
            for (const auto& argument : *arguments)
            {
                command_line.append(L" ");
                command_line.append(Encoding::FromUTF8(argument));
            }
        }

        // Prepare environment variables
        std::vector<wchar_t> environment = PrepareEnvars(envars);

        // Fill process startup information
        STARTUPINFOW si;
        GetStartupInfoW(&si);
        si.cb = sizeof(STARTUPINFOW);
        si.lpReserved = nullptr;
        si.lpDesktop = nullptr;
        si.lpTitle = nullptr;
        si.dwFlags = STARTF_FORCEOFFFEEDBACK;
        si.cbReserved2 = 0;
        si.lpReserved2 = nullptr;

        // Get the current process handle
        HANDLE hCurrentProcess = GetCurrentProcess();

        // Prepare input communication pipe
        if (input != nullptr)
            DuplicateHandle(hCurrentProcess, input->reader(), hCurrentProcess, &si.hStdInput, 0, TRUE, DUPLICATE_SAME_ACCESS);
        else
        {
            HANDLE hStdHandle = GetStdHandle(STD_INPUT_HANDLE);
            if (hStdHandle != nullptr)
                DuplicateHandle(hCurrentProcess, hStdHandle, hCurrentProcess, &si.hStdInput, 0, TRUE, DUPLICATE_SAME_ACCESS);
            else
                si.hStdInput = nullptr;
        }

        // Prepare output communication pipe
        if (output != nullptr)
            DuplicateHandle(hCurrentProcess, output->writer(), hCurrentProcess, &si.hStdOutput, 0, TRUE, DUPLICATE_SAME_ACCESS);
        else
        {
            HANDLE hStdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
            if (hStdHandle != nullptr)
                DuplicateHandle(hCurrentProcess, hStdHandle, hCurrentProcess, &si.hStdOutput, 0, TRUE, DUPLICATE_SAME_ACCESS);
            else
                si.hStdOutput = nullptr;
        }

        // Prepare error communication pipe
        if (error != nullptr)
            DuplicateHandle(hCurrentProcess, error->writer(), hCurrentProcess, &si.hStdError, 0, TRUE, DUPLICATE_SAME_ACCESS);
        else
        {
            HANDLE hStdHandle = GetStdHandle(STD_ERROR_HANDLE);
            if (hStdHandle != nullptr)
                DuplicateHandle(hCurrentProcess, hStdHandle, hCurrentProcess, &si.hStdError, 0, TRUE, DUPLICATE_SAME_ACCESS);
            else
                si.hStdError = nullptr;
        }

        // Close pipes endpoints
        if (input != nullptr)
            input->CloseRead();
        if (output != nullptr)
            output->CloseWrite();
        if (error != nullptr)
            error->CloseWrite();

        // Override standard communication pipes of the process
        if ((si.hStdInput != nullptr) || (si.hStdOutput != nullptr) || (si.hStdError != nullptr))
        {
            bInheritHandles = TRUE;
            si.dwFlags |= STARTF_USESTDHANDLES;
        }

        // Create a new process
        PROCESS_INFORMATION pi;
        if (!CreateProcessW(nullptr, (wchar_t*)command_line.c_str(), nullptr, nullptr, bInheritHandles, CREATE_UNICODE_ENVIRONMENT, environment.empty() ? nullptr : (LPVOID)environment.data(), (directory == nullptr) ? nullptr : Encoding::FromUTF8(*directory).c_str(), &si, &pi))
            throwex SystemException(CppCommon::format("Failed to execute a new process with command '{}'!", command));

        // Close standard handles
        if (si.hStdInput != nullptr)
            CloseHandle(si.hStdInput);
        if (si.hStdOutput != nullptr)
            CloseHandle(si.hStdOutput);
        if (si.hStdError != nullptr)
            CloseHandle(si.hStdError);

        // Close thread handle
        CloseHandle(pi.hThread);

        // Return result process
        Process result;
        result.impl()._pid = pi.dwProcessId;
        result.impl()._process = pi.hProcess;
        return result;
#endif
    }

#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    static std::vector<char> PrepareEnvars(const std::map<std::string, std::string>* envars)
#elif defined(_WIN32) || defined(_WIN64)
    static std::vector<wchar_t> PrepareEnvars(const std::map<std::string, std::string>* envars)
#endif
    {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        std::vector<char> result;
#elif defined(_WIN32) || defined(_WIN64)
        std::vector<wchar_t> result;
#endif

        if (envars == nullptr)
            return result;

        for (const auto& envar : *envars)
        {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
            std::string key = envar.first;
            std::string value = envar.second;
#elif defined(_WIN32) || defined(_WIN64)
            std::wstring key = Encoding::FromUTF8(envar.first);
            std::wstring value = Encoding::FromUTF8(envar.second);
#endif
            result.insert(result.end(), key.begin(), key.end());
            result.insert(result.end(), '=');
            result.insert(result.end(), value.begin(), value.end());
            result.insert(result.end(), '\0');
        }
        result.insert(result.end(), '\0');

        return result;
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

Process::Process()
{
    // Check implementation storage parameters
    [[maybe_unused]] ValidateAlignedStorage<sizeof(Impl), alignof(Impl), StorageSize, StorageAlign> _;
    static_assert((StorageSize >= sizeof(Impl)), "Process::StorageSize must be increased!");
    static_assert(((StorageAlign % alignof(Impl)) == 0), "Process::StorageAlign must be adjusted!");

    // Create the implementation instance
    new(&_storage)Impl();
}

Process::Process(uint64_t id)
{
    // Check implementation storage parameters
    [[maybe_unused]] ValidateAlignedStorage<sizeof(Impl), alignof(Impl), StorageSize, StorageAlign> _;
    static_assert((StorageSize >= sizeof(Impl)), "Process::StorageSize must be increased!");
    static_assert(((StorageAlign % alignof(Impl)) == 0), "Process::StorageAlign must be adjusted!");

    // Create the implementation instance
    new(&_storage)Impl(id);
}

Process::Process(const Process& process)
{
    // Check implementation storage parameters
    [[maybe_unused]] ValidateAlignedStorage<sizeof(Impl), alignof(Impl), StorageSize, StorageAlign> _;
    static_assert((StorageSize >= sizeof(Impl)), "Process::StorageSize must be increased!");
    static_assert(((StorageAlign % alignof(Impl)) == 0), "Process::StorageAlign must be adjusted!");

    // Create the implementation instance
    new(&_storage)Impl(process.pid());
}

Process::Process(Process&& process) noexcept : Process()
{
    process.swap(*this);
}

Process::~Process()
{
    // Delete the implementation instance
    reinterpret_cast<Impl*>(&_storage)->~Impl();
}

Process& Process::operator=(const Process& process)
{
    Process(process).swap(*this);
    return *this;
}

Process& Process::operator=(Process&& process) noexcept
{
    Process(std::move(process)).swap(*this);
    return *this;
}

uint64_t Process::pid() const noexcept { return impl().pid(); }

bool Process::IsRunning() const { return impl().IsRunning(); }

void Process::Kill() { return impl().Kill(); }
int Process::Wait() { return impl().Wait(); }
int Process::WaitFor(const Timespan& timespan) { return impl().WaitFor(timespan); }

uint64_t Process::CurrentProcessId() noexcept { return Impl::CurrentProcessId(); }
uint64_t Process::ParentProcessId() noexcept { return Impl::ParentProcessId(); }

void Process::Exit(int result) { return Impl::Exit(result); }

Process Process::Execute(const std::string& command, const std::vector<std::string>* arguments, const std::map<std::string, std::string>* envars, const std::string* directory, Pipe* input, Pipe* output, Pipe* error)
{
    return Impl::Execute(command, arguments, envars, directory, input, output, error);
}

void Process::swap(Process& process) noexcept
{
    using std::swap;
    swap(_storage, process._storage);
}

} // namespace CppCommon
