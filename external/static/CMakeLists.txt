#[[ THIS LIBRARY should be provided as regular static library (two ways):
  *
  * 1. by installing to the system:
  *	   > library  =  place to system default libraries directory
  *    > .h		  =  place to system default headers directory
  *    
  * 2. by placing directly to the consumer project 
  *    and specifying paths to find both library and .h files
  *
  * THIS LIBRARY might be used as cmake importable target, see:
  * /project/external/CMakeLists.txt::[IMPORTED]
  * 
  * ]]
ADD_LIBRARY(static STATIC static.c)
TARGET_INCLUDE_DIRECTORIES(static INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# provide library file without installing
SET_TARGET_PROPERTIES(static PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out)

# provide header file without installing
FILE(COPY static.h DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/out)