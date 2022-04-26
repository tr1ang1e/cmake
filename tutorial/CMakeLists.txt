#[[ LEGEND
  * 
	* mv = 'my variable'            : neither environmental nor reserved
	* ev = environmental variable   : affect cmake processing, set from the inside of CMakeLists or by environment default
  * iv = inner variable           : affect cmake processing, set from the inside of CMakeLists or by cmake default
	*
  * ]]

CMAKE_MINIMUM_REQUIRED(VERSION 3.16.3)

PROJECT (
  TUTORIAL
  VERSION 1.0.0.0
)

SET(mv_INCLUDE_DIR include)

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
CONFIGURE_FILE (version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/proj_version.h)

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

  ADD_EXECUTABLE(main main.c)
  TARGET_INCLUDE_DIRECTORIES(main PRIVATE ${mv_INCLUDE_DIR})