/*!
    \file mpmc_ring_queue.h
    \brief Multiple producers / multiple consumers wait-free ring queue definition
    \author Ivan Shynkarenka
    \date 19.01.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_THREADS_MPMC_RING_QUEUE_H
#define CPPCOMMON_THREADS_MPMC_RING_QUEUE_H

#include <atomic>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <utility>

namespace CppCommon {

//! Multiple producers / multiple consumers wait-free ring queue
/*!
    Multiple producers / multiple consumers wait-free ring queue use only atomic operations to provide thread-safe
    enqueue and dequeue operations. Ring queue size is limited to the capacity provided in the constructor.

    FIFO order is guaranteed!

    Thread-safe.

    C++ implementation of Dmitry Vyukov's non-intrusive lock free unbound MPSC queue
    http://www.1024cores.net/home/lock-free-algorithms/queues/non-intrusive-mpsc-node-based-queue
*/
template<typename T>
class MPMCRingQueue
{
public:
    //! Default class constructor
    /*!
        \param capacity - Ring queue capacity (must be a power of two)
    */
    explicit MPMCRingQueue(size_t capacity);
    MPMCRingQueue(const MPMCRingQueue&) = delete;
    MPMCRingQueue(MPMCRingQueue&&) = delete;
    ~MPMCRingQueue() { delete[] _buffer; }

    MPMCRingQueue& operator=(const MPMCRingQueue&) = delete;
    MPMCRingQueue& operator=(MPMCRingQueue&&) = delete;

    //! Check if the queue is not empty
    explicit operator bool() const noexcept { return !empty(); }

    //! Is ring queue empty?
    bool empty() const noexcept { return (size() == 0); }
    //! Get ring queue capacity
    size_t capacity() const noexcept { return _capacity; }
    //! Get ring queue size
    size_t size() const noexcept;

    //! Enqueue an item into the ring queue (multiple producers threads method)
    /*!
        The item will be copied into the ring queue.

        Will not block.

        \param item - Item to enqueue
        \return 'true' if the item was successfully enqueue, 'false' if the ring queue is full
    */
    bool Enqueue(const T& item);
    //! Enqueue an item into the ring queue (multiple producers threads method)
    /*!
        The item will be moved into the ring queue.

        Will not block.

        \param item - Item to enqueue
        \return 'true' if the item was successfully enqueue, 'false' if the ring queue is full
    */
    bool Enqueue(T&& item);

    //! Dequeue an item from the ring queue (multiple consumers threads method)
    /*!
        The item will be moved from the ring queue.

        Will not block.

        \param item - Item to dequeue
        \return 'true' if the item was successfully dequeue, 'false' if the ring queue is empty
    */
    bool Dequeue(T& item);

private:
    struct Node
    {
        std::atomic<size_t> sequence;
        T value;
    };

    typedef char cache_line_pad[128];

    cache_line_pad _pad0;
    const size_t _capacity;
    const size_t _mask;
    Node* const _buffer;

    cache_line_pad _pad1;
    std::atomic<size_t> _head;
    cache_line_pad _pad2;
    std::atomic<size_t> _tail;
    cache_line_pad _pad3;
};

/*! \example threads_mpmc_ring_queue.cpp Multiple producers / multiple consumers wait-free ring queue example */

} // namespace CppCommon

#include "mpmc_ring_queue.inl"

#endif // CPPCOMMON_THREADS_MPMC_RING_QUEUE_H
