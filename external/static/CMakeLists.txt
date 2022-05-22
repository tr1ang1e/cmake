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
SET(STATIC_BUILD_DIR ${CMAKE_SOURCE_DIR}/out/build/static)

# provide library and header files without installing (place to convinient directory)
SET_TARGET_PROPERTIES(static PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${STATIC_BUILD_DIR})
FILE(COPY static.h DESTINATION ${STATIC_BUILD_DIR})

# install to provide with CPack install package (see top CMakeLists.txt:__7__:CPack)
# pay attention to the difference between this approach and creating relocatable .cmake module:
#	> this library : all paths, include directories etc. should be specified manually in a project-consumer
#   > cmake module : all paths, include directories etc. are known by module
INSTALL(TARGETS static DESTINATION libs/lib)
INSTALL(FILES static.h DESTINATION libs/include)