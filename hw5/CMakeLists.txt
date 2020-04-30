cmake_minimum_required(VERSION 3.10)

project(hw5 CXX)

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
# Target shmem library
#-------------------------------------------------------------------------------
set(SHMEM_LIB_NAME "shmem")
add_subdirectory(shmem)


#-------------------------------------------------------------------------------
# Target examples
#-------------------------------------------------------------------------------
add_subdirectory(examples)
