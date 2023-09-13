cmake_minimum_required(VERSION 3.16.0)

project(imgui)

option(DEBUG "Compile with O0" OFF)

include_directories(${CMAKE_BINARY_DIR}/../3rdparty/imgui/)
include_directories(${CMAKE_BINARY_DIR}/../3rdparty/imgui/backends/)
include_directories(${CMAKE_BINARY_DIR}/../3rdparty/imgui/misc/)

file(GLOB SOURCES ${CMAKE_BINARY_DIR}/../3rdparty/imgui/misc/cpp/*.cpp
    ${CMAKE_BINARY_DIR}/../3rdparty/imgui/backends/imgui_impl_glfw.cpp
    ${CMAKE_BINARY_DIR}/../3rdparty/imgui/backends/imgui_impl_opengl3.cpp
    ${CMAKE_BINARY_DIR}/../3rdparty/imgui/*.cpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_link_libraries(${PROJECT_NAME} LINK_PUBLIC glfw)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC OpenGL::GL)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${CMAKE_DL_LIBS})
