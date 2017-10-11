/*!
    \file uint128.h
    \brief Unsigned 128-bit integer type definition
    \author Ivan Shynkarenka
    \date 11.10.2017
    \copyright MIT License
*/

#ifndef CPPCOMMON_UINT128_H
#define CPPCOMMON_UINT128_H

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace CppCommon {

//! Unsigned 128-bit integer type
/*!
    Represents unsigned 128-bit integer type and provides basic arithmetic operations.
*/
class uint128_t
{
public:
    uint128_t() noexcept = default;
    template <typename TValue>
    uint128_t(const TValue& value) noexcept;
    template <typename TUpper, typename TLower>
    uint128_t(const TUpper& upper, const TLower& lower) noexcept;
    uint128_t(const uint128_t&) noexcept = default;
    uint128_t(uint128_t&&) noexcept = default;
    ~uint128_t() noexcept = default;

    template <typename TValue>
    uint128_t& operator=(const TValue& value) noexcept;
    uint128_t& operator=(const uint128_t&) noexcept = default;
    uint128_t& operator=(uint128_t&&) noexcept = default;

    //! Get the upper part of the 128-bit integer
    uint64_t upper() const noexcept { return _upper; }
    //! Get the lower part of the 128-bit integer
    uint64_t lower() const noexcept { return _lower; }

private:
    uint64_t _upper;
    uint64_t _lower;
};

} // namespace CppCommon

#include "uint128.inl"

#endif // CPPCOMMON_UINT128_H
