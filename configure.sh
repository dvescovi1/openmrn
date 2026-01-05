#!/bin/bash
# Quick start script for OpenMRN CMake build

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

echo "OpenMRN CMake Build System - Quick Start"
echo "========================================"
echo ""

# Check for CMake
if ! command -v cmake &> /dev/null; then
    echo "ERROR: CMake not found. Please install CMake 3.20 or later."
    exit 1
fi

echo "CMake version: $(cmake --version | head -1)"
echo ""

# Check for ARM toolchain
if [ -z "$ARMGCCPATH" ]; then
    if command -v arm-none-eabi-gcc &> /dev/null; then
        echo "Found ARM GCC in PATH"
    else
        echo "ERROR: ARM GCC toolchain not found."
        echo "Please set ARMGCCPATH environment variable:"
        echo "  export ARMGCCPATH=/path/to/arm-none-eabi"
        exit 1
    fi
else
    echo "ARM GCC path: $ARMGCCPATH"
fi

echo ""

# Check for FreeRTOS
if [ -z "$FREERTOSPATH" ]; then
    echo "WARNING: FREERTOSPATH not set. FreeRTOS build may fail."
    echo "Set it with: export FREERTOSPATH=/path/to/FreeRTOS"
    echo ""
fi

# Parse command line arguments
BUILD_TYPE="Release"
BOARD="st-stm32f767zi-nucleo"
BUILD_APPS="ON"
CLEAN=false

while [[ $# -gt 0 ]]; do
    case $1 in
        --debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        --board)
            BOARD="$2"
            shift 2
            ;;
        --no-apps)
            BUILD_APPS="OFF"
            shift
            ;;
        --clean)
            CLEAN=true
            shift
            ;;
        --help)
            echo "Usage: $0 [options]"
            echo "Options:"
            echo "  --debug          Build with debug symbols"
            echo "  --board BOARD    Target board (default: st-stm32f767zi-nucleo)"
            echo "  --no-apps        Don't build applications"
            echo "  --clean          Clean build directory first"
            echo "  --help           Show this help message"
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            exit 1
            ;;
    esac
done

# Clean if requested
if [ "$CLEAN" = true ]; then
    echo "Cleaning build directory..."
    rm -rf "$BUILD_DIR"
fi

# Create and configure build directory
if [ ! -d "$BUILD_DIR" ]; then
    echo "Creating build directory: $BUILD_DIR"
    mkdir -p "$BUILD_DIR"
fi

cd "$BUILD_DIR"

echo ""
echo "Configuring CMake..."
echo "  Build Type:        $BUILD_TYPE"
echo "  Target Board:      $BOARD"
echo "  Build Applications: $BUILD_APPS"
echo ""

cmake \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DOPENMRN_BOARD="$BOARD" \
    -DBUILD_APPLICATIONS="$BUILD_APPS" \
    "$SCRIPT_DIR"

echo ""
echo "Configuration complete!"
echo ""
echo "To build, run:"
echo "  cd $BUILD_DIR"
echo "  cmake --build ."
echo ""
echo "To build a specific target:"
echo "  cmake --build . --target async_blink_${BOARD}"
echo ""
