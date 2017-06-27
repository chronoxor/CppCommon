//
// Created by Ivan Shynkarenka on 27.06.2017
//

#include "benchmark/cppbenchmark.h"

#include "intrusive/bintree.h"
#include "intrusive/bintree_aa.h"
#include "intrusive/bintree_avl.h"
#include "intrusive/bintree_rb.h"
#include "intrusive/bintree_splay.h"
#include "memory/allocator.h"
#include "memory/allocator_pool.h"

#include <algorithm>
#include <set>
#include <vector>

using namespace CppCommon;

const int items = 100000;

struct MyBinTreeNode
{
    int value;

    MyBinTreeNode* parent;
    MyBinTreeNode* left;
    MyBinTreeNode* right;
    char balance;
    size_t level;
    bool rb;

    MyBinTreeNode(int v) : value(v) {}
    friend bool operator<(const MyBinTreeNode& node1, const MyBinTreeNode& node2)
    { return node1.value < node2.value; }
};

DefaultMemoryManager auxiliary;
PoolMemoryManager<DefaultMemoryManager> pool(auxiliary);
PoolAllocator<MyBinTreeNode> allocator(pool);

template <class T>
class InsertFixture : public virtual CppBenchmark::Fixture
{
protected:
    T tree;
    std::vector<int> values;

    InsertFixture()
    {
        for (int i = 0; i < items; ++i)
            values.push_back(i);
    }

    void Initialize(CppBenchmark::Context& context) override
    {
        std::random_shuffle(values.begin(), values.end());
    }

    void Cleanup(CppBenchmark::Context& context) override
    {
        while (tree)
            allocator.Release(tree.pop(*tree.root()));
        pool.reset();
    }
};

template <class T>
class FindFixture : public virtual CppBenchmark::Fixture
{
protected:
    T tree;
    std::vector<int> values;

    FindFixture()
    {
        for (int i = 0; i < items; ++i)
            values.push_back(i);
    }

    void Initialize(CppBenchmark::Context& context) override
    {
        std::random_shuffle(values.begin(), values.end());
        for (auto value : values)
            tree.push(*allocator.Create(value));
        std::random_shuffle(values.begin(), values.end());
    }

    void Cleanup(CppBenchmark::Context& context) override
    {
        while (tree)
            allocator.Release(tree.pop(*tree.root()));
        pool.reset();
    }
};

BENCHMARK_FIXTURE(InsertFixture<BinTree<MyBinTreeNode>>, "BinTree: Insert")
{
    for (auto value : values)
        tree.push(*allocator.Create(value));

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
}

BENCHMARK_FIXTURE(InsertFixture<BinTreeAA<MyBinTreeNode>>, "BinTreeAA: Insert")
{
    for (auto value : values)
        tree.push(*allocator.Create(value));

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
}

BENCHMARK_FIXTURE(InsertFixture<BinTreeAVL<MyBinTreeNode>>, "BinTreeAVL: Insert")
{
    for (auto value : values)
        tree.push(*allocator.Create(value));

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
}

BENCHMARK_FIXTURE(InsertFixture<BinTreeRB<MyBinTreeNode>>, "BinTreeRB: Insert")
{
    for (auto value : values)
        tree.push(*allocator.Create(value));

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
}

BENCHMARK_FIXTURE(InsertFixture<BinTreeSplay<MyBinTreeNode>>, "BinTreeSplay: Insert")
{
    for (auto value : values)
        tree.push(*allocator.Create(value));

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
}

BENCHMARK_FIXTURE(FindFixture<BinTree<MyBinTreeNode>>, "BinTree: Find")
{
    uint64_t crc = 0;

    for (auto value : values)
    {
        MyBinTreeNode node(value);
        crc += tree.find(node)->value;
    }

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<BinTreeAA<MyBinTreeNode>>, "BinTreeAA: Find")
{
    uint64_t crc = 0;

    for (auto value : values)
    {
        MyBinTreeNode node(value);
        crc += tree.find(node)->value;
    }

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<BinTreeAVL<MyBinTreeNode>>, "BinTreeAVL: Find")
{
    uint64_t crc = 0;

    for (auto value : values)
    {
        MyBinTreeNode node(value);
        crc += tree.find(node)->value;
    }

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<BinTreeRB<MyBinTreeNode>>, "BinTreeRB: Find")
{
    uint64_t crc = 0;

    for (auto value : values)
    {
        MyBinTreeNode node(value);
        crc += tree.find(node)->value;
    }

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<BinTreeSplay<MyBinTreeNode>>, "BinTreeSplay: Find")
{
    uint64_t crc = 0;

    for (auto value : values)
    {
        MyBinTreeNode node(value);
        crc += tree.find(node)->value;
    }

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<BinTree<MyBinTreeNode>>, "BinTree: Find & Remove")
{
    uint64_t crc = 0;

    for (auto value : values)
    {
        MyBinTreeNode node(value);
        crc += tree.pop(*tree.find(node))->value;
    }

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<BinTreeAA<MyBinTreeNode>>, "BinTreeAA: Find & Remove")
{
    uint64_t crc = 0;

    for (auto value : values)
    {
        MyBinTreeNode node(value);
        crc += tree.pop(*tree.find(node))->value;
    }

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<BinTreeAVL<MyBinTreeNode>>, "BinTreeAVL: Find & Remove")
{
    uint64_t crc = 0;

    for (auto value : values)
    {
        MyBinTreeNode node(value);
        crc += tree.pop(*tree.find(node))->value;
    }

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<BinTreeRB<MyBinTreeNode>>, "BinTreeRB: Find & Remove")
{
    uint64_t crc = 0;

    for (auto value : values)
    {
        MyBinTreeNode node(value);
        crc += tree.pop(*tree.find(node))->value;
    }

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<BinTreeSplay<MyBinTreeNode>>, "BinTreeSplay: Find & Remove")
{
    uint64_t crc = 0;

    for (auto value : values)
    {
        MyBinTreeNode node(value);
        crc += tree.pop(*tree.find(node))->value;
    }

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_MAIN()
