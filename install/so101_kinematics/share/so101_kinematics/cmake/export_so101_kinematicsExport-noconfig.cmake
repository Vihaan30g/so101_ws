#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "so101_kinematics::so101_kinematics" for configuration ""
set_property(TARGET so101_kinematics::so101_kinematics APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(so101_kinematics::so101_kinematics PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libso101_kinematics.so"
  IMPORTED_SONAME_NOCONFIG "libso101_kinematics.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS so101_kinematics::so101_kinematics )
list(APPEND _IMPORT_CHECK_FILES_FOR_so101_kinematics::so101_kinematics "${_IMPORT_PREFIX}/lib/libso101_kinematics.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
