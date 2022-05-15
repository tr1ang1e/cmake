#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "exportable" for configuration "Debug"
set_property(TARGET exportable APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(exportable PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/exportable_libs/exportable.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS exportable )
list(APPEND _IMPORT_CHECK_FILES_FOR_exportable "${_IMPORT_PREFIX}/exportable_libs/exportable.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
