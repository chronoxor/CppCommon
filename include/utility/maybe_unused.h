/*!
    \file maybe_unused.h
    \brief Maybe unused definition
    \author Ivan Shynkarenka
    \date 14.04.2018
    \copyright MIT License
*/

#ifndef CPPCOMMON_UTILITY_MAYBE_UNUSED_H
#define CPPCOMMON_UTILITY_MAYBE_UNUSED_H

#if (__CYGWIN__)
#define MAYBE_UNUSED
#else
#define MAYBE_UNUSED [[maybe_unused]]
#endif

#endif // CPPCOMMON_UTILITY_MAYBE_UNUSED_H
