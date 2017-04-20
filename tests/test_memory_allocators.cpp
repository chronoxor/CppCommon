//
// Created by Ivan Shynkarenka on 27.07.2016
//

#include "catch.hpp"

#include "memory/allocator.h"
#include "memory/allocator_arena.h"
#include "memory/allocator_hybrid.h"
#include "memory/allocator_null.h"
#include "memory/allocator_stack.h"

#include <list>
#include <map>
#include <vector>
#include <unordered_map>

using namespace CppCommon;

TEST_CASE("Default allocator", "[CppCommon][Memory]")
{
    DefaultMemoryManager manger;
    DefaultAllocator<uint8_t> alloc(manger);

    uint8_t* ptr = alloc.allocate(1);
    REQUIRE(ptr != nullptr);
    alloc.deallocate(ptr, 1);

    ptr = alloc.allocate(10);
    REQUIRE(ptr != nullptr);
    alloc.deallocate(ptr, 10);
}

TEST_CASE("Null allocator", "[CppCommon][Memory]")
{
    NullMemoryManager manger;
    NullAllocator<uint8_t, true> alloc(manger);

    uint8_t* ptr = alloc.allocate(1);
    REQUIRE(ptr == nullptr);
    alloc.deallocate(ptr, 1);

    ptr = alloc.allocate(10);
    REQUIRE(ptr == nullptr);
    alloc.deallocate(ptr, 10);
}

TEST_CASE("Stack allocator", "[CppCommon][Memory]")
{
    StackMemoryManager<11, 1> manger;
    StackAllocator<uint8_t, 11, true, 1> alloc(manger);

    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 0);

    uint8_t* ptr = alloc.allocate(1);
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
    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 0);

    ptr = alloc.allocate(1);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.size() == 1);
    alloc.deallocate(ptr, 1);
}

TEST_CASE("Arena allocator", "[CppCommon][Memory]")
{
    ArenaMemoryManager<1> manger(11);
    ArenaAllocator<uint8_t, true, 1> alloc(manger);

    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 0);

    uint8_t* ptr = alloc.allocate(1);
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
    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 0);

    ptr = alloc.allocate(1);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.size() == 1);
    alloc.deallocate(ptr, 1);
}

TEST_CASE("Hybrid allocator", "[CppCommon][Memory]")
{
    DefaultMemoryManager auxiliary;
    HybridMemoryManager<DefaultMemoryManager, 1> manger(auxiliary, 11);
    HybridAllocator<uint8_t, DefaultMemoryManager, true, 1> alloc(manger);

    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 0);

    uint8_t* ptr = alloc.allocate(1);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.size() == 1);
    alloc.deallocate(ptr, 1);

    ptr = alloc.allocate(10);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.size() == 11);
    alloc.deallocate(ptr, 10);

    ptr = alloc.allocate(1);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.size() == 11);
    alloc.deallocate(ptr, 1);

    alloc.reset();
    REQUIRE(manger.capacity() == 12);
    REQUIRE(manger.size() == 0);

    ptr = alloc.allocate(1);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.size() == 1);
    alloc.deallocate(ptr, 1);
}

TEST_CASE("Hybrid allocator with stl containers", "[CppCommon][Memory]")
{
    DefaultMemoryManager auxiliary;
    HybridMemoryManager<DefaultMemoryManager> manger(auxiliary, 10);
    HybridAllocator<int, DefaultMemoryManager> alloc(manger);

    std::vector<int, decltype(alloc)> v(alloc);
    v.push_back(0);
    v.push_back(1);
    v.push_back(2);
    v.clear();

    std::list<int, decltype(alloc)> l(alloc);
    l.push_back(0);
    l.push_back(1);
    l.push_back(2);
    l.clear();

    std::map<int, int, std::less<int>, decltype(alloc)> m(alloc);
    m[0] = 0;
    m[1] = 10;
    m[2] = 20;
    m.clear();

    std::unordered_map<int, int, std::hash<int>, std::equal_to<int>, decltype(alloc)> u(alloc);
    u[0] = 0;
    u[1] = 10;
    u[2] = 20;
    u.clear();
}
