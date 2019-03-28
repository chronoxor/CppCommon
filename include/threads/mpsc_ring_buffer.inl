/*!
    \file mpsc_ring_buffer.inl
    \brief Multiple producers / single consumer wait-free ring buffer class inline implementation
    \author Ivan Shynkarenka
    \date 26.01.2016
    \copyright MIT License
*/

namespace CppCommon {

inline MPSCRingBuffer::MPSCRingBuffer(size_t capacity, size_t concurrency) : _capacity(capacity - 1), _concurrency(concurrency), _consumer(0)
{
    // Initialize producers' ring buffer
    for (size_t i = 0; i < concurrency; ++i)
        _producers.push_back(std::make_shared<Producer>(capacity));
}

inline size_t MPSCRingBuffer::size() const noexcept
{
    size_t size = 0;
    for (const auto& producer : _producers)
        size += producer->buffer.size();
    return size;
}

inline bool MPSCRingBuffer::Enqueue(const void* chunk, size_t size)
{
    // Get producer index for the current thread based on RDTS value
    size_t index = Timestamp::rdts() % _concurrency;

    // Lock the chosen producer using its spin-lock
    Locker<SpinLock> lock(_producers[index]->lock);

    // Enqueue the item into the producer's ring buffer
    return _producers[index]->buffer.Enqueue(chunk, size);
}

inline bool MPSCRingBuffer::Dequeue(void* chunk, size_t& size)
{
    // Try to dequeue one item from the one of producer's ring buffers
    for (size_t i = 0; i < _concurrency; ++i)
    {
        size_t temp = size;
        if (_producers[_consumer++ % _concurrency]->buffer.Dequeue(chunk, temp))
        {
            size = temp;
            return true;
        }
    }

    size = 0;
    return false;
}

} // namespace CppCommon
