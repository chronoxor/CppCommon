/*!
    \file wait_ring.h
    \brief Multiple producers / multiple consumers wait ring definition
    \author Ivan Shynkarenka
    \date 05.10.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_THREADS_WAIT_RING_H
#define CPPCOMMON_THREADS_WAIT_RING_H

#include "condition_variable.h"

#include <cassert>
#include <vector>

namespace CppCommon {

//! Multiple producers / multiple consumers wait ring
/*!
    Multiple producers / multiple consumers wait ring provides a classic solution
    for producer-consumer problem using fixed ring and monitor synchronization
    primitive (mutex with condition variable).

    FIFO order is guaranteed!

    https://en.wikipedia.org/wiki/Producer%E2%80%93consumer_problem
*/
template<typename T>
class WaitRing
{
public:
    //! Default class constructor
    /*!
        \param capacity - Ring capacity (must be a power of two)
    */
    explicit WaitRing(size_t capacity);
    WaitRing(const WaitRing&) = delete;
    WaitRing(WaitRing&&) = delete;
    ~WaitRing();

    WaitRing& operator=(const WaitRing&) = delete;
    WaitRing& operator=(WaitRing&&) = delete;

    //! Check if the wait ring is not empty
    explicit operator bool() const noexcept { return !closed() && !empty(); }

    //! Is wait ring closed?
    bool closed() const;

    //! Is wait ring empty?
    bool empty() const { return (size() == 0); }
    //! Get wait ring capacity
    size_t capacity() const { return _capacity; }
    //! Get wait ring size
    size_t size() const;

    //! Enqueue an item into the wait ring
    /*!
        The item will be copied into the wait ring.

        Will block.

        \param item - Item to enqueue
        \return 'true' if the item was successfully enqueue, 'false' if the wait ring is closed
    */
    bool Enqueue(const T& item);
    //! Enqueue an item into the wait ring
    /*!
        The item will be moved into the wait ring.

        Will block.

        \param item - Item to enqueue
        \return 'true' if the item was successfully enqueue, 'false' if the wait ring is closed
    */
    bool Enqueue(T&& item);

    //! Dequeue an item from the wait ring
    /*!
        The item will be moved from the wait ring.

        Will block.

        \param item - Item to dequeue
        \return 'true' if the item was successfully dequeue, 'false' if the wait ring is closed
    */
    bool Dequeue(T& item);

    //! Close the wait ring
    /*!
        Will block.
    */
    void Close();

private:
    bool _closed;
    mutable CriticalSection _cs;
    ConditionVariable _cv1;
    ConditionVariable _cv2;
    const size_t _capacity;
    const size_t _mask;
    size_t _head;
    size_t _tail;
    std::vector<T> _ring;
};

/*! \example threads_wait_ring.cpp Multiple producers / multiple consumers wait ring example */

} // namespace CppCommon

#include "wait_ring.inl"

#endif // CPPCOMMON_THREADS_WAIT_RING_H
