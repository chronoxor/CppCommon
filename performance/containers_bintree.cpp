//
// Created by Ivan Shynkarenka on 27.06.2017
//

#include "benchmark/cppbenchmark.h"

#include "containers/bintree.h"
#include "containers/bintree_aa.h"
#include "containers/bintree_avl.h"
#include "containers/bintree_rb.h"
#include "containers/bintree_splay.h"
#include "memory/allocator.h"
#include "memory/allocator_pool.h"

#include <algorithm>
#include <random>
#include <set>
#include <unordered_set>
#include <vector>

using namespace CppCommon;

const int items = 1000000;

struct MyBinTreeNode
{
    int value;

    MyBinTreeNode* parent;
    MyBinTreeNode* left;
    MyBinTreeNode* right;
    char balance;
    size_t level;
    bool rb;

    explicit MyBinTreeNode(int v) : value(v) {}
    friend bool operator<(const MyBinTreeNode& node1, const MyBinTreeNode& node2)
    { return node1.value < node2.value; }
};

template <class T>
class InsertFixture : public virtual CppBenchmark::Fixture
{
protected:
    T tree;
    std::set<int> set;
    std::unordered_set<int> unordered_set;
    std::vector<int> values;

    DefaultMemoryManager auxiliary;
    PoolMemoryManager<DefaultMemoryManager> pool;
    PoolAllocator<MyBinTreeNode> allocator;

    InsertFixture() : pool(auxiliary), allocator(pool)
    {
        for (int i = 0; i < items; ++i)
            values.push_back(i);
    }

    void Initialize(CppBenchmark::Context& context) override
    {
        std::default_random_engine random;
        std::shuffle(values.begin(), values.end(), random);
    }

    void Cleanup(CppBenchmark::Context& context) override
    {
        set.clear();
        unordered_set.clear();
        while (tree)
            allocator.Release(tree.erase(*tree.root()));
        pool.reset();
    }
};

template <class T>
class FindFixture : public InsertFixture<T>
{
protected:
    void Initialize(CppBenchmark::Context& context) override
    {
        std::default_random_engine random;
        std::shuffle(this->values.begin(), this->values.end(),  random);
        for (const auto& value : this->values)
        {
            this->set.insert(value);
            this->unordered_set.insert(value);
            this->tree.insert(*this->allocator.Create(value));
        }
        std::shuffle(this->values.begin(), this->values.end(), random);
    }
};

BENCHMARK_FIXTURE(InsertFixture<BinTree<MyBinTreeNode>>, "Insert: std::set")
{
    for (const auto& value : this->values)
        this->set.insert(value);

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
}

BENCHMARK_FIXTURE(InsertFixture<BinTree<MyBinTreeNode>>, "Insert: std::unordered_set")
{
    for (const auto& value : this->values)
        this->unordered_set.insert(value);

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
}

BENCHMARK_FIXTURE(InsertFixture<BinTree<MyBinTreeNode>>, "Insert: BinTree")
{
    for (const auto& value : this->values)
        this->tree.insert(*this->allocator.Create(value));

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
}

BENCHMARK_FIXTURE(InsertFixture<BinTreeAA<MyBinTreeNode>>, "Insert: BinTreeAA")
{
    for (const auto& value : this->values)
        this->tree.insert(*this->allocator.Create(value));

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
}

BENCHMARK_FIXTURE(InsertFixture<BinTreeAVL<MyBinTreeNode>>, "Insert: BinTreeAVL")
{
    for (const auto& value : this->values)
        this->tree.insert(*this->allocator.Create(value));

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
}

BENCHMARK_FIXTURE(InsertFixture<BinTreeRB<MyBinTreeNode>>, "Insert: BinTreeRB")
{
    for (const auto& value : this->values)
        this->tree.insert(*this->allocator.Create(value));

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
}

BENCHMARK_FIXTURE(FindFixture<BinTree<MyBinTreeNode>>, "Find: std::set")
{
    uint64_t crc = 0;

    for (const auto& value : this->values)
        crc += *this->set.find(value);

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<BinTree<MyBinTreeNode>>, "Find: std::unordered_set")
{
    uint64_t crc = 0;

    for (const auto& value : this->values)
        crc += *this->unordered_set.find(value);

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<BinTree<MyBinTreeNode>>, "Find: BinTree")
{
    uint64_t crc = 0;

    for (const auto& value : this->values)
    {
        MyBinTreeNode node(value);
        crc += this->tree.find(node)->value;
    }

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<BinTreeAA<MyBinTreeNode>>, "Find: BinTreeAA")
{
    uint64_t crc = 0;

    for (const auto& value : this->values)
    {
        MyBinTreeNode node(value);
        crc += this->tree.find(node)->value;
    }

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<BinTreeAVL<MyBinTreeNode>>, "Find: BinTreeAVL")
{
    uint64_t crc = 0;

    for (const auto& value : this->values)
    {
        MyBinTreeNode node(value);
        crc += this->tree.find(node)->value;
    }

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<BinTreeRB<MyBinTreeNode>>, "Find: BinTreeRB")
{
    uint64_t crc = 0;

    for (const auto& value : this->values)
    {
        MyBinTreeNode node(value);
        crc += this->tree.find(node)->value;
    }

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<BinTreeSplay<MyBinTreeNode>>, "Find: BinTreeSplay")
{
    uint64_t crc = 0;

    for (const auto& value : this->values)
    {
        MyBinTreeNode node(value);
        crc += this->tree.find(node)->value;
    }

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<BinTree<MyBinTreeNode>>, "Remove: std::set")
{
    uint64_t crc = 0;

    for (const auto& value : this->values)
    {
        auto it = this->set.find(value);
        crc += *it;
        this->set.erase(it);
    }

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<BinTree<MyBinTreeNode>>, "Remove: std::unordered_set")
{
    uint64_t crc = 0;

    for (const auto& value : this->values)
    {
        auto it = this->unordered_set.find(value);
        crc += *it;
        this->unordered_set.erase(it);
    }

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<BinTreeAA<MyBinTreeNode>>, "Remove: BinTreeAA")
{
    uint64_t crc = 0;

    for (const auto& value : this->values)
    {
        MyBinTreeNode node(value);
        crc += this->tree.erase(*this->tree.find(node))->value;
    }

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<BinTreeAVL<MyBinTreeNode>>, "Remove: BinTreeAVL")
{
    uint64_t crc = 0;

    for (const auto& value : this->values)
    {
        MyBinTreeNode node(value);
        crc += this->tree.erase(*this->tree.find(node))->value;
    }

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<BinTreeRB<MyBinTreeNode>>, "Remove: BinTreeRB")
{
    uint64_t crc = 0;

    for (const auto& value : this->values)
    {
        MyBinTreeNode node(value);
        crc += this->tree.erase(*this->tree.find(node))->value;
    }

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_MAIN()
