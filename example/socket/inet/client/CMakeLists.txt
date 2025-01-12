include(${PROJECT_SOURCE_DIR}/example/socket/build.cmake)

# For testing using desktop gcc compiler
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

add_example_target(iclient)
