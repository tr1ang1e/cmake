cmake_minimum_required(VERSION 3.10)        # always start with this line

project(
  PROJECTNAME       # set the project name + place to {PROJECT_NAME}
  VERSION 1.0       # set the project version (major.minor) 
)                              

# spicify the C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

add_executable(PROJECTNAME main.c)          # add the executable

configure_file(ProjectConfig.in ProjectConfig.h)
#              ^                ^
# ....... take this file -to--> this 

# path for cmake to find include directories
target_include_directories(PROJECTNAME PUBLIC "${PROJECT_BINARY_DIR}") 




# FLOW
#
#   1. > create CMakeLists.txt                                  ~ touch CMakeLists.txt
#   2. > create dir for build                                   ~ mkdir <build_dir> && cd <build_dir>
#   3. > CG = configure + generate (path to CMakeLists.txt)     ~ cmake .. 
#   4. > BP = build project (path to created cmake metainfo     ~ cmake --build .

# EMBEDDED VARIABLES
# 
#   {PROJECT_BINARY_DIR}        = directory from (FLOW, 3.)

