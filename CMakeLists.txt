cmake_minimum_required(VERSION 3.16)
project(Harmonograph)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(ext/yaml-cpp)
add_subdirectory(ext/glad)
add_subdirectory(ext/glfw)

add_subdirectory(harmonograph)
