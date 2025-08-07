cmake_minimum_required(VERSION 3.25)

project(BoredGameEngine
	VERSION 0.0.0
	DESCRIPTION "Simple Game Engine"
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

# Engine code
add_subdirectory("Engine")

# Tests on our code
add_subdirectory("tests")

# Documentations
add_subdirectory("docs")

# Demo projects
add_subdirectory("demo")
