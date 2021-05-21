#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "tvision::tvision" for configuration "Release"
set_property(TARGET tvision::tvision APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(tvision::tvision PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libtvision.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS tvision::tvision )
list(APPEND _IMPORT_CHECK_FILES_FOR_tvision::tvision "${_IMPORT_PREFIX}/lib/libtvision.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
