cmake_minimum_required(VERSION 3.10)

project(${EPOLL_SERVER_LIB_NAME} CXX)

set(EPOLL_SERVER_LIB_SRCS
    "${PROJECT_SOURCE_DIR}/src/utils/FileDescriptor.cpp"

    "${PROJECT_SOURCE_DIR}/src/Connection.cpp"
    "${PROJECT_SOURCE_DIR}/src/Exception.cpp"
    "${PROJECT_SOURCE_DIR}/src/Server.cpp"
)

add_library(${PROJECT_NAME} STATIC ${EPOLL_SERVER_LIB_SRCS})

target_link_libraries(${PROJECT_NAME}
    ${LOG_LIB_NAME}
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
