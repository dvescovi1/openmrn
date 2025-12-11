#!/bin/bash
# Quick build script for OpenMRN using CMake

set -e

# Default values
BUILD_TYPE="Release"
BUILD_DIR="build"
CLEAN_BUILD=false
RUN_TESTS=false
BUILD_DOCS=false

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
        --test)
            RUN_TESTS=true
            shift
            ;;
        --docs)
            BUILD_DOCS=true
            shift
            ;;
        --help)
            echo "OpenMRN CMake Build Script"
            echo ""
            echo "Usage: $0 [options]"
            echo ""
            echo "Options:"
            echo "  --debug       Build in Debug mode (default: Release)"
            echo "  --clean       Clean build directory before building"
            echo "  --test        Run tests after building"
            echo "  --docs        Build documentation"
            echo "  --help        Show this help message"
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
echo "Configuring CMake (BUILD_TYPE=$BUILD_TYPE)..."
cmake .. -DCMAKE_BUILD_TYPE="$BUILD_TYPE"

# Build
echo "Building..."
cmake --build . -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

# Run tests if requested
if [ "$RUN_TESTS" = true ]; then
    echo "Running tests..."
    ctest --output-on-failure
fi

# Build documentation if requested
if [ "$BUILD_DOCS" = true ]; then
    echo "Building documentation..."
    cmake --build . --target docs
    echo "Documentation built in $BUILD_DIR/doc/html/"
fi

echo ""
echo "Build completed successfully!"
echo "Build directory: $BUILD_DIR"
echo "Build type: $BUILD_TYPE"
