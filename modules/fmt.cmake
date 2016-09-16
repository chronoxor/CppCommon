if(NOT TARGET fmt)
  option(FMT_USE_CPP11 "Enable the addition of C++11 compiler flags." ON)
  add_subdirectory("fmt/fmt")
endif()
