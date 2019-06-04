/*!
    \file secure.h
    \brief Secure string definition
    \author Ivan Shynkarenka
    \date 04.06.2019
    \copyright MIT License
*/

#ifndef CPPCOMMON_STRING_SECURE_H
#define CPPCOMMON_STRING_SECURE_H

#include <string>

namespace CppCommon {

//! Secure zero memory buffer
/*!
    Thread-safe.

    \param buffer - Memory buffer
    \param size - Memory buffer size
*/
void SecureZeroMemoryBuffer(void* buffer, size_t size);

//! Secure string allocator
template <class T>
class SecureAllocator : public std::allocator<T>
{
public:
    template<class U> struct rebind { typedef SecureAllocator<U> other; };

    SecureAllocator() throw() {}
    SecureAllocator(const SecureAllocator&) throw() {}
    template <class U> SecureAllocator(const SecureAllocator<U>&) throw() {}

    void deallocate(pointer p, size_type n)
    {
        SecureZeroMemoryBuffer(p, n * sizeof(T));
        std::allocator<T>::deallocate(p, n);
    }
};

//! Secure string
/*!
    Secure string extends std::string with a secure zero memory destructor
    which will fill string memory with zeros to avoid keep password in the
    wasted process memory.

    Not thread-safe.
*/
typedef std::basic_string<char, std::char_traits<char>, SecureAllocator<char>> SecureString;

} // namespace CppCommon

#endif // CPPCOMMON_STRING_SECURE_H
