if(NOT TARGET vld)

  # Try to find VLD library and include path.
  # Once done this will define
  #
  # LIBVLD_FOUND
  # LIBVLD_INCLUDE_DIR
  # LIBVLD_LIBRARIES
  
  find_path(LIBVLD_INCLUDE_DIR vld.h HINTS vld)
  find_library(LIBVLD_LIBRARY vld.lib HINTS vld)
  
  # Handle the REQUIRED argument and set LIBVLD_FOUND
  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(LIBVLD DEFAULT_MSG LIBVLD_LIBRARY LIBVLD_INCLUDE_DIR)
  
  mark_as_advanced(LIBVLD_INCLUDE_DIR)
  mark_as_advanced(LIBVLD_LIBRARY)
  
  if(LIBVLD_FOUND)
    add_definitions(-DLIBVLD_SUPPORT)
    set(LIBVLD_LIBRARIES ${LIBVLD_LIBRARY})
    set(vld ${LIBVLD_INCLUDE_DIR} PARENT_SCOPE)
  endif()
 
endif()
