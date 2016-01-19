/*!
    \file wf_ring_queue.h
    \brief Wait-free ring queue class definition
    \author Ivan Shynkarenka
    \date 15.01.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_WF_RING_QUEUE_H
#define CPPCOMMON_WF_RING_QUEUE_H

#include <atomic>
#include <cassert>

namespace CppCommon {

//! Wait-free ring queue
/*!
    Single producer / single consumer wait-free ring queue use only atomic operations to provide thread-safe enqueue
    and dequeue operations. Ring queue is bounded to the fixed capacity provided in the constructor.

    A combination of the algorithms described by the circular buffers documentation found in the Linux kernel, and the
    bounded MPMC queue by Dmitry Vyukov. Implemented in pure C++11. Should work across most CPU architectures.
    http://www.1024cores.net/home/lock-free-algorithms/queues/bounded-mpmc-queue
*/
template<typename T>
class WFRingQueue
{
public:
    //! Default class constructor
    /*!
        \param capacity - ring queue capacity (must be a power of two)
    */
    WFRingQueue(int64_t capacity);
    WFRingQueue(const WFRingQueue&) = delete;
    WFRingQueue(WFRingQueue&&) = delete;
    ~WFRingQueue() { delete[] _buffer; }

    WFRingQueue& operator=(const WFRingQueue&) = delete;
    WFRingQueue& operator=(WFRingQueue&&) = delete;

    //! Get ring queue capacity
    int64_t capacity() const { return _capacity; }
    //! Get ring queue size
    int64_t size() const;

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
    const int64_t _capacity;
    const int64_t _mask;
    T* const _buffer;

    cache_line_pad _pad1;
    std::atomic<int64_t> _head;

    cache_line_pad _pad2;
    std::atomic<int64_t> _tail;
};

} // namespace CppCommon

#include "wf_ring_queue.inl"

#endif //CPPCOMMON_WF_RING_QUEUE_H
