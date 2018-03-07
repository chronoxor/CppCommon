/*!
    \file memory_leaks.h
    \brief Memory leaks detecting tool. Include this file to detect memory leaks in release build.
    \author Ivan Shynkarenka
    \date 26.02.2018
    \copyright MIT License
*/

#ifndef CPPCOMMON_MEMORY_MEMORY_LEAKS_H
#define CPPCOMMON_MEMORY_MEMORY_LEAKS_H

#if defined(__clang__)
#pragma clang system_header
#elif defined(__GNUC__)
#pragma GCC system_header
#elif defined(_MSC_VER)
#pragma system_header
#endif

//! @cond INTERNALS
#if defined(_MSC_VER)
#define VLD_FORCE_ENABLE
#include <vld.h>
#endif
//! @endcond

/*! \example memory_leaks.cpp Memory leaks detection example */

#endif // CPPCOMMON_MEMORY_MEMORY_LEAKS_H
