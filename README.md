# CppCommon

[![Linux build status](https://img.shields.io/travis/com/chronoxor/CppCommon/master?label=Linux)](https://travis-ci.com/chronoxor/CppCommon)
[![OSX build status](https://img.shields.io/travis/com/chronoxor/CppCommon/master?label=OSX)](https://travis-ci.com/chronoxor/CppCommon)
[![Cygwin build status](https://img.shields.io/appveyor/build/chronoxor/CppCommon/master?label=Cygwin)](https://ci.appveyor.com/project/chronoxor/CppCommon)
[![MSYS2 build status](https://img.shields.io/appveyor/build/chronoxor/CppCommon/master?label=MSYS2)](https://ci.appveyor.com/project/chronoxor/CppCommon)
[![MinGW build status](https://img.shields.io/appveyor/build/chronoxor/CppCommon/master?label=MinGW)](https://ci.appveyor.com/project/chronoxor/CppCommon)
[![Windows build status](https://img.shields.io/appveyor/build/chronoxor/CppCommon/master?label=Windows)](https://ci.appveyor.com/project/chronoxor/CppCommon)

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
* Linux (binutils-dev uuid-dev)
* OSX
* Windows 10
* [cmake](https://www.cmake.org)
* [gcc](https://gcc.gnu.org)
* [git](https://git-scm.com)
* [gil](https://github.com/chronoxor/gil.git)
* [python3](https://www.python.org)

Optional:
* [clang](https://clang.llvm.org)
* [CLion](https://www.jetbrains.com/clion)
* [Cygwin](https://cygwin.com)
* [MSYS2](https://www.msys2.org)
* [MinGW](https://mingw-w64.org/doku.php)
* [Visual Studio](https://www.visualstudio.com)

# How to build?

### Install [gil (git links) tool](https://github.com/chronoxor/gil)
```shell
pip3 install gil
```

### Setup repository
```shell
git clone https://github.com/chronoxor/CppCommon.git
cd CppCommon
gil update
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
unix.bat
```

### Windows (MSYS2)
```shell
cd build
unix.bat
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
