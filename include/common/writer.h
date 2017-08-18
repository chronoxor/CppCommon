/*!
    \file writer.h
    \brief Writer interface definition
    \author Ivan Shynkarenka
    \date 07.12.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_WRITER_H
#define CPPCOMMON_WRITER_H

#include <string>
#include <vector>

namespace CppCommon {

//! Writer interface
/*!
    Writer interface is based on a write byte buffer method and provides
    functionality to write bytes buffer, text or text lines.
*/
class Writer
{
public:
    Writer() noexcept = default;
    Writer(const Writer&) noexcept = default;
    Writer(Writer&&) noexcept = default;
    virtual ~Writer() noexcept = default;

    Writer& operator=(const Writer&) noexcept = default;
    Writer& operator=(Writer&&) noexcept = default;

    //! Write a byte buffer base method
    /*!
        \param buffer - Buffer to write
        \param size - Buffer size
        \return Count of written bytes
    */
    virtual size_t Write(const void* buffer, size_t size) = 0;

    //! Write a text string
    /*!
        \param text - Text string
        \return Count of written characters
    */
    size_t Write(const std::string& text);
    //! Write text lines
    /*!
        \param lines - Text lines
        \return Count of written lines
    */
    size_t Write(const std::vector<std::string>& lines);

    //! Flush the writer
    virtual void Flush() {}
};

} // namespace CppCommon

#endif // CPPCOMMON_WRITER_H
