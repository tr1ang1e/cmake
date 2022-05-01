#[[ ADD_LIBRARY
  * add target of library type (STATIC or SHARED or ...)
  *		- flag for default type is stored in BUILD_SHARED_LIBS 
  *     - default flag value = FALSE (so STATIC is default library type)
  *
  * SHARED library might include STATIC library as it's own part
  * it is the source of potential PIC (positional independent code) linker conflict 
  *		- shared : PIC 
  *		- static : no PIC
  * to solve: SET_TARGET_PROPERTIES(<staticlibname> PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
  *
  * ]]
ADD_LIBRARY(logging STATIC logging.c)

#[[ TARGET_INCLUDE_DIRECTORIES modern usage
  *
  * INTERFACE <DIRECTORY>			= <DIRECTORY> will not be included to CURRENT target / will be included to target, which LINK this library to 
  * ...
  *
  * ]]
TARGET_INCLUDE_DIRECTORIES(logging
	INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

#[[ PROPERTIES <WHAT>_OUTPUT_DIRECTORY
  * set the place which built files would be placed to
  *
  * target : <WHAT>
  *		- executable     : runtime 
  *     - library STATIC : archive
  *     - library SHARED : library (runtime for WIN32)
  *
  * ]]
SET_TARGET_PROPERTIES(logging
	PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

INSTALL(TARGETS logging DESTINATION ${mv_INSTALL_DIR})
INSTALL(FILES logging.h DESTINATION ${mv_INSTALL_DIR})