# User

*Coming soon!*

# Contributor

## Installation

- Install [CMake](https://cmake.org/)
- Install [Ninja](https://ninja-build.org/) - Build system
    - Only because I want the compile_commands.json file from CMake
- Install [Clang](https://releases.llvm.org/download.html) - C/C++ compiler

## CMake presets

- Can create personal presets using CMakeUserPresets.json file

## compile_commands.json file

- CMake generates the file at `out/build/<config-preset>/compile_commands.json`. Create a symlink to the file is recommended.

## Build

- `cmake --workflow --preset <wf-preset>`
    - There is only Editor workflow for now
