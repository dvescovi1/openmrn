# CMake Migration Guide for OpenMRN

This document describes the transition of the OpenMRN project from Make-based build system to CMake.

## Overview

The OpenMRN project has been converted from a Make-based build system (with 642+ Makefiles) to a modern CMake-based build system. This provides better cross-platform support, improved dependency management, and easier integration with modern IDEs.

## Build Structure

### Directory Layout

```
openmrn/
├── CMakeLists.txt           # Root CMakeLists.txt
├── cmake/                   # CMake modules and configuration
│   ├── default.cmake        # Default compiler flags and options
│   ├── openmrn_library.cmake  # Helper functions for library creation
│   └── <target>.cmake       # Target-specific configurations
├── src/                     # Source code (core libraries)
├── include/                 # Public header files
├── targets/                 # Target-specific builds
│   └── freertos.armv7m/    # FreeRTOS ARM v7-M target
├── applications/            # Application projects
└── doc/                     # Documentation
```

### Available Targets

The only supported target architecture in this tree is:

- **freertos.armv7m** - FreeRTOS ARM v7-M (default)

## Building

### Basic Configuration and Build

```bash
# Configure for default target (freertos.armv7m)
cmake -B build

# Build all libraries
cmake --build build

# Build with verbose output
cmake --build build -- VERBOSE=1

# Clean build artifacts
cmake --build build --target clean
```

### Build Options

```bash
# Enable testing
cmake -B build -DBUILD_TESTING=ON

# Enable Link Time Optimization
cmake -B build -DENABLE_LTO=ON

# Build static libraries (default)
cmake -B build -DBUILD_SHARED_LIBS=OFF

# Build shared libraries
cmake -B build -DBUILD_SHARED_LIBS=ON
```

### Using with IDE

#### Visual Studio Code
```bash
cmake -B build -G "Unix Makefiles"  # Or "Ninja" if Ninja is installed
```

#### Visual Studio
```bash
cmake -B build -G "Visual Studio 17 2022"
```

#### Xcode
```bash
cmake -B build -G "Xcode"
```

## Project Structure

### Adding a New Application

Create a new directory in `applications/` with a `CMakeLists.txt`:

```cmake
project(my_app)

# Find and link required libraries
find_package(openmrn REQUIRED)

# Create executable
add_executable(my_app main.cxx)

# Link libraries
target_link_libraries(my_app PRIVATE
    openlcb
    executor
    os_main
)

# Set include directories
target_include_directories(my_app PRIVATE
    ${OPENMRNPATH}/include
    ${OPENMRNPATH}/src
)
```

### Adding a New Target Architecture

1. Create a new directory in `targets/` (e.g., `targets/my_target/`)
2. Create `CMakeLists.txt` in each module subdirectory
3. Set target-specific compiler flags in `cmake/my_target.cmake`

## Library Organization

Core libraries are organized by module:

- **core** - Core OpenMRN functionality
- **os** - Operating system abstractions
- **executor** - Async execution framework
- **dcc** - DCC (Digital Command Control) support
- **openlcb** - OpenLCB protocol implementation
- **utils** - Utility functions
- **withrottle** - WiThrottle support
- **if** - Interface layer
- **console** - Console interface
- **ble** - Bluetooth Low Energy support
- **cue** - CUE protocol support
- **traction_modem** - Traction modem support

## Migration Notes

### What Changed

1. **Removed**: All 642+ Makefiles
2. **Removed**: `etc/` directory (Make configuration files)
3. **Added**: Root `CMakeLists.txt`
4. **Added**: Target-specific `CMakeLists.txt` files
5. **Added**: CMake helper modules in `cmake/` directory

### Backwards Compatibility

- Build artifacts are in `build/` directory instead of target-specific directories
- Library files are in `build/lib/` instead of scattered throughout `targets/`
- Binaries are in `build/bin/` instead of scattered throughout `targets/`

### Known Limitations

### Known Limitations

- Only the freertos.armv7m target is provided in this tree
- Some advanced Make features may need to be implemented in CMake
- Test infrastructure is still being migrated

## Troubleshooting

### CMake not finding compiler

Specify the compiler explicitly:
```bash
cmake -B build -DCMAKE_C_COMPILER=/path/to/gcc -DCMAKE_CXX_COMPILER=/path/to/g++
## Future Improvements

1. Implement automated test discovery and execution
2. Create package configuration files for library dependencies
3. Add support for more IDEs and build systems
4. Create binary packaging targets
```

### Cross-compilation for ARM

Use a CMake toolchain file:
```bash
cmake -B build -DCMAKE_TOOLCHAIN_FILE=cmake/arm-toolchain.cmake
```

### Dependency not found

Check that the required development packages are installed:
- Linux: `apt-get install build-essential`

- Windows: Install a C++ compiler (Visual Studio, MinGW, etc.)

## Future Improvements

1. Create CMakeLists.txt files for remaining target architectures
2. Implement automated test discovery and execution
3. Create package configuration files for library dependencies
4. Add support for more IDEs and build systems
5. Implement Doxygen documentation generation
6. Create binary packaging targets

## References

- [CMake Documentation](https://cmake.org/cmake/help/latest/)
- [OpenMRN GitHub Repository](https://github.com/bakerstu/openmrn/)
- [OpenLCB Documentation](http://openlcb.org/)
