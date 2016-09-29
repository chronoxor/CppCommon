cmake_minimum_required(VERSION 2.8)

# Global properties
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Project name
project(cppcommon)

# Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
  set(DOXYGEN "doxygen")
  if(NOT TARGET ${DOXYGEN})
    add_custom_command(OUTPUT "Doxyfile" COMMAND ${DOXYGEN_EXECUTABLE} "Doxyfile" WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/documents")
    add_custom_target(${DOXYGEN} DEPENDS "Doxyfile")
    set_target_properties(${DOXYGEN} PROPERTIES FOLDER doxygen)
  endif()
endif()

# CMake module path
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Compiler features
include(SetCompilerFeatures)
include(SetCompilerWarningAll)
include(SetCrtSecureWarningsOff)
include(SetPlatformFeatures)

# External packages
find_package(Threads REQUIRED)
if(UNIX)
  find_package(LibBFD)
  find_package(LibDL)
  find_package(LibRT)
  find_package(LibUUID)
endif()
if(WIN32)
  find_package(DbgHelp)
  find_package(Userenv)
endif()

# System link libraries
list(APPEND SYSTEM Threads::Threads)
if(UNIX)
  list(APPEND SYSTEM ${LIBBFD_LIBRARIES})
  list(APPEND SYSTEM ${LIBDL_LIBRARIES})
  list(APPEND SYSTEM ${LIBRT_LIBRARIES})
  list(APPEND SYSTEM ${LIBUUID_LIBRARIES})
endif()
if(WIN32)
  list(APPEND SYSTEM ${DBGHELP_LIBRARIES})
  list(APPEND SYSTEM ${USERENV_LIBRARIES})
endif()

# Modules
add_subdirectory("modules")

# Includes
include_directories("include")
include_directories("modules/catch/single_include")
include_directories("modules/CppBenchmark/include")
include_directories("modules/fmt")

# Library
file(GLOB_RECURSE SOURCE_FILES "source/*.cpp")
add_library(cppcommon ${SOURCE_FILES})
target_link_libraries(cppcommon fmt)
set_target_properties(cppcommon PROPERTIES FOLDER libraries)
list(APPEND INSTALL_TARGETS cppcommon)

# Full module components: examples, tests and install
if(NOT CPPCOMMON_MODULE)

  # Examples
  file(GLOB EXAMPLE_FILES RELATIVE "${PROJECT_SOURCE_DIR}/examples" "examples/*.cpp")
  foreach(EXAMPLE_FILE ${EXAMPLE_FILES})
    string(REGEX REPLACE "(.*)\\.cpp" "\\1" EXAMPLE_NAME ${EXAMPLE_FILE})
    set(EXAMPLE_TARGET "cppcommon-example-${EXAMPLE_NAME}")
    add_executable(${EXAMPLE_TARGET} examples/${EXAMPLE_FILE})
    target_link_libraries(${EXAMPLE_TARGET} cppcommon ${SYSTEM})
    set_target_properties(${EXAMPLE_TARGET} PROPERTIES FOLDER examples)
    list(APPEND INSTALL_TARGETS ${EXAMPLE_TARGET})
    list(APPEND INSTALL_TARGETS_PDB ${EXAMPLE_TARGET})
  endforeach()

  # Performance benchmarks
  file(GLOB BENCHMARK_FILES RELATIVE "${PROJECT_SOURCE_DIR}/performance" "performance/*.cpp")
  foreach(BENCHMARK_FILE ${BENCHMARK_FILES})
    string(REGEX REPLACE "(.*)\\.cpp" "\\1" BENCHMARK_NAME ${BENCHMARK_FILE})
    set(BENCHMARK_TARGET "cppcommon-benchmark-${BENCHMARK_NAME}")
    add_executable(${BENCHMARK_TARGET} performance/${BENCHMARK_FILE})
    target_link_libraries(${BENCHMARK_TARGET} cppcommon cppbenchmark ${SYSTEM})
    set_target_properties(${BENCHMARK_TARGET} PROPERTIES FOLDER performance)
    list(APPEND INSTALL_TARGETS ${BENCHMARK_TARGET})
    list(APPEND INSTALL_TARGETS_PDB ${BENCHMARK_TARGET})
  endforeach()

  # Tests
  file(GLOB TESTS_SOURCE_FILES "tests/*.cpp")
  add_executable(cppcommon-tests ${CATCH_HEADER} ${TESTS_SOURCE_FILES})
  target_link_libraries(cppcommon-tests cppcommon ${SYSTEM})
  set_target_properties(cppcommon-tests PROPERTIES FOLDER tests)
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

endif()
