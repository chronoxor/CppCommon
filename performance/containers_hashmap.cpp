//
// Created by Ivan Shynkarenka on 13.07.2017
//

#include "benchmark/cppbenchmark.h"

#include "containers/hashmap.h"

#include <map>
#include <unordered_map>

using namespace CppCommon;

const int items = 1000000;

typedef std::map<int, int> Map;
typedef std::unordered_map<int, int> UnorderedMap;
typedef HashMap<int, int> Hash;

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
        std::random_shuffle(this->values.begin(), this->values.end());
        for (auto& value : this->values)
            this->map.emplace(value, value);
        std::random_shuffle(this->values.begin(), this->values.end());
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

BENCHMARK_FIXTURE(InsertFixture<Hash>, "Insert: HashMap")
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

BENCHMARK_FIXTURE(FindFixture<Hash>, "Find: HashMap")
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

BENCHMARK_FIXTURE(FindFixture<Hash>, "Remove: HashMap")
{
    uint64_t crc = 0;

    for (auto& value : this->values)
        crc += this->map.erase(value);

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_MAIN()
