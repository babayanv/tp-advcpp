cmake_minimum_required(VERSION 3.10)

set(EX_ECHO_SRCS
    "src/echo.cpp"
)
set(EX_ECHO_MAIN
    "src/main.cpp"
)

set(EX_ECHO_CHILD_NAME "${EX_ECHO_NAME}_child")
set(EX_ECHO_MAIN_NAME "${EX_ECHO_NAME}_main")

add_executable(${EX_ECHO_CHILD_NAME} ${EX_ECHO_SRCS})
add_executable(${EX_ECHO_MAIN_NAME} ${EX_ECHO_MAIN})

target_link_libraries(${EX_ECHO_MAIN_NAME}
    ${PROC_LIB_NAME}
)

set_target_properties(${EX_ECHO_CHILD_NAME} ${EX_ECHO_MAIN_NAME}
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/echo"
)
