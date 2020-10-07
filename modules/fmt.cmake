if(NOT TARGET fmt)

  # Module library
  file(GLOB SOURCE_FILES "fmt/src/*.cc")
  add_library(fmt ${SOURCE_FILES})
  if(MSVC)
    set_target_properties(fmt PROPERTIES COMPILE_FLAGS "${PEDANTIC_COMPILE_FLAGS}")
  else()
    set_target_properties(zlib PROPERTIES COMPILE_FLAGS "${PEDANTIC_COMPILE_FLAGS} -Wno-shadow")
  endif()
  target_include_directories(fmt PUBLIC "fmt/include")
  target_link_libraries(fmt)

  # Module folder
  set_target_properties(fmt PROPERTIES FOLDER "modules/fmt")

endif()
