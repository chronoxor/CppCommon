/*!
    \file mpmc_ring_queue_ex.h
    \brief Multiple producers / multiple consumers wait-free ring queue class definition
    \author Ivan Shynkarenka
    \date 21.01.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_MPMC_RING_QUEUE_EX_H
#define CPPCOMMON_MPMC_RING_QUEUE_EX_H

#include <atomic>
#include <cassert>

namespace CppCommon {

//! Multiple producers / multiple consumers wait-free ring queue
/*!
    Multiple producers / multiple consumers wait-free ring queue use only atomic operations to provide thread-safe
    enqueue and dequeue operations. Ring queue size is limited to the capacity provided in the constructor.

    Based on CodeProject article - Yet another implementation of a lock-free circular array queue
    http://www.codeproject.com/Articles/153898/Yet-another-implementation-of-a-lock-free-circular
*/
template<typename T>
class MPMCRingQueueEx
{
public:
    //! Default class constructor
    /*!
        \param capacity - ring queue capacity (must be a power of two)
    */
    explicit MPMCRingQueueEx(int64_t capacity);
    MPMCRingQueueEx(const MPMCRingQueueEx&) = delete;
    MPMCRingQueueEx(MPMCRingQueueEx&&) = default;
    ~MPMCRingQueueEx() { delete[] _buffer; }

    MPMCRingQueueEx& operator=(const MPMCRingQueueEx&) = delete;
    MPMCRingQueueEx& operator=(MPMCRingQueueEx&&) = default;

    //! Get ring queue capacity
    int64_t capacity() const { return _capacity; }
    //! Get ring queue size
    int64_t size() const;

    //! Enqueue an item into the ring queue (multiple producers threads method)
    /*!
        The item will be copied into the ring queue.

        \param item - item to enqueue
        \return 'true' if the item was successfully enqueue, 'false' if the ring queue is full
    */
    bool Enqueue(const T& item);

    //! Dequeue an item from the ring queue (multiple consumers threads method)
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
    std::atomic<int64_t> _middle;

    cache_line_pad _pad3;
    std::atomic<int64_t> _tail;
};

} // namespace CppCommon

#include "mpmc_ring_queue_ex.inl"

#endif //CPPCOMMON_MPMC_RING_QUEUE_EX_H
