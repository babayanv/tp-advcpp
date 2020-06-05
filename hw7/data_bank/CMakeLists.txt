cmake_minimum_required(VERSION 3.17)

project(${DATA_BANK_LIB_NAME} CXX)

set(DATA_BANK_LIB_SRCS
    "${PROJECT_SOURCE_DIR}/src/errors.cpp"

    "${PROJECT_SOURCE_DIR}/src/external_sort/partition.cpp"
    "${PROJECT_SOURCE_DIR}/src/external_sort/cache_file.cpp"

    "${PROJECT_SOURCE_DIR}/src/utils/file_descriptor.cpp"
    "${PROJECT_SOURCE_DIR}/src/utils/mem_mapped_file.cpp"
    "${PROJECT_SOURCE_DIR}/src/utils/temp_file.cpp"
    "${PROJECT_SOURCE_DIR}/src/utils/file.cpp"
)

add_library(${PROJECT_NAME} STATIC ${DATA_BANK_LIB_SRCS})

target_link_libraries(${PROJECT_NAME}
    ${CMAKE_THREAD_LIBS_INIT}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${PROJECT_NAME}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${PROJECT_NAME}"
)
