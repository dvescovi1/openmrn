# OpenMRN Embedded ARM Library Build Script for Windows
# PowerShell script to build OpenMRN embedded library

param(
    [switch]$Debug,
    [switch]$Clean,
    [string]$Toolchain = "",
    [switch]$Help
)

if ($Help) {
    Write-Host "OpenMRN Embedded ARM Library Build Script"
    Write-Host ""
    Write-Host "Usage: .\build.ps1 [options]"
    Write-Host ""
    Write-Host "Options:"
    Write-Host "  -Debug               Build in Debug mode (default: Release)"
    Write-Host "  -Clean               Clean build directory before building"
    Write-Host "  -Toolchain <file>    Use specific CMake toolchain file"
    Write-Host "  -Help                Show this help message"
    Write-Host ""
    Write-Host "Examples:"
    Write-Host "  .\build.ps1"
    Write-Host "  .\build.ps1 -Debug -Clean"
    Write-Host "  .\build.ps1 -Toolchain cmake/arm-none-eabi.toolchain.cmake.example"
    exit 0
}

$BuildType = if ($Debug) { "Debug" } else { "Release" }
$BuildDir = "build-arm"

# Clean build if requested
if ($Clean) {
    Write-Host "Cleaning build directory..."
    if (Test-Path $BuildDir) {
        Remove-Item -Recurse -Force $BuildDir
    }
}

# Create build directory
if (-not (Test-Path $BuildDir)) {
    New-Item -ItemType Directory -Path $BuildDir | Out-Null
}

Set-Location $BuildDir

# Configure
Write-Host "Configuring CMake for Embedded ARM Library (BUILD_TYPE=$BuildType)..."
if ($Toolchain -ne "") {
    Write-Host "Using toolchain: $Toolchain"
    cmake .. -DCMAKE_BUILD_TYPE="$BuildType" -DCMAKE_TOOLCHAIN_FILE="../$Toolchain"
} else {
    cmake .. -DCMAKE_BUILD_TYPE="$BuildType"
}

if ($LASTEXITCODE -ne 0) {
    Write-Error "CMake configuration failed"
    exit 1
}

# Build
Write-Host "Building embedded library..."
cmake --build . --config $BuildType

if ($LASTEXITCODE -ne 0) {
    Write-Error "Build failed"
    exit 1
}

Write-Host ""
Write-Host "Build completed successfully!" -ForegroundColor Green
Write-Host "Build directory: $BuildDir"
Write-Host "Build type: $BuildType"
Write-Host "Library: $BuildDir\src\libopenmrn.a (or .lib on Windows)"
Write-Host ""
Write-Host "To use in your firmware project:"
Write-Host "  target_link_libraries(your_target PRIVATE OpenMRN::openmrn)"

Set-Location ..
