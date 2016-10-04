/*!
    \file wait_queue.inl
    \brief Multiple producers / multiple consumers wait queue inline implementation
    \author Ivan Shynkarenka
    \date 04.10.2016
    \copyright MIT License
*/

namespace CppCommon {

template<typename T>
inline WaitQueue<T>::WaitQueue() : _destroyed(false)
{
}

template<typename T>
inline WaitQueue<T>::~WaitQueue()
{
    Locker<CriticalSection> locker(_cs);
    _destroyed = true;
    _cv.NotifyAll();
}

template<typename T>
inline size_t WaitQueue<T>::size() const
{
    Locker<CriticalSection> locker(_cs);
    return _queue.size();
}

template<typename T>
inline bool WaitQueue<T>::Enqueue(const T& item)
{
    Locker<CriticalSection> locker(_cs);
    if (_destroyed)
        return false;
    _queue.push(item);
    _cv.NotifyOne();
    return true;
}

template<typename T>
inline bool WaitQueue<T>::Enqueue(T&& item)
{
    Locker<CriticalSection> locker(_cs);
    if (_destroyed)
        return false;
    _queue.emplace(item);
    _cv.NotifyOne();
    return true;
}

template<typename T>
inline bool WaitQueue<T>::Dequeue(T& item)
{
    Locker<CriticalSection> locker(_cs);

    if (!_queue.empty())
    {
        item = _queue.front();
        _queue.pop();
        return true;
    }

    if (_destroyed)
        return false;

    _cv.Wait(_cs, [this]() { return (_destroyed || !_queue.empty()); });

    if (!_queue.empty())
    {
        item = _queue.front();
        _queue.pop();
        return true;
    }

    if (_destroyed)
        return false;

    // Never occurred...
    return false;
}

} // namespace CppCommon
