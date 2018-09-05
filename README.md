# CppCommon

[![Linux build status](https://img.shields.io/travis/chronoxor/CppCommon/master.svg?label=Linux)](https://travis-ci.org/chronoxor/CppCommon)
[![OSX build status](https://img.shields.io/travis/chronoxor/CppCommon/master.svg?label=OSX)](https://travis-ci.org/chronoxor/CppCommon)
[![Cygwin build status](https://img.shields.io/appveyor/ci/chronoxor/CppCommon/master.svg?label=Cygwin)](https://ci.appveyor.com/project/chronoxor/CppCommon)
[![MinGW build status](https://img.shields.io/appveyor/ci/chronoxor/CppCommon/master.svg?label=MinGW)](https://ci.appveyor.com/project/chronoxor/CppCommon)
[![Windows build status](https://img.shields.io/appveyor/ci/chronoxor/CppCommon/master.svg?label=Windows)](https://ci.appveyor.com/project/chronoxor/CppCommon)

C++ Common Library contains reusable components and patterns for error and
exceptions handling, filesystem manipulations, math, string format and
encoding, shared memory, threading, time management and others.

[CppCommon API reference](https://chronoxor.github.io/CppCommon/index.html)

# Contents
  * [Features](#features)
  * [Requirements](#requirements)
  * [How to build?](#how-to-build)

# Features
* Cross platform (Linux, OSX, Windows)
* Exception handling model
* Filesystem manipulations
* String encoding converters
* String format using [{fmt} library](http://fmtlib.net)
* CPU, memory, environment
* Shared memory
* Stack trace
* UUID generator
* Thread extensions (priority, affinity, yield)
* Thread barrier, latch
* Synchronization primitives
* Named synchronization primitives
* Producer/consumer queues
* Time management
* Utilities

# Requirements
* Linux (gcc g++ cmake doxygen graphviz binutils-dev uuid-dev)
* OSX (clang cmake doxygen graphviz)
* Windows 7 / Windows 10
* [cmake](https://www.cmake.org)
* [git](https://git-scm.com)
* [gcc](https://gcc.gnu.org)

Optional:
* [clang](https://clang.llvm.org)
* [clion](https://www.jetbrains.com/clion)
* [Cygwin](https://cygwin.com)
* [MinGW](https://mingw-w64.org/doku.php)
* [Visual Studio](https://www.visualstudio.com)

# How to build?

### Clone repository with submodules
```shell
git clone https://github.com/chronoxor/CppCommon.git
cd CppCommon
git submodule update --init --recursive --remote
```

### Linux
```shell
cd build
./unix.sh
```

### OSX
```shell
cd build
./unix.sh
```

### Windows (Cygwin)
```shell
cd build
cygwin.bat
```

### Windows (MinGW)
```shell
cd build
mingw.bat
```

### Windows (Visual Studio)
```shell
cd build
vs.bat
```
