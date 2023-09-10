# CMakeList.txt : CMake project for BGE3, include source and define
# project specific logic here.
#

cmake_minimum_required(VERSION 3.25)
project(BoredEngine VERSION 2.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

<<<<<<< HEAD
configure_file(EngineConfig.h.in ${CMAKE_BINARY_DIR}/EngineConfig.h)

enable_testing()

=======
enable_testing()

>>>>>>> 4e6a5731f9735e74d72d6c720ee52d3e46d4b991
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


add_subdirectory("Engine")
add_subdirectory("Editor")
add_subdirectory("examples")
