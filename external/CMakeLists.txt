CMAKE_MINIMUM_REQUIRED(VERSION 3.15)

PROJECT(EXPORTABLE)

ADD_EXECUTABLE(main main.c)


# ****************** GENERATE STATIC LIBRARY ******************** #
# this part is separeted from following parts
ADD_SUBDIRECTORY(static)
TARGET_LINK_LIBRARIES(main PRIVATE static)


# ***************** GENERATE EXPORTABLE MODULE ****************** #


#[[ __0__ : GENERAL DESCRIPTION
  *
  * The main idea is to:
  *		> mark all desired targets as a part of particular export rule
  *     > according to this rule generate one module file (*.cmake)
  *			- if this file just generated and not installed  >>  it would know about dependent on current projects paths
  *			- if this file is installed  >>  it would be independent on current project and mignt be provided with libraries as itself
  *
  * The result file might be relocated and IMPORTED or INCLUDED to another projects,
  * and all of targets mentioned above would be available to it:
  *		> as it's own targets = IMPORTED
  *	    > as external sources = INCLUDED
  *
  * See source files tree in file explorer
  * The result (src = source directory, bin = default binary directory)
  *		after configure cache	
  *			__5__ : export	>>  src/exportable/out/exportable.cmake	(dependent of current project)
  *			__6__ : config	>>	src/exportable/out/exportable-config.cmake
  *			__6__ : version	>>  src/exportable/out/exportable-config-version.cmake
  *		after build:
  *			main.c		  >>  bin/main.out
  *			exportable.c  >>  src/exportable/out/exportable.a
  *			exportable.h  >>  src/exportable/exportable.h (doesn't change directory)
  *		after install:
  *			__3__ : install(TARGETS)  >>  CMAKE_INSTALL_PREFIX/exportable_libs/exportable.a
  *			__3__ : install(FILES)	  >>  CMAKE_INSTALL_PREFIX/exportable_libs/exportable.h
  *			__4__ : install(EXPORT)	  >>  CMAKE_INSTALL_PREFIX/exportable_libs/include/exportable.cmake
  *         __6__ : install(FILES)	  >>  CMAKE_INSTALL_PREFIX/exportable_libs/exportable-config.cmake + exportable-config-version.cmake
  *
  * ]]
SET(EXPORT_RULE_NAME cmake_exportable)
SET(EXPORT_MODULE_NAME exportable.cmake)
SET(EXPORT_BUILD_DIR ${CMAKE_SOURCE_DIR}/exportable/out)
SET(EXPORT_INSTALL_DIR exportable_libs)

# print service info
MESSAGE("                                                                                          ")
MESSAGE(":: ------------------------------------ SERVICE INFO ------------------------------------ ")
MESSAGE(":: CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT = ${CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT}")
MESSAGE(":: CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")
MESSAGE(":: -------------------------------- END OF SERVICE INFO --------------------------------- ")
MESSAGE("                                                                                          ")


#[[ __1__ : ADD USUAL MODULE
  *	
  * [!] DIFFERENT INCLUDE DIRECTORIES MUST BE SPECIFIED
  * 
  *		BUILD_INTERFACE will be used while building 
  *		so System-, CMake- and Project-dependent path might be used
  * 
  *		INSTALL_INTERFACE will be used while installing 
  *		exportable module (and doesn't affect regular installation),
  *		so must be used only 
  *			- universal (System-independent) and 
  *			- absolute path (CMake- and Project-independent) path
  *		To affect the path change CMAKE_INSTALL_PREFIX inner variable corresponding to the path prefix
  *     The default CMAKE_INSTALL_PREFIX value:
  *			- unix = /usr/local
  *         - windows = "C:/Program Files/${PROJECT_NAME}" 
  *
  * [!]
  * some IDEs (e.g. MicrosoftVisualStudio) might set their own CMAKE_INSTALL_PREFIX
  * using 'cmake --install . --prefix <path>'
  * 
  * ]]
ADD_LIBRARY(exportable ${CMAKE_CURRENT_SOURCE_DIR}/exportable/exportable.c)
TARGET_INCLUDE_DIRECTORIES(exportable INTERFACE 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/exportable>
	$<INSTALL_INTERFACE:exportable_libs/include>  # universal path = CMAKE_INSTALL_PREFIX/exportable_libs/include
)
SET_TARGET_PROPERTIES(exportable PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${EXPORT_BUILD_DIR})

#[[ __2__ : LINK MODULE TO CURRENT RPOJECT
  *
  * if neccessary to use this module in the current project
  * it must be linked as usual, because the same project
  * cannot be simultaneously:
  *	  - both producer
  *   - and consumer of *.cmake module
  *
  * ]]														
TARGET_LINK_LIBRARIES(main PRIVATE exportable)						

#[[ __3__ : CREATE EXPORT RULE FOR EXPORTABLE MODULES
  *
  *	TARGETS	= list of targets that would be collect into one exportable module
  *	
  * DESTINATION	 =  directory to place library file (also affected by CMAKE_INSTALL_PREFIX)
  *				    place given targets result files into specified directory
  *	if necessary in GNU/Linux
  *   > differ target by types, use  properties  LIBRARY DESTINATION / ARCHIVE DESTINATION / RUNTIME DESTINATION / INCLUDES DESTINATION
  *   > place targets by predefined ways, use    ${CMAKE_INSTALL_LIBDIR} / ${CMAKE_INSTALL_BINDIR} / ${CMAKE_INSTALL_INCLUDEDIR} ...
  * see https://cmake.org/cmake/help/latest/module/GNUInstallDirs.html
  * 
  * EXPORT	= do two things
  *			1. target is not installed directly nomore --> now the EXPORT RULE would be installed
  *			2. add target to EXPORT RULE with specific name
  *			   the same name might be used:
  *			     - as much times as necessary
  *			     - from as much CMakeLists' as necessary = ADD_SUBDIRECTORY(...) and go on
  *
  * [!]
  * do regular INSTALL(...) to place other necessary files (e.g. header files) to required places
  *
  * [!]
  * pay attention that project which will use exportable module will search it's headers 
  * by TARGET_INCLUDE_DIRECTORIES >> $<INSTALL_INTERFACE: ... > path. Scheme:
  *		> current project:		
  *			output = cmake_exportable.cmake 
  *			header path, declared by target_include_directories >> $<INSTALL_INTERFACE: ... > = <path>
  *		> another project:
  *			main.c = #include "exportable.h";
  *			include(cmake_exportable.cmake)
  *			target_link_libraries(... exportable ...)
  * after that compiler of another project will go by <path> in search of "exportable.h"
  * so current project must provide headar file to declared by itself path:
  *		> current project:
  *			install(FILES exportable.h DESTINATION <path>)
  *
  * ]]
SET(TARGETS_TO_EXPORT exportable)
INSTALL(
	TARGETS ${TARGETS_TO_EXPORT}
	DESTINATION ${EXPORT_INSTALL_DIR}  
	EXPORT ${EXPORT_RULE_NAME}			 
)
INSTALL(
	FILES ${CMAKE_CURRENT_SOURCE_DIR}/exportable/exportable.h
	DESTINATION ${EXPORT_INSTALL_DIR}/include
)

#[[ __4__ : GENERATE and INSTALL *.cmake EXPORTABLE FILE
  *
  * EXPORT			= name of installing rule (__3__)
  * FILE			= name of .cmake file that would be generated
  * DESTINATION		= directory to place generated *.cmake file (also affected by CMAKE_INSTALL_PREFIX)
  *
  * ]]
INSTALL(
	EXPORT ${EXPORT_RULE_NAME}				
	FILE ${EXPORT_MODULE_NAME}			 
	DESTINATION ${EXPORT_INSTALL_DIR}			
)

#[[ __5__ : GENERATE *.cmake EXPORTBLE FILE without installing 
  *
  * now the module is available without installing = directly from project build tree	
  * but TARGET_SOURCE_DIRECTORY is for  $<BUILD_INTERFACE: ... > and not relocatable
  *
  * ]]
EXPORT(
	EXPORT ${EXPORT_RULE_NAME}
	FILE ${EXPORT_BUILD_DIR}/${EXPORT_MODULE_NAME}
)


# ********************** CREATE PACKAGES ************************ #
# exportable module is already done and might be provided
# but following steps make our module users life easier
INCLUDE(CMakePackageConfigHelpers) # for CONFIGURE_PACKAGE_CONFIG_FILE() and WRITE_BASIC_PACKAGE_VERSION_FILE()

#[[ __6__ : GENERATE and INSTALL *-config.cmake SERVICE FILE
  *			special file for FIND_PACKAGE() to find portable module
  * 
  * config.cmake.in = file with information about path to .cmake module
  * see comments inside this file
  *
  * CONFIGURE_PACKAGE_CONFIG_FILE
  *		> input    =  path to config.cmake.template file (relative to CMAKE_CURRENT_SOURCE_DIR)
  *     > output   =  path to *-config.cmake result file (relative to CMAKE_CURRENT_BUILD_DIR)
  *     > install  =  path which was specified for *.cmake module file installing,  see __4__ : INTALL(... DESTINATION ...)
  *
  * WRITE_BASIC_PACKAGE_VERSION_FILE
  *		> output   =  path to *-config-version.cmake result file (relative to CMAKE_CURRENT_BUILD_DIR)
  *		> version  =  *.cmake module version (must be specified explicitely here or implicetely in PROJECT( ... VERSION ... ))
  *		> compatability  =  included module will be cosidered as compatible if:
  *			- AnyNewerVersion :: if it is newer or exactly the same as the requested in INCLUDE() version
  *			- ExactVersion	  :: if it is exactly the same (not cosidering TWEAK version)
  *			- ...
  *
  * INSTALL 
  *     - configured config file
  *		- configured version file
  *
  * ]]

SET(EXPORT_CONFIG_NAME exportable-config.cmake)
SET(EXPORT_VERSION_NAME exportable-config-version.cmake)
SET(EXPORT_VERSION 1.0.0)

CONFIGURE_PACKAGE_CONFIG_FILE(
	exportable/config.cmake.in
	${EXPORT_BUILD_DIR}/${EXPORT_CONFIG_NAME}
	INSTALL_DESTINATION ${EXPORT_INSTALL_DIR}
)

WRITE_BASIC_PACKAGE_VERSION_FILE(
	${EXPORT_BUILD_DIR}/${EXPORT_VERSION_NAME}
	VERSION ${EXPORT_VERSION}
	COMPATIBILITY AnyNewerVersion
)

INSTALL(FILES
	${EXPORT_BUILD_DIR}/${EXPORT_CONFIG_NAME}
	${EXPORT_BUILD_DIR}/${EXPORT_VERSION_NAME}
	DESTINATION ${EXPORT_INSTALL_DIR}
)


################################################################################################
#[[
  *

Typically, projects are built and installed before being used by an outside project

  ######################## CPACK (provide not entire, but already built and installed project)
  #### 1 way = downloading repo, build, intall << use .cmake from nother cmake projects
  #### 2 way = downloading cpack package, install << use .cmake from other cmake projects
  #### 3 way = downloading .cmake and .h files << ... 
  
  *
  * ]]