# CMake-vcpkg-ninja template

A simple project for C++ development using:

- [CMake](https://cmake.org/)
- [Ninja](https://ninja-build.org/) (Optional) - Build system
- [Clang](https://releases.llvm.org/download.html) - C/C++ compiler
- [Doxygen](https://www.doxygen.nl/manual/install.html) - Generating code documentation
- [GTest](https://google.github.io/googletest/) - Testing and mocking framework.
  - Do not need to install this independently. It is already installed with vcpkg manifest.

## Folder structure:
- `build/`: the output folder for CMake config and build
    - `build/<config-preset>/docs/html/index.html`: the entrypoint for the Doxygen documentation for our project
- `docs/`: documentation related files
    - `docs/Doxyfile.in`: configuration file for generating the final Doxyfile with CMake to be used with doxygen
- `src/`: source code
- `tests/`: test code
- `vcpkg/`: git submodule of the original vcpkg repository
- `vcpkg.json`: vcpkg manifest file

## CMake presets

List all presets:
```sh
cmake --list-presets=all
```

Presets in this template:
1. Configuration presets:
    - ninja-vcpkg
2. Build presets:
    - build
    - build-main
3. Test presets:
    - test

## compile_commands.json file

- CMake generates the file at `build/<config-preset>/compile_commands.json`. Create a symlink to the file is recommended.
