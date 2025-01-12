include(${PROJECT_SOURCE_DIR}/example/common/common.cmake)

function(add_example_target targetname)

    file(GLOB_RECURSE SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        ${PROJECT_SOURCE_DIR}/src/Serial.cpp
        ${PROJECT_SOURCE_DIR}/src/usocket/*.cpp
        ${PROJECT_SOURCE_DIR}/src/utils/*.c
        ${PROJECT_SOURCE_DIR}/src/utils/*.cpp
    )

    # Buid TEST_TARGET target
    add_executable(${targetname}

        # common
        ${COMMON_SOURCES}

        # libs
        ${PROJECT_SOURCE_DIR}/libs/debug/log_libs.c

        # project
        ${SOURCES}
    )

    # Include paths
    target_include_directories(${targetname} PRIVATE
        # common
        ${COMMON_INCLUDES}
        ${PROJECT_SOURCE_DIR}
    )

    # Add pthread
    target_link_libraries(${targetname} pthread)

    target_compile_definitions(${targetname} PRIVATE "FORCE_LOG")

endfunction(add_example_target)
