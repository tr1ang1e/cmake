cmake_minimum_required(VERSION 3.8)

project(main)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

set(HEADERS ${CMAKE_SOURCE_DIR}/headers)
add_executable(main main.c "${HEADERS}/header.h")

include(CheckSymbolExists)
CHECK_SYMBOL_EXISTS(str "${HEADERS}/header.h" strExists)