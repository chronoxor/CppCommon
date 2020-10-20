/*!
    \file exceptions_handler.h
    \brief Exceptions handler definition
    \author Ivan Shynkarenka
    \date 22.04.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_ERRORS_EXCEPTIONS_HANDLER_H
#define CPPCOMMON_ERRORS_EXCEPTIONS_HANDLER_H

#include "filesystem/exceptions.h"
#include "system/stack_trace.h"
#include "utility/singleton.h"

#include <cassert>
#include <functional>
#include <memory>

namespace CppCommon {

//! Exceptions handler
/*!
    Exceptions handler allows to setup special handlers for all process and thread exceptions.
    This allows to catch different kinds of unhandled exceptions, signals, process abort and
    termination. As the result corresponding exception will be routed to the global exceptions
    handler function (default one will print the exception in std::cerr with a full stack-trace)
    and the dump file will be created.

    Not thread-safe.
*/
class ExceptionsHandler : public CppCommon::Singleton<ExceptionsHandler>
{
   friend Singleton<ExceptionsHandler>;

public:
    ExceptionsHandler(const ExceptionsHandler&) = delete;
    ExceptionsHandler(ExceptionsHandler&&) = delete;
    ~ExceptionsHandler();

    ExceptionsHandler& operator=(const ExceptionsHandler&) = delete;
    ExceptionsHandler& operator=(ExceptionsHandler&&) = delete;

    //! Setup new global exceptions handler function
    /*!
        This method should be called once for the current process.
        It is recommended to call the method just after the current process start!

        \param handler - Exceptions handler function
    */
    static void SetupHandler(const std::function<void (const SystemException&, const StackTrace&)>& handler);
    //! Setup exceptions handler for the current process
    /*!
        This method should be called once for the current process.
        It is recommended to call the method just after the current process start!
    */
    static void SetupProcess();
    //! Setup exceptions handler for the current thread
    /*!
        This method should be called once for the current thread.
        It is recommended to call the method just after the current thread start!
    */
    static void SetupThread();

private:
    class Impl;

    Impl& impl() noexcept { return reinterpret_cast<Impl&>(_storage); }
    const Impl& impl() const noexcept { return reinterpret_cast<Impl const&>(_storage); }

    static const size_t StorageSize = 72;
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    static const size_t StorageAlign = 16;
#else
    static const size_t StorageAlign = 8;
#endif
    std::aligned_storage<StorageSize, StorageAlign>::type _storage;

    ExceptionsHandler();
};

/*! \example errors_exceptions_handler.cpp Exceptions handler example */

} // namespace CppCommon

#endif // CPPCOMMON_ERRORS_EXCEPTIONS_HANDLER_H
