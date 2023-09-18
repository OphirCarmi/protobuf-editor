cmake_minimum_required(VERSION 3.16.0)

project(protobuf-editor)

option(DEBUG "Compile with O0" OFF)

set(PBE_CXX_FLAGS "-Wcast-align \
-Wcast-qual \
-Wsign-conversion \
-Wfloat-equal \
-Wfloat-conversion \
-Wzero-as-null-pointer-constant \
-Wunused-parameter \
-Wshadow \
-Wpedantic \
-Werror \
")

if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(PBE_CXX_FLAGS "${PBE_CXX_FLAGS} -Wuseless-cast")
endif()


include_directories(${CMAKE_BINARY_DIR}/../your_schema/)
include_directories(${CMAKE_BINARY_DIR}/../3rdparty/)
include_directories(${CMAKE_BINARY_DIR}/../3rdparty/imgui/)
include_directories(${CMAKE_BINARY_DIR}/../3rdparty/icecream-cpp/)

file(GLOB SOURCES *.cpp)

if(DEBUG)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")
  message("Compiling executables with -O0 -g")
else()
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
  message("Compiling executables with -O3")
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${PBE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ${PBE_CXX_FLAGS}")


add_executable(${PROJECT_NAME} ${SOURCES})

add_dependencies(${PROJECT_NAME} schema)

target_link_libraries(${PROJECT_NAME} LINK_PUBLIC libprotobuf)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC clip)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC schema)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC imgui)
