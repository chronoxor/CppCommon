cmake_minimum_required(VERSION 3.6)

# Global properties
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Project name
project(cppcommon)

# Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
  set(DOXYGEN "doxygen")
  if(NOT TARGET ${DOXYGEN})
    add_custom_command(OUTPUT "Doxyfile" COMMAND ${DOXYGEN_EXECUTABLE} "Doxyfile" WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/documents")
    add_custom_target(${DOXYGEN} DEPENDS "Doxyfile")
    set_target_properties(${DOXYGEN} PROPERTIES FOLDER doxygen)
  endif()
endif()

# CMake module path
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Compiler features
include(SetCompilerFeatures)
include(SetCompilerWarnings)
include(SetPlatformFeatures)
include(SystemInformation)

# External packages
find_package(Threads REQUIRED)
if(UNIX AND NOT APPLE AND NOT CYGWIN AND NOT MSYS)
  find_package(LibBFD)
  find_package(LibDL)
  find_package(LibRT)
  find_package(LibUUID)
endif()
if(WIN32 OR MSYS)
  find_package(DbgHelp)
  find_package(RPC)
  find_package(Userenv)
endif()

# Modules
add_subdirectory("modules")

# Link libraries
list(APPEND LINKLIBS Threads::Threads)
if(UNIX AND NOT APPLE AND NOT CYGWIN AND NOT MSYS)
  list(APPEND LINKLIBS ${LIBBFD_LIBRARIES})
  list(APPEND LINKLIBS ${LIBDL_LIBRARIES})
  list(APPEND LINKLIBS ${LIBRT_LIBRARIES})
  list(APPEND LINKLIBS ${LIBUUID_LIBRARIES})
endif()
if(WIN32 OR MSYS)
  list(APPEND LINKLIBS ${DBGHELP_LIBRARIES})
  list(APPEND LINKLIBS ${RPC_LIBRARIES})
  list(APPEND LINKLIBS ${USERENV_LIBRARIES})
  list(APPEND LINKLIBS ${LIBVLD_LIBRARIES})
endif()

# System directories
include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/modules")
include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/plugins")

# Library
file(GLOB_RECURSE LIB_HEADER_FILES "include/*.h")
file(GLOB_RECURSE LIB_INLINE_FILES "include/*.inl")
file(GLOB_RECURSE LIB_SOURCE_FILES "source/*.cpp")
set_source_files_properties(${LIB_SOURCE_FILES} PROPERTIES COMPILE_FLAGS "${PEDANTIC_COMPILE_FLAGS}")
add_library(cppcommon ${LIB_HEADER_FILES} ${LIB_INLINE_FILES} ${LIB_SOURCE_FILES})
target_include_directories(cppcommon PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" PUBLIC ${vld})
target_link_libraries(cppcommon ${LINKLIBS} fmt)
set_target_properties(cppcommon PROPERTIES FOLDER libraries)
list(APPEND INSTALL_TARGETS cppcommon)
list(APPEND LINKLIBS cppcommon)

# Additional module components: benchmarks, examples, plugins, tests, tools and install
if(NOT CPPCOMMON_MODULE)

  # Plugins
  file(GLOB PLUGIN_NAMES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/plugins" "plugins/*")
  foreach(PLUGIN_NAME ${PLUGIN_NAMES})
    file(GLOB PLUGIN_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/plugins/${PLUGIN_NAME}/*.h")
    file(GLOB PLUGIN_INLINE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/plugins/${PLUGIN_NAME}/*.inl")
    file(GLOB PLUGIN_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/plugins/${PLUGIN_NAME}/*.cpp")
    set_source_files_properties(${PLUGIN_SOURCE_FILES} PROPERTIES COMPILE_DEFINITIONS "EXPORTS")
    set_source_files_properties(${PLUGIN_SOURCE_FILES} PROPERTIES COMPILE_FLAGS "${PEDANTIC_COMPILE_FLAGS}")
    set(PLUGIN_TARGET "plugin-${PLUGIN_NAME}")
    add_library(${PLUGIN_TARGET} SHARED ${PLUGIN_HEADER_FILES} ${PLUGIN_INLINE_FILES} ${PLUGIN_SOURCE_FILES})
    target_link_libraries(${PLUGIN_TARGET} ${LINKLIBS})
    set_target_properties(${PLUGIN_TARGET} PROPERTIES FOLDER plugins)
    list(APPEND PLUGINS ${PLUGIN_TARGET})
    list(APPEND INSTALL_TARGETS ${PLUGIN_TARGET})
    list(APPEND INSTALL_TARGETS_PDB ${PLUGIN_TARGET})
  endforeach()

  # Examples
  file(GLOB EXAMPLE_HEADER_FILES "examples/*.h")
  file(GLOB EXAMPLE_INLINE_FILES "examples/*.inl")
  file(GLOB EXAMPLE_SOURCE_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/examples" "examples/*.cpp")
  foreach(EXAMPLE_SOURCE_FILE ${EXAMPLE_SOURCE_FILES})
    string(REGEX REPLACE "(.*)\\.cpp" "\\1" EXAMPLE_NAME ${EXAMPLE_SOURCE_FILE})
    set(EXAMPLE_TARGET "cppcommon-example-${EXAMPLE_NAME}")
    set_source_files_properties(examples/${EXAMPLE_SOURCE_FILE} PROPERTIES COMPILE_FLAGS "${PEDANTIC_COMPILE_FLAGS}")
    add_executable(${EXAMPLE_TARGET} ${EXAMPLE_HEADER_FILES} ${EXAMPLE_INLINE_FILES} examples/${EXAMPLE_SOURCE_FILE})
    target_link_libraries(${EXAMPLE_TARGET} ${LINKLIBS})
    set_target_properties(${EXAMPLE_TARGET} PROPERTIES FOLDER examples)
    add_dependencies(${EXAMPLE_TARGET} ${PLUGINS})
    list(APPEND INSTALL_TARGETS ${EXAMPLE_TARGET})
    list(APPEND INSTALL_TARGETS_PDB ${EXAMPLE_TARGET})
  endforeach()

  # Benchmarks
  file(GLOB BENCHMARK_HEADER_FILES "performance/*.h")
  file(GLOB BENCHMARK_INLINE_FILES "performance/*.inl")
  file(GLOB BENCHMARK_SOURCE_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/performance" "performance/*.cpp")
  foreach(BENCHMARK_SOURCE_FILE ${BENCHMARK_SOURCE_FILES})
    string(REGEX REPLACE "(.*)\\.cpp" "\\1" BENCHMARK_NAME ${BENCHMARK_SOURCE_FILE})
    set(BENCHMARK_TARGET "cppcommon-performance-${BENCHMARK_NAME}")
    set_source_files_properties(performance/${BENCHMARK_SOURCE_FILE} PROPERTIES COMPILE_FLAGS "${PEDANTIC_COMPILE_FLAGS}")
    add_executable(${BENCHMARK_TARGET} ${BENCHMARK_HEADER_FILES} ${BENCHMARK_INLINE_FILES} performance/${BENCHMARK_SOURCE_FILE})
    target_link_libraries(${BENCHMARK_TARGET} ${LINKLIBS} cppbenchmark)
    set_target_properties(${BENCHMARK_TARGET} PROPERTIES FOLDER performance)
    list(APPEND INSTALL_TARGETS ${BENCHMARK_TARGET})
    list(APPEND INSTALL_TARGETS_PDB ${BENCHMARK_TARGET})
  endforeach()

  # Tests
  file(GLOB TESTS_HEADER_FILES "tests/*.h")
  file(GLOB TESTS_INLINE_FILES "tests/*.inl")
  file(GLOB TESTS_SOURCE_FILES "tests/*.cpp")
  set_source_files_properties(${TESTS_SOURCE_FILES} PROPERTIES COMPILE_FLAGS "${PEDANTIC_COMPILE_FLAGS}")
  add_executable(cppcommon-tests ${TESTS_HEADER_FILES} ${TESTS_INLINE_FILES} ${TESTS_SOURCE_FILES} ${Catch2})
  target_include_directories(cppcommon-tests PRIVATE ${Catch2})
  target_link_libraries(cppcommon-tests ${LINKLIBS})
  set_target_properties(cppcommon-tests PROPERTIES FOLDER tests)
  add_dependencies(cppcommon-tests ${PLUGINS})
  list(APPEND INSTALL_TARGETS cppcommon-tests)
  list(APPEND INSTALL_TARGETS_PDB cppcommon-tests)

  # CTest
  enable_testing()
  add_test(cppcommon-tests cppcommon-tests --durations yes --order lex)

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
