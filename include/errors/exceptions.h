/*!
    \file exceptions.h
    \brief Exceptions definition
    \author Ivan Shynkarenka
    \date 09.02.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_EXCEPTIONS_H
#define CPPCOMMON_EXCEPTIONS_H

#include "debug/call_stack.h"
#include "debug/source_location.h"

#include <exception> 

//! Throw extended exception macro
/*!
    Macro with throw extended exception with the current location and call stack.
*/
#define throwex throw __LOCATION__ + __CALL_STACK__ +

namespace CppCommon {

//! Exception
/*!
    Exception base interface.
*/
class Exception : public std::exception
{
public:
    //! Default class constructor
    /*!
        \param message - Exception message (default is "")
    */
    Exception(const std::string& message = "") : _message(message) {}
    Exception(const Exception&) = default;
    Exception(Exception&&) = default;
    virtual ~Exception() = default;

    Exception& operator=(const Exception&) = default;
    Exception& operator=(Exception&&) = default;

    //! Get exception message
    const std::string& message() const noexcept { return _message; }

    //! Output exception into the given output stream
    friend std::ostream& operator<< (std::ostream& os, const Exception& instance)
    { os << instance.message(); return os; }

    //! Insert source location into the given exception
    template<class T>
    friend T& operator+(const SourceLocation& location, T& instance)
    { instance._message.append("\nLocation: " + location.to_string()); return instance; }

    //! Insert call stack into the given exception
    template<class T>
    friend T& operator+(const CallStack& trace, T& instance)
    { instance._message.append("\nCall stack:\n" + trace.to_string()); return instance; }

    //! Get string identifying exception
    virtual const char* what() const throw() { return _message.c_str(); }

protected:
    std::string _message;
};

//! Runtime exception
class RuntimeException : public Exception {};

//! System exception
/*!
    System exception provides some additional information such as system error code and error description.
*/
class SystemException : public Exception
{
public:
    //! Default class constructor
    /*!
        \param message - Exception message (default is "")
    */
    SystemException(const std::string& message = "");
    //! Create system exception based on the given system error code
    /*!
        \param error - System error code
        \param message - Exception message (default is "")
    */
    SystemException(int error, const std::string& message = "");

    //! Get system error code
    int system_error() const noexcept { return _system_error; }
    //! Get system error message
    const std::string& system_message() const noexcept { return _system_message; }

private:
    int _system_error;
    std::string _system_message;
};

} // namespace CppCommon

#endif //CPPCOMMON_EXCEPTIONS_H
