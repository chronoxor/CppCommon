/*!
    \file allocator.inl
    \brief Memory allocator wrapper inline implementation
    \author Ivan Shynkarenka
    \date 17.04.2017
    \copyright MIT License
*/

namespace CppCommon {

template <bool nothrow>
inline void* MemoryManagerDefault<nothrow>::allocate(size_t num, const void* hint)
{
    void* result = malloc(num);
    if (result != nullptr)
        return result;

    if (nothrow)
        return nullptr;
    else
        throw std::bad_alloc();
}

template <bool nothrow>
void MemoryManagerDefault<nothrow>::deallocate(void* ptr, size_type num)
{
    free(ptr);
}

} // namespace CppCommon
