//
// Created by Ivan Shynkarenka on 15.08.2016.
//

#include "catch.hpp"

#include "filesystem/path.h"

using namespace CppCommon;

TEST_CASE("Path", "[CppCommon][FileSystem]")
{
    // Test make preferred method
    REQUIRE(Path("test/test\\test").MakePreferred() == Path(std::string("test") + Path::separator() + std::string("test") + Path::separator() + std::string("test")));

    // Test path concatenate operator
    REQUIRE((Path() / "test").MakePreferred() == Path("test").MakePreferred());
    REQUIRE((Path("/") / "test").MakePreferred() == Path("/test").MakePreferred());
    REQUIRE((Path("test") / "test").MakePreferred() == Path("test/test").MakePreferred());
    REQUIRE((Path("test/") / "test").MakePreferred() == Path("test/test").MakePreferred());

    // Test path append operator
    REQUIRE((Path() + "test").MakePreferred() == Path("test").MakePreferred());
    REQUIRE((Path("/") + "test").MakePreferred() == Path("/test").MakePreferred());
    REQUIRE((Path("/") + "/test").MakePreferred() == Path("//test").MakePreferred());
    REQUIRE((Path("test") + "test").MakePreferred() == Path("testtest").MakePreferred());
    REQUIRE((Path("test/") + "test").MakePreferred() == Path("test/test").MakePreferred());
    REQUIRE((Path("test/") + "/test").MakePreferred() == Path("test//test").MakePreferred());

    // Test replace filename method
    REQUIRE(Path().ReplaceFilename("test").MakePreferred() == Path("test").MakePreferred());
    REQUIRE(Path("foobar").ReplaceFilename("test").MakePreferred() == Path("test").MakePreferred());
    REQUIRE(Path("foo.bar").ReplaceFilename("test").MakePreferred() == Path("test").MakePreferred());
    REQUIRE(Path("/foobar").ReplaceFilename("test").MakePreferred() == Path("/test").MakePreferred());
    REQUIRE(Path("/foobar/").ReplaceFilename("test").MakePreferred() == Path("/foobar/test").MakePreferred());
    REQUIRE(Path("/foo.bar").ReplaceFilename("test").MakePreferred() == Path("/test").MakePreferred());
    REQUIRE(Path("foo/bar").ReplaceFilename("test").MakePreferred() == Path("foo/test").MakePreferred());
    REQUIRE(Path("foo/bar.goo").ReplaceFilename("test").MakePreferred() == Path("foo/test").MakePreferred());

    // Test replace extension method
    REQUIRE(Path().ReplaceExtension(".").MakePreferred() == Path(".").MakePreferred());
    REQUIRE(Path().ReplaceExtension("test").MakePreferred() == Path(".test").MakePreferred());
    REQUIRE(Path().ReplaceExtension(".test").MakePreferred() == Path(".test").MakePreferred());
    REQUIRE(Path(".").ReplaceExtension(".").MakePreferred() == Path(".").MakePreferred());
    REQUIRE(Path(".").ReplaceExtension("test").MakePreferred() == Path(".test").MakePreferred());
    REQUIRE(Path(".").ReplaceExtension(".test").MakePreferred() == Path(".test").MakePreferred());
    REQUIRE(Path("foobar").ReplaceExtension(".").MakePreferred() == Path("foobar.").MakePreferred());
    REQUIRE(Path("foobar").ReplaceExtension("test").MakePreferred() == Path("foobar.test").MakePreferred());
    REQUIRE(Path("foobar").ReplaceExtension(".test").MakePreferred() == Path("foobar.test").MakePreferred());
    REQUIRE(Path("foobar.").ReplaceExtension(".").MakePreferred() == Path("foobar.").MakePreferred());
    REQUIRE(Path("foobar.").ReplaceExtension("test").MakePreferred() == Path("foobar.test").MakePreferred());
    REQUIRE(Path("foobar.").ReplaceExtension(".test").MakePreferred() == Path("foobar.test").MakePreferred());
    REQUIRE(Path("foo.bar").ReplaceExtension(".").MakePreferred() == Path("foo.").MakePreferred());
    REQUIRE(Path("foo.bar").ReplaceExtension("test").MakePreferred() == Path("foo.test").MakePreferred());
    REQUIRE(Path("foo.bar").ReplaceExtension(".test").MakePreferred() == Path("foo.test").MakePreferred());
    REQUIRE(Path("/foobar").ReplaceExtension(".").MakePreferred() == Path("/foobar.").MakePreferred());
    REQUIRE(Path("/foobar").ReplaceExtension("test").MakePreferred() == Path("/foobar.test").MakePreferred());
    REQUIRE(Path("/foobar").ReplaceExtension(".test").MakePreferred() == Path("/foobar.test").MakePreferred());
    REQUIRE(Path("/foobar/").ReplaceExtension(".").MakePreferred() == Path("/foobar/.").MakePreferred());
    REQUIRE(Path("/foobar/").ReplaceExtension("test").MakePreferred() == Path("/foobar/.test").MakePreferred());
    REQUIRE(Path("/foobar/").ReplaceExtension(".test").MakePreferred() == Path("/foobar/.test").MakePreferred());
    REQUIRE(Path("/foo.bar").ReplaceExtension(".").MakePreferred() == Path("/foo.").MakePreferred());
    REQUIRE(Path("/foo.bar").ReplaceExtension("test").MakePreferred() == Path("/foo.test").MakePreferred());
    REQUIRE(Path("/foo.bar").ReplaceExtension(".test").MakePreferred() == Path("/foo.test").MakePreferred());
    REQUIRE(Path("foo/bar").ReplaceExtension(".").MakePreferred() == Path("foo/bar.").MakePreferred());
    REQUIRE(Path("foo/bar").ReplaceExtension("test").MakePreferred() == Path("foo/bar.test").MakePreferred());
    REQUIRE(Path("foo/bar").ReplaceExtension(".test").MakePreferred() == Path("foo/bar.test").MakePreferred());
    REQUIRE(Path("foo/bar.goo").ReplaceExtension(".").MakePreferred() == Path("foo/bar.").MakePreferred());
    REQUIRE(Path("foo/bar.goo").ReplaceExtension("test").MakePreferred() == Path("foo/bar.test").MakePreferred());
    REQUIRE(Path("foo/bar.goo").ReplaceExtension(".test").MakePreferred() == Path("foo/bar.test").MakePreferred());

    // Test remove filename method
    REQUIRE(Path().RemoveFilename().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("foobar").RemoveFilename().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("foo.bar").RemoveFilename().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("/foobar").RemoveFilename().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("/foobar/").RemoveFilename().MakePreferred() == Path("/foobar").MakePreferred());
    REQUIRE(Path("/foo.bar").RemoveFilename().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("foo/bar").RemoveFilename().MakePreferred() == Path("foo").MakePreferred());
    REQUIRE(Path("foo/bar.goo").RemoveFilename().MakePreferred() == Path("foo").MakePreferred());

    // Test remove extension method
    REQUIRE(Path().RemoveExtension().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path(".").RemoveExtension().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("foobar").RemoveExtension().MakePreferred() == Path("foobar").MakePreferred());
    REQUIRE(Path("foobar.").RemoveExtension().MakePreferred() == Path("foobar").MakePreferred());
    REQUIRE(Path("foo.bar").RemoveExtension().MakePreferred() == Path("foo").MakePreferred());
    REQUIRE(Path("foo.bar.").RemoveExtension().MakePreferred() == Path("foo.bar").MakePreferred());
    REQUIRE(Path("/foobar").RemoveExtension().MakePreferred() == Path("/foobar").MakePreferred());
    REQUIRE(Path("/foobar.").RemoveExtension().MakePreferred() == Path("/foobar").MakePreferred());
    REQUIRE(Path("/foobar/").RemoveExtension().MakePreferred() == Path("/foobar/").MakePreferred());
    REQUIRE(Path("/foobar/.").RemoveExtension().MakePreferred() == Path("/foobar/").MakePreferred());
    REQUIRE(Path("/foo.bar").RemoveExtension().MakePreferred() == Path("/foo").MakePreferred());
    REQUIRE(Path("/foo.bar.").RemoveExtension().MakePreferred() == Path("/foo.bar").MakePreferred());
    REQUIRE(Path("foo/bar").RemoveExtension().MakePreferred() == Path("foo/bar").MakePreferred());
    REQUIRE(Path("foo/bar.").RemoveExtension().MakePreferred() == Path("foo/bar").MakePreferred());
    REQUIRE(Path("foo/bar.goo").RemoveExtension().MakePreferred() == Path("foo/bar").MakePreferred());
    REQUIRE(Path("foo/bar.goo.").RemoveExtension().MakePreferred() == Path("foo/bar.goo").MakePreferred());

    // Test remove trailing separators method
    REQUIRE(Path().RemoveTrailingSeparators().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("/").RemoveTrailingSeparators().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("//").RemoveTrailingSeparators().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("///").RemoveTrailingSeparators().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("foobar").RemoveTrailingSeparators().MakePreferred() == Path("foobar").MakePreferred());
    REQUIRE(Path("foobar/").RemoveTrailingSeparators().MakePreferred() == Path("foobar").MakePreferred());
    REQUIRE(Path("foobar//").RemoveTrailingSeparators().MakePreferred() == Path("foobar").MakePreferred());
    REQUIRE(Path("foobar///").RemoveTrailingSeparators().MakePreferred() == Path("foobar").MakePreferred());
    REQUIRE(Path("/foo/bar/").RemoveTrailingSeparators().MakePreferred() == Path("/foo/bar").MakePreferred());
    REQUIRE(Path("/foo/bar//").RemoveTrailingSeparators().MakePreferred() == Path("/foo/bar").MakePreferred());
    REQUIRE(Path("/foo/bar///").RemoveTrailingSeparators().MakePreferred() == Path("/foo/bar").MakePreferred());

    // Test swap method
    Path swap_path_1("foo");
    Path swap_path_2("bar");
    swap(swap_path_1, swap_path_2);
    REQUIRE(swap_path_1 == Path("bar"));
    REQUIRE(swap_path_2 == Path("foo"));
}
