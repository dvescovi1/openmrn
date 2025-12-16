#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "OpenMRN::openmrn-core" for configuration "Debug"
set_property(TARGET OpenMRN::openmrn-core APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(OpenMRN::openmrn-core PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C;CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/openmrn-core.lib"
  )

list(APPEND _cmake_import_check_targets OpenMRN::openmrn-core )
list(APPEND _cmake_import_check_files_for_OpenMRN::openmrn-core "${_IMPORT_PREFIX}/lib/openmrn-core.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
