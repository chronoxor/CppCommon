/*!
    \file pipe.h
    \brief Pipe definition
    \author Ivan Shynkarenka
    \date 30.11.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_SYSTEM_PIPE_H
#define CPPCOMMON_SYSTEM_PIPE_H

#include <memory>
#include <vector>

namespace CppCommon {

//! Pipe
/*!
    A pipe is a section of shared memory that processes use for communication.  The
    process that creates a pipe is the pipe server. A process that  connects  to  a
    pipe is a pipe client. One process writes information to  the  pipe,  then  the
    other process reads the information from the pipe. This overview describes  how
    to create, manage, and use pipes.

    Not thread-safe.
*/
class Pipe
{
public:
    Pipe();
    Pipe(const Pipe& pipe);
    Pipe(Pipe&& pipe) noexcept;
    ~Pipe();

    Pipe& operator=(const Pipe& pipe);
    Pipe& operator=(Pipe&& pipe) noexcept;

    //! Read a bytes buffer from the pipe
    /*!
        If the pipe is not opened for reading the method will raise
        a system exception!

        \param buffer - Buffer to read
        \param size - Buffer size
        \return Count of read bytes
    */
    size_t Read(uint8_t* buffer, size_t size);

    //! Read all bytes from the pipe
    /*!
        If the pipe is not opened for reading the method will raise
        a system exception!

        \return Bytes buffer
    */
    std::vector<uint8_t> ReadAllBytes();
    //! Read all text from the pipe
    /*!
        If the pipe is not opened for reading the method will raise
        a system exception!

        \return Text string
    */
    std::string ReadAllText();
    //! Read all text lines from the pipe
    /*!
        If the pipe is not opened for reading the method will raise
        a system exception!

        \return Text lines
    */
    std::vector<std::string> ReadAllLines();

    //! Write a byte buffer into the pipe
    /*!
        If the pipe is not opened for writing the method will raise
        a system exception!

        \param buffer - Buffer to write
        \param size - Buffer size
        \return Count of written bytes
    */
    size_t Write(const uint8_t* buffer, size_t size);
    //! Write a text string into the pipe
    /*!
        \param text - Text string
        \return Count of written characters
    */
    size_t Write(const std::string& text);
    //! Write text lines into the pipe
    /*!
        \param lines - Text lines
        \return Count of written lines
    */
    size_t Write(const std::vector<std::string>& lines);

    //! Swap two instances
    void swap(Pipe& pipe) noexcept;
    friend void swap(Pipe& pipe1, Pipe& pipe2) noexcept;

private:
    class Impl;
    std::unique_ptr<Impl> _pimpl;
};

/*! \example system_pipe.cpp Pipe example */

} // namespace CppCommon

#include "pipe.inl"

#endif // CPPCOMMON_SYSTEM_PIPE_H
