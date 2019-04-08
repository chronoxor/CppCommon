/*!
    \file endian.h
    \brief Big/Little-endian utilities definition
    \author Ivan Shynkarenka
    \date 21.07.2017
    \copyright MIT License
*/

#ifndef CPPCOMMON_UTILITY_ENDIAN_H
#define CPPCOMMON_UTILITY_ENDIAN_H

#include <cstdint>
#include <cstring>

namespace CppCommon {

//! Big/Little-endian utilities
/*!
    Big/Little-endian utilities contains methods for big<->little endian conversions.

    Thread-safe.
*/
class Endian
{
public:
    Endian() = delete;
    Endian(const Endian&) = delete;
    Endian(Endian&&) = delete;
    ~Endian() = delete;

    Endian& operator=(const Endian&) = delete;
    Endian& operator=(Endian&&) = delete;

    //! Is big-endian system?
    static bool IsBigEndian();
    //! Is little-endian system?
    static bool IsLittleEndian();

    //! Read big-endian signed 16-bit integer value from the given buffer
    /*!
        \param buffer - Buffer to read
        \param value - Signed 16-bit integer value
        \return Count of read bytes
    */
    static size_t ReadBigEndian(const void* buffer, int16_t& value);
    //! Read big-endian unsigned 16-bit integer value from the given buffer
    /*!
        \param buffer - Buffer to read
        \param value - Unsigned 16-bit integer value
        \return Count of read bytes
    */
    static size_t ReadBigEndian(const void* buffer, uint16_t& value);
    //! Read big-endian signed 32-bit integer value from the given buffer
    /*!
        \param buffer - Buffer to read
        \param value - Signed 32-bit integer value
        \return Count of read bytes
    */
    static size_t ReadBigEndian(const void* buffer, int32_t& value);
    //! Read big-endian unsigned 32-bit integer value from the given buffer
    /*!
        \param buffer - Buffer to read
        \param value - Unsigned 32-bit integer value
        \return Count of read bytes
    */
    static size_t ReadBigEndian(const void* buffer, uint32_t& value);
    //! Read big-endian signed 64-bit integer value from the given buffer
    /*!
        \param buffer - Buffer to read
        \param value - Signed 64-bit integer value
        \return Count of read bytes
    */
    static size_t ReadBigEndian(const void* buffer, int64_t& value);
    //! Read big-endian unsigned 64-bit integer value from the given buffer
    /*!
        \param buffer - Buffer to read
        \param value - Unsigned 64-bit integer value
        \return Count of read bytes
    */
    static size_t ReadBigEndian(const void* buffer, uint64_t& value);

    //! Read little-endian signed 16-bit integer value from the given buffer
    /*!
        \param buffer - Buffer to read
        \param value - Signed 16-bit integer value
        \return Count of read bytes
    */
    static size_t ReadLittleEndian(const void* buffer, int16_t& value);
    //! Read little-endian unsigned 16-bit integer value from the given buffer
    /*!
        \param buffer - Buffer to read
        \param value - Unsigned 16-bit integer value
        \return Count of read bytes
    */
    static size_t ReadLittleEndian(const void* buffer, uint16_t& value);
    //! Read little-endian signed 32-bit integer value from the given buffer
    /*!
        \param buffer - Buffer to read
        \param value - Signed 32-bit integer value
        \return Count of read bytes
    */
    static size_t ReadLittleEndian(const void* buffer, int32_t& value);
    //! Read little-endian unsigned 32-bit integer value from the given buffer
    /*!
        \param buffer - Buffer to read
        \param value - Unsigned 32-bit integer value
        \return Count of read bytes
    */
    static size_t ReadLittleEndian(const void* buffer, uint32_t& value);
    //! Read little-endian signed 64-bit integer value from the given buffer
    /*!
        \param buffer - Buffer to read
        \param value - Signed 64-bit integer value
        \return Count of read bytes
    */
    static size_t ReadLittleEndian(const void* buffer, int64_t& value);
    //! Read little-endian unsigned 64-bit integer value from the given buffer
    /*!
        \param buffer - Buffer to read
        \param value - Unsigned 64-bit integer value
        \return Count of read bytes
    */
    static size_t ReadLittleEndian(const void* buffer, uint64_t& value);

    //! Write big-endian signed 16-bit integer value into the given buffer
    /*!
        \param buffer - Buffer to write
        \param value - Signed 16-bit integer value
        \return Count of written bytes
    */
    static size_t WriteBigEndian(void* buffer, int16_t value);
    //! Write big-endian unsigned 16-bit integer value into the given buffer
    /*!
        \param buffer - Buffer to write
        \param value - Unsigned 16-bit integer value
        \return Count of written bytes
    */
    static size_t WriteBigEndian(void* buffer, uint16_t value);
    //! Write big-endian signed 32-bit integer value into the given buffer
    /*!
        \param buffer - Buffer to write
        \param value - Signed 32-bit integer value
        \return Count of written bytes
    */
    static size_t WriteBigEndian(void* buffer, int32_t value);
    //! Write big-endian unsigned 32-bit integer value into the given buffer
    /*!
        \param buffer - Buffer to write
        \param value - Unsigned 32-bit integer value
        \return Count of written bytes
    */
    static size_t WriteBigEndian(void* buffer, uint32_t value);
    //! Write big-endian signed 64-bit integer value into the given buffer
    /*!
        \param buffer - Buffer to write
        \param value - Signed 64-bit integer value
        \return Count of written bytes
    */
    static size_t WriteBigEndian(void* buffer, int64_t value);
    //! Write big-endian unsigned 64-bit integer value into the given buffer
    /*!
        \param buffer - Buffer to write
        \param value - Unsigned 64-bit integer value
        \return Count of written bytes
    */
    static size_t WriteBigEndian(void* buffer, uint64_t value);

    //! Write little-endian signed 16-bit integer value into the given buffer
    /*!
        \param buffer - Buffer to write
        \param value - Signed 16-bit integer value
        \return Count of written bytes
    */
    static size_t WriteLittleEndian(void* buffer, int16_t value);
    //! Write little-endian unsigned 16-bit integer value into the given buffer
    /*!
        \param buffer - Buffer to write
        \param value - Unsigned 16-bit integer value
        \return Count of written bytes
    */
    static size_t WriteLittleEndian(void* buffer, uint16_t value);
    //! Write little-endian signed 32-bit integer value into the given buffer
    /*!
        \param buffer - Buffer to write
        \param value - Signed 32-bit integer value
        \return Count of written bytes
    */
    static size_t WriteLittleEndian(void* buffer, int32_t value);
    //! Write little-endian unsigned 32-bit integer value into the given buffer
    /*!
        \param buffer - Buffer to write
        \param value - Unsigned 32-bit integer value
        \return Count of written bytes
    */
    static size_t WriteLittleEndian(void* buffer, uint32_t value);
    //! Write little-endian signed 64-bit integer value into the given buffer
    /*!
        \param buffer - Buffer to write
        \param value - Signed 64-bit integer value
        \return Count of written bytes
    */
    static size_t WriteLittleEndian(void* buffer, int64_t value);
    //! Write little-endian unsigned 64-bit integer value into the given buffer
    /*!
        \param buffer - Buffer to write
        \param value - Unsigned 64-bit integer value
        \return Count of written bytes
    */
    static size_t WriteLittleEndian(void* buffer, uint64_t value);
};

} // namespace CppCommon

#include "endian.inl"

#endif // CPPCOMMON_UTILITY_ENDIAN_H
