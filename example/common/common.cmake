file(GLOB_RECURSE COMMON_SOURCES
    ${PROJECT_SOURCE_DIR}/example/common/*.c
    ${PROJECT_SOURCE_DIR}/example/common/*.cpp
    ${PROJECT_SOURCE_DIR}/libs/debug/log_libs.c
)

set(COMMON_INCLUDES
    ${PROJECT_SOURCE_DIR}/example/common
    ${PROJECT_SOURCE_DIR}/libs/debug
)