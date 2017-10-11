/*!
    \file uint256.h
    \brief Unsigned 256-bit integer type definition
    \author Ivan Shynkarenka
    \date 11.10.2017
    \copyright MIT License
*/

#ifndef CPPCOMMON_UINT256_H
#define CPPCOMMON_UINT256_H

#include "uint128.h"

namespace CppCommon {

//! Unsigned 256-bit integer type
/*!
    Represents unsigned 256-bit integer type and provides basic arithmetic operations.
*/
class uint256_t
{
public:
    uint256_t() noexcept = default;
    template <typename TValue>
    uint256_t(const TValue& value) noexcept;
    template <typename TUpper, typename TLower>
    uint256_t(const TUpper& upper, const TLower& lower) noexcept;
    template <typename TUpperUpper, typename TUpperLower, typename TLowerUpper, typename TLowerLower>
    uint256_t(const TUpperUpper& upper_upper, const TUpperLower& upper_lower, const TLowerUpper& lower_upper, const TLowerLower& lower_lower) noexcept;
    uint256_t(const uint256_t&) noexcept = default;
    uint256_t(uint256_t&&) noexcept = default;
    ~uint256_t() noexcept = default;

    template <typename TValue>
    uint256_t& operator=(const TValue& value) noexcept;
    uint256_t& operator=(const uint256_t&) noexcept = default;
    uint256_t& operator=(uint256_t&&) noexcept = default;

    //! Get the upper part of the 256-bit integer
    uint128_t upper() const noexcept { return _upper; }
    //! Get the lower part of the 256-bit integer
    uint128_t lower() const noexcept { return _lower; }

private:
    uint128_t _upper;
    uint128_t _lower;
};

} // namespace CppCommon

#include "uint256.inl"

#endif // CPPCOMMON_UINT256_H
