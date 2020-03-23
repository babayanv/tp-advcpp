cmake_minimum_required(VERSION 3.10)

project(${EX_THREADED_LOG_NAME} CXX)

set(EX_THREADED_LOG_SRCS
    "${PROJECT_SOURCE_DIR}/threaded_log.cpp"
)

add_executable(${PROJECT_NAME} ${EX_THREADED_LOG_SRCS})

target_link_libraries(${PROJECT_NAME}
    ${LOG_LIB_NAME}
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}"
)
