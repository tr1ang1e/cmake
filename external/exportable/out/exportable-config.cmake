# nesessary string for cmake to replace by service code
# must be the first executable string (excluding empty strings and comments)

####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was config.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

# relative path:
#  > from = *-config.cmake service file
#  > to   = *.cmake module file
INCLUDE ( "${CMAKE_CURRENT_LIST_DIR}/exportable.cmake" )

# check targets or other required components are provided by *.cmake module
# must be here even if no components are provided
# if at least one of components isn't provided, <PACKAGE>_FOUND variable is set to FALSE
# and the package is considered to be not found
CHECK_REQUIRED_COMPONENTS(exportable)
