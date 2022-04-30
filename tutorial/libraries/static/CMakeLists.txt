ADD_LIBRARY(logging logging.c)

#[[ TARGET_INCLUDE_DIRECTORIES modern usage
  *
  * INTERFACE <DIRECTORY>			= <DIRECTORY> will not be included to CURRENT target / will be included to target, which LINK this library to 
  * ...
  *
  * ]]
TARGET_INCLUDE_DIRECTORIES(logging
	INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

INSTALL(TARGETS logging DESTINATION ${mv_INSTALL_DIR})
INSTALL(FILES logging.h DESTINATION ${mv_INSTALL_DIR})