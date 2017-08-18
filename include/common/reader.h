/*!
    \file reader.h
    \brief Reader interface definition
    \author Ivan Shynkarenka
    \date 07.12.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_READER_H
#define CPPCOMMON_READER_H

#include <string>
#include <vector>

namespace CppCommon {

//! Reader interface
/*!
    Reader interface is based on a read byte buffer method and provides
    functionality to read all bytes, text or text lines.
*/
class Reader
{
public:
    Reader() noexcept = default;
    Reader(const Reader&) noexcept = default;
    Reader(Reader&&) noexcept = default;
    virtual ~Reader() noexcept = default;

    Reader& operator=(const Reader&) noexcept = default;
    Reader& operator=(Reader&&) noexcept = default;

    //! Read a bytes buffer base method
    /*!
        \param buffer - Buffer to read
        \param size - Buffer size
        \return Count of read bytes
    */
    virtual size_t Read(void* buffer, size_t size) = 0;

    //! Read all bytes
    /*!
        \return Bytes buffer
    */
    std::vector<uint8_t> ReadAllBytes();
    //! Read all text
    /*!
        \return Text string
    */
    std::string ReadAllText();
    //! Read all text lines
    /*!
        \return Text lines
    */
    std::vector<std::string> ReadAllLines();
};

} // namespace CppCommon

#endif // CPPCOMMON_READER_H
