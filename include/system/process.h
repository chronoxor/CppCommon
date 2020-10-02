/*!
    \file process.h
    \brief Process definition
    \author Ivan Shynkarenka
    \date 01.12.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_SYSTEM_PROCESS_H
#define CPPCOMMON_SYSTEM_PROCESS_H

#include "errors/exceptions.h"
#include "system/pipe.h"
#include "time/timestamp.h"

#include <limits>
#include <map>
#include <memory>
#include <string>
#include <vector>

//! Current process Id macro
/*!
    Get the current process Id.
*/
#define __PROCESS__ CppCommon::Process::CurrentProcessId()

namespace CppCommon {

//! Process abstraction
/*!
    Process contains different kinds of process manipulation functionality such as
    retrive the current process Id, execute or terminate one.

    Thread-safe.
*/
class Process
{
public:
    Process(const Process& process);
    Process(Process&& process) noexcept;
    ~Process();

    Process& operator=(const Process& process);
    Process& operator=(Process&& process) noexcept;

    //! Get the process Id
    uint64_t pid() const noexcept;

    //! Is the process is running?
    bool IsRunning() const;

    //! Kill the process
    void Kill();

    //! Wait the process to exit
    /*!
        Will block.

        \return Process exit result
    */
    int Wait();

    //! Wait the process to exit for the given timespan
    /*!
        Will block for the given timespan in the worst case.

        \param timespan - Timespan to wait for the process
        \return Process exit result (std::numeric_limits<int>::min() in case of timeout)
    */
    int WaitFor(const Timespan& timespan);
    //! Wait the process to exit until the given timestamp
    /*!
        Will block until the given timestamp in the worst case.

        \param timestamp - Timestamp to stop wait for the process
        \return Process exit result (std::numeric_limits<int>::min() in case of timeout)
    */
    int WaitUntil(const UtcTimestamp& timestamp)
    { return WaitFor(timestamp - UtcTimestamp()); }

    //! Get the current process Id
    /*!
        \return Current process Id
    */
    static uint64_t CurrentProcessId() noexcept;
    //! Get the parent process Id
    /*!
        \return Parent process Id
    */
    static uint64_t ParentProcessId() noexcept;

    //! Get the current process
    /*!
        \return Current process
    */
    static Process CurrentProcess();
    //! Get the parent process
    /*!
        \return Parent process
    */
    static Process ParentProcess();

    //! Exit the current process
    /*!
        \param result - Result is returned to the parent
    */
    static void Exit(int result);

    //! Execute a new process
    /*!
        If input/output/error communication pipe is not provided then
        new process will use equivalent standard stream of the parent
        process.

        \param command - Command to execute
        \param arguments - Pointer to arguments vector (default is nullptr)
        \param envars - Pointer to environment variables map (default is nullptr)
        \param directory - Initial working directory (default is nullptr)
        \param input - Input communication pipe (default is nullptr)
        \param output - Output communication pipe (default is nullptr)
        \param error - Error communication pipe (default is nullptr)
        \return Created process
    */
    static Process Execute(const std::string& command, const std::vector<std::string>* arguments = nullptr, const std::map<std::string, std::string>* envars = nullptr, const std::string* directory = nullptr, Pipe* input = nullptr, Pipe* output = nullptr, Pipe* error = nullptr);

    //! Swap two instances
    void swap(Process& process) noexcept;
    friend void swap(Process& process1, Process& process2) noexcept;

private:
    class Impl;

    Impl& impl() noexcept { return reinterpret_cast<Impl&>(_storage); }
    const Impl& impl() const noexcept { return reinterpret_cast<Impl const&>(_storage); }

    static const size_t StorageSize = 16;
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    static const size_t StorageAlign = 4;
#else
    static const size_t StorageAlign = 8;
#endif
    std::aligned_storage<StorageSize, StorageAlign>::type _storage;

    Process();
    Process(uint64_t pid);
};

/*! \example system_process.cpp Process example */
/*! \example system_process_pipes.cpp Process pipes example */

} // namespace CppCommon

#include "process.inl"

#endif // CPPCOMMON_SYSTEM_PROCESS_H
