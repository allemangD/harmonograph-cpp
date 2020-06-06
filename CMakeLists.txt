cmake_minimum_required(VERSION 3.16)
project(Harmonograph)

set(CMAKE_CXX_STANDARD 17)

set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_INSTALL OFF)

set(YAML_CPP_BUILD_CONTRIB OFF)
set(YAML_CPP_BUILD_TOOLS OFF)

add_subdirectory(ext/yaml-cpp)
add_subdirectory(ext/glad)
add_subdirectory(ext/glfw)

add_subdirectory(harmonograph)
