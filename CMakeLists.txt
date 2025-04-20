cmake_minimum_required(VERSION 3.25)

# TODO: change project name, version number, and description
project(my-project
	VERSION 1.0.0
	DESCRIPTION "A simple CMake project template for developing C++ applications"
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

# Source code
add_subdirectory("src")

# Tests on our code
add_subdirectory("tests")

# Documentations
add_subdirectory("docs")
