/*!
    \file uuid.h
    \brief Universally unique identifier (UUID) definition
    \author Ivan Shynkarenka
    \date 18.08.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_SYSTEM_UUID_H
#define CPPCOMMON_SYSTEM_UUID_H

#include "errors/exceptions.h"

#include <array>
#include <string>

namespace CppCommon {

//! Universally unique identifier (UUID)
/*!
    A universally unique identifier (UUID) is an identifier standard used
    in software construction. This implementation generates UUID with the
    system provided functionality (CoCreateGuid(), uuid_generate(), etc).

    A UUID is simply a 128-bit value: "123e4567-e89b-12d3-a456-426655440000"

    Not thread-safe.
*/
class UUID
{
public:
    //! Default constructor
    UUID() : _data() { _data.fill(0); }
    //! Initialize UUID with a given string
    /*!
        \param uuid - UUID string
    */
    explicit UUID(const std::string& uuid);
    //! Initialize UUID with a given 16 bytes data buffer
    /*!
        \param data - UUID 16 bytes data buffer
    */
    explicit UUID(const std::array<uint8_t, 16>& data) : _data(data) {}
    UUID(const UUID&) = default;
    UUID(UUID&&) noexcept = default;
    ~UUID() = default;

    UUID& operator=(const std::string& uuid)
    { _data = UUID(uuid).data(); return *this; }
    UUID& operator=(const std::array<uint8_t, 16>& data)
    { _data = data; return *this; }
    UUID& operator=(const UUID&) = default;
    UUID& operator=(UUID&&) noexcept = default;

    // UUID comparison
    friend bool operator==(const UUID& uuid1, const UUID& uuid2)
    { return uuid1._data == uuid2._data; }
    friend bool operator!=(const UUID& uuid1, const UUID& uuid2)
    { return uuid1._data != uuid2._data; }
    friend bool operator<(const UUID& uuid1, const UUID& uuid2)
    { return uuid1._data < uuid2._data; }
    friend bool operator>(const UUID& uuid1, const UUID& uuid2)
    { return uuid1._data > uuid2._data; }
    friend bool operator<=(const UUID& uuid1, const UUID& uuid2)
    { return uuid1._data <= uuid2._data; }
    friend bool operator>=(const UUID& uuid1, const UUID& uuid2)
    { return uuid1._data >= uuid2._data; }

    //! Get the UUID data buffer
    std::array<uint8_t, 16>& data() noexcept { return _data; }
    //! Get the UUID data buffer
    const std::array<uint8_t, 16>& data() const noexcept { return _data; }

    //! Get string from the current UUID in format "00000000-0000-0000-0000-000000000000"
    std::string string() const;
    //! Get wide string from the current UUID in format "00000000-0000-0000-0000-000000000000"
    std::wstring wstring() const;

    //! Generate an empty UUID
    static UUID Empty() { return UUID(); }
    //! Generate a sequential UUID1 (time based version)
    static UUID Sequential();
    //! Generate a random UUID4 (Randomly or pseudo-randomly generated version)
    static UUID Random();

    //! Output instance into the given output stream
    friend std::ostream& operator<<(std::ostream& os, const UUID& uuid)
    { os << uuid.string(); return os; }
    //! Output instance into the given wide output stream
    friend std::wostream& operator<<(std::wostream& os, const UUID& uuid)
    { os << uuid.wstring(); return os; }

    //! Swap two instances
    void swap(UUID& uuid) noexcept;
    friend void swap(UUID& uuid1, UUID& uuid2) noexcept;

private:
    std::array<uint8_t, 16> _data;
};

/*! \example system_uuid.cpp Universally unique identifier (UUID) example */

} // namespace CppCommon

#include "uuid.inl"

#endif // CPPCOMMON_SYSTEM_UUID_H
