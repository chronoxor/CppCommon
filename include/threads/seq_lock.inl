/*!
    \file seq_lock.inl
    \brief Sequential lock synchronization primitive inline implementation
    \author Ivan Shynkarenka
    \date 17.08.2017
    \copyright MIT License
*/

namespace CppCommon {

template <typename T>
inline SeqLock<T>::SeqLock() : _seq(0)
{
    memset(_pad0, 0, sizeof(cache_line_pad));
    memset(_pad1, 0, sizeof(cache_line_pad));
}

template <typename T>
inline SeqLock<T>::SeqLock(const T& data) : SeqLock()
{
    _data = data;
}

template <typename T>
inline SeqLock<T>& SeqLock<T>::operator=(const T& data) noexcept
{
    Write(data);
    return *this;
}

template <typename T>
inline T SeqLock<T>::Read() const noexcept
{
    T data;
    size_t seq0;
    size_t seq1;

    do
    {
        seq0 = _seq.load(std::memory_order_acquire);
        std::atomic_signal_fence(std::memory_order_acq_rel);
        data = _data;
        std::atomic_signal_fence(std::memory_order_acq_rel);
        seq1 = _seq.load(std::memory_order_acquire);
    } while ((seq0 != seq1) || (seq0 & 1));

    return data;
}

template <typename T>
inline void SeqLock<T>::Write(const T& data) noexcept
{
    size_t seq0 = _seq.load(std::memory_order_relaxed);
    _seq.store(seq0 + 1, std::memory_order_release);
    std::atomic_signal_fence(std::memory_order_acq_rel);
    _data = data;
    std::atomic_signal_fence(std::memory_order_acq_rel);
    _seq.store(seq0 + 2, std::memory_order_release);
}

} // namespace CppCommon
