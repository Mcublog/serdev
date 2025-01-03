include(${PROJECT_SOURCE_DIR}/example/common/common.cmake)

# For testing using desktop gcc compiler
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

# Set defines
add_definitions(-DprojCOVERAGE_TEST=0)
add_definitions(-D_WINDOWS_)
add_definitions(-DDESKTOP)

# Set target name
set(TEST_TARGET "sclient")

file(GLOB_RECURSE SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${PROJECT_SOURCE_DIR}/src/Serial.cpp
    ${PROJECT_SOURCE_DIR}/src/usocket/USocketClientDev.cpp
)

# Buid TEST_TARGET target
add_executable(${TEST_TARGET}
    # common
    ${COMMON_SOURCES}
    # libs
    ${PROJECT_SOURCE_DIR}/libs/debug/log_libs.c
    # project
    ${SOURCES}
)

# Include paths
target_include_directories(${TEST_TARGET} PRIVATE
    # common
    ${COMMON_INCLUDES}
    ${PROJECT_SOURCE_DIR}
)

# Add pthread
target_link_libraries(${TEST_TARGET} pthread)

target_compile_definitions(${TEST_TARGET} PRIVATE "FORCE_LOG")
