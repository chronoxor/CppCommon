if(NOT TARGET fmt)

  # Module library
  file(GLOB SOURCE_FILES "fmt/src/*.cc")
  list(FILTER SOURCE_FILES EXCLUDE REGEX ".*/fmt.cc")
  add_library(fmt ${SOURCE_FILES})
  if(MSVC)
    # C4702: unreachable code
    set_target_properties(fmt PROPERTIES COMPILE_FLAGS "${PEDANTIC_COMPILE_FLAGS} /wd4702")
  else()
    set_target_properties(fmt PROPERTIES COMPILE_FLAGS "${PEDANTIC_COMPILE_FLAGS} -Wno-shadow")
  endif()
  target_include_directories(fmt PUBLIC "fmt/include")
  target_link_libraries(fmt)

  # Module folder
  set_target_properties(fmt PROPERTIES FOLDER "modules/fmt")

endif()
