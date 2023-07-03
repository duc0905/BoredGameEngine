param (
  [switch]$Config,
  [switch]$C
)

$BuildMode = $Config -or $C

if ($BuildMode) {
  Write-Output "-------------------------------------------------------------------------------"
  Write-Output "Generating the project CMake files [BuildMode: $BuildMode]"
  Write-Output "-------------------------------------------------------------------------------"
  if (Test-Path -Path build) {

    Write-Output "-------------------------------------------------------------------------------"
    Write-Output "Found existing build folder. Reusing it"
    Write-Output "-------------------------------------------------------------------------------"

    # Run the second cmake command
    cmake -S . -B build
    $exitCode = $LASTEXITCODE

    if ($exitCode -eq 0) {
      Write-Output "-------------------------------------------------------------------------------"
      Write-Host "Generated successfully" -ForegroundColor Green
      Write-Output "-------------------------------------------------------------------------------"
    }
    else {
      Write-Output "-------------------------------------------------------------------------------"
      Write-Host "Generation failed" -ForegroundColor Red
      Write-Output "-------------------------------------------------------------------------------"
      exit 1
    }
  }
  else {
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
        Write-Host "Vcpkg package link found: $vcpkgPackageLink"  -ForegroundColor Green
        Write-Output "-------------------------------------------------------------------------------"
      }
    }
    if ($vcpkgPackageLink -eq "") {
      Write-Output "-------------------------------------------------------------------------------"
      Write-Host "[Error] Vcpkg package link not found. Please add Vcpkg to the PATH" -ForegroundColor Red
      Write-Output "-------------------------------------------------------------------------------"
      exit 1
    }
    # cmake -S ../ -B build -DCMAKE_TOOLCHAIN_FILE="$vcpkgPackageLink"
    $CmakeGencommand = "cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=$vcpkgPackageLink"
    Invoke-Expression -Command $CmakeGencommand
    $exitCode = $LASTEXITCODE

    if ($exitCode -eq 0) {
      Write-Output "-------------------------------------------------------------------------------"
      Write-Host "Generated successfully" -ForegroundColor Green
      Write-Output "-------------------------------------------------------------------------------"
    }
    else {
      Write-Output "-------------------------------------------------------------------------------"
      Write-Host "Generation failed" -ForegroundColor Red
      Write-Output "-------------------------------------------------------------------------------"
      exit 1
    }
  }
}

Write-Output "-------------------------------------------------------------------------------"
Write-Output "Building the project"
Write-Output "-------------------------------------------------------------------------------"

# Run the cmake build command
cmake --build build
$exitCode = $LASTEXITCODE

if ($exitCode -eq 0) {
  Write-Output "-------------------------------------------------------------------------------"
  Write-Host "Build completed successfully"  -ForegroundColor Green
  Write-Output "-------------------------------------------------------------------------------"
}
else {
  Write-Output "-------------------------------------------------------------------------------"
  Write-Host "Build failed" -ForegroundColor Red
  Write-Output "-------------------------------------------------------------------------------"
}

exit 0