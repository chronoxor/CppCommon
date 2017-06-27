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
#include <unordered_set>
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
    std::set<int> set;
    std::unordered_set<int> unordered_set;
    std::vector<int> values;

    InsertFixture()
    {
        for (int i = 0; i < items; ++i)
            values.push_back(i);
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
        std::random_shuffle(values.begin(), values.end());
        for (auto value : values)
        {
            this->set.insert(value);
            this->unordered_set.insert(value);
            this->tree.insert(*allocator.Create(value));
        }
        std::random_shuffle(values.begin(), values.end());
    }
};

BENCHMARK_FIXTURE(InsertFixture<BinTree<MyBinTreeNode>>, "std::set: Insert")
{
    for (auto value : this->values)
        this->set.insert(value);

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
}

BENCHMARK_FIXTURE(InsertFixture<BinTree<MyBinTreeNode>>, "std::unordered_set: Insert")
{
    for (auto value : this->values)
        this->unordered_set.insert(value);

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
}

BENCHMARK_FIXTURE(InsertFixture<BinTreeAA<MyBinTreeNode>>, "BinTreeAA: Insert")
{
    for (auto value : this->values)
        this->tree.insert(*allocator.Create(value));

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
}

BENCHMARK_FIXTURE(InsertFixture<BinTreeAVL<MyBinTreeNode>>, "BinTreeAVL: Insert")
{
    for (auto value : this->values)
        this->tree.insert(*allocator.Create(value));

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
}

BENCHMARK_FIXTURE(InsertFixture<BinTreeRB<MyBinTreeNode>>, "BinTreeRB: Insert")
{
    for (auto value : this->values)
        this->tree.insert(*allocator.Create(value));

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
}

BENCHMARK_FIXTURE(FindFixture<BinTree<MyBinTreeNode>>, "std::set: Find")
{
    uint64_t crc = 0;

    for (auto value : this->values)
        crc += *this->set.find(value);

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<BinTree<MyBinTreeNode>>, "std::unordered_set: Find")
{
    uint64_t crc = 0;

    for (auto value : this->values)
        crc += *this->unordered_set.find(value);

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<BinTree<MyBinTreeNode>>, "BinTree: Find")
{
    uint64_t crc = 0;

    for (auto value : this->values)
    {
        MyBinTreeNode node(value);
        crc += this->tree.find(node)->value;
    }

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<BinTreeAA<MyBinTreeNode>>, "BinTreeAA: Find")
{
    uint64_t crc = 0;

    for (auto value : this->values)
    {
        MyBinTreeNode node(value);
        crc += this->tree.find(node)->value;
    }

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<BinTreeAVL<MyBinTreeNode>>, "BinTreeAVL: Find")
{
    uint64_t crc = 0;

    for (auto value : this->values)
    {
        MyBinTreeNode node(value);
        crc += this->tree.find(node)->value;
    }

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<BinTreeRB<MyBinTreeNode>>, "BinTreeRB: Find")
{
    uint64_t crc = 0;

    for (auto value : this->values)
    {
        MyBinTreeNode node(value);
        crc += this->tree.find(node)->value;
    }

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<BinTreeSplay<MyBinTreeNode>>, "BinTreeSplay: Find")
{
    uint64_t crc = 0;

    for (auto value : this->values)
    {
        MyBinTreeNode node(value);
        crc += this->tree.find(node)->value;
    }

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<BinTree<MyBinTreeNode>>, "std::set: Find & Remove")
{
    uint64_t crc = 0;

    for (auto value : this->values)
    {
        auto it = this->set.find(value);
        crc += *it;
        this->set.erase(it);
    }

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<BinTree<MyBinTreeNode>>, "std::unordered_set: Find & Remove")
{
    uint64_t crc = 0;

    for (auto value : this->values)
    {
        auto it = this->unordered_set.find(value);
        crc += *it;
        this->unordered_set.erase(it);
    }

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<BinTreeAA<MyBinTreeNode>>, "BinTreeAA: Find & Remove")
{
    uint64_t crc = 0;

    for (auto value : this->values)
    {
        MyBinTreeNode node(value);
        crc += this->tree.erase(*this->tree.find(node))->value;
    }

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<BinTreeAVL<MyBinTreeNode>>, "BinTreeAVL: Find & Remove")
{
    uint64_t crc = 0;

    for (auto value : this->values)
    {
        MyBinTreeNode node(value);
        crc += this->tree.erase(*this->tree.find(node))->value;
    }

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<BinTreeRB<MyBinTreeNode>>, "BinTreeRB: Find & Remove")
{
    uint64_t crc = 0;

    for (auto value : this->values)
    {
        MyBinTreeNode node(value);
        crc += this->tree.erase(*this->tree.find(node))->value;
    }

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_MAIN()
