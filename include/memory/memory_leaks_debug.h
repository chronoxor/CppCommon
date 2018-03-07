/*!
    \file memory_leaks_debug.h
    \brief Memory leaks detecting tool. Include this file to detect memory leaks in debug build.
    \author Ivan Shynkarenka
    \date 26.02.2018
    \copyright MIT License
*/

#ifndef CPPCOMMON_MEMORY_MEMORY_LEAKS_DEBUG_H
#define CPPCOMMON_MEMORY_MEMORY_LEAKS_DEBUG_H

#if defined(__clang__)
#pragma clang system_header
#elif defined(__GNUC__)
#pragma GCC system_header
#elif defined(_MSC_VER)
#pragma system_header
#endif

//! @cond INTERNALS
#if defined(_MSC_VER)
#include <vld.h>
#endif
//! @endcond

#endif // CPPCOMMON_MEMORY_MEMORY_LEAKS_DEBUG_H
