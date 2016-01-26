/*!
    \file mpsc_ring_buffer.h
    \brief Multiple producers / single consumer wait-free ring buffer definition
    \author Ivan Shynkarenka
    \date 26.01.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_MPSC_RING_BUFFER_H
#define CPPCOMMON_MPSC_RING_BUFFER_H

#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "system/rdts.h"
#include "threads/spinlock.h"
#include "threads/spsc_ring_buffer.h"

namespace CppCommon {

//! Multiple producers / single consumer wait-free ring buffer
/*!
    Multiple producers / single consumer wait-free ring buffer use only atomic operations to provide thread-safe
    enqueue and dequeue operations. This data structure consist of several SPSC ring buffers which count is
    provided as a hardware concurrency in the constructor. All of them are randomly accessed with a RDTS
    distribution index. All the items available in sesequential or batch mode. All ring buffer sizes are
    limited to the capacity provided in the constructor.

    FIFO order is not guaranteed!
*/
class MPSCRingBuffer
{
public:
    //! Default class constructor
    /*!
        \param capacity - ring buffer capacity (must be a power of two)
        \param concurrency - hardware concurrency (default is std::thread::hardware_concurrency)
    */
    explicit MPSCRingBuffer(uint64_t capacity, uint64_t concurrency = std::thread::hardware_concurrency());
    MPSCRingBuffer(const MPSCRingBuffer&) = delete;
    MPSCRingBuffer(MPSCRingBuffer&&) = default;
    ~MPSCRingBuffer() = default;

    MPSCRingBuffer& operator=(const MPSCRingBuffer&) = delete;
    MPSCRingBuffer& operator=(MPSCRingBuffer&&) = default;

    //! Get ring buffer capacity
    uint64_t capacity() const { return _capacity; }
    //! Get ring buffer concurrency
    uint64_t concurrency() const { return _concurrency; }
    //! Get ring buffer size
    uint64_t size() const;

    //! Enqueue a chunk of bytes into the ring buffer (single producer thread method)
    /*!
        The chunk of bytes will be copied into the ring buffer using 'memcpy()' function.
        Chunk size should not be greater than ring buffer capacity!

        \param chunk - chunk buffer to enqueue
        \param size - chunk buffer size
        \return 'true' if the chunk of bytes was successfully enqueue, 'false' if the ring buffer is full
    */
    bool Enqueue(const void* chunk, uint64_t size);

    //! Dequeue a chunk of bytes from the ring buffer (single consumer thread method)
    /*!
        The chunk of bytes will be copied from the ring buffer using 'memcpy()' function.
        Chunk size should be enough to dequeue the whole data from the ring buffer so it
        should be greater than ring buffer capacity!

        \param chunk - chunk buffer to dequeue
        \param size - chunk buffer size
        \return 'true' if the chunk of bytes was successfully dequeue, 'false' if the ring buffer is empty
    */
    bool Dequeue(void* chunk, uint64_t& size);

private:
    struct Producer
    {
        SpinLock lock;
        SPSCRingBuffer buffer;

        Producer(uint64_t capacity) : buffer(capacity) {}
    };

    uint64_t _capacity;
    uint64_t _concurrency;
    std::vector<std::shared_ptr<Producer>> _producers;
    uint64_t _consumer;
};

} // namespace CppCommon

#include "mpsc_ring_buffer.inl"

#endif //CPPCOMMON_MPSC_RING_BUFFER_H
