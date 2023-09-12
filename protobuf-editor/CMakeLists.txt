cmake_minimum_required(VERSION 3.6.0)

project(protobuf-editor)

option(DEBUG "Compile with O0" OFF)

set(PBE_CXX_FLAGS "-Wuseless-cast \
-Wcast-align \
-Wcast-qual \
-Wsign-conversion \
-Wfloat-equal \
-Wfloat-conversion \
-Wzero-as-null-pointer-constant \
-Wunused-parameter \
-Wshadow \
")
# -Werror \

include_directories(../your_schema/)
include_directories(../3rdparty/)
include_directories(../3rdparty/imgui/)
include_directories(../3rdparty/imgui/backends/)
include_directories(../3rdparty/imgui/misc/)
# include_directories(${CMAKE_BINARY_DIR}/proto)
include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(../3rdparty/icecream-cpp/)

file(GLOB SOURCES *.cpp
    ../3rdparty/imgui/misc/cpp/*.cpp
    ../3rdparty/imgui/backends/imgui_impl_glfw.cpp
    ../3rdparty/imgui/backends/imgui_impl_opengl3.cpp
    ../3rdparty/imgui/*.cpp
    ../your_schema/*.cc
)

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

target_link_libraries(${PROJECT_NAME} LINK_PUBLIC glfw)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC OpenGL::GL)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${CMAKE_DL_LIBS})
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC libprotobuf)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC clip)
