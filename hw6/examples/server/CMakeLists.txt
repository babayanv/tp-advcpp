cmake_minimum_required(VERSION 3.17)

set(EX_SERVER_SRCS
    "utils/parser.cpp"
    "main.cpp"
)

add_executable(${EX_SERVER_NAME} ${EX_SERVER_SRCS})

target_link_libraries(${EX_SERVER_NAME}
    ${HTTP_LIB_NAME}
    ${LOG_LIB_NAME}
)

set_target_properties(${EX_SERVER_NAME}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${EX_SERVER_NAME}"
)
