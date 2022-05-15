
#[[ IMPORTED (target option) =
  * both library and .h files are copied from external source
  * see /external/static/out
  *
  * IMPORTED (target property) = set to TRUE if IMPORTED option is used
  *
  * ]]
ADD_LIBRARY(static STATIC IMPORTED)
SET_TARGET_PROPERTIES(static PROPERTIES IMPORTED_LOCATION  ${CMAKE_CURRENT_SOURCE_DIR}/static/static.lib)
TARGET_INCLUDE_DIRECTORIES(static INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/static)

#[[ INCLUDING =	
  * 
  * 
  *
  *
  * ]]

  # INCLUDE("C:/Users/tr1an/Projects/cmake/exportable/out/install/x64-Debug/exportable_libs/cmake_exportable.cmake")
  # TARGET_LINK_LIBRARIES(main PRIVATE exportable)


# library which would be linked to the top CMakeLists.txt main target
ADD_LIBRARY(external STATIC external.c)
TARGET_INCLUDE_DIRECTORIES(external INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
TARGET_LINK_LIBRARIES(external PRIVATE static)