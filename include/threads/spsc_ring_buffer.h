/*!
    \file spsc_ring_buffer.h
    \brief Single producer / single consumer wait-free ring buffer definition
    \author Ivan Shynkarenka
    \date 16.01.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_THREADS_SPSC_RING_BUFFER_H
#define CPPCOMMON_THREADS_SPSC_RING_BUFFER_H

#include <atomic>
#include <cassert>
#include <cstdio>
#include <cstring>

namespace CppCommon {

//! Single producer / single consumer wait-free ring buffer
/*!
    Single producer / single consumer wait-free ring buffer use only atomic operations to provide thread-safe enqueue
    and dequeue operations. Ring buffer is bounded to the fixed capacity provided in the constructor.

    FIFO order is guaranteed!

    Thread-safe.

    A combination of the algorithms described by the circular buffers documentation found in the Linux kernel, and the
    bounded MPMC queue by Dmitry Vyukov. Implemented in pure C++11. Should work across most CPU architectures.
    http://www.1024cores.net/home/lock-free-algorithms/queues/bounded-mpmc-queue
*/
class SPSCRingBuffer
{
public:
    //! Default class constructor
    /*!
        \param capacity - Ring buffer capacity (must be a power of two)
    */
    explicit SPSCRingBuffer(size_t capacity);
    SPSCRingBuffer(const SPSCRingBuffer&) = delete;
    SPSCRingBuffer(SPSCRingBuffer&&) = delete;
    ~SPSCRingBuffer() { delete[] _buffer; }

    SPSCRingBuffer& operator=(const SPSCRingBuffer&) = delete;
    SPSCRingBuffer& operator=(SPSCRingBuffer&&) = delete;

    //! Check if the buffer is not empty
    explicit operator bool() const noexcept { return !empty(); }

    //! Is ring buffer empty?
    bool empty() const noexcept { return (size() == 0); }
    //! Get ring buffer capacity in bytes
    size_t capacity() const noexcept { return _capacity; }
    //! Get ring buffer size in bytes
    size_t size() const noexcept;

    //! Enqueue a chunk of bytes into the ring buffer (single producer thread method)
    /*!
        The chunk of bytes will be copied into the ring buffer using 'memcpy()' function.
        Chunk size should not be greater than ring buffer capacity!

        Will not block.

        \param chunk - Chunk buffer to enqueue
        \param size - Chunk buffer size
        \return 'true' if the chunk of bytes was successfully enqueue, 'false' if the ring buffer is full
    */
    bool Enqueue(const void* chunk, size_t size);

    //! Dequeue a chunk of bytes from the ring buffer (single consumer thread method)
    /*!
        The chunk of bytes will be copied from the ring buffer using 'memcpy()' function.

        Will not block.

        \param chunk - Chunk buffer to dequeue
        \param size - Chunk buffer size
        \return 'true' if the chunk of bytes was successfully dequeue, 'false' if the ring buffer is empty
    */
    bool Dequeue(void* chunk, size_t& size);

private:
    typedef char cache_line_pad[128];

    cache_line_pad _pad0;
    const size_t _capacity;
    const size_t _mask;
    uint8_t* const _buffer;

    cache_line_pad _pad1;
    std::atomic<size_t> _head;
    cache_line_pad _pad2;
    std::atomic<size_t> _tail;
    cache_line_pad _pad3;
};

/*! \example threads_spsc_ring_buffer.cpp Single producer / single consumer wait-free ring buffer example */

} // namespace CppCommon

#include "spsc_ring_buffer.inl"

#endif // CPPCOMMON_THREADS_SPSC_RING_BUFFER_H
