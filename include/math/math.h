/*!
    \file math.h
    \brief Math definition
    \author Ivan Shynkarenka
    \date 01.09.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_MATH_MATH_H
#define CPPCOMMON_MATH_MATH_H

#include <stdint.h>

namespace CppCommon {

//! Math static class
/*!
    Contains useful math functions.

    Thread-safe.
*/
class Math
{
public:
    Math() = delete;
    Math(const Math&) = delete;
    Math(Math&&) = delete;
    ~Math() = delete;

    Math& operator=(const Math&) = delete;
    Math& operator=(Math&&) = delete;

    //! Calculate (operant * multiplier / divider) with 64-bit unsigned integer values
    /*!
        \param operant - Operant
        \param multiplier - Multiplier
        \param divider - Divider
        \return Calculated value of (operant * multiplier / divider) expression
    */
    static uint64_t MulDiv64(uint64_t operant, uint64_t multiplier, uint64_t divider);
};

/*! \example math_math.cpp Math example */

} // namespace CppCommon

#endif // CPPCOMMON_MATH_MATH_H
