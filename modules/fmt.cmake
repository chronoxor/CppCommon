if(NOT TARGET fmt)
  option(FMT_USE_CPP11 "Disable the addition check for C++11 compiler flags." OFF)
  add_subdirectory("fmt")
endif()
