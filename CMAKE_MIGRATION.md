# OpenMRN CMake Migration Guide

This document describes the conversion of the OpenMRN project from a Make-based build system to CMake.

## What Has Changed

### Build System
- **Old**: Recursive Makefiles with .mk configuration files
- **New**: CMake-based build system with cleaner configuration

### Removed Files
- All `Makefile` files throughout the project
- All `.mk` configuration files in the `etc/` directory
- Build configuration is now handled by CMake configuration files in the `cmake/` directory

### New Structure
```
cmake/
├── toolchain-arm-gcc.cmake         # ARM GCC toolchain definition
├── config-freertos.armv7m.cmake    # FreeRTOS ARM v7-M configuration
└── config-bare.armv7m.cmake        # Bare-metal ARM v7-M configuration
```

## Key Features

### 1. Optional Application Building
Applications are now **optional** and can be enabled/disabled via the `BUILD_APPLICATIONS` CMake option:

```bash
# Build without applications (libraries only)
cmake -DBUILD_APPLICATIONS=OFF ..

# Build with applications (default)
cmake -DBUILD_APPLICATIONS=ON ..
```

### 2. Default Configuration
The default configuration uses:
- **Target Board**: `st-stm32f767zi-nucleo` (STM32F7 Nucleo)
- **Target Platform**: `freertos.armv7m` (FreeRTOS on ARM v7-M)
- **Toolchain**: ARM GCC (auto-detected from ARMGCCPATH or PATH)

### 3. Multiple Board Support
Easy switching between supported boards:

```bash
cmake -DOPENMRN_BOARD=st-stm32f303re-nucleo ..
cmake -DOPENMRN_BOARD=st-stm32f767zi-nucleo ..
cmake -DOPENMRN_BOARD=st-stm32f303re-nucleo-dev-board ..
```

### 4. Platform Targets
Support for multiple platform configurations:

```bash
cmake -DOPENMRN_TARGET=freertos.armv7m ..  # FreeRTOS (default)
cmake -DOPENMRN_TARGET=bare.armv7m ..      # Bare metal
```

## Migration from Make to CMake

### For Developers

If you were previously building with Make:

```bash
# Old way
make -C applications/async_blink

# New way
mkdir build
cd build
cmake ..
cmake --build . --target async_blink_st-stm32f767zi-nucleo
```

### Build Directory

The CMake build system uses an out-of-source build directory (recommended practice):

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

This keeps the source tree clean and allows multiple build configurations simultaneously:

```bash
mkdir build-release
mkdir build-debug
mkdir build-f303
```

### Build Artifacts

Build outputs are now organized:
- **Executables**: `build/bin/`
- **Libraries**: `build/lib/`
- **ELF files**: `build/bin/*.elf`
- **Hex files**: `build/bin/*.hex` (auto-generated)
- **Map files**: `build/bin/*.map` (auto-generated)
- **Object files**: `build/` (in respective directories)

## Compiler Flags Preservation

All original compiler flags from the Make-based system have been preserved:

- ARM GCC compiler settings (optimization, warnings, architecture flags)
- FreeRTOS-specific defines (`__FreeRTOS__`, `GCC_ARMCM3`)
- Linker script integration
- Object file generation options
- Symbol stripping options

## FreeRTOS Integration

The CMake system automatically:
1. Detects FreeRTOS installation from `FREERTOSPATH` environment variable
2. Supports both old (Source/) and new (FreeRTOS-Kernel/) directory structures
3. Compiles FreeRTOS as a static library
4. Links it automatically to all applications

## Build Customization

### Custom Build Types

```bash
# Release (default, optimized for size with -Os)
cmake -DCMAKE_BUILD_TYPE=Release ..

# Debug (with debug symbols and minimal optimization)
cmake -DCMAKE_BUILD_TYPE=Debug ..
```

### Custom Compiler Paths

If ARM GCC is not in your PATH:

```bash
export ARMGCCPATH=/path/to/arm-none-eabi
cmake ..
```

Or specify directly to CMake:

```bash
cmake -DCMAKE_C_COMPILER=/path/to/arm-none-eabi/bin/arm-none-eabi-gcc \
      -DCMAKE_CXX_COMPILER=/path/to/arm-none-eabi/bin/arm-none-eabi-g++ \
      ..
```

## Advantages of CMake

1. **Cleaner Syntax**: CMake files are more readable than recursive Makefiles
2. **IDE Integration**: Generates IDE project files (Visual Studio, XCode, etc.)
3. **Cross-Platform**: Same build files work on Windows, Linux, macOS
4. **Better Dependency Management**: Automatic dependency tracking
5. **Flexibility**: Easy to add custom build rules and targets
6. **Maintainability**: Single set of build rules for all configurations
7. **Performance**: Faster incremental builds

## Adding New Applications

To add a new application to the build system:

1. Create a new directory under `applications/`
2. Add your source files
3. Add an entry to the `APPLICATION_DIRS` list in `applications/CMakeLists.txt`
4. The application will automatically be built for all supported boards

Example:

```cmake
# In applications/CMakeLists.txt
set(APPLICATION_DIRS
    async_blink
    blink_raw
    my_new_app        # Add here
    ...
)
```

## Adding New Libraries

To add a new library to `src/`:

1. Create a new directory under `src/`
2. Create a `CMakeLists.txt` file (see existing examples like `src/utils/CMakeLists.txt`)
3. Add it to the `CORELIBS` list in `src/CMakeLists.txt`

## Troubleshooting

### CMake not found
```bash
# Install CMake 3.20 or later
# Windows: Download from cmake.org or use chocolatey
# Linux: apt-get install cmake
# macOS: brew install cmake
```

### ARM toolchain not found
```bash
# Set ARMGCCPATH environment variable
export ARMGCCPATH=/path/to/arm-none-eabi
# Or install and add to PATH
```

### FreeRTOS not found
```bash
# Set FREERTOSPATH environment variable
export FREERTOSPATH=/path/to/FreeRTOS
```

### Clean build
```bash
cd build
cmake --build . --target clean
# Or remove and recreate build directory
rm -rf ../build
mkdir ../build
cd ../build
cmake ..
```

## See Also

- [CMAKE_BUILD.md](CMAKE_BUILD.md) - Quick start guide with common build commands
- Root [CMakeLists.txt](CMakeLists.txt) - Main CMake configuration
- `cmake/` directory - Toolchain and platform configurations
