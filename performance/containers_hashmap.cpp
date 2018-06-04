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
#pragma GCC diagnostic ignored "-Wshadow"
#elif defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4244) // C4244: 'conversion' conversion from 'type1' to 'type2', possible loss of data
#pragma warning(disable: 4310) // C4310: cast truncates constant value
#pragma warning(disable: 4458) // C4458: declaration of 'identifier' hides class member
#endif
#include "flat_hash_map/flat_hash_map.hpp"
#include "flat_hash_map/bytell_hash_map.hpp"
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
        for (auto& value : this->values)
            this->map.emplace(value, value);
        std::shuffle(this->values.begin(), this->values.end(), random);
    }
};

BENCHMARK_FIXTURE(InsertFixture<Map>, "Insert: std::map")
{
    for (auto& value : this->values)
        this->map.emplace(value, value);

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
}

BENCHMARK_FIXTURE(InsertFixture<UnorderedMap>, "Insert: std::unordered_map")
{
    for (auto& value : this->values)
        this->map.emplace(value, value);

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
}

BENCHMARK_FIXTURE(InsertFixture<HashMap>, "Insert: HashMap")
{
    for (auto& value : this->values)
        this->map.emplace(value, value);

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
}

BENCHMARK_FIXTURE(InsertFixture<FlatHash>, "Insert: FlatHash")
{
    for (auto& value : this->values)
        this->map.emplace(value, value);

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
}

BENCHMARK_FIXTURE(InsertFixture<BytellHash>, "Insert: BytellHash")
{
    for (auto& value : this->values)
        this->map.emplace(value, value);

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
}

BENCHMARK_FIXTURE(FindFixture<Map>, "Find: std::map")
{
    uint64_t crc = 0;

    for (auto& value : this->values)
        crc += this->map.find(value)->second;

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<UnorderedMap>, "Find: std::unordered_map")
{
    uint64_t crc = 0;

    for (auto& value : this->values)
        crc += this->map.find(value)->second;

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<HashMap>, "Find: HashMap")
{
    uint64_t crc = 0;

    for (auto& value : this->values)
        crc += this->map.find(value)->second;

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<FlatHash>, "Find: FlatHash")
{
    uint64_t crc = 0;

    for (auto& value : this->values)
        crc += this->map.find(value)->second;

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<BytellHash>, "Find: BytellHash")
{
    uint64_t crc = 0;

    for (auto& value : this->values)
        crc += this->map.find(value)->second;

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<Map>, "Remove: std::map")
{
    uint64_t crc = 0;

    for (auto& value : this->values)
        crc += this->map.erase(value);

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<UnorderedMap>, "Remove: std::unordered_map")
{
    uint64_t crc = 0;

    for (auto& value : this->values)
        crc += this->map.erase(value);

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<HashMap>, "Remove: HashMap")
{
    uint64_t crc = 0;

    for (auto& value : this->values)
        crc += this->map.erase(value);

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<FlatHash>, "Remove: FlatHash")
{
    uint64_t crc = 0;

    for (auto& value : this->values)
        crc += this->map.erase(value);

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<BytellHash>, "Remove: BytellHash")
{
    uint64_t crc = 0;

    for (auto& value : this->values)
        crc += this->map.erase(value);

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_MAIN()
