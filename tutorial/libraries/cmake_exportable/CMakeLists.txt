ADD_LIBRARY(exportable exportable.c)
TARGET_LINK_LIBRARIES(exportable PRIVATE proj_compile_flags)
SET_TARGET_PROPERTIES(exportable PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

#[[ BUILD_INTERFACE will be used while building 
  * so System-, CMake- and Project-dependent path might be used
  * 
  * INSTALL_INTERFACE will be used while installing 
  * exportable module (and doesn't affect regular installation),
  * so must be used only 
  *		- universal (System-independent) and 
  *     - absolute path (CMake- and Project-independent) path
  * To affect the path change CMAKE_INSTALL_PREFIX inner variable in the top CMakeLists.txt
  *
  * ]]
TARGET_INCLUDE_DIRECTORIES(exportable INTERFACE 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	$<INSTALL_INTERFACE:${mv_INSTALL_DIR}>
)

#[[ INSTALL( ... EXPORT <export_name> ... )
  * modify installing to have possibility to generate <export_name>.cmake file
  * necessary to add corrresponding modifications to the top CMakeLists.txt
  *
  *	DESTINATION is also affected by CMAKE_INSTALL_PREFIX (like as INSTALL_INTERFACE)
  *
  * ]]
SET(target_libs exportable proj_compile_flags)
INSTALL(
	TARGETS ${target_libs}
	EXPORT cmake_exportable			 # create specific name to link it from the top CMakeLists.txt
	DESTINATION exportable_libs		 # directory to place library file 
)

#[[ regular installation if necessary
  *
  * INSTALL(TARGETS exportable DESTINATION ${mv_INSTALL_DIR})
  * INSTALL(FILES exportable.h DESTINATION ${mv_INSTALL_DIR})
  *
  * ]]