/*!
    \file wf_link_queue.inl
    \brief Wait-free link queue class implementation
    \author Ivan Shynkarenka
    \date 18.01.2016
    \copyright MIT License
*/

namespace CppCommon {

template<typename T>
inline WFLinkQueue<T>::WFLinkQueue() : _head(new Node), _tail(_head.load(std::memory_order_relaxed))
{
    // Link queue is initialized with a fake node as a head node
    Node* front = _head.load(std::memory_order_relaxed);
    front->next.store(nullptr, std::memory_order_relaxed);
}

template<typename T>
inline WFLinkQueue<T>::~WFLinkQueue()
{
    // Remove all nodes from the link queue
    T item;
    while (Dequeue(item)) {}

    // Remove the last fake node
    Node* front = _head.load(std::memory_order_relaxed);
    delete front;
}

template<typename T>
inline bool WFLinkQueue<T>::Enqueue(const T& item)
{
    // Create new head node
    Node* node = new Node;
    if (node == nullptr)
        return false;

    // Fill new head node with the given value
    node->value = item;
    node->next.store(nullptr, std::memory_order_relaxed);

    // Insert new head node and link it with the previous one
    Node* prev_head = _head.exchange(node, std::memory_order_acq_rel);
    prev_head->next.store(node, std::memory_order_release);

    return true;
}

template<typename T>
inline bool WFLinkQueue<T>::Dequeue(T& item)
{
    Node* tail = _tail.load(std::memory_order_relaxed);
    Node* next = tail->next.load(std::memory_order_acquire);

    // Check if the link queue is empty
    if (next == nullptr)
        return false;

    // Get the item value
    item = next->value;

    // Update tail node with a next one
    _tail.store(next, std::memory_order_release);

    // Remove the previous tail node
    delete tail;

    return true;
}

} // namespace CppCommon
