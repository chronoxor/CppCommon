//
// Created by Ivan Shynkarenka on 30.08.2016.
//

#include "catch.hpp"

#include "filesystem/directory.h"
#include <filesystem/file.h>
#include "filesystem/symlink.h"

using namespace CppCommon;

TEST_CASE("Symlink", "[CppCommon][FileSystem]")
{
    Directory current = Path::current();

    // Create a temporary file
    std::string str("test");
    std::vector<uint8_t> buffer(str.begin(), str.end());
    File temp(current / "test.tmp");
    REQUIRE(!temp.IsFileExists());
    REQUIRE(!temp.IsFileOpened());
    temp.Create(false, true);
    REQUIRE(temp.IsFileExists());
    REQUIRE(temp.IsFileOpened());
    REQUIRE(temp.offset() == 0);
    REQUIRE(temp.size() == 0);
    temp.Write(buffer.data(), buffer.size());
    temp.Flush();
    REQUIRE(temp.offset() == 4);
    REQUIRE(temp.size() == 4);
    temp.Close();
    REQUIRE(!temp.IsFileOpened());
    REQUIRE(temp.size() == 4);

    // Create a file symlink
    Directory test1 = Directory::Create(current / "test1");
    REQUIRE(test1.IsExists());
    Symlink symlink1 = Symlink::CreateSymlink(temp, test1 / "test1.tmp");
    REQUIRE(symlink1.IsExists());
    REQUIRE(symlink1.IsSymlink());
    REQUIRE(symlink1.IsSymlinkExists());
    REQUIRE(symlink1.IsTargetExists());
    REQUIRE(symlink1.target() == temp);

    // Create a directory symlink
    Symlink symlink2 = Symlink::CreateSymlink(test1, current / "test2");
    REQUIRE(symlink2.IsExists());
    REQUIRE(symlink2.IsSymlink());
    REQUIRE(symlink2.IsSymlinkExists());
    REQUIRE(symlink2.IsTargetExists());
    REQUIRE(symlink2.target() == current / "test1");

    // Create a file hardlink
    Directory test3 = Directory::Create(current / "test3");
    REQUIRE(test3.IsExists());
    Path hardlink3 = Symlink::CreateHardlink(temp, test3 / "test3.tmp");
    REQUIRE(hardlink3.IsExists());
    REQUIRE(hardlink3.IsRegularFile());
    REQUIRE(!hardlink3.IsSymlink());
    REQUIRE(hardlink3.hardlinks() == 2);

    // Remove a file hardlink
    Path::Remove(hardlink3);
    REQUIRE(!hardlink3.IsExists());
    Path::Remove(test3);
    REQUIRE(!test3.IsExists());

    // Remove a directory symlink
    Path::Remove(symlink2);
    REQUIRE(!symlink2.IsExists());

    // Remove a file symlink
    Path::Remove(symlink1);
    REQUIRE(!symlink1.IsExists());
    Path::Remove(test1);
    REQUIRE(!test1.IsExists());

    // Remove the temporary file
    REQUIRE(temp.IsFileExists());
    Path::Remove(temp);
    REQUIRE(!temp.IsFileExists());
}
