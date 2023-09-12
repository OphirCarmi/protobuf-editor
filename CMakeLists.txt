cmake_minimum_required(VERSION 3.6.0)

project(protobuf-editor)

set(CMAKE_CXX_STANDARD 14)

option(DEBUG "Compile with O0" OFF)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(3rdparty/clip)
add_subdirectory(3rdparty/protobuf)
add_subdirectory(protobuf-editor)
