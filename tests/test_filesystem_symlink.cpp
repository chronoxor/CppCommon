//
// Created by Ivan Shynkarenka on 30.08.2016
//

#include "test.h"

#include "filesystem/filesystem.h"

using namespace CppCommon;

TEST_CASE("Symlink", "[CppCommon][FileSystem]")
{
    Directory test = Directory::Create(Path::current() / "test");

    // Create a temporary file
    std::string text("test");
    File temp(test / "test.tmp");
    REQUIRE(File::WriteAllText(temp, text) == text.size());

    // Create a file symlink
    Directory test1 = Directory::Create(test / "test1");
    REQUIRE(test1.IsExists());
    Symlink symlink1 = Symlink::CreateSymlink(temp, test1 / "test1.tmp");
    REQUIRE(symlink1.IsExists());
    REQUIRE(symlink1.IsSymlink());
    REQUIRE(symlink1.IsSymlinkExists());
    REQUIRE(symlink1.IsTargetExists());
    REQUIRE(symlink1.IsEquivalent(temp));
    REQUIRE(symlink1.target() == temp);

    // Create a directory symlink
    Symlink symlink2 = Symlink::CreateSymlink(test1, test / "test2");
    REQUIRE(symlink2.IsExists());
    REQUIRE(symlink2.IsSymlink());
    REQUIRE(symlink2.IsSymlinkExists());
    REQUIRE(symlink2.IsTargetExists());
    REQUIRE(symlink2.IsEquivalent(test1));
    REQUIRE(symlink2.target() == test / "test1");

    // Create a file hardlink
    Directory test3 = Directory::Create(test / "test3");
    REQUIRE(test3.IsExists());
    Path hardlink3 = Symlink::CreateHardlink(temp, test3 / "test3.tmp");
    REQUIRE(hardlink3.IsExists());
    REQUIRE(hardlink3.IsRegularFile());
    REQUIRE(!hardlink3.IsSymlink());
    REQUIRE(hardlink3.IsEquivalent(temp));
    REQUIRE(hardlink3.hardlinks() == 2);

    // Remove complex directory structure
    REQUIRE(Directory::RemoveAll(test) == Path::current());
}
