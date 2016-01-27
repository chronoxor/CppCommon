/*!
    \file spsc_ring_queue.h
    \brief Single producer / single consumer wait-free ring queue definition
    \author Ivan Shynkarenka
    \date 15.01.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_SPSC_RING_QUEUE_H
#define CPPCOMMON_SPSC_RING_QUEUE_H

#include <atomic>
#include <cassert>

namespace CppCommon {

//! Single producer / single consumer wait-free ring queue
/*!
    Single producer / single consumer wait-free ring queue use only atomic operations to provide thread-safe enqueue
    and dequeue operations. Ring queue is bounded to the fixed capacity provided in the constructor.

    FIFO order is guaranteed!

    A combination of the algorithms described by the circular buffers documentation found in the Linux kernel, and the
    bounded MPMC queue by Dmitry Vyukov. Implemented in pure C++11. Should work across most CPU architectures.
    http://www.1024cores.net/home/lock-free-algorithms/queues/bounded-mpmc-queue
*/
template<typename T>
class SPSCRingQueue
{
public:
    //! Default class constructor
    /*!
        \param capacity - ring queue capacity (must be a power of two)
    */
    explicit SPSCRingQueue(uint64_t capacity);
    SPSCRingQueue(const SPSCRingQueue&) = delete;
    SPSCRingQueue(SPSCRingQueue&&) = default;
    ~SPSCRingQueue() { delete[] _buffer; }

    SPSCRingQueue& operator=(const SPSCRingQueue&) = delete;
    SPSCRingQueue& operator=(SPSCRingQueue&&) = default;

    //! Get ring queue capacity
    uint64_t capacity() const { return _capacity; }
    //! Get ring queue size
    uint64_t size() const;

    //! Enqueue an item into the ring queue (single producer thread method)
    /*!
        The item will be copied into the ring queue.

        \param item - item to enqueue
        \return 'true' if the item was successfully enqueue, 'false' if the ring queue is full
    */
    bool Enqueue(const T& item);

    //! Dequeue an item from the ring queue (single consumer thread method)
    /*!
        The item will be copied from the ring queue.

        \param item - item to dequeue
        \return 'true' if the item was successfully dequeue, 'false' if the ring queue is empty
    */
    bool Dequeue(T& item);

private:
    typedef char cache_line_pad[64];

    cache_line_pad _pad0;
    const uint64_t _capacity;
    const uint64_t _mask;
    T* const _buffer;

    cache_line_pad _pad1;
    std::atomic<uint64_t> _head;

    cache_line_pad _pad2;
    std::atomic<uint64_t> _tail;
};

/*! \example threads_spsc_ring_queue.cpp Single producer / single consumer wait-free ring queue example */

} // namespace CppCommon

#include "spsc_ring_queue.inl"

#endif //CPPCOMMON_SPSC_RING_QUEUE_H
