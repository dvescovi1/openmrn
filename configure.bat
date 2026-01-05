@echo off
REM Quick start script for OpenMRN CMake build on Windows

setlocal enabledelayedexpansion

set SCRIPT_DIR=%~dp0
set BUILD_DIR=%SCRIPT_DIR%build

echo OpenMRN CMake Build System - Quick Start
echo ========================================
echo.

REM Check for CMake
where cmake >nul 2>&1
if errorlevel 1 (
    echo ERROR: CMake not found. Please install CMake 3.20 or later.
    exit /b 1
)

for /f "tokens=*" %%i in ('cmake --version 2^>nul') do (
    echo %%i
    goto :done_cmake
)
:done_cmake
echo.

REM Check for ARM toolchain
if not defined ARMGCCPATH (
    where arm-none-eabi-gcc >nul 2>&1
    if errorlevel 1 (
        echo ERROR: ARM GCC toolchain not found.
        echo Please set ARMGCCPATH environment variable:
        echo   set ARMGCCPATH=C:\path\to\arm-none-eabi
        exit /b 1
    ) else (
        echo Found ARM GCC in PATH
    )
) else (
    echo ARM GCC path: %ARMGCCPATH%
)

echo.

REM Check for FreeRTOS
if not defined FREERTOSPATH (
    echo WARNING: FREERTOSPATH not set. FreeRTOS build may fail.
    echo Set it with: set FREERTOSPATH=C:\path\to\FreeRTOS
    echo.
)

REM Parse command line arguments
set BUILD_TYPE=Release
set BOARD=st-stm32f767zi-nucleo
set BUILD_APPS=ON
set CLEAN=false

:parse_args
if "%1"=="" goto :done_args
if "%1"=="--debug" (
    set BUILD_TYPE=Debug
    shift
    goto :parse_args
)
if "%1"=="--board" (
    set BOARD=%2
    shift
    shift
    goto :parse_args
)
if "%1"=="--no-apps" (
    set BUILD_APPS=OFF
    shift
    goto :parse_args
)
if "%1"=="--clean" (
    set CLEAN=true
    shift
    goto :parse_args
)
if "%1"=="--help" (
    echo Usage: %0 [options]
    echo Options:
    echo   --debug          Build with debug symbols
    echo   --board BOARD    Target board (default: st-stm32f767zi-nucleo)
    echo   --no-apps        Don't build applications
    echo   --clean          Clean build directory first
    echo   --help           Show this help message
    exit /b 0
)
shift
goto :parse_args

:done_args

REM Clean if requested
if "%CLEAN%"=="true" (
    echo Cleaning build directory...
    if exist "%BUILD_DIR%" rmdir /s /q "%BUILD_DIR%"
)

REM Create and configure build directory
if not exist "%BUILD_DIR%" (
    echo Creating build directory: %BUILD_DIR%
    mkdir "%BUILD_DIR%"
)

cd /d "%BUILD_DIR%"

echo.
echo Configuring CMake...
echo   Build Type:         %BUILD_TYPE%
echo   Target Board:       %BOARD%
echo   Build Applications: %BUILD_APPS%
echo.

cmake ^
    -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
    -DOPENMRN_BOARD=%BOARD% ^
    -DBUILD_APPLICATIONS=%BUILD_APPS% ^
    "%SCRIPT_DIR%"

echo.
echo Configuration complete!
echo.
echo To build, run:
echo   cd %BUILD_DIR%
echo   cmake --build .
echo.
echo To build a specific target:
echo   cmake --build . --target async_blink_%BOARD%
echo.

endlocal
