cmake_minimum_required(VERSION 3.17)

project(${HTTP_LIB_NAME} CXX)

set(HTTP_LIB_SRCS
    "${PROJECT_SOURCE_DIR}/src/network/protocol/http.cpp"
    "${PROJECT_SOURCE_DIR}/src/network/protocol/http_request.cpp"
    "${PROJECT_SOURCE_DIR}/src/network/protocol/http_response.cpp"
    "${PROJECT_SOURCE_DIR}/src/network/connection.cpp"

    "${PROJECT_SOURCE_DIR}/src/utils/file_descriptor.cpp"
    "${PROJECT_SOURCE_DIR}/src/utils/coroutine.cpp"

    "${PROJECT_SOURCE_DIR}/src/server/server.cpp"
    "${PROJECT_SOURCE_DIR}/src/server/server_worker.cpp"
    "${PROJECT_SOURCE_DIR}/src/server/client_handler.cpp"

    "${PROJECT_SOURCE_DIR}/src/errors.cpp"
    "${PROJECT_SOURCE_DIR}/src/signal.cpp"
)

find_package (Threads)

add_library(${PROJECT_NAME} STATIC ${HTTP_LIB_SRCS})

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
