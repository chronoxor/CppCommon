# CppCommon
C++ Common Library contains reusable components and patterns for memory
management, networking, threading and others.

[CppCommon API reference](http://chronoxor.github.io/CppCommon/index.html)

# Contents
  * [Features](#features)
  * [Requirements](#requirements)
  * [How to build?](#how-to-build)
    * [Clone repository with submodules](#clone-repository-with-submodules)
    * [Windows (Visaul Studio 2015)](#windows-visaul-studio-2015)
    * [Windows (MinGW with MSYS)](#windows-mingw-with-msys)
    * [Linux](#linux)

# Features
* Memory management
* Networking
* Threading

# Requirements
* Windows 7 / Windows 10
* Linux
* [Visual Studio 2015](https://www.visualstudio.com/)
* [Clion 1.0.5](https://www.jetbrains.com/clion/)
* [MinGW 4.0](http://mingw-w64.org/doku.php)
* [MSYS2](http://msys2.github.io/)
* GCC 5.0.0
* [CMake 3.3.0](http://www.cmake.org/download/)

#How to build?

## Clone repository with submodules
```
git clone git@github.com:chronoxor/CppCommon.git
git submodule update --init --recursive
```

## Windows (Visaul Studio 2015)
```
cd CppCommon\scripts
01-generate-VisualStudio-x64.bat
02-build-VisualStudio.bat
03-tests.bat
04-install-VisualStudio.bat
05-doxygen-VisualStudio.bat
```
If you want 32-bit version use '01-generate-VisualStudio-x32.bat' to generate project files.

## Windows (MinGW with MSYS)
```
cd CppCommon\scripts
01-generate-MSYS.bat
02-build-MSYS.bat
03-tests.bat
04-install-MSYS.bat
05-doxygen-MSYS.bat
```

## Linux
```
cd CppCommon\scripts
01-generate-Unix.sh
02-build-Unix.sh
03-tests.sh
04-install-Unix.sh
05-doxygen-Unix.sh
```
