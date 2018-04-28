//
// Created by Ivan Shynkarenka on 27.07.2016
//

#include "test.h"

#include "memory/allocator.h"
#include "memory/allocator_arena.h"
#include "memory/allocator_heap.h"
#include "memory/allocator_null.h"
#include "memory/allocator_pool.h"
#include "memory/allocator_stack.h"

#include <list>
#include <map>
#include <vector>
#include <unordered_map>

using namespace CppCommon;

TEST_CASE("Default memory manager", "[CppCommon][Memory]")
{
    DefaultMemoryManager manger;
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);

    void* ptr = manger.malloc(1);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.allocated() == 1);
    REQUIRE(manger.allocations() == 1);
    manger.free(ptr, 1);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);

    ptr = manger.malloc(10);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.allocated() == 10);
    REQUIRE(manger.allocations() == 1);
    manger.free(ptr, 10);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
}

TEST_CASE("Heap memory manager", "[CppCommon][Memory]")
{
    HeapMemoryManager manger;
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);

    void* ptr = manger.malloc(1);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.allocated() == 1);
    REQUIRE(manger.allocations() == 1);
    manger.free(ptr, 1);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);

    ptr = manger.malloc(10);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.allocated() == 10);
    REQUIRE(manger.allocations() == 1);
    manger.free(ptr, 10);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
}

TEST_CASE("Null memory manager", "[CppCommon][Memory]")
{
    NullMemoryManager manger;
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);

    void* ptr = manger.malloc(1);
    REQUIRE(ptr == nullptr);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);

    ptr = manger.malloc(10);
    REQUIRE(ptr == nullptr);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
}

TEST_CASE("Stack memory manager", "[CppCommon][Memory]")
{
    StackMemoryManager<11> manger;
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 0);

    void* ptr = manger.malloc(1, 1);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.allocated() == 1);
    REQUIRE(manger.allocations() == 1);
    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 1);
    manger.free(ptr, 1);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 1);

    ptr = manger.malloc(10, 1);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.allocated() == 10);
    REQUIRE(manger.allocations() == 1);
    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 11);
    manger.free(ptr, 10);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 11);

    ptr = manger.malloc(1, 1);
    REQUIRE(ptr == nullptr);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 11);

    manger.reset();
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 0);

    ptr = manger.malloc(1, 1);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.allocated() == 1);
    REQUIRE(manger.allocations() == 1);
    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 1);
    manger.free(ptr, 1);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 1);
}

TEST_CASE("Arena memory manager with a fixed buffer", "[CppCommon][Memory]")
{
    DefaultMemoryManager auxiliary;
    uint8_t buffer[11];
    ArenaMemoryManager<DefaultMemoryManager> manger(auxiliary, buffer, 11);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 0);

    void* ptr = manger.malloc(1, 1);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.allocated() == 1);
    REQUIRE(manger.allocations() == 1);
    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 1);
    manger.free(ptr, 1);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 1);

    ptr = manger.malloc(10, 1);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.allocated() == 10);
    REQUIRE(manger.allocations() == 1);
    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 11);
    manger.free(ptr, 10);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 11);

    ptr = manger.malloc(1, 1);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.allocated() == 1);
    REQUIRE(manger.allocations() == 1);
    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 11);
    manger.free(ptr, 1);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 11);

    manger.reset();
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 0);

    ptr = manger.malloc(1, 1);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.allocated() == 1);
    REQUIRE(manger.allocations() == 1);
    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 1);
    manger.free(ptr, 1);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
    REQUIRE(manger.capacity() == 11);
    REQUIRE(manger.size() == 1);
}

TEST_CASE("Arena memory manager with a dynamic buffer", "[CppCommon][Memory]")
{
    DefaultMemoryManager auxiliary;
    ArenaMemoryManager<DefaultMemoryManager> manger(auxiliary, 11);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);

    void* ptr = manger.malloc(1, 1);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.allocated() == 1);
    REQUIRE(manger.allocations() == 1);
    manger.free(ptr, 1);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);

    ptr = manger.malloc(10, 1);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.allocated() == 10);
    REQUIRE(manger.allocations() == 1);
    manger.free(ptr, 10);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);

    ptr = manger.malloc(1, 1);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.allocated() == 1);
    REQUIRE(manger.allocations() == 1);
    manger.free(ptr, 1);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);

    manger.reset();
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);

    ptr = manger.malloc(1, 1);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.allocated() == 1);
    REQUIRE(manger.allocations() == 1);
    manger.free(ptr, 1);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
}

TEST_CASE("Arena allocator with stl direct access containers", "[CppCommon][Memory]")
{
    DefaultMemoryManager auxiliary;
    ArenaMemoryManager<DefaultMemoryManager> manger(auxiliary);
    ArenaAllocator<int, DefaultMemoryManager> alloc(manger);

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
}

TEST_CASE("Arena allocator with stl associative containers", "[CppCommon][Memory]")
{
    DefaultMemoryManager auxiliary;
    ArenaMemoryManager<DefaultMemoryManager> manger(auxiliary);
    ArenaAllocator<std::pair<const int, int>, DefaultMemoryManager> alloc(manger);

    std::map<int, int, std::less<>, decltype(alloc)> m(alloc);
    m[0] = 0;
    m[1] = 10;
    m[2] = 20;
    m.clear();

    std::unordered_map<int, int, std::hash<int>, std::equal_to<>, decltype(alloc)> u(alloc);
    u[0] = 0;
    u[1] = 10;
    u[2] = 20;
    u.clear();
}

TEST_CASE("Pool memory manager with a fixed buffer", "[CppCommon][Memory]")
{
    DefaultMemoryManager auxiliary;
    uint8_t buffer[80];
    PoolMemoryManager<DefaultMemoryManager> manger(auxiliary, buffer, 80);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
    REQUIRE(manger.chunk() == 80);
    REQUIRE(manger.chunks() == 1);

    void* ptr = manger.malloc(1, 1);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.allocated() == 1);
    REQUIRE(manger.allocations() == 1);
    REQUIRE(manger.chunk() == 80);
    REQUIRE(manger.chunks() == 1);
    manger.free(ptr, 1);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
    REQUIRE(manger.chunk() == 80);
    REQUIRE(manger.chunks() == 1);

    ptr = manger.malloc(10, 1);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.allocated() == 10);
    REQUIRE(manger.allocations() == 1);
    REQUIRE(manger.chunk() == 80);
    REQUIRE(manger.chunks() == 1);
    manger.free(ptr, 10);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
    REQUIRE(manger.chunk() == 80);
    REQUIRE(manger.chunks() == 1);

    ptr = manger.malloc(1, 1);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.allocated() == 1);
    REQUIRE(manger.allocations() == 1);
    REQUIRE(manger.chunk() == 80);
    REQUIRE(manger.chunks() == 1);
    manger.free(ptr, 1);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
    REQUIRE(manger.chunk() == 80);
    REQUIRE(manger.chunks() == 1);

    manger.reset();
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
    REQUIRE(manger.chunk() == 80);
    REQUIRE(manger.chunks() == 1);

    ptr = manger.malloc(1, 1);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.allocated() == 1);
    REQUIRE(manger.allocations() == 1);
    REQUIRE(manger.chunk() == 80);
    REQUIRE(manger.chunks() == 1);
    manger.free(ptr, 1);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
    REQUIRE(manger.chunk() == 80);
    REQUIRE(manger.chunks() == 1);
}

TEST_CASE("Pool memory manager with a dynamic buffer", "[CppCommon][Memory]")
{
    DefaultMemoryManager auxiliary;
    PoolMemoryManager<DefaultMemoryManager> manger(auxiliary, 80);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
    REQUIRE(manger.chunk() == 80);

    void* ptr = manger.malloc(1, 1);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.allocated() == 1);
    REQUIRE(manger.allocations() == 1);
    REQUIRE(manger.chunk() == 80);
    manger.free(ptr, 1);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
    REQUIRE(manger.chunk() == 80);

    ptr = manger.malloc(10, 1);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.allocated() == 10);
    REQUIRE(manger.allocations() == 1);
    REQUIRE(manger.chunk() == 80);
    manger.free(ptr, 10);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
    REQUIRE(manger.chunk() == 80);

    ptr = manger.malloc(1, 1);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.allocated() == 1);
    REQUIRE(manger.allocations() == 1);
    REQUIRE(manger.chunk() == 80);
    manger.free(ptr, 1);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
    REQUIRE(manger.chunk() == 80);

    manger.reset();
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
    REQUIRE(manger.chunk() == 80);

    ptr = manger.malloc(1, 1);
    REQUIRE(ptr != nullptr);
    REQUIRE(manger.allocated() == 1);
    REQUIRE(manger.allocations() == 1);
    REQUIRE(manger.chunk() == 80);
    manger.free(ptr, 1);
    REQUIRE(manger.allocated() == 0);
    REQUIRE(manger.allocations() == 0);
    REQUIRE(manger.chunk() == 80);
}

TEST_CASE("Pool allocator with stl direct access containers", "[CppCommon][Memory]")
{
    DefaultMemoryManager auxiliary;
    PoolMemoryManager<DefaultMemoryManager> manger(auxiliary);
    PoolAllocator<int, DefaultMemoryManager> alloc(manger);

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
}

TEST_CASE("Pool allocator with stl associative containers", "[CppCommon][Memory]")
{
    DefaultMemoryManager auxiliary;
    PoolMemoryManager<DefaultMemoryManager> manger(auxiliary);
    PoolAllocator<std::pair<const int, int>, DefaultMemoryManager> alloc(manger);

    std::map<int, int, std::less<>, decltype(alloc)> m(alloc);
    m[0] = 0;
    m[1] = 10;
    m[2] = 20;
    m.clear();

    std::unordered_map<int, int, std::hash<int>, std::equal_to<>, decltype(alloc)> u(alloc);
    u[0] = 0;
    u[1] = 10;
    u[2] = 20;
    u.clear();
}
