/*!
    \file exceptions.cpp
    \brief Exceptions implementation
    \author Ivan Shynkarenka
    \date 09.02.2016
    \copyright MIT License
*/

#include "errors/exceptions.h"

namespace CppCommon {

SystemException::SystemException(int error, const std::string& message) : Exception(message), _system_error(error), _system_message(SystemError::Convert(error))
{
    _message.append("\nSystem error: " + std::to_string(_system_error));
    _message.append("\nSystem message: " + _system_message);
}

} // namespace CppCommon
