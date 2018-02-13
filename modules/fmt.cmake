if(NOT TARGET fmt)

  # Module library
  file(GLOB SOURCE_FILES "fmt/include/fmt/*.cc")
  if(NOT MSVC)
    set_source_files_properties(${SOURCE_FILES} PROPERTIES COMPILE_FLAGS "${PEDANTIC_COMPILE_FLAGS} -Wno-shadow")
  else()
    # C4244: 'conversion' conversion from 'type1' to 'type2', possible loss of data
    # C4267: conversion from 'size_t' to 'type', possible loss of data
    # C4459: declaration of 'identifier' hides global declaration
    # C4702: unreachable code
    set_source_files_properties(${SOURCE_FILES} PROPERTIES COMPILE_FLAGS "${PEDANTIC_COMPILE_FLAGS} /wd4244 /wd4267 /wd4459 /wd4702")
  endif()
  add_library(fmt ${SOURCE_FILES} ${ASSEMBLER_FILES})
  target_include_directories(fmt PRIVATE "fmt")
  target_link_libraries(fmt)

  # Module folder
  set_target_properties(fmt PROPERTIES FOLDER modules/fmt)

endif()
