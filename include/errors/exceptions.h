/*!
    \file exceptions.h
    \brief Exceptions definition
    \author Ivan Shynkarenka
    \date 09.02.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_ERRORS_EXCEPTIONS_H
#define CPPCOMMON_ERRORS_EXCEPTIONS_H

#include "errors/system_error.h"
#include "system/source_location.h"

#include <exception>
#include <string>
#include <utility>

//! Throw extended exception macro
/*!
    Throw extended exception with the current location.
*/
#define throwex throw __LOCATION__ +

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
    Exception(const std::string& message = "") : _message(message), _location() {}
    Exception(const Exception&) = default;
    Exception(Exception&&) = default;
    virtual ~Exception() = default;

    Exception& operator=(const Exception&) = default;
    Exception& operator=(Exception&&) = default;

    //! Get exception message
    const std::string& message() const noexcept { return _message; }
    //! Get exception location
    const SourceLocation& location() const noexcept { return _location; }

    //! Get string identifying exception
    const char* what() const noexcept override;

    //! Get string from the current exception
    virtual std::string to_string() const;

    //! Output exception into the given output stream
    friend std::ostream& operator<<(std::ostream& os, const Exception& instance)
    { os << instance.to_string(); return os; }

    //! Link exception with source location
    template<class T>
    friend T&& operator+(const SourceLocation& location, T&& instance);

protected:
    mutable std::string _cache;
    std::string _message;
    SourceLocation _location;
};

//! Runtime exception
class RuntimeException : public Exception {};

//! System exception
class SystemException : public Exception
{
public:
    //! Create system exception based on the last system error code
    SystemException()
        : this(SystemError::GetLast())
    {}
    //! Create system exception based on the given system error code
    /*!
        \param error - System error code
    */
    SystemException(int error)
        : this(SystemError::to_string(error), error)
    {}
    //! Create system exception based on the given exception message
    /*!
        \param message - Exception message
    */
    SystemException(const std::string& message)
        : this(message, SystemError::GetLast())
    {}
    //! Create system exception based on the given exception message and system error code
    /*!
        \param message - Exception message
        \param error - System error code
    */
    SystemException(const std::string& message, int error)
        : Exception(message),
          _system_error(error),
          _system_message(SystemError::to_string(error))
    {}

    //! Get system error code
    int system_error() const noexcept { return _system_error; }
    //! Get system error message
    const std::string& system_message() const noexcept { return _system_message; }

    //! Get string from the current system exception
    std::string to_string() const override;

private:
    int _system_error;
    std::string _system_message;
};

} // namespace CppCommon

#include "exceptions.inl"

#endif // CPPCOMMON_ERRORS_EXCEPTIONS_H
