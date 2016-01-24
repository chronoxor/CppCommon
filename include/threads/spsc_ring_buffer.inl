/*!
    \file spsc_ring_buffer.inl
    \brief Single producer / single consumer wait-free ring buffer implementation
    \author Ivan Shynkarenka
    \date 16.01.2016
    \copyright MIT License
*/

namespace CppCommon {

inline SPSCRingBuffer::SPSCRingBuffer(uint64_t capacity) : _capacity(capacity - 1), _mask(capacity - 1), _buffer(new char[capacity]), _head(0), _tail(0)
{
    assert((capacity > 1) && "Ring buffer capacity must be greater than one!");
    assert(((capacity & (capacity - 1)) == 0) && "Ring buffer capacity must be a power of two!");
}

inline uint64_t SPSCRingBuffer::size() const
{
    const uint64_t head = _head.load(std::memory_order_acquire);
    const uint64_t tail = _tail.load(std::memory_order_acquire);

    return (head >= tail) ? (head - tail) : (_capacity + 1 + head - tail);
}

inline bool SPSCRingBuffer::Enqueue(const void* chunk, uint64_t size)
{
    assert((chunk != nullptr) && "Pointer to the chunk should not be equal to 'nullptr'!");
    assert((size > 0) && "Chunk size should be greater than zero!");
    assert((size <= _capacity) && "Chunk size should not be greater than ring buffer capacity!");

    if ((chunk == nullptr) || (size == 0) || (size > _capacity))
        return false;

    const uint64_t head = _head.load(std::memory_order_relaxed);
    const uint64_t tail = _tail.load(std::memory_order_acquire);

    // Check if there is required free space in the ring buffer
    if ((size + ((head - tail) & _mask)) > _capacity)
        return false;

    // Copy chunk of bytes into the ring buffer
    uint64_t remain = (_capacity - head) & _mask;
    uint64_t first = (size > remain) ? remain : size;
    uint64_t last = (size > remain) ? size - remain : 0;
    memcpy(&_buffer[head & _mask], (char*)chunk, first);
    memcpy(_buffer, (char*)chunk + first, last);

    // Increase the head cursor
    _head.store(head + size, std::memory_order_release);

    return true;
}

inline bool SPSCRingBuffer::Dequeue(void* chunk, uint64_t& size)
{
    assert((chunk != nullptr) && "Pointer to the chunk should not be equal to 'nullptr'!");
    assert((size > 0) && "Chunk size should be greater than zero!");
    assert((size >= _capacity) && "Chunk size should be greater than ring buffer capacity!");

    if ((chunk == nullptr) || (size == 0) || (size < _capacity))
        return false;

    const uint64_t tail = _tail.load(std::memory_order_relaxed);
    const uint64_t head = _head.load(std::memory_order_acquire);

    // Get the ring buffer size
    uint64_t available = ((head - tail) & _mask);
    if (size > available)
        size = available;

    // Check if the ring buffer is empty
    if (size == 0)
        return false;

    // Copy chunk of bytes from the ring buffer
    uint64_t remain = (_capacity - tail) & _mask;
    uint64_t first = (size > remain) ? remain : size;
    uint64_t last = (size > remain) ? size - remain : 0;
    memcpy((char*)chunk, &_buffer[tail & _mask], first);
    memcpy((char*)chunk + first, _buffer, last);

    // Increase the tail cursor
    _tail.store(tail + size, std::memory_order_release);

    return true;
}

} // namespace CppCommon
