if(NOT TARGET fmt)

  # Temporary disable some warnings
  # C4127: conditional expression is constant
  # C4702: unreachable code
  if(CMAKE_MAKE_PROGRAM MATCHES "(MSBuild|devenv|msdev|nmake)")
    set(CMAKE_OLD_C_FLAGS ${CMAKE_C_FLAGS})
    set(CMAKE_OLD_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4127 /wd4702")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4127 /wd4702")
  endif()

  # Set module options
  option(FMT_PEDANTIC "Enable extra warnings and expensive tests." ON)
  option(FMT_USE_CPP11 "Disable the addition check for C++11 compiler flags." OFF)

  # Add module subdirectory
  add_subdirectory("fmt")

  # Set module folder
  set_target_properties(fmt PROPERTIES FOLDER modules/fmt)
  
  # Restore default warnings
  if(CMAKE_MAKE_PROGRAM MATCHES "(MSBuild|devenv|msdev|nmake)")
    set(CMAKE_C_FLAGS ${CMAKE_OLD_C_FLAGS})
    set(CMAKE_CXX_FLAGS ${CMAKE_OLD_CXX_FLAGS})
  endif()

endif()
