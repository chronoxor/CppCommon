/*!
    \file allocator_null.inl
    \brief Null memory allocator inline implementation
    \author Ivan Shynkarenka
    \date 17.04.2017
    \copyright MIT License
*/

namespace CppCommon {

inline void* NullMemoryManager::allocate(size_t size, const void* hint)
{
    // Not enough memory...
    return nullptr;
}

} // namespace CppCommon
