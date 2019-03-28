//
// Created by Ivan Shynkarenka on 27.07.2017
//

#include "benchmark/cppbenchmark.h"

#include "containers/flatmap.h"

#include <algorithm>
#include <map>
#include <random>

using namespace CppCommon;

const int items = 10000;

typedef std::map<int, int> Map;
typedef FlatMap<int, int> Flat;

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

BENCHMARK_FIXTURE(InsertFixture<Flat>, "Insert: FlatMap")
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

BENCHMARK_FIXTURE(FindFixture<Flat>, "Find: FlatMap")
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

BENCHMARK_FIXTURE(FindFixture<Flat>, "Remove: FlatMap")
{
    uint64_t crc = 0;

    for (const auto& value : this->values)
        crc += this->map.erase(value);

    // Update benchmark metrics
    context.metrics().AddOperations(items - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_MAIN()
