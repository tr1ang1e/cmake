#[[ LEGEND
  * 
	* MV = my variable = not environmental, not reserved
	*
	*
  * ]]
cmake_minimum_required(VERSION 3.8)

project(VA70xxREGS)

#[[ CMAKE_BUILD_TYPES = environmental variable
  *   - declaration  :  implicitely  =  while first project() or enable_language() is calling
	*   - definition   :  explicitely / implicitely (toolchain-specific default value) 
	*
	* CMAKE_C_FLAGS = inner variable
	*   - declaration  :  explicitely 
  *   - definition   :  explicitely / implicitely (by CFLAGS environmental variable value)
	*                     if defined explicitely, CFLAGS would be ignored

  * ]]
IF("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g3")
ELSEIF("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -g1")
ELSE()
	SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g3")
	MESSAGE (" :: CMAKE_BUILD_TYPE environmental variable of unknown type")
ENDIF()

#[[ CMAKE_<TYPE>_LINKER_FLAGS = inner variable
	*   - declaration  :  explicitely 
  *   - definition   :  explicitely
  *
	* LINKER OPTIONS
	*   - Wl,L=...								tell COMPILE TIME LINKER where find library in COMPILE TIME to
	*   - Wl,rpath=...						tell RUNTIME LINKER (LOADER) where find needed library to ($ORIGIN expands in RUNTIME)
	*   - Wl,--as-needed				  default = DT_NEEDED tag set for a library that satisfies an undefined symbol reference from a regular object file
	*        --no-as-needed				cancel default behavior = DT_NEEDED tag for all of mentioned libraries
	*
	* ]]
SET(CMAKE_EXE_LINKER_FLAGS "-Wl,--no-as-needed -ldl -lpthread")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath=$ORIGIN")
SET(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-as-needed -ldl -lpthread")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-rpath=$ORIGIN")

#[[ CMAKE_<LANG>_STANDART = inner variable
  * spicify the default value of C standard required for all of following targets
	*   - declaration  :  explicitely 
  *   - definition   :  explicitely
	*
	* variable value affects <LANG>_STANDART property
	* the way to set not default but specific value is direct:
	*   set_property(TARGET <TARGET> PROPERTY C_STANDARD 11) 
	*
  * ]]
SET(CMAKE_C_STANDARD 99)
SET(CMAKE_C_STANDARD_REQUIRED TRUE)

SET(MV_VA70xxREGS_DIR ".")

#[[ CREATE SOURCE VARIABLE 
	*   - the variable will be used after for creating target
	*   - both file types (.c and .h) are allowed
	*   - cmake automatically finds entry point for executable
	*
	* ]]
SET(MV_EXE_SRC
	${MV_VA70xxREGS_DIR}/main.c
)
SET(MY_SO_SRC
	${MV_VA70xxREGS_DIR}/rb_tree.c
	${MV_VA70xxREGS_DIR}/va70xx_regs.c
)


# ******************* CREATE SHARED LIBRARY ****************** #

ADD_LIBRARY(regs SHARED ${MY_SO_SRC})
SET_TARGET_PROPERTIES(regs PROPERTIES PREFIX "lib")

#[[ TARGET_INCLUDE_DIRECTORIES
  *   - specifies include directories to use when compiling the target
	*       no need anymore to write full path in project files
	*       "../include/public/header.h" -->-- "header.h"
	*   - affects on INCLUDE_DIRECTORIES property
	*		    might be directly set by set_property(TARGET <TARGET> PROPERTY INCLUDE_DIRECTORIES <PATH>)
	*       if directly, <PATH> must be ABSOLUTE, not relative
	*
  * ]]
TARGET_INCLUDE_DIRECTORIES(regs PRIVATE ${MV_VA70xxREGS_DIR}/include) 


# ******************** CREATE EXECUTABLE ******************* #

ADD_EXECUTABLE(main ${MV_EXE_SRC})
TARGET_INCLUDE_DIRECTORIES(main PRIVATE ${MV_VA70xxREGS_DIR}/include)
# TARGET_LINK_LIBRARIES(main regs)


# ********************* BUILD SUBPROJECT ********************* #

#[[ ADD_SUBDIRECTORY(<SUBPROJECT_TAKE_FROM>, <SUBPROJECT_BUILD_TO>)
  * immediately process CMakeLists.txt from the specified directory
	*   - child cmake process inherits all parent cmake process variables
	*   - child can declare new or override parent variables but it doesn't affect parent
	*   - to affect parent variables set(PARENT_SCOPE) should be called in chiled
	*
  * ]]
ADD_SUBDIRECTORY(inner inner_build)