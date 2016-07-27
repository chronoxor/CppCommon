/*!
    \file environment.h
    \brief Environment management definition
    \author Ivan Shynkarenka
    \date 27.07.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_SYSTEM_ENVIRONMENT_H
#define CPPCOMMON_SYSTEM_ENVIRONMENT_H

#include <string>

namespace CppCommon {

//! Environment management static class
/*!
    Provides environment management functionality to get OS bit version, process bit version,
    debug/release mode.

    Thread-safe.
*/
class Environment
{
public:
    Environment() = delete;
    Environment(const Environment&) = delete;
    Environment(Environment&&) = delete;
    ~Environment() = delete;

    Environment& operator=(const Environment&) = delete;
    Environment& operator=(Environment&&) = delete;

    //! Is 32-bit OS?
    static bool Is32BitOS();
    //! Is 64-bit OS?
    static bool Is64BitOS();
    //! Is 32-bit running process?
    static bool Is32BitProcess();
    //! Is 64-bit running process?
    static bool Is64BitProcess();
    //! Is compiled in debug mode?
    static bool IsDebug();
    //! Is compiled in release mode?
    static bool IsRelease();

    //! Get OS version string
    static std::string OSVersion();
};

/*! \example system_environment.cpp Environment management example */

} // namespace CppCommon

#endif // CPPCOMMON_SYSTEM_ENVIRONMENT_H
