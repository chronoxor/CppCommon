cmake_minimum_required(VERSION 3.2)
project(cppcommon)

# Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
  set(DOXYGEN "doxygen")
  if(NOT TARGET ${DOXYGEN})
    add_custom_command(OUTPUT "Doxyfile" COMMAND ${DOXYGEN_EXECUTABLE} "Doxyfile" WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/documents")
    add_custom_target(${DOXYGEN} DEPENDS "Doxyfile")
  endif()
endif()

# CMake module path
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Compiler features
include(SetCompilerFeatures)
include(SetCompilerWarningAll)
include(SetCrtSecureWarningsOff)

# External packages
find_package(Threads REQUIRED)
if(UNIX)
  find_package(LibDL REQUIRED)
endif()
if(WIN32)
  find_package(DbgHelp REQUIRED)
endif()

# System link libraries
list(APPEND SYSTEM Threads::Threads)
if(UNIX)
  list(APPEND SYSTEM ${LIBDL_LIBRARIES})
endif()
if(WIN32)
  list(APPEND SYSTEM ${DBGHELP_LIBRARIES})
endif()

# Modules
add_subdirectory("modules")

# Includes
include_directories("include")
include_directories("modules/catch/single_include")
include_directories("modules/cpp-optparse")
include_directories("modules/CppBenchmark/include")

# Library
file(GLOB_RECURSE SOURCE_FILES "source/*.cpp")
add_library(cppcommon ${SOURCE_FILES})
target_link_libraries(cppcommon)
list(APPEND INSTALL_TARGETS cppcommon)

# Examples
file(GLOB EXAMPLE_FILES RELATIVE "${PROJECT_SOURCE_DIR}/examples" "examples/*.cpp")
foreach(EXAMPLE_FILE ${EXAMPLE_FILES})
  string(REGEX REPLACE "(.*)\\.cpp" "\\1" EXAMPLE_NAME ${EXAMPLE_FILE})
  set(EXAMPLE_TARGET "cppcommon-example-${EXAMPLE_NAME}")
  add_executable(${EXAMPLE_TARGET} examples/${EXAMPLE_FILE})
  target_link_libraries(${EXAMPLE_TARGET} cppcommon ${SYSTEM})
  list(APPEND INSTALL_TARGETS ${EXAMPLE_TARGET})
  list(APPEND INSTALL_TARGETS_PDB ${EXAMPLE_TARGET})
endforeach()

# Performance
file(GLOB BENCHMARK_FILES RELATIVE "${PROJECT_SOURCE_DIR}/perf" "perf/*.cpp")
foreach(BENCHMARK_FILE ${BENCHMARK_FILES})
  string(REGEX REPLACE "(.*)\\.cpp" "\\1" BENCHMARK_NAME ${BENCHMARK_FILE})
  set(BENCHMARK_TARGET "cppcommon-perf-${BENCHMARK_NAME}")
  add_executable(${BENCHMARK_TARGET} perf/${BENCHMARK_FILE})
  target_link_libraries(${BENCHMARK_TARGET} cppcommon cppbenchmark ${SYSTEM})
  list(APPEND INSTALL_TARGETS ${BENCHMARK_TARGET})
  list(APPEND INSTALL_TARGETS_PDB ${BENCHMARK_TARGET})
endforeach()

# Tests
file(GLOB TESTS_SOURCE_FILES "tests/*.cpp")
add_executable(cppcommon-tests ${CATCH_HEADER} ${TESTS_SOURCE_FILES})
target_link_libraries(cppcommon-tests cppcommon ${SYSTEM})
list(APPEND INSTALL_TARGETS cppcommon-tests)
list(APPEND INSTALL_TARGETS_PDB cppcommon-tests)

# CTest
enable_testing()
add_test(cppcommon-tests cppcommon-tests)

# Install
install(TARGETS ${INSTALL_TARGETS}
  RUNTIME DESTINATION "${PROJECT_SOURCE_DIR}/bin"
  LIBRARY DESTINATION "${PROJECT_SOURCE_DIR}/bin"
  ARCHIVE DESTINATION "${PROJECT_SOURCE_DIR}/bin")

# Install *.pdb files
if(MSVC)
  foreach(INSTALL_TARGET_PDB ${INSTALL_TARGETS_PDB})
    install(FILES $<TARGET_PDB_FILE:${INSTALL_TARGET_PDB}> DESTINATION "${PROJECT_SOURCE_DIR}/bin")
  endforeach()
endif()
