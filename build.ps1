param (
  [Parameter(Position = 0, Mandatory = $false)]
  [switch]$Build
)

Write-Output "Project Path: $Build"

if ($Build) {
  Write-Output "Generating the project CMakeList"
  $envPath = [Environment]::GetEnvironmentVariable("PATH")

  # Split the path into individual folders
  $pathFolders = $envPath -split ";"

  # Variable to store the vcpkg package link
  $vcpkgPackageLink = ""

  # Loop through each folder in the path and check for the vcpkg package link
  foreach ($folder in $pathFolders) {
    if ($folder -like "*vcpkg*") {
      $vcpkgPackageLink = $folder + "/scripts/buildsystems/vcpkg.cmake"
    }
  }
  if ($vcpkgPackageLink = "") {
    Write-Output "[Error] Vcpkg package link not found. Please add Vcpkg to the PATH"
    exit
  }
  cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=$vcpkgPackageLink
  Write-Output "Generated successfully"
}

Write-Output "Building the project"

# Run the second cmake command
cmake -S . -B build

# Run the cmake build command
cmake --build build

Write-Output "Build completed successfully"

exit