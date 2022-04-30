#[[ LEGEND
  * 
  * [mv] = 'my variable'            : neither environmental nor reserved
  * [ev] = environmental variable   : affect cmake processing, set from the inside of CMakeLists or by environment default
  * [iv] = inner variable           : affect cmake processing, set from the inside of CMakeLists or by cmake default
  *
  * [on] = on						: point out corresponding option flag 
  *
  * ]]

CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

PROJECT(
  TUTORIAL
  VERSION 9.0.0.0  # might be used while CONFIGURE_FILE()
)

#[[ RELATIVE PATHS inside CMAKE PROJECT TREE (in fact the paths are absolute)
  *
  * PROJECT_SOURCE_DIR				= directory of the very first call of PROJECT() (set only once and never changes)
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
SET(mv_GENERATED_REQ ${mv_INCLUDE_DIR}/last_compile.h)

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
OPTION(mv_LOGGING_on "" ON)


# ********************** CONFIGURATIONS ********************** #

#[[ CONFIGURE_FILE = cache-generating-time and compile-time
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

  ADD_EXECUTABLE(main main.c ${mv_GENERATED_REQ})
  TARGET_LINK_LIBRARIES(main PRIVATE ${mv_EXTRA_LIBS})
  TARGET_INCLUDE_DIRECTORIES(main PRIVATE 
	${mv_INCLUDE_DIR}
	${mv_EXTRA_INCLUDES}
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


# ********************* CUSTOM COMMAND ********************* #
 
#[[ ADD_CUSTOM_COMMAND = compile-time (not cache-generating-time)
  * 
  * OUPTUT file must be invoked into target definition 
  *		- e.g. ADD_EXECUTABLE(...) = explicitely, OK
  *     - but not TARGET_LINK_DIRECTORIES(...) = inmplicitely, not OK
  * otherwise COMMAND would be ingnored
  *  
  * the example of runnin python script is below
  * to run bash script in use: 
  *		- WIN32 = COMMAND cmd /c "script.bat"						# invoke command line
  *     - LINUX = COMMAND chmod a+x "sript.sh" && "script.sh"		# command line by default
  *
  * ]]
FIND_PACKAGE(PythonInterp 3.9 REQUIRED)
ADD_CUSTOM_COMMAND(
	OUTPUT ${mv_INCLUDE_DIR}/last_compile.h
	COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/last_compile.py ${mv_INCLUDE_DIR}/last_compile.h
	COMMENT "Generating timestamp"
)

#[[ CHECK_SYMBOL_EXISTS
  * check symbol existance if given library would be potentially #include'd
  *   - if relative path specified library is searched in defaut system catalog (e.g. "stdio.h")
  *   - if header doesn't exists an error would not appear - it jast would cause flag set to FALSE
  *   - to use symbol, library should be included explicitely
  *   - flag might be used in source macros
  *     > TARGET_COMPILE_DEFINITIONS( ... )
  *     > in source file = #ifdef ... #endif
  *
  * modifying the way CHECK_SYMBOL_EXISTS works:
  *   - CMAKE_REQUIRED_LIBRARIES  =  search in in linkable libraries but don't link them
  *     > e.g "m" is for "-lm" = link 'libm.so'/'libm.dll' 
  *     > after that any the lib might be specified in repeated CHECK_SYMBOL_EXISTS call
  *
  * [!] pay attention = header 'last_compile.h' is created by ADD_CUSTOM_COMMAND()
  *   - if file is epcent (e.g. was deleted) the flag would be set to FALSE while first cache generating
  *	  - too have flag ON necessary:
  *     > build all (will cause 'last_compile' target execution)
  *     > delete cache and reconfigure (because now file exists)
  *  the above example is synthetic but demonstrates CMake workflow well
  *  to create file while cache generating use CONFIGURE_FILE()   
  *
  *  the right way to get cmake cache generating timestamp (not compiling) is:
  *     > STRING(TIMESTAMP <VARIABLE> [<format_string>] [UTC])
  *     > TARGET_COMPILE_DEFINITION( ... ) or ADD_DEFINITIONS(-DVARIABLE="${VARIABLE}")
  *
  * ]] 
INCLUDE(CheckSymbolExists)
CHECK_SYMBOL_EXISTS(_LC_ "${mv_INCLUDE_DIR}/last_compile.h" mv_LAST_COMPILE)
IF(NOT mv_LAST_COMPILE)
	# unset(mv_LAST_COMPILE CACHE)
    # set(CMAKE_REQUIRED_LIBRARIES "m")
    # check_symbol_exists(LAST_COMPILE_TIME "any.h" mv_LAST_COMPILE)
	# IF(mv_LAST_COMPILE)
	#	  target_linklibraries(main PRIVATE m)
	# ENDIF()
ELSE()
	TARGET_COMPILE_DEFINITIONS(main PRIVATE "mv_LAST_COMPILE")
	LIST(APPEND mv_EXTRA_INCLUDES ${mv_INCLUDE_DIR}/last_compile.h)
ENDIF()


# *********************** PACKAGING ************************ #

# include all of required to the project system libraries into package
INCLUDE(InstallRequiredSystemLibraries)

# give information about package
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/license.txt")
SET(CPACK_PACKAGE_VERSION_MAJOR "${Tutorial_VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${Tutorial_VERSION_MINOR}")

# choose packaging generator
SET(CPACK_SOURCE_GENERATOR "TGZ")

#[[ CPACK 
  * inner cmake utility to create packages
  * might require system utilities, e.g.
  *		- WIN32 = NSIS (Nullsoft Scriptable Install System)
  *     - LINUX = ...
  * 
  * [!] pay attention CMake might be delivered as a part of IDE (e.g. Visual Studio)
  *     so CPack (as well as CMake as intself) doesn't available from th command line
  *     (e.g. for Viausl Studio use embedded command line: Tools - Command Line - Developer PowerShell)
  *
  * to pack project
  *		- configure cache 
  *		- build project
  *     - install project
  *     - run cpack ( do 'cmake'  from CMAKE_CURRENT_BINARY_DIR )
  *					( result would appear in binary dir as well )
  *
  * ]]
INCLUDE(CPack)