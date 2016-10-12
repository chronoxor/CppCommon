if(NOT TARGET fmt)

  # Module library
  file(GLOB SOURCE_FILES "fmt/fmt/*.cc")
  if(NOT MSVC)
    set_source_files_properties(${SOURCE_FILES} PROPERTIES COMPILE_FLAGS "${PEDANTIC_COMPILE_FLAGS}")
  else()
    # C4127: conditional expression is constant
    # C4702: unreachable code
    set_source_files_properties(${SOURCE_FILES} PROPERTIES COMPILE_FLAGS "${PEDANTIC_COMPILE_FLAGS} /wd4127 /wd4702")
  endif()
  add_library(fmt ${SOURCE_FILES} ${ASSEMBLER_FILES})
  target_include_directories(fmt PRIVATE "fmt")
  target_link_libraries(fmt)

  # Module folder
  set_target_properties(fmt PROPERTIES FOLDER modules/fmt)

endif()
