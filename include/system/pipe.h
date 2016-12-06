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
    Pipe(const Pipe&) = delete;
    Pipe(Pipe&& pipe) noexcept;
    ~Pipe();

    Pipe& operator=(const Pipe&) = delete;
    Pipe& operator=(Pipe&& pipe) noexcept;

    //! Read a bytes buffer from the pipe
    /*!
        If the pipe is not opened for reading the method will raise
        a system exception!

        \param buffer - Buffer to read
        \param size - Buffer size
        \return Count of read bytes
    */
    size_t Read(void* buffer, size_t size);

    //! Write a byte buffer into the pipe
    /*!
        If the pipe is not opened for writing the method will raise
        a system exception!

        \param buffer - Buffer to write
        \param size - Buffer size
        \return Count of written bytes
    */
    size_t Write(const void* buffer, size_t size);

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
