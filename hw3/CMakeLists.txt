cmake_minimum_required(VERSION 3.10)

project(hw3 CXX)

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
# Target log library
#-------------------------------------------------------------------------------
set(TCP_LIB_NAME "tcp")
add_subdirectory(tcp)


#-------------------------------------------------------------------------------
# Target examples
#-------------------------------------------------------------------------------
add_subdirectory(examples)
