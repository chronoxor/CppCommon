/*!
    \file wf_bounded_queue.inl
    \brief Wait-free bounded queue class implementation
    \author Ivan Shynkarenka
    \date 19.01.2016
    \copyright MIT License
*/

namespace {
    constexpr bool IsPowerOfTwo(int64_t x) { return x && ((x & (x - 1)) == 0); }
}

namespace CppCommon {

template<typename T>
inline WFBoundedQueue<T>::WFBoundedQueue(int64_t capacity) : _capacity(capacity), _mask(capacity - 1), _buffer(new Node[capacity]), _head(0), _tail(0)
{
    assert((capacity > 1) && "Bounded queue capacity must be greater than one!");
    assert(IsPowerOfTwo(capacity) && "Bounded queue capacity must be a power of two!");

    // Populate the sequence initial values
    for (int64_t i = 0; i < capacity; ++i)
        _buffer[i].sequence.store(i, std::memory_order_relaxed);
}

template<typename T>
inline int64_t WFBoundedQueue<T>::size() const
{
    const int64_t head = _head.load(std::memory_order_relaxed);
    const int64_t tail = _tail.load(std::memory_order_relaxed);

    return (head >= tail) ? (head - tail) : (_capacity + head - tail);
}

template<typename T>
inline bool WFBoundedQueue<T>::Enqueue(const T& item)
{
    int64_t head_sequence = _head.load(std::memory_order_relaxed);

    for (;;)
    {
        Node* node = &_buffer[head_sequence & _mask];
        int64_t node_sequence = node->sequence.load(std::memory_order_acquire);

        // If node sequence and head sequence are the same then it means this slot is empty
        int64_t diff = node_sequence - head_sequence;
        if (diff == 0)
        {
            // Claim our spot by moving head. If head isn't the same
            // as we last checked then that means someone beat us to
            // the punch weak compare is faster, but can return spurious
            // results which in this instance is OK, because it's in the loop
            if (_head.compare_exchange_weak(head_sequence, head_sequence + 1, std::memory_order_relaxed))
            {
                // Store the item value
                node->value = item;

                // Increment the sequence so that the tail knows it's accessible
                node->sequence.store(head_sequence + 1, std::memory_order_release);
                return true;
            }
        }
        else if (diff < 0)
        {
            // If node sequence is less than head sequence then it means this slot is full
            // and therefore buffer is full
            return false;
        }
        else
        {
            // Under normal circumstances this branch should never be taken
            head_sequence = _head.load(std::memory_order_relaxed);
        }
    }

    // Never taken
    return false;
}

template<typename T>
inline bool WFBoundedQueue<T>::Dequeue(T& item)
{
    int64_t tail_sequence = _tail.load(std::memory_order_relaxed);

    for (;;)
    {
        Node* node = &_buffer[tail_sequence & _mask];
        int64_t node_sequence = node->sequence.load(std::memory_order_acquire);

        // If node sequence and head sequence are the same then it means this slot is empty
        int64_t diff = node_sequence - (tail_sequence + 1);
        if (diff == 0)
        {
            // Claim our spot by moving head. If head isn't the same
            // as we last checked then that means someone beat us to
            // the punch weak compare is faster, but can return spurious
            // results which in this instance is OK, because it's in the loop
            if (_tail.compare_exchange_weak(tail_sequence, tail_sequence + 1, std::memory_order_relaxed))
            {
                // Get the item value
                item = node->value;

                // Set the sequence to what the head sequence should be next time around
                node->sequence.store(tail_sequence + _mask + 1, std::memory_order_release);
                return true;
            }
        }
        else if (diff < 0)
        {
            // if seq is less than head seq then it means this slot is full and therefore the buffer is full
            return false;
        }
        else
        {
            // Under normal circumstances this branch should never be taken
            tail_sequence = _tail.load(std::memory_order_relaxed);
        }
    }

    // Never taken
    return false;
}

} // namespace CppCommon
