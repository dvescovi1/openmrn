# OpenMRN CMake Build Script for Windows
# PowerShell script to build OpenMRN

param(
    [switch]$Debug,
    [switch]$Clean,
    [switch]$Test,
    [switch]$Docs,
    [switch]$Help
)

if ($Help) {
    Write-Host "OpenMRN CMake Build Script"
    Write-Host ""
    Write-Host "Usage: .\build.ps1 [options]"
    Write-Host ""
    Write-Host "Options:"
    Write-Host "  -Debug      Build in Debug mode (default: Release)"
    Write-Host "  -Clean      Clean build directory before building"
    Write-Host "  -Test       Run tests after building"
    Write-Host "  -Docs       Build documentation"
    Write-Host "  -Help       Show this help message"
    exit 0
}

$BuildType = if ($Debug) { "Debug" } else { "Release" }
$BuildDir = "build"

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
Write-Host "Configuring CMake (BUILD_TYPE=$BuildType)..."
cmake .. -DCMAKE_BUILD_TYPE="$BuildType"

if ($LASTEXITCODE -ne 0) {
    Write-Error "CMake configuration failed"
    exit 1
}

# Build
Write-Host "Building..."
cmake --build . --config $BuildType

if ($LASTEXITCODE -ne 0) {
    Write-Error "Build failed"
    exit 1
}

# Run tests if requested
if ($Test) {
    Write-Host "Running tests..."
    ctest -C $BuildType --output-on-failure
    
    if ($LASTEXITCODE -ne 0) {
        Write-Warning "Some tests failed"
    }
}

# Build documentation if requested
if ($Docs) {
    Write-Host "Building documentation..."
    cmake --build . --target docs
    Write-Host "Documentation built in $BuildDir\doc\html\"
}

Write-Host ""
Write-Host "Build completed successfully!" -ForegroundColor Green
Write-Host "Build directory: $BuildDir"
Write-Host "Build type: $BuildType"

Set-Location ..
