#!/bin/bash
# Quick build script for OpenMRN Embedded ARM Library

set -e

# Default values
BUILD_TYPE="Release"
BUILD_DIR="build-arm"
CLEAN_BUILD=false
TOOLCHAIN=""

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        --clean)
            CLEAN_BUILD=true
            shift
            ;;
        --toolchain)
            TOOLCHAIN="$2"
            shift 2
            ;;
        --help)
            echo "OpenMRN Embedded ARM Library Build Script"
            echo ""
            echo "Usage: $0 [options]"
            echo ""
            echo "Options:"
            echo "  --debug                Build in Debug mode (default: Release)"
            echo "  --clean                Clean build directory before building"
            echo "  --toolchain <file>     Use specific CMake toolchain file"
            echo "  --help                 Show this help message"
            echo ""
            echo "Examples:"
            echo "  $0"
            echo "  $0 --debug --clean"
            echo "  $0 --toolchain cmake/arm-none-eabi.toolchain.cmake.example"
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            echo "Use --help for usage information"
            exit 1
            ;;
    esac
done

# Clean build if requested
if [ "$CLEAN_BUILD" = true ]; then
    echo "Cleaning build directory..."
    rm -rf "$BUILD_DIR"
fi

# Create build directory
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure
echo "Configuring CMake for Embedded ARM Library (BUILD_TYPE=$BUILD_TYPE)..."
if [ -n "$TOOLCHAIN" ]; then
    echo "Using toolchain: $TOOLCHAIN"
    cmake .. -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -DCMAKE_TOOLCHAIN_FILE="../$TOOLCHAIN"
else
    cmake .. -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
fi

# Build
echo "Building embedded library..."
cmake --build . -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

echo ""
echo "Build completed successfully!"
echo "Build directory: $BUILD_DIR"
echo "Build type: $BUILD_TYPE"
echo "Library: $BUILD_DIR/src/libopenmrn.a"
echo ""
echo "To use in your firmware project:"
echo "  target_link_libraries(your_target PRIVATE OpenMRN::openmrn)"
