# CppCommon

[![Linux build status](https://img.shields.io/travis/chronoxor/CppCommon/master.svg?label=Linux)](https://travis-ci.org/chronoxor/CppCommon)
[![OSX build status](https://img.shields.io/travis/chronoxor/CppCommon/master.svg?label=OSX)](https://travis-ci.org/chronoxor/CppCommon)
[![Cygwin build status](https://img.shields.io/appveyor/ci/chronoxor/CppCommon/master.svg?label=Cygwin)](https://ci.appveyor.com/project/chronoxor/CppCommon)
[![MinGW build status](https://img.shields.io/appveyor/ci/chronoxor/CppCommon/master.svg?label=MinGW)](https://ci.appveyor.com/project/chronoxor/CppCommon)
[![Windows build status](https://img.shields.io/appveyor/ci/chronoxor/CppCommon/master.svg?label=Windows)](https://ci.appveyor.com/project/chronoxor/CppCommon)

C++ Common Library contains reusable components and patterns for error and
exceptions handling, filesystem manipulations, math, string format and
encoding, shared memory, threading, time management and others.

[CppCommon API reference](http://chronoxor.github.io/CppCommon/index.html)

# Contents
  * [Features](#features)
  * [Requirements](#requirements)
  * [How to build?](#how-to-build)
    * [Clone repository with submodules](#clone-repository-with-submodules)
    * [Linux](#linux)
    * [OSX](#osx)
    * [Windows (Cygwin)](#windows-cygwin)
    * [Windows (MinGW)](#windows-mingw)
    * [Windows (MinGW with MSYS)](#windows-mingw-with-msys)
    * [Windows (Visaul Studio 2015)](#windows-visaul-studio-2015)

# Features
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
* Linux
* OSX
* Windows 7 / Windows 10
* [CMake](http://www.cmake.org)
* [GIT](https://git-scm.com)
* [GCC](https://gcc.gnu.org)

Optional:
* [Clang](http://clang.llvm.org)
* [Clion](https://www.jetbrains.com/clion)
* [MinGW](http://mingw-w64.org/doku.php)
* [Visual Studio 2015](https://www.visualstudio.com)

#How to build?

## Clone repository with submodules
```
git clone https://github.com/chronoxor/CppCommon.git CppCommon
cd CppCommon
git submodule update --init --recursive --remote
```

## Linux
```
cd build
./unix.sh
```

## OSX
```
cd build
./unix.sh
```

## Windows (Cygwin)
```
cd build
cygwin.bat
```

## Windows (MinGW)
```
cd build
mingw.bat
```

## Windows (Visaul Studio 2015)
```
cd build
vs.bat
```
