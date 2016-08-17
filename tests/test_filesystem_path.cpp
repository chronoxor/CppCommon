//
// Created by Ivan Shynkarenka on 15.08.2016.
//

#include "catch.hpp"

#include "filesystem/path.h"

using namespace CppCommon;

TEST_CASE("Path", "[CppCommon][FileSystem]")
{
    // Test make preferred method
    REQUIRE(Path("test/test\\test").MakePreferred() == Path(std::string("test") + Path::Separator() + std::string("test") + Path::Separator() + std::string("test")));

    // Test root path decomposition method
    REQUIRE(Path().root().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path(".").root().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("..").root().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("/").root().MakePreferred() == Path("/").MakePreferred());
    REQUIRE(Path("/.").root().MakePreferred() == Path("/").MakePreferred());
    REQUIRE(Path("/..").root().MakePreferred() == Path("/").MakePreferred());
    REQUIRE(Path("./").root().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("../").root().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("C:").root().MakePreferred() == Path("C:").MakePreferred());
    REQUIRE(Path("C:/").root().MakePreferred() == Path("C:/").MakePreferred());
    REQUIRE(Path("C:/foobar").root().MakePreferred() == Path("C:/").MakePreferred());
    REQUIRE(Path("C:/foo/bar").root().MakePreferred() == Path("C:/").MakePreferred());
    REQUIRE(Path("\\\\?\\").root().MakePreferred() == Path("\\\\?\\").MakePreferred());
    REQUIRE(Path("\\\\?\\C:").root().MakePreferred() == Path("\\\\?\\C:").MakePreferred());
    REQUIRE(Path("\\\\?\\C:/").root().MakePreferred() == Path("\\\\?\\C:/").MakePreferred());
    REQUIRE(Path("\\\\?\\C:/foobar").root().MakePreferred() == Path("\\\\?\\C:/").MakePreferred());
    REQUIRE(Path("\\\\?\\C:/foo/bar").root().MakePreferred() == Path("\\\\?\\C:/").MakePreferred());
    REQUIRE(Path("//net").root().MakePreferred() == Path("//net").MakePreferred());
    REQUIRE(Path("//net/").root().MakePreferred() == Path("//net/").MakePreferred());
    REQUIRE(Path("//net/foobar").root().MakePreferred() == Path("//net/").MakePreferred());
    REQUIRE(Path("//net/foo/bar").root().MakePreferred() == Path("//net/").MakePreferred());
    REQUIRE(Path("foobar").root().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("foobar.").root().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("foobar..").root().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("foo.bar").root().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("/foobar").root().MakePreferred() == Path("/").MakePreferred());
    REQUIRE(Path("/foobar/").root().MakePreferred() == Path("/").MakePreferred());
    REQUIRE(Path("/foobar/.").root().MakePreferred() == Path("/").MakePreferred());
    REQUIRE(Path("/foobar/..").root().MakePreferred() == Path("/").MakePreferred());
    REQUIRE(Path("/foo/bar").root().MakePreferred() == Path("/").MakePreferred());
    REQUIRE(Path("foo/bar").root().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("foo/bar/goo").root().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("///foo").root().MakePreferred() == Path("/").MakePreferred());
    REQUIRE(Path("///foo/").root().MakePreferred() == Path("/").MakePreferred());
    REQUIRE(Path("///foo///").root().MakePreferred() == Path("/").MakePreferred());
    REQUIRE(Path("///foo///bar").root().MakePreferred() == Path("/").MakePreferred());

    // Test relative path decomposition method
    REQUIRE(Path().relative().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path(".").relative().MakePreferred() == Path(".").MakePreferred());
    REQUIRE(Path("..").relative().MakePreferred() == Path("..").MakePreferred());
    REQUIRE(Path("/").relative().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("/.").relative().MakePreferred() == Path(".").MakePreferred());
    REQUIRE(Path("/..").relative().MakePreferred() == Path("..").MakePreferred());
    REQUIRE(Path("./").relative().MakePreferred() == Path("./").MakePreferred());
    REQUIRE(Path("../").relative().MakePreferred() == Path("../").MakePreferred());
    REQUIRE(Path("C:").relative().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("C:/").relative().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("C:/foobar").relative().MakePreferred() == Path("foobar").MakePreferred());
    REQUIRE(Path("C:/foo/bar").relative().MakePreferred() == Path("foo/bar").MakePreferred());
    REQUIRE(Path("\\\\?\\").relative().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("\\\\?\\C:").relative().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("\\\\?\\C:/").relative().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("\\\\?\\C:/foobar").relative().MakePreferred() == Path("foobar").MakePreferred());
    REQUIRE(Path("\\\\?\\C:/foo/bar").relative().MakePreferred() == Path("foo/bar").MakePreferred());
    REQUIRE(Path("//net").relative().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("//net/").relative().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("//net/foobar").relative().MakePreferred() == Path("foobar").MakePreferred());
    REQUIRE(Path("//net/foo/bar").relative().MakePreferred() == Path("foo/bar").MakePreferred());
    REQUIRE(Path("foobar").relative().MakePreferred() == Path("foobar").MakePreferred());
    REQUIRE(Path("foobar.").relative().MakePreferred() == Path("foobar.").MakePreferred());
    REQUIRE(Path("foobar..").relative().MakePreferred() == Path("foobar..").MakePreferred());
    REQUIRE(Path("foo.bar").relative().MakePreferred() == Path("foo.bar").MakePreferred());
    REQUIRE(Path("/foobar").relative().MakePreferred() == Path("foobar").MakePreferred());
    REQUIRE(Path("/foobar/").relative().MakePreferred() == Path("foobar/").MakePreferred());
    REQUIRE(Path("/foobar/.").relative().MakePreferred() == Path("foobar/.").MakePreferred());
    REQUIRE(Path("/foobar/..").relative().MakePreferred() == Path("foobar/..").MakePreferred());
    REQUIRE(Path("/foo/bar").relative().MakePreferred() == Path("foo/bar").MakePreferred());
    REQUIRE(Path("foo/bar").relative().MakePreferred() == Path("foo/bar").MakePreferred());
    REQUIRE(Path("foo/bar/goo").relative().MakePreferred() == Path("foo/bar/goo").MakePreferred());
    REQUIRE(Path("///foo").relative().MakePreferred() == Path("foo").MakePreferred());
    REQUIRE(Path("///foo/").relative().MakePreferred() == Path("foo/").MakePreferred());
    REQUIRE(Path("///foo///").relative().MakePreferred() == Path("foo///").MakePreferred());
    REQUIRE(Path("///foo///bar").relative().MakePreferred() == Path("foo///bar").MakePreferred());

    // Test parent path decomposition method
    REQUIRE(Path().parent().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path(".").parent().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("..").parent().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("/").parent().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("/.").parent().MakePreferred() == Path("/").MakePreferred());
    REQUIRE(Path("/..").parent().MakePreferred() == Path("/").MakePreferred());
    REQUIRE(Path("./").parent().MakePreferred() == Path(".").MakePreferred());
    REQUIRE(Path("../").parent().MakePreferred() == Path("..").MakePreferred());
    REQUIRE(Path("C:").parent().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("C:/").parent().MakePreferred() == Path("C:").MakePreferred());
    REQUIRE(Path("C:/foobar").parent().MakePreferred() == Path("C:").MakePreferred());
    REQUIRE(Path("C:/foo/bar").parent().MakePreferred() == Path("C:/foo").MakePreferred());
    REQUIRE(Path("\\\\?\\").parent().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("\\\\?\\C:").parent().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("\\\\?\\C:/").parent().MakePreferred() == Path("\\\\?\\C:").MakePreferred());
    REQUIRE(Path("\\\\?\\C:/foobar").parent().MakePreferred() == Path("\\\\?\\C:").MakePreferred());
    REQUIRE(Path("\\\\?\\C:/foo/bar").parent().MakePreferred() == Path("\\\\?\\C:/foo").MakePreferred());
    REQUIRE(Path("//net").parent().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("//net/").parent().MakePreferred() == Path("//net").MakePreferred());
    REQUIRE(Path("//net/foobar").parent().MakePreferred() == Path("//net").MakePreferred());
    REQUIRE(Path("//net/foo/bar").parent().MakePreferred() == Path("//net/foo").MakePreferred());
    REQUIRE(Path("foobar").parent().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("foobar.").parent().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("foobar..").parent().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("foo.bar").parent().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("/foobar").parent().MakePreferred() == Path("/").MakePreferred());
    REQUIRE(Path("/foobar/").parent().MakePreferred() == Path("/foobar").MakePreferred());
    REQUIRE(Path("/foobar/.").parent().MakePreferred() == Path("/foobar").MakePreferred());
    REQUIRE(Path("/foobar/..").parent().MakePreferred() == Path("/foobar").MakePreferred());
    REQUIRE(Path("/foo/bar").parent().MakePreferred() == Path("/foo").MakePreferred());
    REQUIRE(Path("foo/bar").parent().MakePreferred() == Path("foo").MakePreferred());
    REQUIRE(Path("foo/bar/goo").parent().MakePreferred() == Path("foo/bar").MakePreferred());
    REQUIRE(Path("///foo").parent().MakePreferred() == Path("/").MakePreferred());
    REQUIRE(Path("///foo/").parent().MakePreferred() == Path("///foo").MakePreferred());
    REQUIRE(Path("///foo///").parent().MakePreferred() == Path("///foo").MakePreferred());
    REQUIRE(Path("///foo///bar").parent().MakePreferred() == Path("///foo").MakePreferred());

    // Test filename decomposition method
    REQUIRE(Path().filename().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path(".").filename().MakePreferred() == Path(".").MakePreferred());
    REQUIRE(Path("..").filename().MakePreferred() == Path("..").MakePreferred());
    REQUIRE(Path("/").filename().MakePreferred() == Path(".").MakePreferred());
    REQUIRE(Path("C:").filename().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("C:/").filename().MakePreferred() == Path(".").MakePreferred());
    REQUIRE(Path("C:/foobar").filename().MakePreferred() == Path("foobar").MakePreferred());
    REQUIRE(Path("C:/foo.bar").filename().MakePreferred() == Path("foo.bar").MakePreferred());
    REQUIRE(Path("\\\\?\\").filename().MakePreferred() == Path(".").MakePreferred());
    REQUIRE(Path("\\\\?\\C:").filename().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("\\\\?\\C:/").filename().MakePreferred() == Path(".").MakePreferred());
    REQUIRE(Path("\\\\?\\C:/foobar").filename().MakePreferred() == Path("foobar").MakePreferred());
    REQUIRE(Path("\\\\?\\C:/foo.bar").filename().MakePreferred() == Path("foo.bar").MakePreferred());
    REQUIRE(Path("foobar").filename().MakePreferred() == Path("foobar").MakePreferred());
    REQUIRE(Path("foobar.").filename().MakePreferred() == Path("foobar.").MakePreferred());
    REQUIRE(Path("foobar..").filename().MakePreferred() == Path("foobar..").MakePreferred());
    REQUIRE(Path("foo.bar").filename().MakePreferred() == Path("foo.bar").MakePreferred());
    REQUIRE(Path("/foobar").filename().MakePreferred() == Path("foobar").MakePreferred());
    REQUIRE(Path("/foobar/").filename().MakePreferred() == Path(".").MakePreferred());
    REQUIRE(Path("/foobar/.").filename().MakePreferred() == Path(".").MakePreferred());
    REQUIRE(Path("/foobar/..").filename().MakePreferred() == Path("..").MakePreferred());
    REQUIRE(Path("/foo.bar").filename().MakePreferred() == Path("foo.bar").MakePreferred());
    REQUIRE(Path("foo/bar").filename().MakePreferred() == Path("bar").MakePreferred());
    REQUIRE(Path("foo/bar.goo").filename().MakePreferred() == Path("bar.goo").MakePreferred());

    // Test stem decomposition method
    REQUIRE(Path().stem().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path(".").stem().MakePreferred() == Path(".").MakePreferred());
    REQUIRE(Path("..").stem().MakePreferred() == Path("..").MakePreferred());
    REQUIRE(Path("/").stem().MakePreferred() == Path(".").MakePreferred());
    REQUIRE(Path("C:").stem().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("C:/").stem().MakePreferred() == Path(".").MakePreferred());
    REQUIRE(Path("C:/foobar").stem().MakePreferred() == Path("foobar").MakePreferred());
    REQUIRE(Path("C:/foo.bar").stem().MakePreferred() == Path("foo").MakePreferred());
    REQUIRE(Path("\\\\?\\").stem().MakePreferred() == Path(".").MakePreferred());
    REQUIRE(Path("\\\\?\\C:").stem().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("\\\\?\\C:/").stem().MakePreferred() == Path(".").MakePreferred());
    REQUIRE(Path("\\\\?\\C:/foobar").stem().MakePreferred() == Path("foobar").MakePreferred());
    REQUIRE(Path("\\\\?\\C:/foo.bar").stem().MakePreferred() == Path("foo").MakePreferred());
    REQUIRE(Path("foobar").stem().MakePreferred() == Path("foobar").MakePreferred());
    REQUIRE(Path("foobar.").stem().MakePreferred() == Path("foobar.").MakePreferred());
    REQUIRE(Path("foobar..").stem().MakePreferred() == Path("foobar..").MakePreferred());
    REQUIRE(Path("foo.bar").stem().MakePreferred() == Path("foo").MakePreferred());
    REQUIRE(Path("/foobar").stem().MakePreferred() == Path("foobar").MakePreferred());
    REQUIRE(Path("/foobar/").stem().MakePreferred() == Path(".").MakePreferred());
    REQUIRE(Path("/foobar/.").stem().MakePreferred() == Path(".").MakePreferred());
    REQUIRE(Path("/foobar/..").stem().MakePreferred() == Path("..").MakePreferred());
    REQUIRE(Path("/foo.bar").stem().MakePreferred() == Path("foo").MakePreferred());
    REQUIRE(Path("foo/bar").stem().MakePreferred() == Path("bar").MakePreferred());
    REQUIRE(Path("foo/bar.goo").stem().MakePreferred() == Path("bar").MakePreferred());

    // Test extension decomposition method
    REQUIRE(Path().extension().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path(".").extension().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("..").extension().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("/").extension().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("C:").extension().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("C:/").extension().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("C:/foobar").extension().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("C:/foo.bar").extension().MakePreferred() == Path(".bar").MakePreferred());
    REQUIRE(Path("\\\\?\\").extension().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("\\\\?\\C:").extension().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("\\\\?\\C:/").extension().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("\\\\?\\C:/foobar").extension().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("\\\\?\\C:/foo.bar").extension().MakePreferred() == Path(".bar").MakePreferred());
    REQUIRE(Path("foobar").extension().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("foobar.").extension().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("foobar..").extension().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("foo.bar").extension().MakePreferred() == Path(".bar").MakePreferred());
    REQUIRE(Path("/foobar").extension().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("/foobar/").extension().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("/foobar/.").extension().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("/foobar/..").extension().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("/foo.bar").extension().MakePreferred() == Path(".bar").MakePreferred());
    REQUIRE(Path("foo/bar").extension().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("foo/bar.goo").extension().MakePreferred() == Path(".goo").MakePreferred());

    // Test path append method
    REQUIRE(Path().Append("test").MakePreferred() == Path("test").MakePreferred());
    REQUIRE(Path("/").Append("test").MakePreferred() == Path("/test").MakePreferred());
    REQUIRE(Path("C:").Append("test").MakePreferred() == Path("C:/test").MakePreferred());
    REQUIRE(Path("test").Append("test").MakePreferred() == Path("test/test").MakePreferred());
    REQUIRE(Path("test/").Append("test").MakePreferred() == Path("test/test").MakePreferred());

    // Test path concatenate method
    REQUIRE(Path().Concat("test").MakePreferred() == Path("test").MakePreferred());
    REQUIRE(Path("/").Concat("test").MakePreferred() == Path("/test").MakePreferred());
    REQUIRE(Path("C:").Concat("test").MakePreferred() == Path("C:test").MakePreferred());
    REQUIRE(Path("/").Concat("/test").MakePreferred() == Path("//test").MakePreferred());
    REQUIRE(Path("C:/").Concat("/test").MakePreferred() == Path("C://test").MakePreferred());
    REQUIRE(Path("test").Concat("test").MakePreferred() == Path("testtest").MakePreferred());
    REQUIRE(Path("test/").Concat("test").MakePreferred() == Path("test/test").MakePreferred());
    REQUIRE(Path("test/").Concat("/test").MakePreferred() == Path("test//test").MakePreferred());

    // Test replace filename method
    REQUIRE(Path().ReplaceFilename("test").MakePreferred() == Path("test").MakePreferred());
    REQUIRE(Path("foobar").ReplaceFilename("test").MakePreferred() == Path("test").MakePreferred());
    REQUIRE(Path("foo.bar").ReplaceFilename("test").MakePreferred() == Path("test").MakePreferred());
    REQUIRE(Path("/foobar").ReplaceFilename("test").MakePreferred() == Path("/test").MakePreferred());
    REQUIRE(Path("C:foobar").ReplaceFilename("test").MakePreferred() == Path("C:test").MakePreferred());
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
    REQUIRE(Path("..").ReplaceExtension(".").MakePreferred() == Path(".").MakePreferred());
    REQUIRE(Path("..").ReplaceExtension("test").MakePreferred() == Path(".test").MakePreferred());
    REQUIRE(Path("..").ReplaceExtension(".test").MakePreferred() == Path(".test").MakePreferred());
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
    REQUIRE(Path("/foobar/.").ReplaceExtension(".").MakePreferred() == Path("/foobar/.").MakePreferred());
    REQUIRE(Path("/foobar/.").ReplaceExtension("test").MakePreferred() == Path("/foobar/.test").MakePreferred());
    REQUIRE(Path("/foobar/.").ReplaceExtension(".test").MakePreferred() == Path("/foobar/.test").MakePreferred());
    REQUIRE(Path("/foobar/..").ReplaceExtension(".").MakePreferred() == Path("/foobar/.").MakePreferred());
    REQUIRE(Path("/foobar/..").ReplaceExtension("test").MakePreferred() == Path("/foobar/.test").MakePreferred());
    REQUIRE(Path("/foobar/..").ReplaceExtension(".test").MakePreferred() == Path("/foobar/.test").MakePreferred());
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
    REQUIRE(Path("C:").RemoveTrailingSeparators().MakePreferred() == Path("C:").MakePreferred());
    REQUIRE(Path("C:/").RemoveTrailingSeparators().MakePreferred() == Path("C:/").MakePreferred());
    REQUIRE(Path("C://").RemoveTrailingSeparators().MakePreferred() == Path("C:/").MakePreferred());
    REQUIRE(Path("foobar").RemoveTrailingSeparators().MakePreferred() == Path("foobar").MakePreferred());
    REQUIRE(Path("foobar/").RemoveTrailingSeparators().MakePreferred() == Path("foobar").MakePreferred());
    REQUIRE(Path("foobar//").RemoveTrailingSeparators().MakePreferred() == Path("foobar").MakePreferred());
    REQUIRE(Path("foobar///").RemoveTrailingSeparators().MakePreferred() == Path("foobar").MakePreferred());
    REQUIRE(Path("/foo/bar/").RemoveTrailingSeparators().MakePreferred() == Path("/foo/bar").MakePreferred());
    REQUIRE(Path("/foo/bar//").RemoveTrailingSeparators().MakePreferred() == Path("/foo/bar").MakePreferred());
    REQUIRE(Path("/foo/bar///").RemoveTrailingSeparators().MakePreferred() == Path("/foo/bar").MakePreferred());

    // Test other methods
    char separator = Path::Separator();
    REQUIRE(((separator == '\\') || (separator == '/')));
    REQUIRE(!Path::Executable().empty());

    // Test swap method
    Path swap_path_1("foo");
    Path swap_path_2("bar");
    swap(swap_path_1, swap_path_2);
    REQUIRE(swap_path_1 == Path("bar"));
    REQUIRE(swap_path_2 == Path("foo"));
}
