if(NOT TARGET cppbenchmark)

  # Set module flag
  set(CPPBENCHMARK_MODULE Y)

  # Add module subdirectory
  add_subdirectory("CppBenchmark")

  # Set module folder
  set_target_properties(cppbenchmark PROPERTIES FOLDER modules/CppBenchmark)

endif()
