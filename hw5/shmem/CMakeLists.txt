cmake_minimum_required(VERSION 3.10)

project(${SHMEM_LIB_NAME} CXX)

set(SHMEM_LIB_SRCS
    "${PROJECT_SOURCE_DIR}/include/shmem/allocator/LinearAllocator.hpp"

    "${PROJECT_SOURCE_DIR}/include/shmem/container/Map.hpp"

    "${PROJECT_SOURCE_DIR}/include/shmem/utils/bridge/NonCopyable.hpp"
    "${PROJECT_SOURCE_DIR}/include/shmem/utils/sync/Semaphore.hpp"

    "${PROJECT_SOURCE_DIR}/include/shmem/Exception.hpp"
    "${PROJECT_SOURCE_DIR}/include/shmem/SharedMemory.hpp"
)

find_package (Threads)

add_library(${PROJECT_NAME} INTERFACE)
target_sources(${PROJECT_NAME} INTERFACE ${SHMEM_LIB_SRCS})

target_link_libraries(${PROJECT_NAME} INTERFACE
    ${CMAKE_THREAD_LIBS_INIT}
)

target_include_directories(${PROJECT_NAME}
    INTERFACE
        "${PROJECT_SOURCE_DIR}/include"
)
