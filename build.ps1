# OpenMRN is now a header-only library - no build required!
# This script provides usage information.

param(
    [switch]$Verify,
    [switch]$Help
)

if ($Help -or (-not $Verify)) {
    Write-Host "OpenMRN Header-Only Library" -ForegroundColor Cyan
    Write-Host "============================" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "OpenMRN is a header-only library - no compilation is needed!"
    Write-Host ""
    Write-Host "To use OpenMRN in your firmware project:"
    Write-Host ""
    Write-Host "1. Add as git submodule or use FetchContent:"
    Write-Host "   git submodule add https://github.com/bakerstu/openmrn.git libs/openmrn"
    Write-Host ""
    Write-Host "2. In your CMakeLists.txt:"
    Write-Host "   add_subdirectory(libs/openmrn)"
    Write-Host "   target_link_libraries(your_target PRIVATE OpenMRN::openmrn)"
    Write-Host ""
    Write-Host "3. Build your firmware project normally"
    Write-Host ""
    Write-Host "See EMBEDDED_BUILD.md for complete instructions."
    Write-Host ""
    Write-Host "Options:"
    Write-Host "  -Verify    Verify CMake configuration is valid"
    Write-Host "  -Help      Show this message"
    
    if (-not $Verify) {
        exit 0
    }
}

if ($Verify) {
    Write-Host "Verifying CMake configuration..." -ForegroundColor Yellow
    
    if (-not (Test-Path "build-verify")) {
        New-Item -ItemType Directory -Path "build-verify" | Out-Null
    }
    
    Set-Location "build-verify"
    cmake .. *> $null
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host "✓ CMake configuration is valid" -ForegroundColor Green
        Set-Location ..
        Remove-Item -Recurse -Force "build-verify"
        exit 0
    } else {
        Write-Host "✗ CMake configuration failed" -ForegroundColor Red
        Set-Location ..
        Remove-Item -Recurse -Force "build-verify"
        exit 1
    }
}
