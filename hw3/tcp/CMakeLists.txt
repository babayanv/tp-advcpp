cmake_minimum_required(VERSION 3.10)

project(${TCP_LIB_NAME} CXX)

set(TCP_LIB_SRCS
    "${PROJECT_SOURCE_DIR}/src/Connection.cpp"
    "${PROJECT_SOURCE_DIR}/src/Server.cpp"
    "${PROJECT_SOURCE_DIR}/src/Exception.cpp"
)

add_library(${PROJECT_NAME} STATIC ${TCP_LIB_SRCS})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/tcp"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/tcp"
)
