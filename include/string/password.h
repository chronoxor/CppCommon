/*!
    \file password.h
    \brief Password string definition
    \author Ivan Shynkarenka
    \date 04.06.2019
    \copyright MIT License
*/

#ifndef CPPCOMMON_STRING_PASSWORD_H
#define CPPCOMMON_STRING_PASSWORD_H

#include <string>

namespace CppCommon {

//! Zero password memory
/*!
    Thread-safe.

    \param buffer - Password memory buffer
    \param size - Password memory buffer size
*/
void ZeroPasswordMemory(void* buffer, size_t size);

//! Password allocator
template <class T>
class PasswordAllocator : public std::allocator<T>
{
public:
    typedef typename std::allocator<T>::pointer pointer;
    typedef typename std::allocator<T>::size_type size_type;

    template<class U> struct rebind { typedef PasswordAllocator<U> other; };

    PasswordAllocator() throw() {}
    PasswordAllocator(const PasswordAllocator&) throw() {}
    template <class U> PasswordAllocator(const PasswordAllocator<U>&) throw() {}

    void deallocate(pointer p, size_type n)
    {
        ZeroPasswordMemory(p, n * sizeof(T));
        std::allocator<T>::deallocate(p, n);
    }
};

} // namespace CppCommon

namespace std {

//! Password string
/*!
    Password string extends std::string with a secure zero memory destructor
    which will fill string memory with zeros to avoid keep password in the
    wasted process memory.

    Not thread-safe.
*/
class password : public std::basic_string<char, std::char_traits<char>, CppCommon::PasswordAllocator<char>>
{
public:
    using basic_string::basic_string;
    ~password() { CppCommon::ZeroPasswordMemory(data(), size()); }
};

}

#endif // CPPCOMMON_STRING_PASSWORD_H
