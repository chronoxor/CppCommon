/*!
    \file null_allocator.h
    \brief Null memory allocator definition
    \author Ivan Shynkarenka
    \date 17.04.2017
    \copyright MIT License
*/

#ifndef CPPCOMMON_MEMORY_NULL_ALLOCATOR_H
#define CPPCOMMON_MEMORY_NULL_ALLOCATOR_H

#include <memory>

namespace CppCommon {

//! Null memory allocator class
/*!
    Null memory allocator will always return nullptr for any attempt
    to allocate a memory buffer of the given size.

    Thread-safe.
*/
template <typename T>
class NullAllocator
{
public:
    //! Element type
    typedef T value_type;
    //! Pointer to element
    typedef T* pointer;
    //! Reference to element
    typedef T& reference;
    //! Pointer to constant element
    typedef const T* const_pointer;
    //! Reference to constant element
    typedef const T& const_reference;
    //! Quantities of elements
    typedef size_t size_type;
    //! Difference between two pointers
    typedef ptrdiff_t difference_type;

    NullAllocator() = default;
    NullAllocator(const NullAllocator&) = default;
    NullAllocator(NullAllocator&&) = default;
    ~NullAllocator() = default;

    NullAllocator& operator=(const NullAllocator&) = default;
    NullAllocator& operator=(NullAllocator&&) = default;

    //! Get the address of the given reference
    /*!
        \param x - Reference to the element
        \return Pointer to the element
    */
    pointer address(reference x) const { return &x; }
    //! Get the constant address of the given constant reference
    /*!
        \param x - Constant reference to the element
        \return Constant pointer to the element
    */
    const_pointer address(const_reference x) const { return &x; }

    //! Get the maximum number of elements, that could potentially be allocated by the allocator
    /*!
        \return The number of elements that might be allocated as maximum by a call to the allocate() method
    */
    size_type max_size() const throw() { return std::numeric_limits<size_type>::max(); }

    //! Allocate a block of storage suitable to contain the given count of elements
    /*!
        \param n - Number of elements to be allocated
        \param hint - Allocation hint (default is 0)
        \return A pointer to the initial element in the block of storage
    */
    pointer allocate(size_type n, const void* hint = 0) { return nullptr; }
    //! Release a block of storage previously allocated
    /*!
        \param p - Pointer to a block of storage
        \param n - Number of releasing elements
    */
    void deallocate(pointer p, size_type n) {}

    //! Constructs an element object on the given location pointer
    /*!
        \param p - Pointer to a location with enough storage space to contain an element
        \param val - Value to initialize the construced element to
    */
    void construct(pointer p, const_reference val) { new((void*)p) T(val); }
    //! Destroys in-place the object pointed by the given location pointer
    /*!
        \param p - Pointer to the object to be destroyed
    */
    void destroy(pointer p) { ((T*)p)->~T(); }
};

//! Null memory allocator class (void type specialization)
template <>
class NullAllocator<void>
{
public:
    //! Element type
    typedef void value_type;
    //! Pointer to element
    typedef void* pointer;
    //! Pointer to constant element
    typedef const void* const_pointer;
    //! Quantities of elements
    typedef size_t size_type;
    //! Difference between two pointers
    typedef ptrdiff_t difference_type;

    NullAllocator() = default;
    NullAllocator(const NullAllocator&) = default;
    NullAllocator(NullAllocator&&) = default;
    ~NullAllocator() = default;

    NullAllocator& operator=(const NullAllocator&) = default;
    NullAllocator& operator=(NullAllocator&&) = default;
};

} // namespace CppCommon

#endif // CPPCOMMON_MEMORY_NULL_ALLOCATOR_H
