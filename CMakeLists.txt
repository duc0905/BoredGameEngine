# CMakeList.txt : CMake project for BGE3, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.20)

project (BGE3 VERSION 2.0)

# Add source to this project's executable.
# add_executable (BGE3 "BGE3.cpp" "BGE3.h")

# if (CMAKE_VERSION VERSION_GREATER 3.12)
  # set_property(TARGET BGE3 PROPERTY CXX_STANDARD 20)
# endif()

# TODO: Add tests and install targets if needed.
add_subdirectory ("Engine")
add_subdirectory("Editor")

add_subdirectory ("examples/Chess")
