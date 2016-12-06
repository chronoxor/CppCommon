/*!
    \file process.h
    \brief Process definition
    \author Ivan Shynkarenka
    \date 01.12.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_SYSTEM_PROCESS_H
#define CPPCOMMON_SYSTEM_PROCESS_H

#include <memory>

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
    Process(const Process&) = delete;
    Process(Process&&) noexcept;
    ~Process();

    Process& operator=(const Process&) = delete;
    Process& operator=(Process&&) noexcept;

    //! Get the process Id
    uint64_t id() const noexcept;

    //! Is the process is running?
    bool IsRunning() const;

    //! Kill the process
    void Kill();

    //! Wait for the process to exit
    /*!
        Will block.

        \return Process exit result
    */
    int Wait();

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

    //! Swap two instances
    void swap(Process& process) noexcept;
    friend void swap(Process& process1, Process& process2) noexcept;

private:
    class Impl;
    std::unique_ptr<Impl> _pimpl;

    Process();
    Process(uint64_t id);
};

/*! \example system_process.cpp Process abstraction example */

} // namespace CppCommon

#include "process.inl"

#endif // CPPCOMMON_SYSTEM_PROCESS_H
