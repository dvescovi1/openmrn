#!/bin/bash
# OpenMRN is now a header-only library - no build required!
# This script verifies the CMake configuration only.

echo "OpenMRN Header-Only Library"
echo "============================"
echo ""
echo "OpenMRN is a header-only library - no compilation is needed!"
echo ""
echo "To use OpenMRN in your firmware project:"
echo ""
echo "1. Add as git submodule or use FetchContent:"
echo "   git submodule add https://github.com/bakerstu/openmrn.git libs/openmrn"
echo ""
echo "2. In your CMakeLists.txt:"
echo "   add_subdirectory(libs/openmrn)"
echo "   target_link_libraries(your_target PRIVATE OpenMRN::openmrn)"
echo ""
echo "3. Build your firmware project normally"
echo ""
echo "See EMBEDDED_BUILD.md for complete instructions."
echo ""

# Verify CMake configuration works
if [ "$1" = "--verify" ]; then
    echo "Verifying CMake configuration..."
    mkdir -p build-verify
    cd build-verify
    cmake .. > /dev/null 2>&1
    
    if [ $? -eq 0 ]; then
        echo "✓ CMake configuration is valid"
        cd ..
        rm -rf build-verify
        exit 0
    else
        echo "✗ CMake configuration failed"
        cd ..
        rm -rf build-verify
        exit 1
    fi
fi
