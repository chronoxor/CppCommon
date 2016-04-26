/*!
    \file system_error.h
    \brief System error wrapper definition
    \author Ivan Shynkarenka
    \date 09.02.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_ERRORS_SYSTEM_ERROR_H
#define CPPCOMMON_ERRORS_SYSTEM_ERROR_H

#include <string>

namespace CppCommon {

//! System error
/*!
    System exception wrapper provides interface to get, set and clear the last system error.

    Thread-safe.
*/
class SystemError
{
public:
    SystemError() = delete;
    SystemError(const SystemError&) = delete;
    SystemError(SystemError&&) = delete;
    virtual ~SystemError() = delete;

    SystemError& operator=(const SystemError&) = delete;
    SystemError& operator=(SystemError&&) = delete;

    //! Get the last system error code
    /*!
        \return Last system error code
    */
    static int GetLast() noexcept;

    //! Set the last system error code
    /*!
        \param error - Last system error code to set
    */
    static void SetLast(int error) noexcept;

    //! Clear the last system error code
    static void ClearLast() noexcept;

    //! Convert the last system error code to the system error message
    /*!
        \return Last system error message
    */
    static std::string to_string() { return to_string(GetLast()); }
    //! Convert the given system error code to the system error message
    /*!
        \param error - System error code
        \return System error message
    */
    static std::string to_string(int error);
};

/*! \example errors_system_error.cpp System error wrapper example */

} // namespace CppCommon

#endif // CPPCOMMON_ERRORS_SYSTEM_ERROR_H
