/*!
    \file allocator_null.inl
    \brief Null memory allocator inline implementation
    \author Ivan Shynkarenka
    \date 17.04.2017
    \copyright MIT License
*/

namespace CppCommon {

template <bool nothrow>
inline void* NullMemoryManager<nothrow>::allocate(size_t num, const void* hint)
{
    if (nothrow)
        return nullptr;
    else
        throw std::bad_alloc();
}

} // namespace CppCommon
