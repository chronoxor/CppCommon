//
// Created by Ivan Shynkarenka on 29.08.2016.
//

#include "catch.hpp"

#include "filesystem/exceptions.h"
#include "filesystem/file.h"

using namespace CppCommon;

TEST_CASE("File", "[CppCommon][FileSystem]")
{
    std::string str("test");
    std::vector<uint8_t> buffer(str.begin(), str.end());

    // Create a new file
    File test("test.tmp");
    REQUIRE(!test.IsFileExists());
    REQUIRE(!test.IsFileOpened());
    test.Create(false, true);
    REQUIRE(test.IsFileExists());
    REQUIRE(test.IsFileOpened());
    REQUIRE(test.offset() == 0);
    REQUIRE(test.size() == 0);
    test.Write(buffer.data(), buffer.size());
    test.Flush();
    REQUIRE(test.offset() == 4);
    REQUIRE(test.size() == 4);
    test.Close();
    REQUIRE(!test.IsFileOpened());
    REQUIRE(test.size() == 4);

    std::vector<uint8_t> read(buffer.size());

    // Open file for reading
    test.Open(true, false);
    REQUIRE(test.IsFileExists());
    REQUIRE(test.IsFileOpened());
    REQUIRE(test.offset() == 0);
    REQUIRE(test.size() == 4);
    test.Read(read.data(), read.size());
    REQUIRE(read == buffer);
    REQUIRE(test.offset() == 4);
    REQUIRE(test.size() == 4);
    test.Close();
    REQUIRE(!test.IsFileOpened());
    REQUIRE(test.size() == 4);

    // Open file for append
    test.Open(true, true);
    REQUIRE(test.IsFileExists());
    REQUIRE(test.IsFileOpened());
    REQUIRE(test.offset() == 0);
    REQUIRE(test.size() == 4);
    test.Seek(4);
    REQUIRE(test.offset() == 4);
    REQUIRE(test.size() == 4);
    test.Write(read.data(), read.size());
    test.Flush();
    REQUIRE(test.offset() == 8);
    REQUIRE(test.size() == 8);
    test.Seek(0);
    REQUIRE(test.offset() == 0);
    REQUIRE(test.size() == 8);
    test.Read(read.data(), read.size());
    REQUIRE(read == buffer);
    REQUIRE(test.offset() == 4);
    REQUIRE(test.size() == 8);
    test.Read(read.data(), read.size());
    REQUIRE(read == buffer);
    REQUIRE(test.offset() == 8);
    REQUIRE(test.size() == 8);
    test.Close();
    REQUIRE(!test.IsFileOpened());
    REQUIRE(test.size() == 8);

    // Resize the file
    test.Resize(10);
    REQUIRE(test.size() == 10);
    test.Resize(4);
    REQUIRE(test.size() == 4);

    // Rename the file
    test = Path::Rename(test, "test.renamed.tmp");

    // Remove the file
    REQUIRE(test.IsFileExists());
    Path parent = Path::Remove(test);
    REQUIRE(parent == test.parent());
    REQUIRE(!test.IsFileExists());
}
