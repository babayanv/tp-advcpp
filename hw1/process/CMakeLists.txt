project(${PROC_LIB_NAME} CXX)

set(PROC_LIB_SRCS
    "${PROJECT_SOURCE_DIR}/src/Process.cpp"
)

add_library(${PROJECT_NAME} STATIC ${PROC_LIB_SRCS})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib"
)
