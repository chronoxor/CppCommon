/*!
    \file spsc_ring_queue.h
    \brief Single producer / single consumer wait-free ring queue definition
    \author Ivan Shynkarenka
    \date 15.01.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_THREADS_SPSC_RING_QUEUE_H
#define CPPCOMMON_THREADS_SPSC_RING_QUEUE_H

#include <atomic>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <utility>

namespace CppCommon {

//! Single producer / single consumer wait-free ring queue
/*!
    Single producer / single consumer wait-free ring queue use only atomic operations to provide thread-safe enqueue
    and dequeue operations. Ring queue is bounded to the fixed capacity provided in the constructor.

    FIFO order is guaranteed!

    Thread-safe.

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
        \param capacity - Ring queue capacity (must be a power of two)
    */
    explicit SPSCRingQueue(size_t capacity);
    SPSCRingQueue(const SPSCRingQueue&) = delete;
    SPSCRingQueue(SPSCRingQueue&&) = delete;
    ~SPSCRingQueue() { delete[] _buffer; }

    SPSCRingQueue& operator=(const SPSCRingQueue&) = delete;
    SPSCRingQueue& operator=(SPSCRingQueue&&) = delete;

    //! Check if the queue is not empty
    explicit operator bool() const noexcept { return !empty(); }

    //! Is ring queue empty?
    bool empty() const noexcept { return (size() == 0); }
    //! Get ring queue capacity
    size_t capacity() const noexcept { return _capacity; }
    //! Get ring queue size
    size_t size() const noexcept;

    //! Enqueue an item into the ring queue (single producer thread method)
    /*!
        The item will be copied into the ring queue.

        Will not block.

        \param item - Item to enqueue
        \return 'true' if the item was successfully enqueue, 'false' if the ring queue is full
    */
    bool Enqueue(const T& item);
    //! Enqueue an item into the ring queue (single producer thread method)
    /*!
        The item will be moved into the ring queue.

        Will not block.

        \param item - Item to enqueue
        \return 'true' if the item was successfully enqueue, 'false' if the ring queue is full
    */
    bool Enqueue(T&& item);

    //! Dequeue an item from the ring queue (single consumer thread method)
    /*!
        The item will be moved from the ring queue.

        Will not block.

        \param item - Item to dequeue
        \return 'true' if the item was successfully dequeue, 'false' if the ring queue is empty
    */
    bool Dequeue(T& item);

private:
    typedef char cache_line_pad[128];

    cache_line_pad _pad0;
    const size_t _capacity;
    const size_t _mask;
    T* const _buffer;

    cache_line_pad _pad1;
    std::atomic<size_t> _head;
    cache_line_pad _pad2;
    std::atomic<size_t> _tail;
    cache_line_pad _pad3;
};

/*! \example threads_spsc_ring_queue.cpp Single producer / single consumer wait-free ring queue example */

} // namespace CppCommon

#include "spsc_ring_queue.inl"

#endif // CPPCOMMON_THREADS_SPSC_RING_QUEUE_H
