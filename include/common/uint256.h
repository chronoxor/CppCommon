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
    uint256_t() noexcept;
    template <typename T>
    uint256_t(const T& value) noexcept;
    template <typename TUpper, typename TLower>
    uint256_t(const TUpper& upper, const TLower& lower) noexcept;
    template <typename TUpperUpper, typename TUpperLower, typename TLowerUpper, typename TLowerLower>
    uint256_t(const TUpperUpper& upper_upper, const TUpperLower& upper_lower, const TLowerUpper& lower_upper, const TLowerLower& lower_lower) noexcept;
    uint256_t(const uint256_t&) noexcept = default;
    uint256_t(uint256_t&&) noexcept = default;
    ~uint256_t() noexcept = default;

    template <typename T>
    uint256_t& operator=(const T& value) noexcept;
    uint256_t& operator=(const uint256_t&) noexcept = default;
    uint256_t& operator=(uint256_t&&) noexcept = default;

    // Type cast
    operator bool() const noexcept { return (bool)(_upper || _lower); }
    operator uint8_t() const noexcept { return (uint8_t)_lower; }
    operator uint16_t() const noexcept { return (uint16_t)_lower; }
    operator uint32_t() const noexcept { return (uint32_t)_lower; }
    operator uint64_t() const noexcept { return (uint64_t)_lower; }
    operator uint128_t() const noexcept { return _lower; }

    // Logical operators
    bool operator!() const noexcept { return !(bool)(_upper | _lower); }

    bool operator&&(const uint128_t& value) const noexcept;
    bool operator&&(const uint256_t& value) const noexcept;
    bool operator||(const uint128_t& value) const noexcept;
    bool operator||(const uint256_t& value) const noexcept;
    template <typename T>
    bool operator&&(const T& value) const noexcept;
    template <typename T>
    bool operator||(const T& value) const noexcept;
    template <typename T>
    friend bool operator&&(const T& value1, const uint256_t& value2) noexcept;
    template <typename T>
    friend bool operator||(const T& value1, const uint256_t& value2) noexcept;

    // Bit operators
    uint256_t operator~() const noexcept { return uint256_t(~_upper, ~_lower); }

    uint256_t& operator&=(const uint128_t& value) noexcept;
    uint256_t& operator&=(const uint256_t& value) noexcept;
    uint256_t& operator|=(const uint128_t& value) noexcept;
    uint256_t& operator|=(const uint256_t& value) noexcept;
    uint256_t& operator^=(const uint128_t& value) noexcept;
    uint256_t& operator^=(const uint256_t& value) noexcept;
    template <typename T>
    uint256_t& operator&=(const T& value) noexcept;
    template <typename T>
    uint256_t& operator|=(const T& value) noexcept;
    template <typename T>
    uint256_t& operator^=(const T& value) noexcept;
    template <typename T>
    friend T& operator&=(T& value1, const uint256_t& value2) noexcept;
    template <typename T>
    friend T& operator|=(T& value1, const uint256_t& value2) noexcept;
    template <typename T>
    friend T& operator^=(T& value1, const uint256_t& value2) noexcept;

    uint256_t operator&(const uint128_t& value) const noexcept;
    uint256_t operator&(const uint256_t& value) const noexcept;
    uint256_t operator|(const uint128_t& value) const noexcept;
    uint256_t operator|(const uint256_t& value) const noexcept;
    uint256_t operator^(const uint128_t& value) const noexcept;
    uint256_t operator^(const uint256_t& value) const noexcept;
    template <typename T>
    uint256_t operator&(const T& value) const noexcept;
    template <typename T>
    uint256_t operator|(const T& value) const noexcept;
    template <typename T>
    uint256_t operator^(const T& value) const noexcept;
    template <typename T>
    friend uint256_t operator&(const T& value1, const uint256_t& value2) noexcept;
    template <typename T>
    friend uint256_t operator|(const T& value1, const uint256_t& value2) noexcept;
    template <typename T>
    friend uint256_t operator^(const T& value1, const uint256_t& value2) noexcept;

    // Shift operators
    uint256_t& operator<<=(const uint128_t& value) noexcept;
    uint256_t& operator<<=(const uint256_t& value) noexcept;
    uint256_t& operator>>=(const uint128_t& value) noexcept;
    uint256_t& operator>>=(const uint256_t& value) noexcept;
    template <typename T>
    uint256_t& operator<<=(const T& value) noexcept;
    template <typename T>
    uint256_t& operator>>=(const T& value) noexcept;
    template <typename T>
    friend T& operator<<=(T& value1, const uint256_t& value2) noexcept;
    template <typename T>
    friend T& operator>>=(T& value1, const uint256_t& value2) noexcept;

    uint256_t operator<<(const uint128_t& value) const noexcept;
    uint256_t operator<<(const uint256_t& value) const noexcept;
    uint256_t operator>>(const uint128_t& value) const noexcept;
    uint256_t operator>>(const uint256_t& value) const noexcept;
    template <typename T>
    uint256_t operator<<(const T& value) const noexcept;
    template <typename T>
    uint256_t operator>>(const T& value) const noexcept;
    template <typename T>
    friend uint256_t operator<<(const T& value1, const uint256_t& value2) noexcept;
    template <typename T>
    friend uint256_t operator>>(const T& value1, const uint256_t& value2) noexcept;

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
