/*!
    \file mpsc_ring_buffer.inl
    \brief Multiple producers / single consumer wait-free ring buffer class implementation
    \author Ivan Shynkarenka
    \date 26.01.2016
    \copyright MIT License
*/

namespace CppCommon {

inline MPSCRingBuffer::MPSCRingBuffer(uint64_t capacity, uint64_t concurrency) : _capacity(capacity - 1), _concurrency(concurrency), _consumer(0)
{
    // Initialize producers' ring buffer
    for (uint64_t i = 0; i < concurrency; ++i)
        _producers.push_back(std::make_shared<Producer>(capacity));
}

inline uint64_t MPSCRingBuffer::size() const
{
    uint64_t size = 0;
    for (auto producer : _producers)
        size += producer->buffer.size();
    return size;
}

bool MPSCRingBuffer::Enqueue(const void* chunk, uint64_t size)
{
    // Get producer index for the current thread based on RDTS value
    uint64_t timestamp = RDTS::current();
    uint64_t index = timestamp % _concurrency;

    // Lock the chosen producer using its spin-lock
    std::lock_guard<SpinLock> lock(_producers[index]->lock);

    // Enqueue the item into the producer's ring buffer
    return _producers[index]->buffer.Enqueue(chunk, size);
}

bool MPSCRingBuffer::Dequeue(void* chunk, uint64_t& size)
{
    // Try to dequeue one item from the one of producer's ring buffers
    for (uint64_t i = 0; i < _concurrency; ++i)
    {
        uint64_t temp = size;
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
