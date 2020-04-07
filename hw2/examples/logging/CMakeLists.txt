cmake_minimum_required(VERSION 3.10)

set(EX_LOGGING_SRCS
    "logging.cpp"
)

add_executable(${EX_LOGGING_NAME} ${EX_LOGGING_SRCS})

target_link_libraries(${EX_LOGGING_NAME}
    ${LOG_LIB_NAME}
)

set_target_properties(${EX_LOGGING_NAME}
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${EX_LOGGING_NAME}"
)
