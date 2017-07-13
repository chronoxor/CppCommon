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

BENCHMARK_FIXTURE(InsertFixture<Map>, "std::map: Insert")
{
    for (auto& value : this->values)
        this->map.emplace(value, value);

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
}

BENCHMARK_FIXTURE(InsertFixture<UnorderedMap>, "std::unordered_map: Insert")
{
    for (auto& value : this->values)
        this->map.emplace(value, value);

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
}

BENCHMARK_FIXTURE(InsertFixture<Hash>, "HashMap: Insert")
{
    for (auto& value : this->values)
        this->map.emplace(value, value);

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
}

BENCHMARK_FIXTURE(FindFixture<Map>, "std::map: Find")
{
    uint64_t crc = 0;

    for (auto& value : this->values)
        crc += this->map.find(value)->second;

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<UnorderedMap>, "std::unordered_map: Find")
{
    uint64_t crc = 0;

    for (auto& value : this->values)
        crc += this->map.find(value)->second;

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<Hash>, "HashMap: Find")
{
    uint64_t crc = 0;

    for (auto& value : this->values)
        crc += this->map.find(value)->second;

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<Map>, "std::map: Remove")
{
    uint64_t crc = 0;

    for (auto& value : this->values)
        crc += this->map.erase(value);

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<UnorderedMap>, "std::unordered_map: Remove")
{
    uint64_t crc = 0;

    for (auto& value : this->values)
        crc += this->map.erase(value);

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(FindFixture<Hash>, "HashMap: Remove")
{
    uint64_t crc = 0;

    for (auto& value : this->values)
        crc += this->map.erase(value);

    // Update benchmark metrics
    context.metrics().AddIterations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_MAIN()
