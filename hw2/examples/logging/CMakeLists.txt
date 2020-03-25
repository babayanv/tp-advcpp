cmake_minimum_required(VERSION 3.10)

project(${EX_LOGGING_NAME} CXX)

set(EX_LOGGING_SRCS
    "${PROJECT_SOURCE_DIR}/logging.cpp"
)

add_executable(${PROJECT_NAME} ${EX_LOGGING_SRCS})

target_link_libraries(${PROJECT_NAME}
    ${LOG_LIB_NAME}
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}"
)
