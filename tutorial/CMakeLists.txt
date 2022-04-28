#[[ LEGEND
  * 
  * [mv] = 'my variable'            : neither environmental nor reserved
  * [ev] = environmental variable   : affect cmake processing, set from the inside of CMakeLists or by environment default
  * [iv] = inner variable           : affect cmake processing, set from the inside of CMakeLists or by cmake default
  *
  * [on] = on						: point out corresponding option flag 
  *
  * ]]

CMAKE_MINIMUM_REQUIRED(VERSION 3.16.3)

PROJECT(
  TUTORIAL
  VERSION 4.0.0.0  # might be used while CONFIGURE_FILE()
)

#[[ RELATIVE PATHS inside CMAKE PROJECT TREE (in fact the paths are absolute)
  *
  * PROJECT_SOURCE_DIR				= directory of the veri first call of PROJECT() (set only once and never changes)
  *
  * CMAKE_CURRENT_SOURCE_DIR		= directory of CMakeLists.txt which is processing
  * CMAKE_CURRENT_BINARY_DIR		= directory of binary files of CMakeLists.txt which is processing (e.g. 'build')
  * CMAKE_SOURCE_DIR				= directory of the most parent of all CMakeLists.txt
  * CMAKE_BINARY_DIR				= directory of binary files of the most parent CMakeLists.txt 
  * 
  * ]]
SET(mv_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
SET(mv_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/libraries)
SET(mv_INSTALL_DIR _TUTORIAL)


# ************************** OPTIONS ************************* #

#[[ OPTION
  * turn option ON or OFF (only possible values)
  *   - might be used while CONFIGURE_FILE()
  *   - to set value through command line 
  *		> delete cache
  *     > cmake .. -D<OPTIONNAME>=<ON/OFF>
  *
  * [!] prompt is necessary for correct OPTION() working
  *
  * ]]
OPTION(mv_LOGGING_on "" OFF)


# ********************** CONFIGURATIONS ********************** #

#[[ CONFIGURE_FILE
  *
  * transform template of format 'name.extension.in'
  * to target file format 'name.extension'
  *
  * source directory = relative from current CMakeLists.txt folder
  * destination directory = relative from current 'build' folder
  * use absolute paths or specific inner variables (e.d CMAKE_CURRENT_SOURCE_DIR) to set different paths
  *
  * ]]
CONFIGURE_FILE(config.h.in ${mv_INCLUDE_DIR}/cmake_config.h)

#[[ CMAKE_<LANG>_STANDART = [iv]
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


# ********************* BUILD SUBPROJECT ********************* #

#[[ ADD_SUBDIRECTORY(<SUBPROJECT_TAKE_FROM>, <SUBPROJECT_BUILD_TO>)
  * immediately process CMakeLists.txt from the specified directory
  *   - child cmake process inherits all parent cmake process variables
  *   - child can declare new or override parent variables but it doesn't affect parent
  *   - to affect parent variables set(PARENT_SCOPE) should be called in child
  *
  * LIST(APPEND <LIST> <OBJECT>)
  * append <OBJECT> to the <LIST>
  *   - <LIST> is created at the first call of LIST(APPEND ...)
  *
  * ]]
 IF(mv_LOGGING_on)
	SET(mv_LOGGING_DIR ${mv_LIBRARIES}/static)
	ADD_SUBDIRECTORY(${mv_LOGGING_DIR})
	LIST(APPEND mv_EXTRA_LIBS logging)
 ENDIF()


# ******************** CREATE EXECUTABLE ******************* #

  ADD_EXECUTABLE(main main.c)
  TARGET_LINK_LIBRARIES(main PRIVATE ${mv_EXTRA_LIBS})
  TARGET_INCLUDE_DIRECTORIES(main PRIVATE 
	${mv_INCLUDE_DIR}
  )


# ************************ INSTALLING ********************** #

#[[ INSTALL(<WHAT> <WHERE>)
  * place specific unit to specific directory
  *		- <WHERE> is relative or absolute path
  *     - relative path is taken regarding CMAKE_INSTALL_PREFIX [iv]
  *     - to set path throug command line = cmake --install . prefix "new_prefix"
  * 
  * CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT
  * check if user set specific prefix or not
  *
  * SET
  *   - use CACHE to deal with cached variable   
  *	  - use FORCE to guarantee value would be cached
  *   - PATH "..." is for cmake-gui
  *
  * ]]
IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	SET(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "..." FORCE)
ENDIF()
INSTALL(TARGETS main DESTINATION ${mv_INSTALL_DIR})
INSTALL(FILES ${mv_INCLUDE_DIR}/cmake_config.h DESTINATION ${mv_INSTALL_DIR})


# ************************* TESTING ************************ #

ENABLE_TESTING()

ADD_TEST(NAME test_run COMMAND main)  # succeed if program starts and finishes correctly

IF(NOT mv_LOGGING_on)
	ADD_TEST(NAME test_version COMMAND main)
	SET_TESTS_PROPERTIES(test_version PROPERTIES 
		PASS_REGULAR_EXPRESSION "Project version ${TUTORIAL_VERSION_MAJOR}.${TUTORIAL_VERSION_MINOR}.${TUTORIAL_VERSION_PATCH}.${TUTORIAL_VERSION_TWEAK} "
	)
ENDIF()