/*!
    \file allocator.h
    \brief Memory allocator wrapper definition
    \author Ivan Shynkarenka
    \date 17.04.2017
    \copyright MIT License
*/

#ifndef CPPCOMMON_MEMORY_ALLOCATOR_WRAPPER_H
#define CPPCOMMON_MEMORY_ALLOCATOR_WRAPPER_H

#include "memory.h"

namespace CppCommon {

//! Memory allocator wrapper class
/*!
    Memory allocator wrapper implements standard allocator interface and
    wraps the memory manager provided as a template argument.

    Not thread-safe.
*/
template <typename T, class TMemoryManager, bool nothrow = false>
class Allocator
{
    template <typename U, class UMemoryManager, bool flag>
    friend class Allocator;

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

    //! Initialize allocator with a given memory manager
    /*!
        \param manager - Memory manager
    */
    Allocator(TMemoryManager& manager) noexcept : _manager(manager) {}
    template <typename U>
    Allocator(const Allocator<U, TMemoryManager, nothrow>& alloc) noexcept : _manager(alloc._manager) {}
    Allocator(const Allocator<T, TMemoryManager, nothrow>& alloc) noexcept : _manager(alloc._manager) {}
    Allocator(Allocator&&) noexcept = default;
    ~Allocator() noexcept = default;

    template <typename U>
    Allocator& operator=(const Allocator<U, TMemoryManager, nothrow>& alloc) noexcept
    { _manager = alloc._manager; return *this; }
    Allocator& operator=(const Allocator<T, TMemoryManager, nothrow>& alloc) noexcept
    { _manager = alloc._manager; return *this; }
    Allocator& operator=(Allocator&&) noexcept = default;

    //! Get the address of the given reference
    /*!
        \param x - Reference to the element
        \return Pointer to the element
    */
    pointer address(reference x) const noexcept { return &x; }
    //! Get the constant address of the given constant reference
    /*!
        \param x - Constant reference to the element
        \return Constant pointer to the element
    */
    const_pointer address(const_reference x) const noexcept { return &x; }

    //! Get the maximum number of elements, that could potentially be allocated by the allocator
    /*!
        \return The number of elements that might be allocated as maximum by a call to the allocate() method
    */
    size_type max_size() const noexcept { return _manager.max_size(); }

    //! Allocate a block of storage suitable to contain the given count of elements
    /*!
        \param num - Number of elements to be allocated
        \param hint - Allocation hint (default is 0)
        \return A pointer to the initial element in the block of storage
    */
    pointer allocate(size_type num, const void* hint = 0);
    //! Release a block of storage previously allocated
    /*!
        \param ptr - Pointer to a block of storage
        \param num - Number of releasing elements
    */
    void deallocate(pointer ptr, size_type num);

    //! Reset the allocator
    void reset() { _manager.reset(); }

    //! Constructs an element object on the given location pointer
    /*!
        \param ptr - Pointer to a location with enough storage space to contain an element
        \param val - Value to initialize the construced element to
    */
    template <typename U, class... Args>
    void construct(U* ptr, Args&&... args) { new((void*)ptr) U(args...); }
    //! Destroys in-place the object pointed by the given location pointer
    /*!
        \param ptr - Pointer to the object to be destroyed
    */
    template <typename U>
    void destroy(U* ptr) { ((U*)ptr)->~U(); }

    //! Rebind allocator
    template <typename TOther> struct rebind { using other = Allocator<TOther, TMemoryManager, nothrow>; };

private:
    TMemoryManager& _manager;
};

//! Memory allocator wrapper class (void specialization)
template <class TMemoryManager, bool nothrow>
class Allocator<void, TMemoryManager, nothrow>
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

    Allocator(TMemoryManager& manager) noexcept : _manager(manager) {}
    template <typename U>
    Allocator(const Allocator<U, TMemoryManager, nothrow>& alloc) noexcept : _manager(alloc._manager) {}
    Allocator(Allocator&&) noexcept = default;
    ~Allocator() noexcept = default;

    template <typename U>
    Allocator& operator=(const Allocator<U, TMemoryManager, nothrow>& alloc) noexcept
    { _manager = alloc._manager; return *this; }
    Allocator& operator=(Allocator&&) noexcept = default;

    template <typename T, typename U>
    friend bool operator==(const Allocator<T, TMemoryManager, nothrow>& alloc1, const Allocator<U, TMemoryManager, nothrow>& alloc2) noexcept
    { return true; }
    template <typename T, typename U>
    friend bool operator!=(const Allocator<T, TMemoryManager, nothrow>& alloc1, const Allocator<U, TMemoryManager, nothrow>& alloc2) noexcept
    { return false; }

    //! Rebind allocator
    template <typename TOther> struct rebind { using other = Allocator<TOther, TMemoryManager, nothrow>; };

private:
    TMemoryManager _manager;
};

//! Default memory manager class
/*!
    Default memory manager uses malloc() and free() system functions
    to allocate and deallocate memory.

    Not thread-safe.
*/
class DefaultMemoryManager
{
public:
    DefaultMemoryManager() noexcept = default;
    DefaultMemoryManager(const DefaultMemoryManager&) noexcept = default;
    DefaultMemoryManager(DefaultMemoryManager&&) noexcept = default;
    ~DefaultMemoryManager() noexcept = default;

    DefaultMemoryManager& operator=(const DefaultMemoryManager&) noexcept = default;
    DefaultMemoryManager& operator=(DefaultMemoryManager&&) noexcept = default;

    //! Get the maximum number of elements, that could potentially be allocated by the allocator
    /*!
        \return The number of elements that might be allocated as maximum by a call to the allocate() method
    */
    size_t max_size() const noexcept { return std::numeric_limits<size_t>::max(); }

    //! Allocate a block of storage suitable to contain the given count of elements
    /*!
        \param num - Number of elements to be allocated
        \param hint - Allocation hint (default is 0)
        \return A pointer to the initial element in the block of storage
    */
    void* allocate(size_t num, const void* hint = 0);
    //! Release a block of storage previously allocated
    /*!
        \param ptr - Pointer to a block of storage
        \param num - Number of releasing elements
    */
    void deallocate(void* ptr, size_t num);

    //! Reset the memory manager
    void reset() {}
};

//! Default memory allocator class
template <typename T, bool nothrow = false>
using DefaultAllocator = Allocator<T, DefaultMemoryManager, nothrow>;

} // namespace CppCommon

#include "allocator.inl"

#endif // CPPCOMMON_MEMORY_ALLOCATOR_WRAPPER_H
