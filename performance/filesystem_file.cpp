//
// Created by Ivan Shynkarenka on 31.08.2016
//

#include "benchmark/cppbenchmark.h"

#include "filesystem/file.h"

#include <array>

using namespace CppCommon;

const uint64_t operations = 100000;
const int chunk = 8192;

class FileWriteFixture : public virtual CppBenchmark::Fixture
{
protected:
    File file;
    std::array<uint8_t, chunk> buffer;

    FileWriteFixture() : file("test.tmp")
    {
        for (size_t i = 0; i < buffer.size(); ++i)
            buffer[i] = i % 256;
    }

    void Initialize(CppBenchmark::Context& context) override
    {
        // Open file for writing
        file.Create(false, true);
    }

    void Cleanup(CppBenchmark::Context& context) override
    {
        file.Close();
        File::Remove(file);
    }
};

class FileReadFixture : public FileWriteFixture
{
protected:
    void Initialize(CppBenchmark::Context& context) override
    {
        CreateTestFile();

        // Open file for reading
        file.Open(true, false);
    }

private:
    void CreateTestFile()
    {
        file.Create(false, true);
        for (uint64_t i = 0; i < operations; ++i)
            file.Write(buffer.data(), buffer.size());
        file.Close();
    }
};

BENCHMARK_FIXTURE(FileWriteFixture, "File::Write()", operations)
{
    file.Write(buffer.data(), buffer.size());
    context.metrics().AddBytes(buffer.size());
}

BENCHMARK_FIXTURE(FileReadFixture, "File::Read()", operations)
{
    file.Read(buffer.data(), buffer.size());
    context.metrics().AddBytes(buffer.size());
}

BENCHMARK_MAIN()
