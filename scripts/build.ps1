param (
  [Parameter(Position = 0, Mandatory = $false)]
  [switch]$Build
)

Write-Output "-------------------------------------------------------------------------------"
Write-Output "Generating the project CMake files [Build: $Build]]"
Write-Output "-------------------------------------------------------------------------------"
if ($Build) {
  $envPath = [Environment]::GetEnvironmentVariable("PATH")

  # Split the path into individual folders
  $pathFolders = $envPath -split ";"

  # Variable to store the vcpkg package link
  $vcpkgPackageLink = ""

  # Loop through each folder in the path and check for the vcpkg package link
  foreach ($folder in $pathFolders) {
    if ($folder -like "*vcpkg*") {
      $vcpkgPackageLink = $folder + "\scripts\buildsystems\vcpkg.cmake"
      Write-Output "-------------------------------------------------------------------------------"
      Write-Output "Vcpkg package link found: $vcpkgPackageLink"
      Write-Output "-------------------------------------------------------------------------------"
    }
  }
  if ($vcpkgPackageLink -eq "") {
    Write-Output "-------------------------------------------------------------------------------"
    Write-Output "[Error] Vcpkg package link not found. Please add Vcpkg to the PATH"
    Write-Output "-------------------------------------------------------------------------------"
    exit
  }
  # cmake -S ../ -B build -DCMAKE_TOOLCHAIN_FILE="$vcpkgPackageLink"
  $CmakeGencommand = "cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=$vcpkgPackageLink"
  Write-Output $CmakeGencommand
  if (Invoke-Expression -Command $CmakeGencommand) {
    Write-Output "-------------------------------------------------------------------------------"
    Write-Output "Generated successfully"
    Write-Output "-------------------------------------------------------------------------------"
  }
  else {
    Write-Output "-------------------------------------------------------------------------------"
    Write-Output "Generation failed"
    Write-Output "-------------------------------------------------------------------------------"

  }
}
else {
  # Run the second cmake command
  if (cmake -S . -B build) {
    Write-Output "-------------------------------------------------------------------------------"
    Write-Output "Generated successfully"
    Write-Output "-------------------------------------------------------------------------------"
  }
  else {
    Write-Output "-------------------------------------------------------------------------------"
    Write-Output "Generation failed"
    Write-Output "-------------------------------------------------------------------------------"
  }
}

Write-Output "-------------------------------------------------------------------------------"
Write-Output "Building the project"
Write-Output "-------------------------------------------------------------------------------"

# Run the cmake build command
if (cmake --build build) {
  Write-Output "-------------------------------------------------------------------------------"
  Write-Output "Build completed successfully"
  Write-Output "-------------------------------------------------------------------------------"
}


exit