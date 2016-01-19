/*!
    \file wf_ring_buffer.inl
    \brief Wait-free ring buffer class implementation
    \author Ivan Shynkarenka
    \date 16.01.2016
    \copyright MIT License
*/

namespace {
    constexpr bool IsPowerOfTwo(int64_t x) { return x && ((x & (x - 1)) == 0); }
}

namespace CppCommon {

inline WFRingBuffer::WFRingBuffer(int64_t capacity) : _capacity(capacity - 1), _mask(capacity - 1), _buffer(new char[capacity]), _head(0), _tail(0)
{
    assert((capacity > 1) && "Ring buffer capacity must be greater than one!");
    assert(IsPowerOfTwo(capacity) && "Ring buffer capacity must be a power of two!");
}

inline int64_t WFRingBuffer::size() const
{
    const int64_t head = _head.load(std::memory_order_relaxed);
    const int64_t tail = _tail.load(std::memory_order_relaxed);

    return (head >= tail) ? (head - tail) : (_capacity + 1 + head - tail);
}

inline bool WFRingBuffer::Enqueue(const void* chunk, int64_t size)
{
    if ((chunk == nullptr) || (size == 0) || (size > _capacity))
        return false;

    const int64_t head = _head.load(std::memory_order_relaxed);
    const int64_t tail = _tail.load(std::memory_order_acquire);

    // Check if there is required free space in the ring buffer
    if ((size + ((head - tail) & _mask)) > _capacity)
        return false;

    // Copy chunk of bytes into the ring buffer
    int64_t remain = (_capacity - head) & _mask;
    int64_t first = (size > remain) ? remain : size;
    int64_t last = (size > remain) ? size - remain : 0;
    memcpy(&_buffer[head & _mask], (char*)chunk, first);
    memcpy(_buffer, (char*)chunk + first, last);

    // Increase the head cursor
    _head.store(head + size, std::memory_order_release);

    return true;
}

inline bool WFRingBuffer::Dequeue(void* chunk, int64_t& size)
{
    const int64_t tail = _tail.load(std::memory_order_relaxed);
    const int64_t head = _head.load(std::memory_order_acquire);

    // Get the ring buffer size
    int64_t available = ((head - tail) & _mask);
    if (size > available)
        size = available;

    // Check if the ring buffer is empty
    if (size == 0)
        return false;

    // Copy chunk of bytes from the ring buffer
    int64_t remain = (_capacity - tail) & _mask;
    int64_t first = (size > remain) ? remain : size;
    int64_t last = (size > remain) ? size - remain : 0;
    memcpy((char*)chunk, &_buffer[tail & _mask], first);
    memcpy((char*)chunk + first, _buffer, last);

    // Increase the tail cursor
    _tail.store(tail + size, std::memory_order_release);

    return true;
}

} // namespace CppCommon
