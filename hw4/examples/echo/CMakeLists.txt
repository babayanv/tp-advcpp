cmake_minimum_required(VERSION 3.10)

set(EX_SERVER_SRCS
    "server.cpp"
)

add_executable(${EX_ECHO_NAME} ${EX_SERVER_SRCS})

target_link_libraries(${EX_ECHO_NAME}
    ${EPOLL_SERVER_LIB_NAME}
)

set_target_properties(${EX_ECHO_NAME}
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${EX_ECHO_NAME}"
)
