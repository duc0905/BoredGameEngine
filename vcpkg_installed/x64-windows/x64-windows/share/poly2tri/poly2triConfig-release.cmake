#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "poly2tri::poly2tri" for configuration "Release"
set_property(TARGET poly2tri::poly2tri APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(poly2tri::poly2tri PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/poly2tri.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS poly2tri::poly2tri )
list(APPEND _IMPORT_CHECK_FILES_FOR_poly2tri::poly2tri "${_IMPORT_PREFIX}/lib/poly2tri.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
