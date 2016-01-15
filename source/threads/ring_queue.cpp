/*!
    \file ring_queue.h
    \brief Wait-free ring queue class definition
    \author Ivan Shynkarenka
    \date 15.01.2016
    \copyright MIT License
*/

#include "threads/ring_queue.h"

namespace CppCommon {

//! @cond
namespace Internals {

constexpr bool IsPowerOfTwo(size_t x)
{
    return x && ((x & (x-1)) == 0);
}

} // namespace Internals
//! @endcond

RingQueue(size_t size) : _size(size), _mask(size - 1), _buffer(new T[size]), _head(0), _tail(0)
{
    static_assert(IsPowerOfTwo(size), "Ring queue size must be a power of two!");
}

bool RingQueue::push(const T& item)
{
    const size_t head = _head.load(std::memory_order_relaxed);
    const size_t tail = _tail.load(std::memory_order_acquire);

    // Check if the ring queue is full
    if (((tail - head + 1) & _mask) == 0)
        return false;

    // Store item value
    _buffer[head & _mask] = item;

    // Increase head cursor
    _head.store(head + 1, std::memory_order_release);
    return true;
}

bool RingQueue::pop(T& item)
{
    const size_t tail = _tail.load(std::memory_order_relaxed);
    const size_t head = _head.load(std::memory_order_acquire);

    // Check if the ring queue is empty
    if (((head - tail) & _mask) == 0)
        return false;

    // Get item value
    item = _buffer[_tail & _mask];

    // Increase tail cursor
    _tail.store(tail + 1, std::memory_order_release);
    return true;
}

} // namespace CppCommon
