//
// Created by Ivan Shynkarenka on 30.08.2016.
//

#include "catch.hpp"

#include "filesystem/directory.h"

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
