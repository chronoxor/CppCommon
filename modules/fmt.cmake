if(NOT TARGET fmt)

  # Module library
  file(GLOB SOURCE_FILES "fmt/src/*.cc")
  if(MSVC)
    # C4127: conditional expression is constant
    set_source_files_properties(${SOURCE_FILES} PROPERTIES COMPILE_FLAGS "${PEDANTIC_COMPILE_FLAGS} /wd4127")
  else()
    set_source_files_properties(${SOURCE_FILES} PROPERTIES COMPILE_FLAGS "${PEDANTIC_COMPILE_FLAGS} -Wno-gnu-string-literal-operator-template")
  endif()
  add_library(fmt ${SOURCE_FILES})
  target_include_directories(fmt PUBLIC "fmt/include")
  target_link_libraries(fmt)

  # Module folder
  set_target_properties(fmt PROPERTIES FOLDER modules/fmt)

endif()
