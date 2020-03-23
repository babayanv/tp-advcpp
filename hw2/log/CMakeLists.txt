cmake_minimum_required(VERSION 3.10)

project(${LOG_LIB_NAME} CXX)

set(LIB_LIB_SRCS
    "${PROJECT_SOURCE_DIR}/src/BaseLogger.cpp"
    "${PROJECT_SOURCE_DIR}/src/FileLogger.cpp"
    "${PROJECT_SOURCE_DIR}/src/StdoutLogger.cpp"
    "${PROJECT_SOURCE_DIR}/src/StderrLogger.cpp"
    "${PROJECT_SOURCE_DIR}/src/Logger.cpp"
    "${PROJECT_SOURCE_DIR}/src/LogFormatter.cpp"
)

add_library(${PROJECT_NAME} STATIC ${LIB_LIB_SRCS})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/log"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/log"
)
