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
    uint128_t() noexcept;
    template <typename T>
    uint128_t(const T& value) noexcept;
    template <typename TUpper, typename TLower>
    uint128_t(const TUpper& upper, const TLower& lower) noexcept;
    uint128_t(const uint128_t&) noexcept = default;
    uint128_t(uint128_t&&) noexcept = default;
    ~uint128_t() noexcept = default;

    template <typename T>
    uint128_t& operator=(const T& value) noexcept;
    uint128_t& operator=(const uint128_t&) noexcept = default;
    uint128_t& operator=(uint128_t&&) noexcept = default;

    // Type cast
    operator bool() const noexcept { return (bool)(_upper || _lower); }
    operator uint8_t() const noexcept { return (uint8_t)_lower; }
    operator uint16_t() const noexcept { return (uint16_t)_lower; }
    operator uint32_t() const noexcept { return (uint32_t)_lower; }
    operator uint64_t() const noexcept { return (uint64_t)_lower; }

    // Logical operators
    bool operator!() const noexcept { return !(bool)(_upper | _lower); }

    bool operator&&(const uint128_t& value) const noexcept;
    bool operator||(const uint128_t& value) const noexcept;
    template <typename T>
    bool operator&&(const T& value) const noexcept;
    template <typename T>
    bool operator||(const T& value) const noexcept;
    template <typename T>
    friend bool operator&&(const T& value1, const uint128_t& value2) noexcept;
    template <typename T>
    friend bool operator||(const T& value1, const uint128_t& value2) noexcept;

    // Bit operators
    uint128_t operator~() const noexcept { return uint128_t(~_upper, ~_lower); }

    uint128_t& operator&=(const uint128_t& value) noexcept;
    uint128_t& operator|=(const uint128_t& value) noexcept;
    uint128_t& operator^=(const uint128_t& value) noexcept;
    template <typename T>
    uint128_t& operator&=(const T& value) noexcept;
    template <typename T>
    uint128_t& operator|=(const T& value) noexcept;
    template <typename T>
    uint128_t& operator^=(const T& value) noexcept;
    template <typename T>
    friend T& operator&=(T& value1, const uint128_t& value2) noexcept;
    template <typename T>
    friend T& operator|=(T& value1, const uint128_t& value2) noexcept;
    template <typename T>
    friend T& operator^=(T& value1, const uint128_t& value2) noexcept;

    uint128_t operator&(const uint128_t& value) const noexcept;
    uint128_t operator|(const uint128_t& value) const noexcept;
    uint128_t operator^(const uint128_t& value) const noexcept;
    template <typename T>
    uint128_t operator&(const T& value) const noexcept;
    template <typename T>
    uint128_t operator|(const T& value) const noexcept;
    template <typename T>
    uint128_t operator^(const T& value) const noexcept;
    template <typename T>
    friend uint128_t operator&(const T& value1, const uint128_t& value2) noexcept;
    template <typename T>
    friend uint128_t operator|(const T& value1, const uint128_t& value2) noexcept;
    template <typename T>
    friend uint128_t operator^(const T& value1, const uint128_t& value2) noexcept;

    // Shift operators
    uint128_t& operator<<=(const uint128_t& value) noexcept;
    uint128_t& operator>>=(const uint128_t& value) noexcept;
    template <typename T>
    uint128_t& operator<<=(const T& value) noexcept;
    template <typename T>
    uint128_t& operator>>=(const T& value) noexcept;
    template <typename T>
    friend T& operator<<=(T& value1, const uint128_t& value2) noexcept;
    template <typename T>
    friend T& operator>>=(T& value1, const uint128_t& value2) noexcept;

    uint128_t operator<<(const uint128_t& value) const noexcept;
    uint128_t operator>>(const uint128_t& value) const noexcept;
    template <typename T>
    uint128_t operator<<(const T& value) const noexcept;
    template <typename T>
    uint128_t operator>>(const T& value) const noexcept;
    template <typename T>
    friend uint128_t operator<<(const T& value1, const uint128_t& value2) noexcept;
    template <typename T>
    friend uint128_t operator>>(const T& value1, const uint128_t& value2) noexcept;

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
