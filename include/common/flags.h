/*!
    \file flags.h
    \brief Enum-based flags definition
    \author Ivan Shynkarenka
    \date 25.08.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_FLAGS_H
#define CPPCOMMON_FLAGS_H

#include <bitset>
#include <type_traits>

namespace CppCommon {

//! Enum-based flags false checker
template <typename TEnum>
struct IsEnumFlags : public std::false_type {};

//! Register a new enum-based flags macro
/*!
    Should be used to get access to AND/OR/XOR logical operators with a given enum values.

    Example:
    \code{.cpp}
    enum class MyFlags
    {
        None  = 0x0,
        One   = 0x1,
        Two   = 0x2,
        Three = 0x4,
        Four  = 0x8
    };

    ENUM_FLAGS(MyFlags)

    int main()
    {
        auto mask = MyFlags::One | MyFlags::Two;
        if (mask & MyFlags::Two)
        {
            ...
        }
    }
    \endcode
*/
#define ENUM_FLAGS(type)\
using CppCommon::operator&;\
using CppCommon::operator|;\
using CppCommon::operator^;\
namespace CppCommon {\
template <> struct IsEnumFlags<type> : std::true_type {};\
}

//! Enum-based flags
/*!
    Helper class for enum based flags which wraps particular enum as a template parameter
    and provides flags manipulation operators and methods.

    Not thread-safe.
*/
template <typename TEnum>
class Flags
{
    //! Enum underlying type
    typedef typename std::make_unsigned<typename std::underlying_type<TEnum>::type>::type type;

public:
    Flags() noexcept : _value(0) {}
    Flags(type value) noexcept : _value(value) {}
    Flags(TEnum value) noexcept : _value((type)value) {}
    Flags(const Flags&) noexcept = default;
    Flags(Flags&&) noexcept = default;
    ~Flags() noexcept = default;

    Flags& operator=(type value) noexcept
    { _value = value; return *this; }
    Flags& operator=(TEnum value) noexcept
    { _value = (type)value; return *this; }
    Flags& operator=(const Flags&) noexcept = default;
    Flags& operator=(Flags&&) noexcept = default;

    //! Is any flag set?
    explicit operator bool() const noexcept { return isset(); }

    //! Is no flag set?
    bool operator!() const noexcept { return !isset(); }

    //! Reverse all flags
    Flags operator~() const noexcept { return Flags(~_value); }

    //! Flags logical assign operators
    Flags& operator&=(const Flags& flags) noexcept
    { _value &= flags._value; return *this; }
    Flags& operator|=(const Flags& flags) noexcept
    { _value |= flags._value; return *this; }
    Flags& operator^=(const Flags& flags) noexcept
    { _value ^= flags._value; return *this; }

    //! Flags logical friend operators
    friend Flags operator&(const Flags& flags1, const Flags& flags2) noexcept
    { return Flags(flags1._value & flags2._value); }
    friend Flags operator|(const Flags& flags1, const Flags& flags2) noexcept
    { return Flags(flags1._value | flags2._value); }
    friend Flags operator^(const Flags& flags1, const Flags& flags2) noexcept
    { return Flags(flags1._value ^ flags2._value); }

    // Flags comparison
    friend bool operator==(const Flags& flags1, const Flags& flags2) noexcept
    { return flags1._value == flags2._value; }
    friend bool operator!=(const Flags& flags1, const Flags& flags2) noexcept
    { return flags1._value != flags2._value; }

    //! Convert to the enum value
    operator TEnum() const noexcept { return (TEnum)_value; }

    //! Is any flag set?
    bool isset() const noexcept { return (_value != 0); }
    //! Is the given flag set?
    bool isset(type value) const noexcept { return (_value & value) != 0; }
    //! Is the given flag set?
    bool isset(TEnum value) const noexcept { return (_value & (type)value) != 0; }

    //! Get the enum value
    TEnum value() const noexcept { return (TEnum)_value; }
    //! Get the underlying enum value
    type underlying() const noexcept { return _value; }
    //! Get the bitset value
    std::bitset<sizeof(type) * 8> bitset() const noexcept { return {_value}; }

    //! Swap two instances
    void swap(Flags& flags) noexcept { using std::swap; swap(_value, flags._value); }
    template <typename UEnum>
    friend void swap(Flags<UEnum>& flags1, Flags<UEnum>& flags2) noexcept;

private:
    type _value;
};

/*! \example common_flags.cpp Enum-based flags example */

} // namespace CppCommon

#include "flags.inl"

#endif // CPPCOMMON_FLAGS_H
