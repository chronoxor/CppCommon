//
// Created by Ivan Shynkarenka on 15.08.2016
//

#include "test.h"

#include "filesystem/filesystem.h"

using namespace CppCommon;

TEST_CASE("Path common", "[CppCommon][FileSystem]")
{
    // Test other methods
    std::string deprecated = Path::deprecated();
    REQUIRE(deprecated.size() > 0);
    REQUIRE(!Path::deprecated('.'));
    REQUIRE(!Path::deprecated(','));
    REQUIRE(!Path::deprecated('-'));
    REQUIRE(!Path::deprecated('0'));
    REQUIRE(!Path::deprecated('a'));
    REQUIRE(!Path::deprecated('Z'));
    REQUIRE(Path::deprecated('\\'));
    REQUIRE(Path::deprecated(L'/'));
    REQUIRE(Path::deprecated('?'));
    REQUIRE(Path::deprecated(L'%'));
    REQUIRE(Path::deprecated('*'));
    REQUIRE(Path::deprecated(L':'));
    REQUIRE(Path::deprecated('|'));
    REQUIRE(Path::deprecated(L'\"'));
    REQUIRE(Path::deprecated('<'));
    REQUIRE(Path::deprecated(L'>'));
    char separator = Path::separator();
    REQUIRE(((separator == '\\') || (separator == '/')));

    // Test swap method
    Path swap_path_1("foo");
    Path swap_path_2("bar");
    swap(swap_path_1, swap_path_2);
    REQUIRE(swap_path_1 == Path("bar"));
    REQUIRE(swap_path_2 == Path("foo"));
}

TEST_CASE("Path preferred", "[CppCommon][FileSystem]")
{
    // Test make preferred method
    REQUIRE(Path("test/test\\test").MakePreferred() == Path(std::string("test") + Path::separator() + std::string("test") + Path::separator() + std::string("test")));
}

TEST_CASE("Path decompositions", "[CppCommon][FileSystem]")
{
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
    REQUIRE(Path("foo/bar").root().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("foo/bar/goo").root().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("/foobar").root().MakePreferred() == Path("/").MakePreferred());
    REQUIRE(Path("/foobar/").root().MakePreferred() == Path("/").MakePreferred());
    REQUIRE(Path("/foobar/.").root().MakePreferred() == Path("/").MakePreferred());
    REQUIRE(Path("/foobar/..").root().MakePreferred() == Path("/").MakePreferred());
    REQUIRE(Path("/foo/bar").root().MakePreferred() == Path("/").MakePreferred());
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
    REQUIRE(Path("foo/bar").relative().MakePreferred() == Path("foo/bar").MakePreferred());
    REQUIRE(Path("foo/bar/goo").relative().MakePreferred() == Path("foo/bar/goo").MakePreferred());
    REQUIRE(Path("/foobar").relative().MakePreferred() == Path("foobar").MakePreferred());
    REQUIRE(Path("/foobar/").relative().MakePreferred() == Path("foobar/").MakePreferred());
    REQUIRE(Path("/foobar/.").relative().MakePreferred() == Path("foobar/.").MakePreferred());
    REQUIRE(Path("/foobar/..").relative().MakePreferred() == Path("foobar/..").MakePreferred());
    REQUIRE(Path("/foo/bar").relative().MakePreferred() == Path("foo/bar").MakePreferred());
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
    REQUIRE(Path("C:/").parent().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("C:/foobar").parent().MakePreferred() == Path("C:/").MakePreferred());
    REQUIRE(Path("C:/foo/bar").parent().MakePreferred() == Path("C:/foo").MakePreferred());
    REQUIRE(Path("\\\\?\\").parent().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("\\\\?\\C:").parent().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("\\\\?\\C:/").parent().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("\\\\?\\C:/foobar").parent().MakePreferred() == Path("\\\\?\\C:/").MakePreferred());
    REQUIRE(Path("\\\\?\\C:/foo/bar").parent().MakePreferred() == Path("\\\\?\\C:/foo").MakePreferred());
    REQUIRE(Path("//net").parent().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("//net/").parent().MakePreferred() == Path("//net").MakePreferred());
    REQUIRE(Path("//net/foobar").parent().MakePreferred() == Path("//net").MakePreferred());
    REQUIRE(Path("//net/foo/bar").parent().MakePreferred() == Path("//net/foo").MakePreferred());
    REQUIRE(Path("foobar").parent().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("foobar.").parent().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("foobar..").parent().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("foo.bar").parent().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("foo/bar").parent().MakePreferred() == Path("foo").MakePreferred());
    REQUIRE(Path("foo/bar/goo").parent().MakePreferred() == Path("foo/bar").MakePreferred());
    REQUIRE(Path("/foobar").parent().MakePreferred() == Path("/").MakePreferred());
    REQUIRE(Path("/foobar/").parent().MakePreferred() == Path("/foobar").MakePreferred());
    REQUIRE(Path("/foobar/.").parent().MakePreferred() == Path("/foobar").MakePreferred());
    REQUIRE(Path("/foobar/..").parent().MakePreferred() == Path("/foobar").MakePreferred());
    REQUIRE(Path("/foo/bar").parent().MakePreferred() == Path("/foo").MakePreferred());
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
    REQUIRE(Path("foo/bar").filename().MakePreferred() == Path("bar").MakePreferred());
    REQUIRE(Path("foo/bar.goo").filename().MakePreferred() == Path("bar.goo").MakePreferred());
    REQUIRE(Path("/foobar").filename().MakePreferred() == Path("foobar").MakePreferred());
    REQUIRE(Path("/foobar/").filename().MakePreferred() == Path(".").MakePreferred());
    REQUIRE(Path("/foobar/.").filename().MakePreferred() == Path(".").MakePreferred());
    REQUIRE(Path("/foobar/..").filename().MakePreferred() == Path("..").MakePreferred());
    REQUIRE(Path("/foo.bar").filename().MakePreferred() == Path("foo.bar").MakePreferred());

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
    REQUIRE(Path("foo/bar").stem().MakePreferred() == Path("bar").MakePreferred());
    REQUIRE(Path("foo/bar.goo").stem().MakePreferred() == Path("bar").MakePreferred());
    REQUIRE(Path("/foobar").stem().MakePreferred() == Path("foobar").MakePreferred());
    REQUIRE(Path("/foobar/").stem().MakePreferred() == Path(".").MakePreferred());
    REQUIRE(Path("/foobar/.").stem().MakePreferred() == Path(".").MakePreferred());
    REQUIRE(Path("/foobar/..").stem().MakePreferred() == Path("..").MakePreferred());
    REQUIRE(Path("/foo.bar").stem().MakePreferred() == Path("foo").MakePreferred());

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
    REQUIRE(Path("foo/bar").extension().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("foo/bar.goo").extension().MakePreferred() == Path(".goo").MakePreferred());
    REQUIRE(Path("/foobar").extension().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("/foobar/").extension().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("/foobar/.").extension().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("/foobar/..").extension().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("/foo.bar").extension().MakePreferred() == Path(".bar").MakePreferred());
}

TEST_CASE("Path canonization", "[CppCommon][FileSystem]")
{
    // Test canonical path decomposition method
    REQUIRE(Path().canonical().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path(".").canonical().MakePreferred() == Path::current().MakePreferred());
    REQUIRE(Path("..").canonical().MakePreferred() == Path::current().parent().MakePreferred());
    REQUIRE(Path("/").canonical().MakePreferred() == Path("/").MakePreferred());
    REQUIRE(Path("/.").canonical().MakePreferred() == Path("/").MakePreferred());
    REQUIRE(Path("/..").canonical().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("./").canonical().MakePreferred() == Path::current().MakePreferred());
    REQUIRE(Path("../").canonical().MakePreferred() == Path::current().parent().MakePreferred());
    REQUIRE(Path("C:").canonical().MakePreferred() == Path("C:").MakePreferred());
    REQUIRE(Path("C:.").canonical().MakePreferred() == Path("C:").MakePreferred());
    REQUIRE(Path("C:..").canonical().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("C:/").canonical().MakePreferred() == Path("C:/").MakePreferred());
    REQUIRE(Path("C:/.").canonical().MakePreferred() == Path("C:/").MakePreferred());
    REQUIRE(Path("C:/..").canonical().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("C:/foobar").canonical().MakePreferred() == Path("C:/foobar").MakePreferred());
    REQUIRE(Path("C:/foobar.").canonical().MakePreferred() == Path("C:/foobar.").MakePreferred());
    REQUIRE(Path("C:/foobar/.").canonical().MakePreferred() == Path("C:/foobar").MakePreferred());
    REQUIRE(Path("C:/foobar/..").canonical().MakePreferred() == Path("C:/").MakePreferred());
    REQUIRE(Path("C:/.foobar").canonical().MakePreferred() == Path("C:/.foobar").MakePreferred());
    REQUIRE(Path("C:/..foobar").canonical().MakePreferred() == Path("C:/..foobar").MakePreferred());
    REQUIRE(Path("C:/./foobar").canonical().MakePreferred() == Path("C:/foobar").MakePreferred());
    REQUIRE(Path("C:/..foobar").canonical().MakePreferred() == Path("C:/..foobar").MakePreferred());
    REQUIRE(Path("C:/../foobar").canonical().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("C:/../../foobar").canonical().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("C:/foo/bar").canonical().MakePreferred() == Path("C:/foo/bar").MakePreferred());
    REQUIRE(Path("C:/.foo/.bar").canonical().MakePreferred() == Path("C:/.foo/.bar").MakePreferred());
    REQUIRE(Path("C:/..foo/..bar").canonical().MakePreferred() == Path("C:/..foo/..bar").MakePreferred());
    REQUIRE(Path("C:/.foo/../bar").canonical().MakePreferred() == Path("C:/bar").MakePreferred());
    REQUIRE(Path("\\\\?\\").canonical().MakePreferred() == Path("\\\\?\\").MakePreferred());
    REQUIRE(Path("\\\\?\\C:").canonical().MakePreferred() == Path("\\\\?\\C:").MakePreferred());
    REQUIRE(Path("\\\\?\\C:.").canonical().MakePreferred() == Path("\\\\?\\C:").MakePreferred());
    REQUIRE(Path("\\\\?\\C:..").canonical().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("\\\\?\\C:/").canonical().MakePreferred() == Path("\\\\?\\C:/").MakePreferred());
    REQUIRE(Path("\\\\?\\C:/.").canonical().MakePreferred() == Path("\\\\?\\C:/").MakePreferred());
    REQUIRE(Path("\\\\?\\C:/..").canonical().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("\\\\?\\C:/foobar").canonical().MakePreferred() == Path("\\\\?\\C:/foobar").MakePreferred());
    REQUIRE(Path("\\\\?\\C:/./foobar").canonical().MakePreferred() == Path("\\\\?\\C:/foobar").MakePreferred());
    REQUIRE(Path("\\\\?\\C:/../foobar").canonical().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("\\\\?\\C:/../../foobar").canonical().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("\\\\?\\C:/foo/bar").canonical().MakePreferred() == Path("\\\\?\\C:/foo/bar").MakePreferred());
    REQUIRE(Path("//net").canonical().MakePreferred() == Path("//net").MakePreferred());
    REQUIRE(Path("//net/").canonical().MakePreferred() == Path("//net/").MakePreferred());
    REQUIRE(Path("//net/.").canonical().MakePreferred() == Path("//net/").MakePreferred());
    REQUIRE(Path("//net/..").canonical().MakePreferred() == Path("//net").MakePreferred());
    REQUIRE(Path("//net/foobar").canonical().MakePreferred() == Path("//net/foobar").MakePreferred());
    REQUIRE(Path("//net/foobar.").canonical().MakePreferred() == Path("//net/foobar.").MakePreferred());
    REQUIRE(Path("//net/foobar..").canonical().MakePreferred() == Path("//net/foobar..").MakePreferred());
    REQUIRE(Path("//net/foobar/.").canonical().MakePreferred() == Path("//net/foobar").MakePreferred());
    REQUIRE(Path("//net/foobar/..").canonical().MakePreferred() == Path("//net").MakePreferred());
    REQUIRE(Path("//net/.foobar").canonical().MakePreferred() == Path("//net/.foobar").MakePreferred());
    REQUIRE(Path("//net/..foobar").canonical().MakePreferred() == Path("//net/..foobar").MakePreferred());
    REQUIRE(Path("//net/./foobar").canonical().MakePreferred() == Path("//net/foobar").MakePreferred());
    REQUIRE(Path("//net/../foobar").canonical().MakePreferred() == Path("//net/foobar").MakePreferred());
    REQUIRE(Path("//net/foo/bar").canonical().MakePreferred() == Path("//net/foo/bar").MakePreferred());
    REQUIRE(Path("//net/foo/./bar").canonical().MakePreferred() == Path("//net/foo/bar").MakePreferred());
    REQUIRE(Path("//net/foo/../bar").canonical().MakePreferred() == Path("//net/bar").MakePreferred());
    REQUIRE(Path("//net/foo/../../bar").canonical().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("//net/foo/../../../bar").canonical().MakePreferred() == Path().MakePreferred());
    REQUIRE(Path("foobar").canonical().MakePreferred() == (Path::current() / "foobar").MakePreferred());
    REQUIRE(Path("foobar.").canonical().MakePreferred() == (Path::current() / "foobar.").MakePreferred());
    REQUIRE(Path("foobar..").canonical().MakePreferred() == (Path::current() / "foobar..").MakePreferred());
    REQUIRE(Path(".foobar").canonical().MakePreferred() == (Path::current() / ".foobar").MakePreferred());
    REQUIRE(Path("..foobar").canonical().MakePreferred() == (Path::current() / "..foobar").MakePreferred());
    REQUIRE(Path("./foobar").canonical().MakePreferred() == (Path::current() / "foobar").MakePreferred());
    REQUIRE(Path("../foobar").canonical().MakePreferred() == (Path::current().parent() / "foobar").MakePreferred());
    REQUIRE(Path("foo.bar").canonical().MakePreferred() == (Path::current() / "foo.bar").MakePreferred());
    REQUIRE(Path("foo/bar").canonical().MakePreferred() == (Path::current() / "foo/bar").MakePreferred());
    REQUIRE(Path("foo/bar/goo").canonical().MakePreferred() == (Path::current() / "foo/bar/goo").MakePreferred());
    REQUIRE(Path("foo/bar/./goo").canonical().MakePreferred() == (Path::current() / "foo/bar/goo").MakePreferred());
    REQUIRE(Path("foo/./bar/./goo").canonical().MakePreferred() == (Path::current() / "foo/bar/goo").MakePreferred());
    REQUIRE(Path("foo/bar/../goo").canonical().MakePreferred() == (Path::current() / "foo/goo").MakePreferred());
    REQUIRE(Path("foo/../bar/../goo").canonical().MakePreferred() == (Path::current() / "goo").MakePreferred());
    REQUIRE(Path("/foobar").canonical().MakePreferred() == Path("/foobar").MakePreferred());
    REQUIRE(Path("/foobar/").canonical().MakePreferred() == Path("/foobar").MakePreferred());
    REQUIRE(Path("/foobar/.").canonical().MakePreferred() == Path("/foobar").MakePreferred());
    REQUIRE(Path("/foobar/..").canonical().MakePreferred() == Path("/").MakePreferred());
    REQUIRE(Path("/foo/bar").canonical().MakePreferred() == Path("/foo/bar").MakePreferred());
    REQUIRE(Path("///foo").canonical().MakePreferred() == Path("/foo").MakePreferred());
    REQUIRE(Path("///foo/").canonical().MakePreferred() == Path("/foo").MakePreferred());
    REQUIRE(Path("///foo///").canonical().MakePreferred() == Path("/foo").MakePreferred());
    REQUIRE(Path("///foo///bar").canonical().MakePreferred() == Path("/foo/bar").MakePreferred());
}

TEST_CASE("Path validation", "[CppCommon][FileSystem]")
{
    // Test path validation method
    REQUIRE(Path("foo:bar").validate().MakePreferred() == Path("foo_bar").MakePreferred());
    REQUIRE(Path("test/foo<bar/test%test").validate('-').MakePreferred() == Path("test/foo-bar/test-test").MakePreferred());
}

TEST_CASE("Path manipulations", "[CppCommon][FileSystem]")
{
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
}

#if defined(unix) || defined(__unix) || defined(__unix__)
TEST_CASE("Path permissions", "[CppCommon][FileSystem]")
{
    Path current = Path::current();
    auto old_permissions = current.permissions();
    auto new_permissions = old_permissions | FilePermissions::IRWXU | FilePermissions::IRWXG | FilePermissions::IRWXO;
    Path::SetPermissions(current, new_permissions);
    REQUIRE(current.permissions() == new_permissions);
    Path::SetPermissions(current, old_permissions);
    REQUIRE(current.permissions() == old_permissions);
}
#endif

#if defined(_WIN32) || defined(_WIN64)
TEST_CASE("Path attributes", "[CppCommon][FileSystem]")
{
    Path current = Path::current();
    auto old_attributes = current.attributes();
    auto new_attributes = old_attributes | FileAttributes::ARCHIVED | FileAttributes::READONLY;
    Path::SetAttributes(current, new_attributes);
    REQUIRE(current.attributes() == new_attributes);
    Path::SetAttributes(current, old_attributes);
    REQUIRE(current.attributes() == old_attributes);
}
#endif

TEST_CASE("Path timestamps", "[CppCommon][FileSystem]")
{
    Path executable = Path::executable();
    REQUIRE((executable.created().total() > 0));
    REQUIRE((executable.created().total() <= UtcTimestamp()));
    REQUIRE((executable.modified().total() > 0));
    REQUIRE((executable.modified().total() <= UtcTimestamp()));
}

TEST_CASE("Path hardlinks", "[CppCommon][FileSystem]")
{
    Path executable = Path::executable();
    REQUIRE(executable.hardlinks() > 0);
}

TEST_CASE("Path space", "[CppCommon][FileSystem]")
{
    Path current = Path::current();
    REQUIRE((current.space().capacity > 0));
    REQUIRE((current.space().free > 0));
    REQUIRE((current.space().available > 0));
}

TEST_CASE("Path copy & remove", "[CppCommon][FileSystem]")
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

    // Copy file
    File copy1 = Path::Copy(test1tmp, Path::current() / "test1.copy");
    REQUIRE(copy1.IsFileExists());
    REQUIRE(Path::Remove(copy1) == Path::current());

    // Copy directory
    Directory copy2 = Path::Copy(test2, Path::current() / "test2.copy");
    REQUIRE(copy2.IsDirectoryExists());
    REQUIRE(Path::Remove(copy2) == Path::current());

    // Copy symbolic link file
    Symlink copy3 = Path::Copy(test44tmp, Path::current() / "test3.copy");
    REQUIRE(copy3.IsSymlinkExists());
    REQUIRE(copy3.IsTargetExists());
    REQUIRE(Path::Remove(copy3) == Path::current());

    // Copy symbolic link directory
    Symlink copy4 = Path::Copy(test5, Path::current() / "test4.copy");
    REQUIRE(copy4.IsSymlinkExists());
    REQUIRE(copy4.IsTargetExists());
    REQUIRE(Path::Remove(copy4) == Path::current());

    // Copy complex directory structure
    Directory copy = Path::CopyAll(test, Path::current() / "copy");

    // Check copy directory entries
    REQUIRE(copy.GetEntries().size() == 8);
    REQUIRE(copy.GetEntries("test1.*").size() == 2);
    REQUIRE(copy.GetEntriesRecursive().size() == 22);
    REQUIRE(copy.GetEntriesRecursive("test2.*").size() == 9);
    REQUIRE(copy.GetDirectories().size() == 5);
    REQUIRE(copy.GetDirectories("test1.*").size() == 1);
    REQUIRE(copy.GetDirectoriesRecursive().size() == 9);
    REQUIRE(copy.GetDirectoriesRecursive("test2.*").size() == 3);
    REQUIRE(copy.GetFiles().size() == 3);
    REQUIRE(copy.GetFiles("test1.*").size() == 1);
    REQUIRE(copy.GetFilesRecursive().size() == 13);
    REQUIRE(copy.GetFilesRecursive("test2.*").size() == 6);
    REQUIRE(copy.GetSymlinks().size() == 1);
    REQUIRE(copy.GetSymlinks("test5.*").size() == 1);
    REQUIRE(copy.GetSymlinksRecursive().size() == 2);
    REQUIRE(copy.GetSymlinksRecursive("test4.*").size() == 1);

    // Remove complex directory structure
    REQUIRE(Path::RemoveAll(copy) == Path::current());
    REQUIRE(Path::RemoveAll(test) == Path::current());
}

TEST_CASE("Path constants of the current process", "[CppCommon][FileSystem]")
{
    Path initial = Path::initial();
    REQUIRE(!initial.empty());
    REQUIRE(initial.IsExists());
    REQUIRE(initial.IsDirectory());

    Path current = Path::current();
    REQUIRE(!current.empty());
    REQUIRE(current.IsExists());
    REQUIRE(current.IsDirectory());

    Path executable = Path::executable();
    REQUIRE(!executable.empty());
    REQUIRE(executable.IsExists());
    REQUIRE(executable.IsRegularFile());

    Path home = Path::home();
    REQUIRE(!home.empty());
    REQUIRE(home.IsExists());
    REQUIRE(home.IsDirectory());

    Path temp = Path::temp();
    REQUIRE(!temp.empty());
    REQUIRE(temp.IsExists());
    REQUIRE(temp.IsDirectory());

    Path unique = Path::unique();
    REQUIRE(!unique.empty());
    REQUIRE(!unique.IsExists());

    REQUIRE(Path(".").absolute() == current);
    REQUIRE(Path("..").absolute() == current.parent());

    REQUIRE(!current.IsEquivalent(executable));
    REQUIRE(current.IsEquivalent(current));
    REQUIRE(executable.IsEquivalent(executable));

    Path parent = current.parent();
    Path::SetCurrent(parent);
    REQUIRE(Path::current() == parent);
    Path::SetCurrent(current);
    REQUIRE(Path::current() == current);
}
