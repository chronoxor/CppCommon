/*!
    \file allocator_pool.inl
    \brief Memory pool allocator inline implementation
    \author Ivan Shynkarenka
    \date 08.05.2017
    \copyright MIT License
*/

namespace CppCommon {

template <class TAuxMemoryManager, std::size_t alignment>
inline PoolMemoryManager<TAuxMemoryManager, alignment>::PoolMemoryManager(TAuxMemoryManager& auxiliary, size_t chunk, size_t chunks)
    : _allocated(0),
      _allocations(0),
      _auxiliary(auxiliary),
      _current(nullptr),
      _chunk(chunk),
      _chunks(chunks),
      _free_block(nullptr)
{
    reset(chunk, chunks);
}

template <class TAuxMemoryManager, std::size_t alignment>
inline PoolMemoryManager<TAuxMemoryManager, alignment>::PoolMemoryManager(TAuxMemoryManager& auxiliary, uint8_t* buffer, size_t size)
    : _allocated(0),
      _allocations(0),
      _auxiliary(auxiliary),
      _current(nullptr),
      _chunk(chunk),
      _chunks(chunks),
      _free_block(nullptr)
{
    reset(buffer, size);
}

template <class TAuxMemoryManager, std::size_t alignment>
inline void* PoolMemoryManager<TAuxMemoryManager, alignment>::malloc(size_t size, const void* hint)
{

}

template <class TAuxMemoryManager, std::size_t alignment>
inline void PoolMemoryManager<TAuxMemoryManager, alignment>::free(void* ptr, size_t size)
{

}

template <class TAuxMemoryManager, std::size_t alignment>
inline void PoolMemoryManager<TAuxMemoryManager, alignment>::reset()
{

}

template <class TAuxMemoryManager, std::size_t alignment>
inline void PoolMemoryManager<TAuxMemoryManager, alignment>::reset(size_t chunk, size_t chunks)
{

}

template <class TAuxMemoryManager, std::size_t alignment>
inline void PoolMemoryManager<TAuxMemoryManager, alignment>::reset(uint8_t* buffer, size_t size)
{

}

template <class TAuxMemoryManager, std::size_t alignment>
inline void PoolMemoryManager<TAuxMemoryManager, alignment>::clear()
{

}

} // namespace CppCommon
