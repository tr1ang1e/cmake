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

#[[ CHECK_SYMBOL_EXISTS
  * check symbol existance if given library would be potentially #include'd
  *   - to use symbol, library should be included explicitely
  *   - flag might be used in source macros, e.g.: #if defined(mv_HAVE_LOGARITHM) ... #endif
  *
  * modifying the way CHECK_SYMBOL_EXISTS works:
  *   - CMAKE_REQUIRED_LIBRARIES  =  searchin in linkable libraries ("m" is for "-lm" = link 'libm.so'/'libm.dll') but don't link them
  *
  * 

INCLUDE(CheckSymbolExists)
CHECK_SYMBOL_EXISTS(log "math.h" mv_HAVE_LOGARITHM)
IF(mv_HAVE_LOGARITHM)
  unset(mv_HAVE_LOGARITHM CACHE)
  set(CMAKE_REQUIRED_LIBRARIES "m")
  check_symbol_exists(log "math.h" mv_HAVE_LOGARITHM)
  IF(mv_HAVE_LOGARITHM)
    target_link_libraries(MathFunctions PRIVATE m)
  ENDIF()
ENDIF()

  * ]]

INSTALL(TARGETS logging DESTINATION ${mv_INSTALL_DIR})
INSTALL(FILES logging.h DESTINATION ${mv_INSTALL_DIR})