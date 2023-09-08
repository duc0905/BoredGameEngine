# CMakeList.txt : CMake project for BGE3, include source and define
# project specific logic here.
#

cmake_minimum_required(VERSION 3.25)
project(BoredEngine VERSION 2.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

configure_file(EngineConfig.h.in ${CMAKE_BINARY_DIR}/EngineConfig.h)

add_subdirectory("Engine")
add_subdirectory("Editor")
add_subdirectory("examples")
