/*!
    \file stack_trace_manager.h
    \brief Stack trace manager definition
    \author Ivan Shynkarenka
    \date 10.02.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_SYSTEM_STACK_TRACE_MANAGER_H
#define CPPCOMMON_SYSTEM_STACK_TRACE_MANAGER_H

#include "errors/exceptions.h"
#include "utility/singleton.h"

#include <memory>

namespace CppCommon {

//! Stack trace manager
/*!
    Provides interface to initialize and cleanup stack trace snapshots capturing.

    Not thread-safe.
*/
class StackTraceManager : public CppCommon::Singleton<StackTraceManager>
{
   friend Singleton<StackTraceManager>;

public:
    StackTraceManager(const StackTraceManager&) = delete;
    StackTraceManager(StackTraceManager&&) = delete;
    ~StackTraceManager();

    StackTraceManager& operator=(const StackTraceManager&) = delete;
    StackTraceManager& operator=(StackTraceManager&&) = delete;

    //! Initialize stack trace manager
    /*!
        This method should be called before you start capture any stack trace snapshots.
        It is recommended to call the method just after the current process start!
    */
    static void Initialize();
    //! Cleanup stack trace manager
    /*!
        This method should be called just before the current process exits!
    */
    static void Cleanup();

private:
    class Impl;
    std::unique_ptr<Impl> _pimpl;

    StackTraceManager();
};

} // namespace CppCommon

#endif // CPPCOMMON_SYSTEM_STACK_TRACE_MANAGER_H
