cmake_minimum_required(VERSION 3.8)

project(APhySdk)

IF(WIN32)
	set(CMAKE_C_FLAGS "/W3")
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
ELSE()
	set(CMAKE_C_FLAGS "-Wall")
ENDIF(WIN32)

IF("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	IF(WIN32)		
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Od")		
	ELSE()
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g")		
	ENDIF()
	
ELSEIF("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	IF(WIN32)
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
	ELSE()
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -g1")
ENDIF()	
	
ELSE()
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g")
	message("Undefined build type... Debug options will be used")	
ENDIF()

IF(WIN32)
	message("Using the default linker options on Windows")
ELSE()
	set(CMAKE_EXE_LINKER_FLAGS "-Wl,--no-as-needed -ldl -lpthread")
	set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath=$ORIGIN")
	set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-as-needed -ldl -lpthread")
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-rpath=$ORIGIN")
ENDIF()

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

set(VALENS_APHY_SRC_DIR "valens_aphy/src")
set(VALENS_APHY_PUBLIC_INCLUDE_DIR "valens_aphy/include/public")
set(VALENS_APHY_LIBS_SRC_DIR "libs")
set(VALENS_APHY_LIBS_INCLUDE_DIR "libs")

set(VALENS_APHY_PUBLIC_INCLUDE_DIRS
	${VALENS_APHY_PUBLIC_INCLUDE_DIR}
	${VALENS_APHY_LIBS_INCLUDE_DIR}
	)

set(VALENS_APHY_PRIVATE_INCLUDE_DIR "valens_aphy/include/private")

set(VALENS_APHY_PRIVATE_INCLUDE_DIRS
	${VALENS_APHY_PRIVATE_INCLUDE_DIR}
	)


set(VALENS_APHY_PRIVATE_HEADERS
	${VALENS_APHY_LIBS_INCLUDE_DIR}/totalphase/aardvark.h	
	)

set(VALENS_APHY_PUBLIC_HEADERS
	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/autogen_stamp.h
	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/aphy_sdk_version.h	
	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/valens_aphy.h	

	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/common/rif.h	
	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/common/aphy_types.h
	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/common/utils.h
	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/common/thread_utils.h
	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/common/containers.h
	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/common/logging.h
	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/common/log_writer.h
	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/common/fwlog.h
	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/common/enums.h	
	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/common/aphy_settings.h

	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/direct/acmp_core.h
	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/direct/native_adapters.h	
	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/direct/aphy_native.h

	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/flash/va7031_eeprom_defines.h
	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/flash/aphy_va7031_eeprom.h
	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/flash/va7031_acmp_eeprom.h
	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/flash/aphy_va7044_fifo.h
	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/flash/aphy_fifo_items.h
	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/flash/aphy_programmer.h
	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/flash/flash_utils.h
	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/flash/vcp.h	

	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/app/core/app_msg.h
	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/app/core/app_transport.h 
	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/app/core/app_transport_types.h	
	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/app/core/aphy_defines.h
	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/app/core/notifications.h
	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/app/client/aphy_client.h
	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/app/client/aphy_client_sync.h
	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/app/client/aphy_client_sync_types.h	
	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/app/client/aphy_client_bist.h
	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/app/api/app_commands.h
	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/app/api/general_api.h
	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/app/api/csi_api.h
	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/app/api/fusa_api.h
	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/app/api/i2c_api.h
	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/app/api/link_api.h	
	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/app/api/rclk_api.h	
	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/app/api/msio_api.h
	${VALENS_APHY_PUBLIC_INCLUDE_DIR}/app/api/bist_api.h	
	)

set(valens_aphy_src
	${VALENS_APHY_PUBLIC_HEADERS}

	${VALENS_APHY_PRIVATE_HEADERS}

	${VALENS_APHY_SRC_DIR}/valens_aphy.c	

	${VALENS_APHY_SRC_DIR}/common/utils.c
	${VALENS_APHY_SRC_DIR}/common/thread_utils.c	
	${VALENS_APHY_SRC_DIR}/common/containers.c
	${VALENS_APHY_SRC_DIR}/common/logging.c
	${VALENS_APHY_SRC_DIR}/common/log_writer.c
	${VALENS_APHY_SRC_DIR}/common/fwlog.c
	${VALENS_APHY_SRC_DIR}/common/rif.c	
	${VALENS_APHY_SRC_DIR}/common/aphy_settings.c

	${VALENS_APHY_SRC_DIR}/direct/acmp_core.c		
	${VALENS_APHY_SRC_DIR}/direct/native_adapters.c	
	${VALENS_APHY_SRC_DIR}/direct/aphy_native.c
	
	${VALENS_APHY_SRC_DIR}/flash/aphy_va7031_eeprom.c
	${VALENS_APHY_SRC_DIR}/flash/va7031_acmp_eeprom.c
	${VALENS_APHY_SRC_DIR}/flash/aphy_va7044_fifo.c
	${VALENS_APHY_SRC_DIR}/flash/aphy_programmer.c
	${VALENS_APHY_SRC_DIR}/flash/flash_utils.c
	${VALENS_APHY_SRC_DIR}/flash/vcp.c	

	${VALENS_APHY_SRC_DIR}/app/core/app_msg.c
    ${VALENS_APHY_SRC_DIR}/app/core/app_transport.c
	${VALENS_APHY_SRC_DIR}/app/core/aphy_defines.c
	${VALENS_APHY_SRC_DIR}/app/core/notifications.c
	${VALENS_APHY_SRC_DIR}/app/client/aphy_client.c
	${VALENS_APHY_SRC_DIR}/app/client/aphy_client_sync.c
	${VALENS_APHY_SRC_DIR}/app/client/aphy_client_sync_types.c	
	${VALENS_APHY_SRC_DIR}/app/client/aphy_client_bist.c
	${VALENS_APHY_SRC_DIR}/app/api/general_api.c
	${VALENS_APHY_SRC_DIR}/app/api/app_commands.c
	${VALENS_APHY_SRC_DIR}/app/api/csi_api.c
	${VALENS_APHY_SRC_DIR}/app/api/fusa_api.c
	${VALENS_APHY_SRC_DIR}/app/api/i2c_api.c
	${VALENS_APHY_SRC_DIR}/app/api/link_api.c	
	${VALENS_APHY_SRC_DIR}/app/api/rclk_api.c	
	${VALENS_APHY_SRC_DIR}/app/api/msio_api.c
	${VALENS_APHY_SRC_DIR}/app/api/bist_api.c

	${VALENS_APHY_LIBS_SRC_DIR}/totalphase/aardvark.c
	)

IF(${SKIP_AUTOGEN})
	message(STATUS "Autogeneration is disabled (-DSKIP_AUTOGEN=${SKIP_AUTOGEN})")
ELSE()
	set(AUTOGEN_STAMP_FILE ${CMAKE_CURRENT_SOURCE_DIR}/valens_aphy/include/public/autogen_stamp.h)
	IF(WIN32)		
		add_custom_command(
			OUTPUT ${AUTOGEN_STAMP_FILE}
			COMMAND cmd /c "\"${CMAKE_CURRENT_SOURCE_DIR}/valens_aphy/autogen.bat\""
			COMMENT "Autogenerating file \"${AUTOGEN_STAMP_FILE}\""
			)		
	ELSE()
		add_custom_command(
			OUTPUT ${AUTOGEN_STAMP_FILE}
			COMMAND chmod a+x "\"${CMAKE_CURRENT_SOURCE_DIR}/valens_aphy/autogen.sh\"" && "\"${CMAKE_CURRENT_SOURCE_DIR}/valens_aphy/autogen.sh\""
			COMMENT "Autogenerating file \"${AUTOGEN_STAMP_FILE}\""
			)
	ENDIF(WIN32)
ENDIF()

set(APHY_HOST_INCLUDE_DIR	"aphy_host")

set(APHY_HOST_INCLUDE_DIRS
	${APHY_HOST_INCLUDE_DIR}
	${VALENS_APHY_PUBLIC_INCLUDE_DIR}
	)

set(aphy_host_src	
	${APHY_HOST_INCLUDE_DIR}/host_command.h
	${APHY_HOST_INCLUDE_DIR}/host_prog_commands.h	
	${APHY_HOST_INCLUDE_DIR}/i2c_mapping_types.h

	${APHY_HOST_INCLUDE_DIR}/commands/command_app_client.h
	${APHY_HOST_INCLUDE_DIR}/commands/command_deviceinfo.h
	${APHY_HOST_INCLUDE_DIR}/commands/command_eeprom_prog.h
	${APHY_HOST_INCLUDE_DIR}/commands/command_emc.h
	${APHY_HOST_INCLUDE_DIR}/commands/command_flash_iram_prog.h
	${APHY_HOST_INCLUDE_DIR}/commands/command_fwlogs.h
	${APHY_HOST_INCLUDE_DIR}/commands/command_ports.h
	${APHY_HOST_INCLUDE_DIR}/commands/command_mapping_table.h
	${APHY_HOST_INCLUDE_DIR}/commands/command_raw.h
	${APHY_HOST_INCLUDE_DIR}/commands/command_read_write.h
	${APHY_HOST_INCLUDE_DIR}/commands/command_scan.h

	${APHY_HOST_INCLUDE_DIR}/app_client/msio_menu.h
	${APHY_HOST_INCLUDE_DIR}/app_client/csi_menu.h
	${APHY_HOST_INCLUDE_DIR}/app_client/bist_menu.h
	${APHY_HOST_INCLUDE_DIR}/app_client/bist_display.h
	${APHY_HOST_INCLUDE_DIR}/app_client/fusa_menu.h
	${APHY_HOST_INCLUDE_DIR}/app_client/rclk_menu.h
	${APHY_HOST_INCLUDE_DIR}/app_client/i2c_menu.h
	${APHY_HOST_INCLUDE_DIR}/app_client/link_menu.h
	${APHY_HOST_INCLUDE_DIR}/app_client/system_menu.h
	${APHY_HOST_INCLUDE_DIR}/app_client/app_client_utils.h
	${APHY_HOST_INCLUDE_DIR}/app_client/tui.h

	${APHY_HOST_INCLUDE_DIR}/terminal.h
	${APHY_HOST_INCLUDE_DIR}/emc_blackbox/bbox_defines.h
	${APHY_HOST_INCLUDE_DIR}/emc_blackbox/bbox_sys_types.h	
	${APHY_HOST_INCLUDE_DIR}/emc_blackbox/bbox_system.h	
	${APHY_HOST_INCLUDE_DIR}/emc_blackbox/gpio_chip.h


  aphy_host/main.c	
	aphy_host/host_command.c	
	aphy_host/host_prog_commands.c	

	aphy_host/commands/command_app_client.c
	aphy_host/commands/command_deviceinfo.c
	aphy_host/commands/command_eeprom_prog.c
	aphy_host/commands/command_emc.c
	aphy_host/commands/command_flash_iram_prog.c
	aphy_host/commands/command_fwlogs.c
	aphy_host/commands/command_ports.c
	aphy_host/commands/command_mapping_table.c
	aphy_host/commands/command_raw.c
	aphy_host/commands/command_read_write.c
	aphy_host/commands/command_scan.c

	aphy_host/app_client/msio_menu.c
	aphy_host/app_client/csi_menu.c
	aphy_host/app_client/bist_menu.c
	aphy_host/app_client/bist_display.c
	aphy_host/app_client/fusa_menu.c
	aphy_host/app_client/rclk_menu.c
	aphy_host/app_client/i2c_menu.c
	aphy_host/app_client/link_menu.c
	aphy_host/app_client/system_menu.c
	aphy_host/app_client/app_client_utils.c
	aphy_host/app_client/tui.c

	aphy_host/terminal.c	
	aphy_host/emc_blackbox/bbox_sys_types.c
	aphy_host/emc_blackbox/bbox_system.c	
	aphy_host/emc_blackbox/gpio_chip.c

	)


file(COPY aphy_host/bist-configs DESTINATION ${CMAKE_BINARY_DIR})
file(COPY aphy_host/deploy-emc.sh DESTINATION ${CMAKE_BINARY_DIR})
file(COPY aphy_host/aphy-host-emc.service DESTINATION ${CMAKE_BINARY_DIR})


if (WIN32)
	file(COPY libs/totalphase/x86 DESTINATION ${CMAKE_BINARY_DIR})
	file(COPY libs/totalphase/x64 DESTINATION ${CMAKE_BINARY_DIR})	
endif (WIN32)


file(STRINGS "${VALENS_APHY_PUBLIC_INCLUDE_DIR}/aphy_sdk_version.h" VERSION_HEADER_CONTENT)
string(REGEX MATCHALL "#define[ \t]+APHY_SDK_VERSION_MAJOR[ \t]+([0-9]+)" APHY_SDK_VERSION_MAJOR_LINE "${VERSION_HEADER_CONTENT}")
set(APHY_SDK_VERSION_MAJOR ${CMAKE_MATCH_1})
string(REGEX MATCHALL "#define[ \t]+APHY_SDK_VERSION_MINOR[ \t]+([0-9]+)" APHY_SDK_VERSION_MINOR_LINE "${VERSION_HEADER_CONTENT}")
set(APHY_SDK_VERSION_MINOR ${CMAKE_MATCH_1})
string(REGEX MATCHALL "#define[ \t]+APHY_SDK_VERSION_PATCH[ \t]+([0-9]+)" APHY_SDK_VERSION_PATCH_LINE "${VERSION_HEADER_CONTENT}")
set(APHY_SDK_VERSION_PATCH ${CMAKE_MATCH_1})
if(APHY_SDK_VERSION_MAJOR STREQUAL "" OR APHY_SDK_VERSION_MINOR STREQUAL "" OR APHY_SDK_VERSION_PATCH STREQUAL "")
	message(FATAL_ERROR "APHY SDK version could not be parsed from ${VALENS_APHY_PUBLIC_INCLUDE_DIR}/aphy_sdk_version.h")
endif()
set(APHY_SDK_VERSION ${APHY_SDK_VERSION_MAJOR}.${APHY_SDK_VERSION_MINOR}.${APHY_SDK_VERSION_PATCH})
message("Parsed APHY_SDK_VERSION: " ${APHY_SDK_VERSION})

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_INSTALL_RPATH "$$ORIGIN")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

add_library(valens_aphy SHARED ${valens_aphy_src})
set_target_properties(valens_aphy PROPERTIES PREFIX "lib")
target_include_directories(valens_aphy PRIVATE ${VALENS_APHY_PRIVATE_INCLUDE_DIRS})
target_include_directories(valens_aphy PUBLIC ${VALENS_APHY_PUBLIC_INCLUDE_DIRS})

add_executable(aphy_host ${aphy_host_src})
target_link_libraries(aphy_host valens_aphy)
target_include_directories(aphy_host PRIVATE ${APHY_HOST_INCLUDE_DIRS})


#*********** UNIT TESTS (Experimental) *********************************************************
IF(WIN32)
# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  unit_tests  
  unit_tests/pch.cpp unit_tests/pch.h
  unit_tests/utils.cpp unit_tests/utils.h

  unit_tests/app_transport_tests.cpp
  unit_tests/containers_tests.cpp
  unit_tests/flash_utils_tests.cpp
  unit_tests/fwlog_tests.cpp
  unit_tests/va7031_eeprom_tests.cpp
)

target_link_libraries(unit_tests gtest_main valens_aphy)
target_include_directories(unit_tests PUBLIC ${VALENS_APHY_PUBLIC_INCLUDE_DIR})

include(GoogleTest)
gtest_discover_tests(unit_tests)

ENDIF(WIN32)
#*************************************************************************************************