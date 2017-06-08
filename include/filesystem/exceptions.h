/*!
    \file include/filesystem/exceptions.h
    \brief File system exceptions definition
    \author Ivan Shynkarenka
    \date 24.08.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_FILESYSTEM_EXCEPTIONS_H
#define CPPCOMMON_FILESYSTEM_EXCEPTIONS_H

#include "errors/exceptions.h"
#include "filesystem/path.h"

namespace CppCommon {

//! File system exception
class FileSystemException : public SystemException
{
public:
    using SystemException::SystemException;

    //! Get exception path
    const Path& path() const noexcept { return _path; }

    //! Get string from the current exception
    std::string string() const override;

    //! Attach the given path to the exception
    /*!
        \param path - Exception path
    */
    FileSystemException& Attach(const Path& path)
    { _path = path; return *this; }
    //! Attach the given source and destination paths to the exception
    /*!
        \param src - Exception source path
        \param dst - Exception destination path
    */
    FileSystemException& Attach(const Path& src, const Path& dst)
    { _src = src; _dst = dst; return *this; }

protected:
    //! Filesystem exception path
    Path _path;
    //! Filesystem exception source path
    Path _src;
    //! Filesystem exception destination path
    Path _dst;
};

} // namespace CppCommon

#endif // CPPCOMMON_FILESYSTEM_EXCEPTIONS_H
