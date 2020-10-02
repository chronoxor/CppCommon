/*!
    \file stream.h
    \brief Standard input/output/error stream definition
    \author Ivan Shynkarenka
    \date 23.07.2017
    \copyright MIT License
*/

#ifndef CPPCOMMON_SYSTEM_STREAM_H
#define CPPCOMMON_SYSTEM_STREAM_H

#include "common/reader.h"
#include "common/writer.h"
#include "errors/exceptions.h"

#include <memory>

namespace CppCommon {

//! Standard input stream
/*!
    Thread-safe.
*/
class StdInput : public Reader
{
public:
    StdInput();
    StdInput(const StdInput&) = delete;
    StdInput(StdInput&& stream) = delete;
    virtual ~StdInput();

    StdInput& operator=(const StdInput&) = delete;
    StdInput& operator=(StdInput&& stream) = delete;

    //! Check if the stream is valid
    explicit operator bool() const noexcept { return IsValid(); }

    //! Get the native stream handler
    void* stream() const noexcept;

    //! Is stream valid?
    bool IsValid() const noexcept;

    //! Read a bytes buffer from the stream
    /*!
        \param buffer - Buffer to read
        \param size - Buffer size
        \return Count of read bytes
    */
    size_t Read(void* buffer, size_t size) override;

    using Reader::ReadAllBytes;
    using Reader::ReadAllText;
    using Reader::ReadAllLines;

    //! Swap two instances
    void swap(StdInput& stream) noexcept;
    friend void swap(StdInput& stream1, StdInput& stream2) noexcept;

private:
    class Impl;

    Impl& impl() noexcept { return reinterpret_cast<Impl&>(_storage); }
    const Impl& impl() const noexcept { return reinterpret_cast<Impl const&>(_storage); }

    static const size_t StorageSize = 8;
    static const size_t StorageAlign = 8;
    std::aligned_storage<StorageSize, StorageAlign>::type _storage;
};

//! Standard output stream
/*!
    Thread-safe.
*/
class StdOutput : public Writer
{
public:
    StdOutput();
    StdOutput(const StdOutput&) = delete;
    StdOutput(StdOutput&& stream) = delete;
    virtual ~StdOutput();

    StdOutput& operator=(const StdOutput&) = delete;
    StdOutput& operator=(StdOutput&& stream) = delete;

    //! Check if the stream is valid
    explicit operator bool() const noexcept { return IsValid(); }

    //! Get the native stream handler
    void* stream() const noexcept;

    //! Is stream valid?
    bool IsValid() const noexcept;

    //! Write a byte buffer into the stream
    /*!
        \param buffer - Buffer to write
        \param size - Buffer size
        \return Count of written bytes
    */
    size_t Write(const void* buffer, size_t size) override;

    using Writer::Write;

    //! Flush the stream
    void Flush() override;

    //! Swap two instances
    void swap(StdOutput& stream) noexcept;
    friend void swap(StdOutput& stream1, StdOutput& stream2) noexcept;

private:
    class Impl;

    Impl& impl() noexcept { return reinterpret_cast<Impl&>(_storage); }
    const Impl& impl() const noexcept { return reinterpret_cast<Impl const&>(_storage); }

    static const size_t StorageSize = 8;
    static const size_t StorageAlign = 8;
    std::aligned_storage<StorageSize, StorageAlign>::type _storage;
};

//! Standard error stream
/*!
    Thread-safe.
*/
class StdError : public Writer
{
public:
    StdError();
    StdError(const StdError&) = delete;
    StdError(StdError&& stream) = delete;
    virtual ~StdError();

    StdError& operator=(const StdError&) = delete;
    StdError& operator=(StdError&& stream) = delete;

    //! Check if the stream is valid
    explicit operator bool() const { return IsValid(); }

    //! Get the native stream handler
    void* stream() const noexcept;

    //! Is stream valid?
    bool IsValid() const noexcept;

    //! Write a byte buffer into the stream
    /*!
        \param buffer - Buffer to write
        \param size - Buffer size
        \return Count of written bytes
    */
    size_t Write(const void* buffer, size_t size) override;

    using Writer::Write;

    //! Flush the stream
    void Flush() override;

    //! Swap two instances
    void swap(StdError& stream) noexcept;
    friend void swap(StdError& stream1, StdError& stream2) noexcept;

private:
    class Impl;

    Impl& impl() noexcept { return reinterpret_cast<Impl&>(_storage); }
    const Impl& impl() const noexcept { return reinterpret_cast<Impl const&>(_storage); }

    static const size_t StorageSize = 8;
    static const size_t StorageAlign = 8;
    std::aligned_storage<StorageSize, StorageAlign>::type _storage;
};

} // namespace CppCommon

#include "stream.inl"

#endif // CPPCOMMON_SYSTEM_STREAM_H
