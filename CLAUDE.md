# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

CppCommon is a cross-platform (Linux / macOS / Windows) C++ library of reusable building blocks: error handling, filesystem, math, string formatting/encoding, shared memory, threading and synchronization primitives (including named/cross-process variants), producer/consumer queues, time, system info (CPU, environment, stack trace, UUID), and assorted utilities. It builds as a static library (`cppcommon`) plus a Catch2 test binary, ~78 example executables, ~40 CppBenchmark performance binaries, and shared-library plugins.

## Repository bootstrap

The `build/` and `cmake/` directories and the contents of `modules/` (Catch2, CppBenchmark, fmt, vld) are **not** in this repo — they are pulled in by the [gil](https://github.com/chronoxor/gil) "git links" tool from the URLs declared in [.gitlinks](.gitlinks). After cloning, you must run:

```shell
pip3 install gil
gil update
```

before any build will work. If `build/` or `modules/Catch2/` is missing, that's the first thing to check.

On Linux the system also needs `binutils-dev` and `uuid-dev` (used by `LibBFD` for stack traces and `LibUUID` for UUIDs — see [CMakeLists.txt:31-36](CMakeLists.txt:31)).

## Build & test

The `build/` scripts (from CppBuildScripts) are the canonical entry points used by CI:

| Platform | Command |
|---|---|
| Linux / macOS / Cygwin / MSYS2 | `cd build && ./unix.sh` |
| Windows MinGW | `cd build && mingw.bat` |
| Windows Visual Studio | `cd build && vs.bat` |

These scripts wrap `cmake` + the platform's build tool and drop binaries in `bin/`.

To run the test suite directly:

```shell
./bin/cppcommon-tests --durations yes --order lex     # all tests (this is what CTest runs)
./bin/cppcommon-tests "[Threads]"                      # by Catch2 tag
./bin/cppcommon-tests "Thread"                         # by test case name
./bin/cppcommon-tests --list-tests                     # discover names/tags
ctest --test-dir build                                 # via CTest
```

Tests use Catch2 v3 (amalgamated, included via `#include <catch_amalgamated.hpp>` from [tests/test.h](tests/test.h)). Each test case is tagged like `[CppCommon][Threads]` so Catch2 tag filters work well for running subsets.

## Architecture

### Code layout

- [include/](include) — public headers, organized by domain: `algorithms`, `cache`, `common` (flags, function, uint128/256, version, reader/writer), `containers` (intrusive bintree variants AA/AVL/RB/Splay, flatmap, hashmap, list, queue, stack), `errors`, `filesystem`, `math`, `memory`, `string`, `system` (CPU, console, DLL, environment, pipe, process, shared_memory, source_location, stack_trace, UUID), `threads`, `time`, `utility`. Files come in `.h` + sibling `.inl` pairs — the `.inl` holds template/inline definitions and is included from the bottom of the matching `.h`.
- [source/](source) — non-template `.cpp` implementations mirroring the same domain folders. Platform-specific code lives inline behind `#if defined(_WIN32) || defined(_WIN64)` / `#if defined(unix) || defined(__APPLE__)` guards rather than separate per-platform files.
- [tests/](tests) — one `test_<area>_<thing>.cpp` per feature, built into a single `cppcommon-tests` Catch2 executable.
- [examples/](examples) — one runnable `.cpp` per feature, each producing `cppcommon-example-<name>`.
- [performance/](performance) — CppBenchmark drivers, one per feature, each producing `cppcommon-performance-<name>`.
- [plugins/](plugins) — each subdirectory becomes a separate **shared** library `plugin-<name>` (compiled with `EXPORTS` defined). Used by examples/tests that demonstrate dynamic loading via `system/dll.h`'s `API` / `EXPORT` macros.

### CMake structure

[CMakeLists.txt](CMakeLists.txt) is a single top-level file. Key behaviors:

- `CPPCOMMON_MODULE` switch: when this repo is consumed as a submodule by another project (set by the parent's CMake), only the `cppcommon` library target is built — examples, tests, benchmarks, plugins and install rules are skipped (see the `if(NOT CPPCOMMON_MODULE)` block at line 77).
- Sources are picked up via `file(GLOB_RECURSE)`, so adding a new `.h`/`.cpp`/`.inl` under `include/` or `source/` does not require touching CMake — but it does require re-running CMake to refresh the glob.
- A new example/benchmark is just a new `.cpp` in `examples/` or `performance/`; the loop generates a target per file. New tests are likewise auto-picked up into `cppcommon-tests`.
- Adding a new plugin = create `plugins/<name>/*.{h,cpp}`; a `plugin-<name>` shared library target is generated automatically.
- Compiler flags come from `cmake/` modules (`SetCompilerFeatures`, `SetCompilerWarnings`, `SetPlatformFeatures`, `SystemInformation`) provided by the gil-managed CppCMakeScripts repo. `PEDANTIC_COMPILE_FLAGS` is applied to library/example/test/benchmark/plugin targets.

### Cross-platform conventions

- Everything lives in `namespace CppCommon`.
- The library links different OS facilities depending on platform: on Linux, `LibBFD` (stack symbols), `LibDL`, `LibRT`, `LibUUID`; on Windows/MSYS, `DbgHelp`, `RPC`, `Userenv` (and `vld` if available). Use `CMakeLists.txt:31-58` as the reference when adding code that needs a new system dependency.
- `system/dll.h` defines `API` / `EXPORT` macros that expand to `__declspec(dllexport/dllimport)` on Windows and visibility attributes on POSIX — use these for any symbol crossing a shared-library boundary.

## Documentation

API reference is generated by Doxygen from [documents/Doxyfile](documents/Doxyfile). When the Doxygen package is found by CMake, a `doxygen` custom target is added; the published site lives at https://chronoxor.github.io/CppCommon/.
