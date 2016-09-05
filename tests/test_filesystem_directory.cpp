//
// Created by Ivan Shynkarenka on 30.08.2016.
//

#include "catch.hpp"

#include "filesystem/directory.h"
#include <filesystem/file.h>
#include "filesystem/symlink.h"

using namespace CppCommon;

TEST_CASE("Directory", "[CppCommon][FileSystem]")
{
    Directory current = Path::current();
    REQUIRE(current.IsExists());
    REQUIRE(current.IsDirectory());
    REQUIRE(current.IsDirectoryExists());
    REQUIRE(!current.IsDirectoryEmpty());

    // Create directory
    Directory test1 = Directory::Create(current / "test1");
    REQUIRE(test1.IsDirectoryExists());
    REQUIRE(test1.IsDirectoryEmpty());
    Directory test2 = Directory::Create(test1 / "test2");
    REQUIRE(test2.IsDirectoryExists());
    REQUIRE(test2.IsDirectoryEmpty());
    REQUIRE(!test1.IsDirectoryEmpty());

    // Remove directory
    REQUIRE(Path::Remove(test2) == test1);
    REQUIRE(!test2.IsDirectoryExists());
    REQUIRE(Path::Remove(test1) == current);
    REQUIRE(!test1.IsDirectoryExists());

    // Create directory tree
    Directory test3 = Directory::CreateTree(current / "test1" / "test2" / "test3");
    REQUIRE(test3.IsDirectoryExists());
    REQUIRE(test3.IsDirectoryEmpty());

    // Remove directory tree
    Directory test4 = Path::Remove(test3);
    REQUIRE(test4.IsDirectoryExists());
    REQUIRE(test4.IsDirectoryEmpty());
    REQUIRE(test4 == current / "test1" / "test2");
    Directory test5 = Path::Remove(test4);
    REQUIRE(test5.IsDirectoryExists());
    REQUIRE(test5.IsDirectoryEmpty());
    REQUIRE(test5 == current / "test1");
    Directory test6 = Path::Remove(test5);
    REQUIRE(test6.IsDirectoryExists());
    REQUIRE(!test6.IsDirectoryEmpty());
    REQUIRE(test6 == current);
}

TEST_CASE("Directory iterator", "[CppCommon][FileSystem]")
{
    std::string text("test");

    // Create complex directory structure
    Directory test = Directory::Create(Path::current() / "test");
    File test1tmp = test / "test1.tmp";
    REQUIRE(File::WriteAllText(test1tmp, text) == text.size());
    File test2tmp = test / "test2.tmp";
    REQUIRE(File::WriteAllText(test2tmp, text) == text.size());
    File test3tmp = test / "test3.tmp";
    REQUIRE(File::WriteAllText(test3tmp, text) == text.size());

    Directory test1 = Directory::Create(test / "test1");
    File test11tmp = test1 / "test11.tmp";
    REQUIRE(File::WriteAllText(test11tmp, text) == text.size());
    Directory test11 = Directory::Create(test1 / "test11");
    File test111tmp = test11 / "test111.tmp";
    REQUIRE(File::WriteAllText(test111tmp, text) == text.size());
    Directory test111 = Directory::Create(test11 / "test111");
    File test1111tmp = test111 / "test1111.tmp";
    REQUIRE(File::WriteAllText(test1111tmp, text) == text.size());
    File test2222tmp = test111 / "test2222.tmp";
    REQUIRE(File::WriteAllText(test2222tmp, text) == text.size());

    Directory test2 = Directory::Create(test / "test2");
    File test22tmp = test2 / "test22.tmp";
    REQUIRE(File::WriteAllText(test22tmp, text) == text.size());
    Directory test22 = Directory::Create(test2 / "test22");
    File test222tmp = test22 / "test222.tmp";
    REQUIRE(File::WriteAllText(test222tmp, text) == text.size());

    Directory test3 = Directory::Create(test / "test3");
    File test33tmp = test3 / "test33.tmp";
    REQUIRE(File::WriteAllText(test33tmp, text) == text.size());

    Directory test4 = Directory::Create(test / "test4");
    Symlink test44tmp = Symlink::CreateSymlink(test1tmp, test4 / "test44.tmp");

    Symlink test5 = Symlink::CreateSymlink(test2, test / "test5");

    // Check directory entries
    REQUIRE(test.GetEntries().size() == 8);
    REQUIRE(test.GetEntries("test1.*").size() == 2);
    REQUIRE(test.GetEntriesRecurse().size() == 22);
    REQUIRE(test.GetEntriesRecurse("test2.*").size() == 9);
    REQUIRE(test.GetDirectories().size() == 5);
    REQUIRE(test.GetDirectories("test1.*").size() == 1);
    REQUIRE(test.GetDirectoriesRecurse().size() == 9);
    REQUIRE(test.GetDirectoriesRecurse("test2.*").size() == 3);
    REQUIRE(test.GetFiles().size() == 3);
    REQUIRE(test.GetFiles("test1.*").size() == 1);
    REQUIRE(test.GetFilesRecurse().size() == 13);
    REQUIRE(test.GetFilesRecurse("test2.*").size() == 6);
    REQUIRE(test.GetSymlinks().size() == 1);
    REQUIRE(test.GetSymlinks("test5.*").size() == 1);
    REQUIRE(test.GetSymlinksRecurse().size() == 2);
    REQUIRE(test.GetSymlinksRecurse("test4.*").size() == 1);

    // Remove complex directory structure
    Path::Remove(test5);
    Path::Remove(test44tmp);
    Path::Remove(test4);
    Path::Remove(test33tmp);
    Path::Remove(test3tmp);
    Path::Remove(test3);
    Path::Remove(test222tmp);
    Path::Remove(test22tmp);
    Path::Remove(test22);
    Path::Remove(test2tmp);
    Path::Remove(test2);
    Path::Remove(test1111tmp);
    Path::Remove(test2222tmp);
    Path::Remove(test111tmp);
    Path::Remove(test111);
    Path::Remove(test11tmp);
    Path::Remove(test11);
    Path::Remove(test1tmp);
    Path::Remove(test1);
    Path::Remove(test);
}
