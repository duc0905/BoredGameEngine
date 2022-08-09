#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "minizip::minizip" for configuration "Release"
set_property(TARGET minizip::minizip APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(minizip::minizip PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/minizip.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS minizip::minizip )
list(APPEND _IMPORT_CHECK_FILES_FOR_minizip::minizip "${_IMPORT_PREFIX}/lib/minizip.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
