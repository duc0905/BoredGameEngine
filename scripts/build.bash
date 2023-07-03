#!/bin/bash

# Check if the build mode is enabled
if [ "$1" = "--Config" ] || [ "$1" = "-C" ]; then
  BuildMode=true
else
  BuildMode=false
fi

if [ "$BuildMode" = true ]; then
  echo "-------------------------------------------------------------------------------"
  echo "Generating the project CMake files [BuildMode: $BuildMode]"
  echo "-------------------------------------------------------------------------------"
  if [ -d "build" ]; then
    echo "-------------------------------------------------------------------------------"
    echo "Found existing build folder. Reusing it"
    echo "-------------------------------------------------------------------------------"

    # Run the second cmake command
    cmake -S . -B build


    if [ $? -eq 0 ]; then
      echo "-------------------------------------------------------------------------------"
      echo -e "\e[32mGenerated successfully\e[0m"
      echo "-------------------------------------------------------------------------------"
    else
      echo "-------------------------------------------------------------------------------"
      echo -e "\e[31mGeneration failed\e[0m"
      echo "-------------------------------------------------------------------------------"
      exit 1
    fi
  else
    envPath="$PATH"

    # Split the path into individual folders
    IFS=":" read -ra pathFolders <<< "$envPath"

    # Variable to store the vcpkg package link
    vcpkgPackageLink=""

    # Loop through each folder in the path and check for the vcpkg package link
    for folder in "${pathFolders[@]}"; do
      if [[ $folder == *"vcpkg"* ]]; then
        vcpkgPackageLink="$folder/scripts/buildsystems/vcpkg.cmake"
        echo "-------------------------------------------------------------------------------"
        echo -e "\e[32mVcpkg package link found: $vcpkgPackageLink\e[0m"
        echo "-------------------------------------------------------------------------------"
      fi
    done

    if [ -z "$vcpkgPackageLink" ]; then
      echo "-------------------------------------------------------------------------------"
      echo -e "\e[31m[Error] Vcpkg package link not found. Please add Vcpkg to the PATH\e[0m"
      echo "-------------------------------------------------------------------------------"
      exit 1
    fi

    cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE="$vcpkgPackageLink"


    if [ $? -eq 0 ]; then
      echo "-------------------------------------------------------------------------------"
      echo -e "\e[32mGenerated successfully\e[0m"
      echo "-------------------------------------------------------------------------------"
    else
      echo "-------------------------------------------------------------------------------"
      echo -e "\e[31mGeneration failed\e[0m"
      echo "-------------------------------------------------------------------------------"
      exit 1
    fi
  fi
fi

echo "-------------------------------------------------------------------------------"
echo "Building the project"
echo "-------------------------------------------------------------------------------"

# Run the cmake build command
cmake --build build
?=$?

if [ $? -eq 0 ]; then
  echo "-------------------------------------------------------------------------------"
  echo -e "\e[32mBuild completed successfully\e[0m"
  echo "-------------------------------------------------------------------------------"
else
  echo "-------------------------------------------------------------------------------"
  echo -e "\e[31mBuild failed\e[0m"
  echo "-------------------------------------------------------------------------------"
fi

exit 0
