cmake_minimum_required(VERSION 3.11.0)
project(VoxelEngine VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)

set(SOURCES_DIR ${PROJECT_SOURCE_DIR}/src)

include_directories(VoxelEngine ${SOURCES_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/vendor)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(spdlog REQUIRED)

set(SOURCE_FILES
  ${SOURCES_DIR}/glad.c

  ${SOURCES_DIR}/main.cpp
  ${SOURCES_DIR}/Buffer.cpp
  ${SOURCES_DIR}/VertexArray.cpp
  ${SOURCES_DIR}/Shader.cpp
  ${SOURCES_DIR}/Texture.cpp
  ${SOURCES_DIR}/Camera.cpp
  ${SOURCES_DIR}/World.cpp
  )

add_executable(VoxelEngine ${SOURCE_FILES})
target_link_libraries(VoxelEngine glfw OpenGL spdlog ${CMAKE_DL_LIBS})

