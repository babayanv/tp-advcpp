cmake_minimum_required(VERSION 3.17)

project(hw6 CXX)

#-------------------------------------------------------------------------------
# Compiler config
#-------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    add_compile_options("-Wall")
    add_compile_options("-Wextra")
    add_compile_options("-Wconversion")
    add_compile_options("-pedantic")
    add_compile_options("-Wfatal-errors")
    add_compile_options("--std=c++2a")
endif()


#-------------------------------------------------------------------------------
# Target server library
#-------------------------------------------------------------------------------
set(HTTP_LIB_NAME "http")
add_subdirectory(http)

#-------------------------------------------------------------------------------
# Target log library
#-------------------------------------------------------------------------------
set(LOG_LIB_NAME "log")
add_subdirectory(log)

#-------------------------------------------------------------------------------
# Target examples
#-------------------------------------------------------------------------------
add_subdirectory(examples)
