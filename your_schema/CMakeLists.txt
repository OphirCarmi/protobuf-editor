cmake_minimum_required(VERSION 3.16.0)

project(schema)

set(CMAKE_CXX_STANDARD 14)

option(DEBUG "Compile with O0" OFF)

include_directories(${CMAKE_BINARY_DIR}/../your_schema/)
include_directories(${CMAKE_BINARY_DIR}/../3rdparty/)
include_directories(${CMAKE_BINARY_DIR}/../3rdparty/protobuf/src/)
include_directories(${CMAKE_BINARY_DIR}/../3rdparty/protobuf/third_party/abseil-cpp/)

file(GLOB SOURCES *.cc)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_link_libraries(${PROJECT_NAME} LINK_PUBLIC libprotobuf)
