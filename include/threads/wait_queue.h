/*!
    \file wait_queue.h
    \brief Multiple producers / multiple consumers wait queue definition
    \author Ivan Shynkarenka
    \date 04.10.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_THREADS_WAIT_QUEUE_H
#define CPPCOMMON_THREADS_WAIT_QUEUE_H

#include "condition_variable.h"

#include <queue>

namespace CppCommon {

//! Multiple producers / multiple consumers wait queue
/*!
    Multiple producers / multiple consumers wait queue provides a classic solution
    for producer-consumer problem using queue and monitor synchronization primitive
    (mutex with condition variable).

    FIFO order is guaranteed!

    https://en.wikipedia.org/wiki/Producer%E2%80%93consumer_problem
*/
template<typename T>
class WaitQueue
{
public:
    //! Default class constructor
    /*!
        \param capacity - Wait queue capacity (default is 0 for unlimited capacity)
    */
    explicit WaitQueue(size_t capacity = 0);
    WaitQueue(const WaitQueue&) = delete;
    WaitQueue(WaitQueue&&) = default;
    ~WaitQueue();

    WaitQueue& operator=(const WaitQueue&) = delete;
    WaitQueue& operator=(WaitQueue&&) = default;

    //! Check if the wait queue is not empty
    explicit operator bool() const noexcept { return !closed() && !empty(); }

    //! Is wait queue closed?
    bool closed() const;

    //! Is wait queue empty?
    bool empty() const { return (size() == 0); }
    //! Get wait queue capacity
    size_t capacity() const;
    //! Get wait queue size
    size_t size() const;

    //! Enqueue an item into the wait queue
    /*!
        The item will be copied into the wait queue.

        Will block.

        \param item - Item to enqueue
        \return 'true' if the item was successfully enqueue, 'false' if the wait queue is closed
    */
    bool Enqueue(const T& item);
    //! Enqueue an item into the wait queue
    /*!
        The item will be moved into the wait queue.

        Will block.

        \param item - Item to enqueue
        \return 'true' if the item was successfully enqueue, 'false' if the wait queue is closed
    */
    bool Enqueue(T&& item);

    //! Dequeue an item from the wait queue
    /*!
        The item will be moved from the wait queue.

        Will block.

        \param item - Item to dequeue
        \return 'true' if the item was successfully dequeue, 'false' if the wait queue is closed
    */
    bool Dequeue(T& item);

    //! Close the wait queue
    /*!
        Will block.
    */
    void Close();

private:
    bool _closed;
    const size_t _capacity;
    mutable CriticalSection _cs;
    ConditionVariable _cv1;
    ConditionVariable _cv2;
    std::queue<T> _queue;
};

/*! \example threads_wait_queue.cpp Multiple producers / multiple consumers wait queue example */

} // namespace CppCommon

#include "wait_queue.inl"

#endif // CPPCOMMON_THREADS_WAIT_QUEUE_H
