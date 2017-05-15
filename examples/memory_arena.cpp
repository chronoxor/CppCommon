/*!
    \file memory_arena.cpp
    \brief Arena memory allocator example
    \author Ivan Shynkarenka
    \date 15.05.2017
    \copyright MIT License
*/

#include "memory/allocator_arena.h"

#include <iostream>

int main(int argc, char** argv)
{
    CppCommon::DefaultMemoryManager auxiliary;
    CppCommon::ArenaMemoryManager<CppCommon::DefaultMemoryManager> manger(auxiliary);
    CppCommon::ArenaAllocator<int, CppCommon::DefaultMemoryManager> alloc(manger);

    int* v = alloc.Create(123);
    std::cout << "v = " << *v << std::endl;
    alloc.Release(v);

    int* a = alloc.CreateArray(3, 123);
    std::cout << "a[0] = " << a[0] << std::endl;
    std::cout << "a[1] = " << a[1] << std::endl;
    std::cout << "a[2] = " << a[2] << std::endl;
    alloc.ReleaseArray(a);

    return 0;
}
