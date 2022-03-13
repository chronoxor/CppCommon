//
// Created by Ivan Shynkarenka on 30.08.2016
//

#include "test.h"

#include "filesystem/filesystem.h"

using namespace CppCommon;

TEST_CASE("Directory", "[CppCommon][FileSystem]")
{
    Directory test = Directory::Create(Path::current() / "test");
    REQUIRE(test.IsExists());
    REQUIRE(test.IsDirectory());
    REQUIRE(test.IsDirectoryExists());
    REQUIRE(test.IsDirectoryEmpty());

    // Create directory
    Directory test1 = Directory::Create(test / "test1");
    REQUIRE(test1.IsDirectoryExists());
    REQUIRE(test1.IsDirectoryEmpty());
    Directory test2 = Directory::Create(test1 / "test2");
    REQUIRE(test2.IsDirectoryExists());
    REQUIRE(test2.IsDirectoryEmpty());
    REQUIRE(!test1.IsDirectoryEmpty());

    // Remove directory
    REQUIRE(Directory::Remove(test2) == test1);
    REQUIRE(!test2.IsDirectoryExists());
    REQUIRE(Directory::Remove(test1) == test);
    REQUIRE(!test1.IsDirectoryExists());

    // Create directory tree
    Directory test3 = Directory::CreateTree(test / "test1" / "test2" / "test3");
    REQUIRE(test3.IsDirectoryExists());
    REQUIRE(test3.IsDirectoryEmpty());

    // Remove directory tree
    REQUIRE(Directory::RemoveAll(test) == Path::current());
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
    REQUIRE(test.GetEntriesRecursive().size() == 22);
    REQUIRE(test.GetEntriesRecursive("test2.*").size() == 9);
    REQUIRE(test.GetDirectories().size() == 5);
    REQUIRE(test.GetDirectories("test1.*").size() == 1);
    REQUIRE(test.GetDirectoriesRecursive().size() == 9);
    REQUIRE(test.GetDirectoriesRecursive("test2.*").size() == 3);
    REQUIRE(test.GetFiles().size() == 3);
    REQUIRE(test.GetFiles("test1.*").size() == 1);
    REQUIRE(test.GetFilesRecursive().size() == 13);
    REQUIRE(test.GetFilesRecursive("test2.*").size() == 6);
    REQUIRE(test.GetSymlinks().size() == 1);
    REQUIRE(test.GetSymlinks("test5.*").size() == 1);
    REQUIRE(test.GetSymlinksRecursive().size() == 2);
    REQUIRE(test.GetSymlinksRecursive("test4.*").size() == 1);

    // Remove complex directory structure
    REQUIRE(Directory::RemoveAll(test) == Path::current());
}
