cmake_minimum_required(VERSION 3.10)

project(hw1 CXX)

#-------------------------------------------------------------------------------
# Compiler config
#-------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    add_compile_options("-Wall")
    add_compile_options("-Wextra")
    add_compile_options("-Wconversion")
    add_compile_options("-pedantic")
    add_compile_options("-Wfatal-errors")
endif()


#-------------------------------------------------------------------------------
# Target process library
#-------------------------------------------------------------------------------
set(PROC_LIB_NAME "process")
add_subdirectory(process)


#-------------------------------------------------------------------------------
# Target examples
#-------------------------------------------------------------------------------
add_subdirectory(examples)
