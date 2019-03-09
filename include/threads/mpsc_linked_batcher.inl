/*!
    \file mpsc_linked_batcher.inl
    \brief Multiple producers / single consumer wait-free linked batcher inline implementation
    \author Ivan Shynkarenka
    \date 19.01.2016
    \copyright MIT License
*/

namespace CppCommon {

template<typename T>
inline MPSCLinkedBatcher<T>::MPSCLinkedBatcher() : _head(nullptr)
{
}

template<typename T>
inline MPSCLinkedBatcher<T>::~MPSCLinkedBatcher()
{
    // Remove all nodes from the linked batcher
    Dequeue([](const T&){});
}

template<typename T>
inline bool MPSCLinkedBatcher<T>::Enqueue(const T& item)
{
    T temp = item;
    return Enqueue(std::forward<T>(temp));
}

template<typename T>
inline bool MPSCLinkedBatcher<T>::Enqueue(T&& item)
{
    // Create new head node
    Node* node = new Node;
    if (node == nullptr)
        return false;

    // Fill new head node with the given value
    node->value = std::move(item);

    // Insert new head node into the batcher and linked it with the previous one
    Node* prev_head = _head.load(std::memory_order_relaxed);
    do
    {
        node->next = prev_head;
    } while (!_head.compare_exchange_weak(prev_head, node, std::memory_order_release));

    return true;
}

template<typename T>
inline bool MPSCLinkedBatcher<T>::Dequeue(const std::function<void(const T&)>& handler)
{
    assert((handler) && "Batch handler must be valid!");

    Node* last = _head.exchange(nullptr, std::memory_order_acq_rel);
    Node* first = nullptr;

    // Check if the linked batcher is empty
    if (last == nullptr)
        return false;

    // Reverse the order to get nodes in FIFO order
    do
    {
        Node* temp = last;
        last = last->next;
        temp->next = first;
        first = temp;
    } while (last != nullptr);

    // Process all items in a batch mode
    do
    {
        Node* temp = first;
        first = first->next;
        // Process the item with the given handler
        handler(temp->value);
        delete temp;
    } while (first != nullptr);

    return true;
}

} // namespace CppCommon
