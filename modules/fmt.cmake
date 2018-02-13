if(NOT TARGET fmt)

  # Module library
  file(GLOB SOURCE_FILES "fmt/include/fmt/*.cc")
  if(NOT MSVC)
    set_source_files_properties(${SOURCE_FILES} PROPERTIES COMPILE_FLAGS "${PEDANTIC_COMPILE_FLAGS}")
  else()
    # C4127: conditional expression is constant
    # C4244: 'conversion' conversion from 'type1' to 'type2', possible loss of data
    # C4702: unreachable code
    set_source_files_properties(${SOURCE_FILES} PROPERTIES COMPILE_FLAGS "${PEDANTIC_COMPILE_FLAGS} /wd4127 /wd4244 /wd4702")
  endif()
  add_library(fmt ${SOURCE_FILES} ${ASSEMBLER_FILES})
  target_include_directories(fmt PRIVATE "fmt")
  target_link_libraries(fmt)

  # Module folder
  set_target_properties(fmt PROPERTIES FOLDER modules/fmt)

endif()
