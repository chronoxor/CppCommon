//
// Created by Ivan Shynkarenka on 13.07.2017
//

#include "benchmark/cppbenchmark.h"

#include "containers/hashmap.h"

#include <algorithm>
#include <map>
#include <random>
#include <unordered_map>

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow" // GCC: warning: declaration of 'expression' shadows a parameter or member
#elif defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4127) // C4127: conditional expression is constant
#pragma warning(disable: 4244) // C4244: 'conversion' conversion from 'type1' to 'type2', possible loss of data
#pragma warning(disable: 4310) // C4310: cast truncates constant value
#pragma warning(disable: 4458) // C4458: declaration of 'identifier' hides class member
#endif
#include "flat_hash_map/flat_hash_map.hpp"
#include "flat_hash_map/bytell_hash_map.hpp"
#include "hopscotch-map/bhopscotch_map.h"
#include "hopscotch-map/hopscotch_map.h"
#include "ordered-map/ordered_map.h"
#include "robin-map/robin_map.h"
#include "sparse-map/sparse_map.h"
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif

const int items = 1000000;

typedef std::map<int, int> Map;
typedef std::unordered_map<int, int> UnorderedMap;
typedef CppCommon::HashMap<int, int> HashMap;
typedef ska::flat_hash_map<int, int> FlatHash;
typedef ska::bytell_hash_map<int, int> BytellHash;
typedef tsl::bhopscotch_map<int, int> BHopscotchHash;
typedef tsl::hopscotch_map<int, int> HopscotchHash;
typedef tsl::ordered_map<int, int> OrderedHash;
typedef tsl::robin_map<int, int> RobinHash;
typedef tsl::sparse_map<int, int> SparseHash;

template <class T>
class InsertFixture : public virtual CppBenchmark::Fixture
{
protected:
    T map;
    std::vector<int> values;

    InsertFixture()
    {
        for (int i = 0; i < items; ++i)
            values.push_back(i + 1);
    }

    void Initialize(CppBenchmark::Context& context) override
    {
        std::default_random_engine random;
        std::shuffle(values.begin(), values.end(), random);
    }

    void Cleanup(CppBenchmark::Context& context) override
    {
        map.clear();
    }
};

template <class T>
class FindFixture : public InsertFixture<T>
{
protected:
    void Initialize(CppBenchmark::Context& context) override
    {
        std::default_random_engine random;
        std::shuffle(this->values.begin(), this->values.end(), random);
        for (const auto& value : this->values)
            this->map.emplace(value, value);
        std::shuffle(this->values.begin(), this->values.end(), random);
    }
};

BENCHMARK_FIXTURE(InsertFixture<Map>, "Insert: std::map")
{
    for (const auto& value : this->values)
        this->map.emplace(value, value);

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
}

BENCHMARK_FIXTURE(InsertFixture<UnorderedMap>, "Insert: std::unordered_map")
{
    for (const auto& value : this->values)
        this->map.emplace(value, value);

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
}

BENCHMARK_FIXTURE(InsertFixture<HashMap>, "Insert: HashMap")
{
    for (const auto& value : this->values)
        this->map.emplace(value, value);

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
}

BENCHMARK_FIXTURE(InsertFixture<FlatHash>, "Insert: FlatHash")
{
    for (const auto& value : this->values)
        this->map.emplace(value, value);

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
}

BENCHMARK_FIXTURE(InsertFixture<BytellHash>, "Insert: BytellHash")
{
    for (const auto& value : this->values)
        this->map.emplace(value, value);

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
}

BENCHMARK_FIXTURE(InsertFixture<BHopscotchHash>, "Insert: BHopscotchHash")
{
    for (const auto& value : this->values)
        this->map.emplace(value, value);

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
}

BENCHMARK_FIXTURE(InsertFixture<HopscotchHash>, "Insert: HopscotchHash")
{
    for (const auto& value : this->values)
        this->map.emplace(value, value);

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
}

BENCHMARK_FIXTURE(InsertFixture<OrderedHash>, "Insert: OrderedHash")
{
    for (const auto& value : this->values)
        this->map.emplace(value, value);

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
}

BENCHMARK_FIXTURE(InsertFixture<RobinHash>, "Insert: RobinHash")
{
    for (const auto& value : this->values)
        this->map.emplace(value, value);

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
}

BENCHMARK_FIXTURE(InsertFixture<SparseHash>, "Insert: SparseHash")
{
    for (const auto& value : this->values)
        this->map.emplace(value, value);

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
}

BENCHMARK_FIXTURE(FindFixture<Map>, "Find: std::map")
{
    uint64_t crc = 0;

    for (const auto& value : this->values)
        crc += this->map.find(value)->second;

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<UnorderedMap>, "Find: std::unordered_map")
{
    uint64_t crc = 0;

    for (const auto& value : this->values)
        crc += this->map.find(value)->second;

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<HashMap>, "Find: HashMap")
{
    uint64_t crc = 0;

    for (const auto& value : this->values)
        crc += this->map.find(value)->second;

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<FlatHash>, "Find: FlatHash")
{
    uint64_t crc = 0;

    for (const auto& value : this->values)
        crc += this->map.find(value)->second;

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<BytellHash>, "Find: BytellHash")
{
    uint64_t crc = 0;

    for (const auto& value : this->values)
        crc += this->map.find(value)->second;

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<BHopscotchHash>, "Find: BHopscotchHash")
{
    uint64_t crc = 0;

    for (const auto& value : this->values)
        crc += this->map.find(value)->second;

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<HopscotchHash>, "Find: HopscotchHash")
{
    uint64_t crc = 0;

    for (const auto& value : this->values)
        crc += this->map.find(value)->second;

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<OrderedHash>, "Find: OrderedHash")
{
    uint64_t crc = 0;

    for (const auto& value : this->values)
        crc += this->map.find(value)->second;

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<RobinHash>, "Find: RobinHash")
{
    uint64_t crc = 0;

    for (const auto& value : this->values)
        crc += this->map.find(value)->second;

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<SparseHash>, "Find: SparseHash")
{
    uint64_t crc = 0;

    for (const auto& value : this->values)
        crc += this->map.find(value)->second;

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<Map>, "Remove: std::map")
{
    uint64_t crc = 0;

    for (const auto& value : this->values)
        crc += this->map.erase(value);

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<UnorderedMap>, "Remove: std::unordered_map")
{
    uint64_t crc = 0;

    for (const auto& value : this->values)
        crc += this->map.erase(value);

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<HashMap>, "Remove: HashMap")
{
    uint64_t crc = 0;

    for (const auto& value : this->values)
        crc += this->map.erase(value);

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<FlatHash>, "Remove: FlatHash")
{
    uint64_t crc = 0;

    for (const auto& value : this->values)
        crc += this->map.erase(value);

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<BytellHash>, "Remove: BytellHash")
{
    uint64_t crc = 0;

    for (const auto& value : this->values)
        crc += this->map.erase(value);

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<BHopscotchHash>, "Remove: BHopscotchHash")
{
    uint64_t crc = 0;

    for (const auto& value : this->values)
        crc += this->map.erase(value);

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<HopscotchHash>, "Remove: HopscotchHash")
{
    uint64_t crc = 0;

    for (const auto& value : this->values)
        crc += this->map.erase(value);

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<OrderedHash>, "Remove: OrderedHash")
{
    uint64_t crc = 0;

    for (const auto& value : this->values)
        crc += this->map.erase(value);

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<RobinHash>, "Remove: RobinHash")
{
    uint64_t crc = 0;

    for (const auto& value : this->values)
        crc += this->map.erase(value);

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<SparseHash>, "Remove: SparseHash")
{
    uint64_t crc = 0;

    for (const auto& value : this->values)
        crc += this->map.erase(value);

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_MAIN()
