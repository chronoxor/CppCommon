# CppCommon
C++ Common Library contains reusable components and patterns for error and
exceptions handling, filesystem manipulations, math, string format and
encoding, shared memory, threading, time management and others.

[CppCommon API reference](http://chronoxor.github.io/CppCommon/index.html)

[![Build status](https://travis-ci.org/chronoxor/CppCommon.svg?branch=master)](https://travis-ci.org/chronoxor/CppCommon)
[![Build status](https://ci.appveyor.com/api/projects/status/6cgcwr1uu0re1o5p?svg=true)](https://ci.appveyor.com/project/chronoxor/CppCommon)

# Contents
  * [Features](#features)
  * [Requirements](#requirements)
  * [How to build?](#how-to-build)
    * [Clone repository with submodules](#clone-repository-with-submodules)
    * [Windows (Visaul Studio 2015)](#windows-visaul-studio-2015)
    * [Windows (MinGW with MSYS)](#windows-mingw-with-msys)
    * [Linux](#linux)

# Features
* Exception handling model
* Filesystem manipulations
* String encoding converters
* String format using [{fmt} library](https://github.com/fmtlib/fmt)
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
* Windows 7 / Windows 10
* Linux
* [CMake](http://www.cmake.org/download/)
* [GIT](https://git-scm.com/)
* [GCC](https://gcc.gnu.org/)

Optional:
* [Visual Studio 2015](https://www.visualstudio.com/)
* [Clion](https://www.jetbrains.com/clion/)
* [MinGW](http://mingw-w64.org/doku.php)
* [MSYS](http://www.mingw.org/wiki/msys)

#How to build?

## Clone repository with submodules
```
git clone https://github.com/chronoxor/CppCommon.git CppCommon
cd CppCommon
git submodule update --init --recursive --remote
```

## Windows (Visaul Studio 2015)
```
cd scripts
01-generate-VisualStudio.bat
02-build-VisualStudio.bat
03-tests.bat
04-install-VisualStudio.bat
05-doxygen-VisualStudio.bat
```

## Windows (MinGW with MSYS)
```
cd scripts
01-generate-MSYS.bat
02-build-MSYS.bat
03-tests.bat
04-install-MSYS.bat
05-doxygen-MSYS.bat
```

## Linux
```
cd scripts
./01-generate-Unix.sh
./02-build-Unix.sh
./03-tests.sh
./04-install-Unix.sh
./05-doxygen-Unix.sh
```
