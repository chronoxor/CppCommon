/*!
    \file allocator_arena.inl
    \brief Arena memory allocator inline implementation
    \author Ivan Shynkarenka
    \date 17.04.2017
    \copyright MIT License
*/

namespace CppCommon {

template <bool nothrow, std::size_t alignment>
inline MemoryManagerArena<nothrow, alignment>::MemoryManagerArena(size_t capacity)
    : _external(false),
      _arena(new uint8_t[capacity]),
      _capacity(capacity),
      _size(0)
{
    assert((_capacity > 0) && "Arena capacity must be greater than zero!");
    assert((_arena != nullptr) && "Arena buffer must be valid!");
}

template <bool nothrow, std::size_t alignment>
inline MemoryManagerArena<nothrow, alignment>::MemoryManagerArena(uint8_t* buffer, size_t size)
    : _external(true),
      _arena(buffer),
      _capacity(size),
      _size(0)
{
    assert((_capacity > 0) && "Arena capacity must be greater than zero!");
    assert((_arena != nullptr) && "Arena buffer must be valid!");
}

template <bool nothrow, std::size_t alignment>
inline MemoryManagerArena<nothrow, alignment>::~MemoryManagerArena()
{
    if (!_external)
        delete [] _arena;
}

template <bool nothrow, std::size_t alignment>
inline void* MemoryManagerArena<nothrow, alignment>::allocate(size_t num, const void* hint)
{

}

template <bool nothrow, std::size_t alignment>
inline void MemoryManagerArena<nothrow, alignment>::reset()
{
    _size = 0;
}

} // namespace CppCommon
