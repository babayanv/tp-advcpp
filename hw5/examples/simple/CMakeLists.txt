cmake_minimum_required(VERSION 3.10)

set(EX_SIMPLE_SRCS
    "main.cpp"
)

add_executable(${EX_SIMPLE_NAME} ${EX_SIMPLE_SRCS})

target_link_libraries(${EX_SIMPLE_NAME} ${SHMEM_LIB_NAME})

set_target_properties(${EX_SIMPLE_NAME}
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${EX_SIMPLE_NAME}"
)
