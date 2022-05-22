CMAKE_MINIMUM_REQUIRED(VERSION 3.15)
PROJECT(exportable VERSION 1.0.0)
ADD_EXECUTABLE(main main.c)  # just for more detailed CMakeLists.txt example = unnecessary for main purpose of project


#[[ INTRODUCTION
  * Ways to provide libraries were built by CMake project:
  *
  *								consumer downloads
  * 1 = propriate library		: lib + header
  * 2 = open-source library     : lib + header + src
  * 3 = .cmake module			: lib + header + .cmake module 
  * 1, 2, 3						: might be provided as CPack installer
  * 4 = open-source project		: project source code >> build it (and install if necessary and possible)
  *
  * ]]


# ****************** GENERATE STATIC LIBRARY ******************** #
# this part is separeted from following parts
ADD_SUBDIRECTORY(static)
TARGET_LINK_LIBRARIES(main PRIVATE static)


# ***************** GENERATE EXPORTABLE MODULE ****************** #
# exportable module is just convinitn way to provide library for using in another cmake project
# in both cases: release library with or without .cmake module file >> lib and header files must be provided to consumer
# the different things for .cmake approach are:
#	> not library but .cmake file is used by consumer in CMakeLits.txt
#   > consumer's CMakeLists.txt must know only about .cmake file location (and .cmake file in it's turn knows about other locations) 

#[[ __0__ : FLOW DESCRIPTION
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
  *		after configure cache: .................................................................................
  *			__5__ : export	>>  src/exportable/out/exportable.cmake	(dependent of current project)             i
  *			__6__ : config	>>	src/exportable/out/exportable-config.cmake                                     i > connected by relative paths >> provided only as project source code, then built
  *			__6__ : version	>>  src/exportable/out/exportable-config-version.cmake                             i                      
  *		after build: ...........................................................................................                                                                                                               
  *			main.c		  >>  bin/main.out																				            
  *			exportable.c  >>  src/exportable/out/exportable.a							
  *			exportable.h  >>  src/exportable/exportable.h (doesn't change directory)	
  *		after install: ........................................................................................i > using absolute paths >> might be provided without project itself (CPACK, archive)
  *			__3__ : install(TARGETS)  >>  CMAKE_INSTALL_PREFIX/libs/exportable.a
  *			__3__ : install(FILES)	  >>  CMAKE_INSTALL_PREFIX/libs/exportable.h
  *			__4__ : install(EXPORT)	  >>  CMAKE_INSTALL_PREFIX/libs/include/exportable.cmake
  *         __6__ : install(FILES)	  >>  CMAKE_INSTALL_PREFIX/libs/exportable-config.cmake + exportable-config-version.cmake
  *
  * ]]
SET(EXPORT_RULE_NAME cmake_exportable)
SET(EXPORT_MODULE_NAME exportable.cmake)
SET(EXPORT_BUILD_DIR ${CMAKE_SOURCE_DIR}/out/build/exportable)
SET(EXPORT_INSTALL_DIR libs)

# print service info
MESSAGE("                                                                                          ")
MESSAGE(":: ------------------------------------ SERVICE INFO ------------------------------------ ")
MESSAGE("																						   ")
MESSAGE(":: CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT = ${CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT}")   # if sefault, set to TRUE, otherwise empty
MESSAGE(":: CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")
MESSAGE("																						   ")
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
  *		Forbidden:
  *			- use even absolute paths to the files which would not be included into provided package
  *			- use $<INSTALL_INTERFACE>:CMAKE_INSTALL_PREFIX/... = because it is duplication (path is already prefixed by CMAKE_INSTALL_PREFIX)
  *			      > instead of this use $<INSTALL_INTERFACE>:$<INSTALL_PREFIX>/ if necessary (read documentation)
  *
  * [!]
  * some IDEs (e.g. MicrosoftVisualStudio) might set their own CMAKE_INSTALL_PREFIX
  * use [$ cmake --install . --prefix <path>] terminal command to set CMAKE_INSTALL_PREFIX
  * if using installation from CPACK pakage ... 
  * 
  * ]]
ADD_LIBRARY(exportable ${CMAKE_CURRENT_SOURCE_DIR}/exportable/exportable.c)
TARGET_INCLUDE_DIRECTORIES(exportable INTERFACE 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/exportable>
	$<INSTALL_INTERFACE:libs/include>  # universal path = CMAKE_INSTALL_PREFIX/libs/include
)
SET_TARGET_PROPERTIES(exportable PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${EXPORT_BUILD_DIR})
FILE(COPY exportable/exportable.h DESTINATION ${EXPORT_BUILD_DIR})

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
	DESTINATION ${EXPORT_INSTALL_DIR}/lib  
	EXPORT ${EXPORT_RULE_NAME}			 
)
INSTALL(
	FILES ${CMAKE_CURRENT_SOURCE_DIR}/exportable/exportable.h
	DESTINATION ${EXPORT_INSTALL_DIR}/include
)

#[[ __4__ : GENERATE and INSTALL *.cmake EXPORTABLE FILE
  *
  * EXPORT			= name of installing rule ( __3__ )
  * FILE			= name of .cmake file that would be generated
  * DESTINATION		= directory to place generated *.cmake file (also affected by CMAKE_INSTALL_PREFIX)
  *
  * ]]
INSTALL(
	EXPORT ${EXPORT_RULE_NAME}				
	FILE ${EXPORT_MODULE_NAME}			 
	DESTINATION ${EXPORT_INSTALL_DIR}/cmake			
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


# ************* CREATE PACKAGE FOR FIND_PACKAGE() *************** #
# exportable module is already done and might be provided
# but following steps make our module users life easier
INCLUDE(CMakePackageConfigHelpers) # for CONFIGURE_PACKAGE_CONFIG_FILE() and WRITE_BASIC_PACKAGE_VERSION_FILE()
# *************************************************************** #

#[[ __6__ : GENERATE and INSTALL *-config.cmake SERVICE FILE
  *			special file for FIND_PACKAGE() to find portable module
  * 
  * config.cmake.in = file with information about path to .cmake module
  * see comments inside this file
  *
  * WRITE_BASIC_PACKAGE_VERSION_FILE
  *		> output   =  path to *-config-version.cmake result file (relative to CMAKE_CURRENT_BUILD_DIR)
  *		> version  =  *.cmake module version (must be specified explicitely here or implicetely in PROJECT( ... VERSION ... ))
  *		> compatability  =  included module will be cosidered as compatible if:
  *			- AnyNewerVersion :: if it is newer or exactly the same as the requested in INCLUDE() version
  *			- ExactVersion	  :: if it is exactly the same (not cosidering TWEAK version)
  *			- ...
  *
  * CONFIGURE_PACKAGE_CONFIG_FILE
  *		> input    =  path to config.cmake.template file (relative to CMAKE_CURRENT_SOURCE_DIR)
  *     > output   =  path to *-config.cmake result file (relative to CMAKE_CURRENT_BUILD_DIR)
  *     > install  =  path which was specified for *.cmake module file installing,  see __4__ : INTALL(... DESTINATION ...)
  *
  * INSTALL 
  *		- configured version file
  *     - configured config file 
  *			pay attention that path from *-config.cmake to *.cmake is specified inside "config.cmake.in"
  *			so config file must know about where module is located by relative path:
  *			*-config.cmake file and *.cmake module ARE PROVIDED TOGETHER 
  *
  * ]]

SET(EXPORT_CONFIG_NAME exportable-config.cmake)
SET(EXPORT_VERSION_NAME exportable-config-version.cmake)
SET(EXPORT_VERSION 1.0.0)

WRITE_BASIC_PACKAGE_VERSION_FILE(
	${EXPORT_BUILD_DIR}/${EXPORT_VERSION_NAME}
	VERSION ${EXPORT_VERSION}
	COMPATIBILITY AnyNewerVersion
)

CONFIGURE_PACKAGE_CONFIG_FILE(
	exportable/config.cmake.in
	${EXPORT_BUILD_DIR}/${EXPORT_CONFIG_NAME}
	INSTALL_DESTINATION ${EXPORT_INSTALL_DIR}/cmake
)

INSTALL(FILES
	${EXPORT_BUILD_DIR}/${EXPORT_CONFIG_NAME}
	${EXPORT_BUILD_DIR}/${EXPORT_VERSION_NAME}
	DESTINATION ${EXPORT_INSTALL_DIR}/cmake
)


# ************************ USING CPACK ************************** #

#[[ __7__ : CPACK 
  * inner cmake utility to create packages
  * might require system utilities, e.g.
  *	  - WIN32 = NSIS (Nullsoft Scriptable Install System)
  *   - LINUX = ...
  * 
  * [!] pay attention CMake might be delivered as a part of IDE (e.g. Visual Studio)
  *     so CPack (as well as CMake as intself) doesn't available from the command line
  *     (e.g. for Viausl Studio use embedded command line: Tools - Command Line - Developer PowerShell)
  *
  * [!] INCLUDE(CPack) must be done after all settings are done
  *     otherwise problems occurs (e.g. ignoring license file)
  *
  * to pack project
  *	  - configure cache 
  *	  - build project
  *   - install project
  *   - run cpack ( do 'cpack'  from CMAKE_CURRENT_BINARY_DIR )
  *		          ( result would appear in binary dir as well )
  * 
  * to installable package would be added all files:
  *   > which INSTALL(...) function was called for
  *   > by their paths related to CMAKE_INSTALL_PREFIX
  *		  - prefix as itself would not be included
  *       - prefix would be chosen while run installator 
  *
  * ]]


# include all of required to the project system libraries into package
INCLUDE(InstallRequiredSystemLibraries)

# give information about package
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/license.txt")
SET(CPACK_PACKAGE_VERSION_MAJOR "${exportable_VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${exportable_VERSION_MINOR}")

# choose packaging generator
SET(CPACK_SOURCE_GENERATOR "TGZ")

# include CPack after all preparations
INCLUDE(CPack)