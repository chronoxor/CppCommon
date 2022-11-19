if(WIN32 AND NOT TARGET vld)

  # Try to find VLD library and include path.
  # Once done this will define
  #
  # VLD_FOUND
  # VLD_INCLUDE_DIR
  # VLD_LIBRARIES
  
  find_path(VLD_INCLUDE_DIR vld.h HINTS vld/bin)
  find_library(VLD_LIBRARY vld.lib HINTS vld/bin)
  
  # Handle the REQUIRED argument and set VLD_FOUND
  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(VLD DEFAULT_MSG VLD_LIBRARY VLD_INCLUDE_DIR)
  
  mark_as_advanced(VLD_INCLUDE_DIR)
  mark_as_advanced(VLD_LIBRARY)
  
  if(VLD_FOUND)
    add_definitions(-DVLD_SUPPORT)
    set(VLD_LIBRARIES ${VLD_LIBRARY})
    set(vld ${VLD_INCLUDE_DIR} PARENT_SCOPE)
  endif()
 
endif()
