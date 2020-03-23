cmake_minimum_required(VERSION 3.10)

project(${LOG_LIB_NAME} CXX)

set(LIB_LIB_SRCS
    "${PROJECT_SOURCE_DIR}/src/loggers/BaseLogger.cpp"
    "${PROJECT_SOURCE_DIR}/src/loggers/FileLogger.cpp"
    "${PROJECT_SOURCE_DIR}/src/loggers/StdoutLogger.cpp"
    "${PROJECT_SOURCE_DIR}/src/loggers/StderrLogger.cpp"
    "${PROJECT_SOURCE_DIR}/src/format/LogFormatter.cpp"
    "${PROJECT_SOURCE_DIR}/src/Logger.cpp"
    "${PROJECT_SOURCE_DIR}/src/ThreadedLogger.cpp"
)

find_package (Threads)

add_library(${PROJECT_NAME} STATIC ${LIB_LIB_SRCS})

target_link_libraries(${PROJECT_NAME}
    ${CMAKE_THREAD_LIBS_INIT}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/log"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/log"
)
