cmake_minimum_required(VERSION 3.16)

project(main)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

add_executable(main main.c ${CMAKE_CURRENT_SOURCE_DIR}/headers/headers2/header.h)
target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/headers/)

include(CheckSymbolExists)
CHECK_SYMBOL_EXISTS(str ${CMAKE_CURRENT_SOURCE_DIR}/headers/header.h strExists)