//
// Created by Ivan Shynkarenka on 29.08.2016
//

#include "test.h"

#include "filesystem/filesystem.h"
#include "utility/countof.h"

using namespace CppCommon;

TEST_CASE("File common", "[CppCommon][FileSystem]")
{
    std::string str("test");
    std::vector<uint8_t> buffer(str.begin(), str.end());

    // Create a new file
    File test("test.tmp");
    REQUIRE(!test.IsFileExists());
    REQUIRE(!test.IsFileOpened());
    test.Create(false, true, File::DEFAULT_ATTRIBUTES, File::DEFAULT_PERMISSIONS, 0);
    REQUIRE(test.IsFileExists());
    REQUIRE(test.IsFileOpened());
    REQUIRE(!test.IsFileReadOpened());
    REQUIRE(test.IsFileWriteOpened());
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
    test.Open(true, false, false, File::DEFAULT_ATTRIBUTES, File::DEFAULT_PERMISSIONS, 0);
    REQUIRE(test.IsFileExists());
    REQUIRE(test.IsFileOpened());
    REQUIRE(test.IsFileReadOpened());
    REQUIRE(!test.IsFileWriteOpened());
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
    test.Open(true, true, false, File::DEFAULT_ATTRIBUTES, File::DEFAULT_PERMISSIONS, 0);
    REQUIRE(test.IsFileExists());
    REQUIRE(test.IsFileOpened());
    REQUIRE(test.IsFileReadOpened());
    REQUIRE(test.IsFileWriteOpened());
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
    Path parent = File::Remove(test);
    REQUIRE(parent == test.parent());
    REQUIRE(!test.IsFileExists());
}

TEST_CASE("File read/write static methods", "[CppCommon][FileSystem]")
{
    char buffer[] = "The quick brown fox jumps over the lazy dog";
    REQUIRE(File::WriteAllBytes("test.tmp", buffer, countof(buffer)) == countof(buffer));
    REQUIRE(File::ReadAllBytes("test.tmp").size() == countof(buffer));
    File::Remove("test.tmp");

    std::vector<std::string> lines = { "one", "two", "three", "four", "five" };
    REQUIRE(File::WriteAllLines("test.tmp", lines) == 5);
    lines = File::ReadAllLines("test.tmp");
    REQUIRE(lines.size() == 5);
    REQUIRE(lines[0] == "one");
    REQUIRE(lines[1] == "two");
    REQUIRE(lines[2] == "three");
    REQUIRE(lines[3] == "four");
    REQUIRE(lines[4] == "five");
    File::Remove("test.tmp");

    std::string text(buffer);
    REQUIRE(File::WriteAllText("test.tmp", text) == text.size());
    REQUIRE(File::ReadAllText("test.tmp") == text);
    File::Remove("test.tmp");
}
