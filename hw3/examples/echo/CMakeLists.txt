cmake_minimum_required(VERSION 3.10)

set(EX_CLIENT_SRCS
    "${PROJECT_SOURCE_DIR}/src/client.cpp"
)
set(EX_SERVER_SRCS
    "${PROJECT_SOURCE_DIR}/src/server.cpp"
)

set(EX_ECHO_CLIENT_NAME "${EX_ECHO_NAME}_client")
set(EX_ECHO_SERVER_NAME "${EX_ECHO_NAME}_server")

add_executable(${EX_ECHO_CLIENT_NAME} ${EX_CLIENT_SRCS})
add_executable(${EX_ECHO_SERVER_NAME} ${EX_SERVER_SRCS})

target_link_libraries(${EX_ECHO_CLIENT_NAME}
    ${TCP_LIB_NAME}
)

target_link_libraries(${EX_ECHO_SERVER_NAME}
    ${TCP_LIB_NAME}
)

set_target_properties(${EX_ECHO_CLIENT_NAME} ${EX_ECHO_SERVER_NAME}
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${EX_ECHO_NAME}"
)
