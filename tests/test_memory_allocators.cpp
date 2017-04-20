//
// Created by Ivan Shynkarenka on 27.07.2016
//

#include "catch.hpp"

#include "memory/allocator.h"
#include "memory/allocator_arena.h"
#include "memory/allocator_null.h"
#include "memory/allocator_stack.h"

using namespace CppCommon;

TEST_CASE("Default allocator", "[CppCommon][Memory]")
{
    DefaultMemoryManager<> manger;
    DefaultAllocator<int> alloc(manger);

    int* ptr = alloc.allocate(1);
    REQUIRE(ptr != nullptr);
    alloc.deallocate(ptr, 1);

    ptr = alloc.allocate(10);
    REQUIRE(ptr != nullptr);
    alloc.deallocate(ptr, 10);
}

TEST_CASE("Null allocator", "[CppCommon][Memory]")
{
    NullMemoryManager<true> manger;
    NullAllocator<int, true> alloc(manger);

    int* ptr = alloc.allocate(1);
    REQUIRE(ptr == nullptr);
    alloc.deallocate(ptr, 1);

    ptr = alloc.allocate(10);
    REQUIRE(ptr == nullptr);
    alloc.deallocate(ptr, 10);
}

TEST_CASE("Arena allocator", "[CppCommon][Memory]")
{
    ArenaMemoryManager<true, 1> manger(11);
    ArenaAllocator<int, true, 1> alloc(manger);

    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 0);

    int* ptr = alloc.allocate(1);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.size() == 1);
    alloc.deallocate(ptr, 1);

    ptr = alloc.allocate(10);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.size() == 11);
    alloc.deallocate(ptr, 10);

    ptr = alloc.allocate(1);
    REQUIRE(ptr == nullptr);

    alloc.reset();
    REQUIRE(manger.size() == 0);

    ptr = alloc.allocate(1);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.size() == 1);
    alloc.deallocate(ptr, 1);
}

TEST_CASE("Stack allocator", "[CppCommon][Memory]")
{
    StackMemoryManager<11, true, 1> manger;
    StackAllocator<int, 11, true, 1> alloc(manger);

    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 0);

    int* ptr = alloc.allocate(1);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.size() == 1);
    alloc.deallocate(ptr, 1);

    ptr = alloc.allocate(10);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.size() == 11);
    alloc.deallocate(ptr, 10);

    ptr = alloc.allocate(1);
    REQUIRE(ptr == nullptr);

    alloc.reset();
    REQUIRE(manger.size() == 0);

    ptr = alloc.allocate(1);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.size() == 1);
    alloc.deallocate(ptr, 1);
}
