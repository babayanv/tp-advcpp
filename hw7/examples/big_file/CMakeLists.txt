cmake_minimum_required(VERSION 3.17)

set(EX_BIG_FILE_SRCS
    "main.cpp"
    "src/db_server.cpp"
)

add_executable(${EX_BIG_FILE_NAME} ${EX_BIG_FILE_SRCS})

target_link_libraries(${EX_BIG_FILE_NAME}
    ${DATA_BANK_LIB_NAME}
    ${HTTP_LIB_NAME}
)

target_include_directories(${EX_BIG_FILE_NAME}
    PUBLIC
    "include"
)

set_target_properties(${EX_BIG_FILE_NAME}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${EX_BIG_FILE_NAME}"
)
