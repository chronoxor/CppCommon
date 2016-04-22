/*!
    \file exceptions_handler.h
    \brief Exceptions handler definition
    \author Ivan Shynkarenka
    \date 22.04.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_ERRORS_EXCEPTIONS_HANDLER_H
#define CPPCOMMON_ERRORS_EXCEPTIONS_HANDLER_H

#include <memory>

namespace CppCommon {

//! Exceptions handler
/*!
    Exceptions handler allows to setup special handlers for all process and thread exceptions.
    This allows to catch different kinds of unhandled exceptions, signals, process abort and
    termination. As the result corresponding exception will be printed in std::cerr with full
    stack-trace and dump file will be created.

    Not thread-safe.
*/
class ExceptionsHandler
{
public:
    ExceptionsHandler() = delete;
    ExceptionsHandler(const ExceptionsHandler&) = delete;
    ExceptionsHandler(ExceptionsHandler&&) = delete;
    virtual ~ExceptionsHandler() = delete;

    ExceptionsHandler& operator=(const ExceptionsHandler&) = delete;
    ExceptionsHandler& operator=(ExceptionsHandler&&) = delete;

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
    static std::unique_ptr<Impl> _pimpl;
};

/*! \example errors_exceptions_handler.cpp Exceptions handler example */

} // namespace CppCommon

#endif // CPPCOMMON_ERRORS_EXCEPTIONS_HANDLER_H
