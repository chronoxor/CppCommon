//
// Created by Ivan Shynkarenka on 17.05.2017
//

#include "benchmark/cppbenchmark.h"

#include "memory/allocator.h"
#include "memory/allocator_arena.h"
#include "memory/allocator_heap.h"
#include "memory/allocator_pool.h"

#include <vector>

using namespace CppCommon;

class MemoryManagerFixture : public virtual CppBenchmark::Fixture
{
protected:
    std::vector<void*> pointers;

    void Initialize(CppBenchmark::Context& context) override { pointers.reserve(context.x()); }
    void Cleanup(CppBenchmark::Context& context) override { pointers.clear(); }

    virtual void Reset() {}
};

class DefaultMemoryManagerFixture : public MemoryManagerFixture
{
protected:
    DefaultMemoryManager manager;

    void Reset() override { manager.reset(); }
};

class HeapMemoryManagerFixture : public MemoryManagerFixture
{
protected:
    HeapMemoryManager manager;

    void Reset() override { manager.reset(); }
};

class ArenaMemoryManagerFixture : public MemoryManagerFixture
{
protected:
    DefaultMemoryManager auxiliary;
    ArenaMemoryManager<DefaultMemoryManager> manager;

    ArenaMemoryManagerFixture() : manager(auxiliary) {}

    void Reset() override { manager.reset(); }
};

class PoolMemoryManagerFixture : public MemoryManagerFixture
{
protected:
    DefaultMemoryManager auxiliary;
    PoolMemoryManager<DefaultMemoryManager> manager;

    PoolMemoryManagerFixture() : manager(auxiliary) {}

    void Reset() override { manager.reset(); }
};

template <class TMemoryManagerFixture>
class MallocFixture : public TMemoryManagerFixture
{
protected:
    void Cleanup(CppBenchmark::Context& context) override
    {
        for (auto ptr : this->pointers)
            this->manager.free(ptr, context.y());
        TMemoryManagerFixture::Reset();
        TMemoryManagerFixture::Cleanup(context);
    }
};

template <class TMemoryManagerFixture>
class FreeFixture : public TMemoryManagerFixture
{
protected:
    void Initialize(CppBenchmark::Context& context) override
    {
        TMemoryManagerFixture::Initialize(context);
        for (int i = 0; i < context.x(); ++i)
            this->pointers.push_back(this->manager.malloc(context.y()));
    }
};

BENCHMARK_FIXTURE(MallocFixture<DefaultMemoryManagerFixture>, "DefaultMemoryManager.malloc", CppBenchmark::Settings().Pair(10000000, 16))
{
    this->pointers.push_back(this->manager.malloc(context.y()));
    context.metrics().AddBytes(context.y());
}

BENCHMARK_FIXTURE(FreeFixture<DefaultMemoryManagerFixture>, "DefaultMemoryManager.free", CppBenchmark::Settings().Pair(10000000, 16))
{
    this->manager.free(this->pointers.back(), context.y());
    this->pointers.pop_back();
    context.metrics().AddBytes(context.y());
}

BENCHMARK_FIXTURE(MallocFixture<DefaultMemoryManagerFixture>, "DefaultMemoryManager.malloc", CppBenchmark::Settings().Pair(1000000, 256))
{
    this->pointers.push_back(this->manager.malloc(context.y()));
    context.metrics().AddBytes(context.y());
}

BENCHMARK_FIXTURE(FreeFixture<DefaultMemoryManagerFixture>, "DefaultMemoryManager.free", CppBenchmark::Settings().Pair(1000000, 256))
{
    this->manager.free(this->pointers.back(), context.y());
    this->pointers.pop_back();
    context.metrics().AddBytes(context.y());
}

BENCHMARK_FIXTURE(MallocFixture<HeapMemoryManagerFixture>, "HeapMemoryManager.malloc", CppBenchmark::Settings().Pair(10000000, 16))
{
    this->pointers.push_back(this->manager.malloc(context.y()));
    context.metrics().AddBytes(context.y());
}

BENCHMARK_FIXTURE(FreeFixture<HeapMemoryManagerFixture>, "HeapMemoryManager.free", CppBenchmark::Settings().Pair(10000000, 16))
{
    this->manager.free(this->pointers.back(), context.y());
    this->pointers.pop_back();
    context.metrics().AddBytes(context.y());
}

BENCHMARK_FIXTURE(MallocFixture<HeapMemoryManagerFixture>, "HeapMemoryManager.malloc", CppBenchmark::Settings().Pair(1000000, 256))
{
    this->pointers.push_back(this->manager.malloc(context.y()));
    context.metrics().AddBytes(context.y());
}

BENCHMARK_FIXTURE(FreeFixture<HeapMemoryManagerFixture>, "HeapMemoryManager.free", CppBenchmark::Settings().Pair(1000000, 256))
{
    this->manager.free(this->pointers.back(), context.y());
    this->pointers.pop_back();
    context.metrics().AddBytes(context.y());
}

BENCHMARK_FIXTURE(MallocFixture<ArenaMemoryManagerFixture>, "ArenaMemoryManager.malloc", CppBenchmark::Settings().Pair(10000000, 16))
{
    this->pointers.push_back(this->manager.malloc(context.y()));
    context.metrics().AddBytes(context.y());
}

BENCHMARK_FIXTURE(FreeFixture<ArenaMemoryManagerFixture>, "ArenaMemoryManager.free", CppBenchmark::Settings().Pair(10000000, 16))
{
    this->manager.free(this->pointers.back(), context.y());
    this->pointers.pop_back();
    context.metrics().AddBytes(context.y());
}

BENCHMARK_FIXTURE(MallocFixture<ArenaMemoryManagerFixture>, "ArenaMemoryManager.malloc", CppBenchmark::Settings().Pair(1000000, 256))
{
    this->pointers.push_back(this->manager.malloc(context.y()));
    context.metrics().AddBytes(context.y());
}

BENCHMARK_FIXTURE(FreeFixture<ArenaMemoryManagerFixture>, "ArenaMemoryManager.free", CppBenchmark::Settings().Pair(1000000, 256))
{
    this->manager.free(this->pointers.back(), context.y());
    this->pointers.pop_back();
    context.metrics().AddBytes(context.y());
}

BENCHMARK_FIXTURE(MallocFixture<PoolMemoryManagerFixture>, "PoolMemoryManager.malloc", CppBenchmark::Settings().Pair(10000000, 16))
{
    this->pointers.push_back(this->manager.malloc(context.y()));
    context.metrics().AddBytes(context.y());
}

BENCHMARK_FIXTURE(FreeFixture<PoolMemoryManagerFixture>, "PoolMemoryManager.free", CppBenchmark::Settings().Pair(10000000, 16))
{
    this->manager.free(this->pointers.back(), context.y());
    this->pointers.pop_back();
    context.metrics().AddBytes(context.y());
}

BENCHMARK_FIXTURE(MallocFixture<PoolMemoryManagerFixture>, "PoolMemoryManager.malloc", CppBenchmark::Settings().Pair(1000000, 256))
{
    this->pointers.push_back(this->manager.malloc(context.y()));
    context.metrics().AddBytes(context.y());
}

BENCHMARK_FIXTURE(FreeFixture<PoolMemoryManagerFixture>, "PoolMemoryManager.free", CppBenchmark::Settings().Pair(1000000, 256))
{
    this->manager.free(this->pointers.back(), context.y());
    this->pointers.pop_back();
    context.metrics().AddBytes(context.y());
}

BENCHMARK_MAIN()
