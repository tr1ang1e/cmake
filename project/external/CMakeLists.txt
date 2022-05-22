#[[ LIBRARY DESCRIPTION
  * this subproject is created to show external libraries usage:
  *   IMPORTED() = library as itself
  *   INCLUDE() or FIND_PACKAGE() = via .cmake exportable module
  * exportable-1.0.0 directory is not a part of this project
  * read /cmake/README.txt to know why it is here
  * 
  * following 'external' library would be linked to main project
  * ]]
ADD_LIBRARY(external STATIC external.c)
TARGET_INCLUDE_DIRECTORIES(external INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
TARGET_LINK_LIBRARIES(external PRIVATE static)


#[[ IMPORTED (target option) = use external library as itself
  * IMPORTED (target property) = set to TRUE if IMPORTED option is used
  *
  * ]]
SET(mv_LIB ${CMAKE_CURRENT_SOURCE_DIR}/exportable-1.0.0/libs/lib)
SET(mv_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/exportable-1.0.0/libs/include)
ADD_LIBRARY(static STATIC IMPORTED)
SET_TARGET_PROPERTIES(static PROPERTIES IMPORTED_LOCATION  ${mv_LIB}/static.lib)
TARGET_INCLUDE_DIRECTORIES(static INTERFACE ${mv_INCLUDE})


#[[ INCLUDING =	use targets of extenal CMake project via .cmake exportable module
  * two variants:
  *		INCLUDE(<path_to_*.cmake_module>)
  *		FIND_PACKAGE( ... )
  *
  * FIND PACKAGE = see documentation for possible signatures
  *	searching rule (see documentation for more detailed rules)
  *		> for all elements of CMAKE_PREFIX_PATH			element = prefix
  *		> for all elements of system-defined-paths		element = path
  *     > try: <prefix>/<path>
  * to specify additional path use LIST(APPEND CMAKE_PREFIX_PATH <additional_path>)
  * 
  * ]]
LIST(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/exportable-1.0.0/libs)
FIND_PACKAGE(exportable 1.0.0 EXACT CONFIG)					# exportable is module name (external.cmake)
TARGET_LINK_LIBRARIES(external PRIVATE exportable)			# exportable is target inside module 