cmake_minimum_required(VERSION 3.16)
project(Harmonograph)

set(CMAKE_CXX_STANDARD 17)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_INSTALL OFF)

option(YAML_CPP_BUILD_CONTRIB OFF)
option(YAML_CPP_BUILD_TOOLS OFF)

add_subdirectory(ext/yaml-cpp)
add_subdirectory(ext/glad)
add_subdirectory(ext/glfw)

add_subdirectory(harmonograph)
