cmake_minimum_required(VERSION 3.10)

set(EX_THREADED_LOG_SRCS
    "${PROJECT_SOURCE_DIR}/threaded_log.cpp"
)

add_executable(${EX_THREADED_LOG_NAME} ${EX_THREADED_LOG_SRCS})

target_link_libraries(${EX_THREADED_LOG_NAME}
    ${LOG_LIB_NAME}
)

set_target_properties(${EX_THREADED_LOG_NAME}
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${EX_THREADED_LOG_NAME}"
)
