/*!
    \file include/system/exceptions.h
    \brief System exceptions definition
    \author Ivan Shynkarenka
    \date 07.06.2017
    \copyright MIT License
*/

#ifndef CPPCOMMON_SYSTEM_EXCEPTIONS_H
#define CPPCOMMON_SYSTEM_EXCEPTIONS_H

#include "filesystem/exceptions.h"

namespace CppCommon {

//! Dynamic link library exception
class DLLException : public FileSystemException
{
public:
    using FileSystemException::FileSystemException;
};

} // namespace CppCommon

#endif // CPPCOMMON_SYSTEM_EXCEPTIONS_H
