/*!
    \file wait_batcher.h
    \brief Multiple producers / multiple consumers wait batcher definition
    \author Ivan Shynkarenka
    \date 13.03.2019
    \copyright MIT License
*/

#ifndef CPPCOMMON_THREADS_WAIT_BATCHER_H
#define CPPCOMMON_THREADS_WAIT_BATCHER_H

#include "condition_variable.h"

#include <cassert>
#include <vector>

namespace CppCommon {

//! Multiple producers / multiple consumers wait batcher
/*!
    Multiple producers / multiple consumers wait batcher provides a classic solution
    for producer-consumer problem using two vectors and hot-swap technique with monitor
    synchronization primitive (mutex with condition variable). It allows a consumer thread
    to process all items in queue in a batch mode.

    FIFO order is guaranteed!

    https://en.wikipedia.org/wiki/Producer%E2%80%93consumer_problem
*/
template<typename T>
class WaitBatcher
{
public:
    //! Default class constructor
    /*!
        \param capacity - Wait batcher capacity (0 for unlimited capacity, default is 0)
        \param initial - Initial wait batcher capacity (default is 0)
    */
    explicit WaitBatcher(size_t capacity = 0, size_t initial = 0);
    WaitBatcher(const WaitBatcher&) = delete;
    WaitBatcher(WaitBatcher&&) = delete;
    ~WaitBatcher();

    WaitBatcher& operator=(const WaitBatcher&) = delete;
    WaitBatcher& operator=(WaitBatcher&&) = delete;

    //! Check if the wait batcher is not empty
    explicit operator bool() const noexcept { return !closed() && !empty(); }

    //! Is wait batcher closed?
    bool closed() const;

    //! Is wait batcher empty?
    bool empty() const { return (size() == 0); }
    //! Get wait batcher capacity
    size_t capacity() const;
    //! Get wait batcher size
    size_t size() const;

    //! Enqueue an item into the wait batcher
    /*!
        The item will be copied into the wait batcher.

        Will block.

        \param item - Item to enqueue
        \return 'true' if the item was successfully enqueue, 'false' if the wait batcher is closed
    */
    bool Enqueue(const T& item);
    //! Enqueue an item into the wait batcher
    /*!
        The item will be moved into the wait batcher.

        Will block.

        \param item - Item to enqueue
        \return 'true' if the item was successfully enqueue, 'false' if the wait batcher is closed
    */
    bool Enqueue(T&& item);
    //! Enqueue all items into the wait batcher
    /*!
        All items will be copied into the wait batcher.

        Will block.

        \param first - Iterator to the first item
        \param last - Iterator to the last item
        \return 'true' if all items were successfully enqueue, 'false' if the wait batcher is closed
    */
    template <class InputIterator>
    bool Enqueue(InputIterator first, InputIterator last);

    //! Dequeue all items from the wait batcher
    /*!
        Will block.

        \param items - Items to dequeue
        \return 'true' if all items was successfully dequeue, 'false' if the wait batcher is closed
    */
    bool Dequeue(std::vector<T>& items);

    //! Close the wait batcher
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
    std::vector<T> _batch;
};

/*! \example threads_wait_batcher.cpp Multiple producers / multiple consumers wait batcher example */

} // namespace CppCommon

#include "wait_batcher.inl"

#endif // CPPCOMMON_THREADS_WAIT_BATCHER_H
