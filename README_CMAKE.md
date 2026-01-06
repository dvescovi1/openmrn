# OpenMRN CMake Build System - Index

## Quick Links

- **[CMAKE_BUILD.md](CMAKE_BUILD.md)** - Quick start guide with build commands
- **[CMAKE_MIGRATION.md](CMAKE_MIGRATION.md)** - Detailed migration guide
- **[CMAKE_CONVERSION.md](CMAKE_CONVERSION.md)** - Summary of all changes made

## Getting Started

### 1. Requirements
- CMake 3.20 or later
- ARM GCC toolchain (arm-none-eabi-*)
- FreeRTOS source code

### 2. Set Environment Variables
```bash
# Linux/macOS
export ARMGCCPATH=/path/to/arm-none-eabi
export FREERTOSPATH=/path/to/FreeRTOS

# Windows
set ARMGCCPATH=C:\path\to\arm-none-eabi
set FREERTOSPATH=C:\path\to\FreeRTOS
```

### 3. Build the Project
```bash
# Using the convenience script
./configure.sh          # Linux/macOS
configure.bat           # Windows

# Or manually
mkdir build
cd build
cmake ..
cmake --build .
```

## Configuration Options

### Board Selection
```bash
cmake -DOPENMRN_BOARD=st-stm32f767zi-nucleo ..    # Default (STM32F7)
cmake -DOPENMRN_BOARD=st-stm32f303re-nucleo ..
cmake -DOPENMRN_BOARD=st-stm32f303re-nucleo-dev-board ..
```

### Application Building
```bash
cmake -DBUILD_APPLICATIONS=ON ..   # Build apps (default)
cmake -DBUILD_APPLICATIONS=OFF ..  # Libraries only
```

### Build Type
```bash
cmake -DCMAKE_BUILD_TYPE=Release ..  # Optimized for size (default)
cmake -DCMAKE_BUILD_TYPE=Debug ..    # Debug symbols and minimal optimization
```

### Build Tests
```bash
cmake -DBUILD_TESTS=ON ..
cmake --build . --target tests
```

## Project Structure

```
OpenMRN/
├── CMakeLists.txt                    # Root CMake configuration
├── cmake/                             # CMake toolchain and config files
│   ├── toolchain-arm-gcc.cmake
│   ├── config-freertos.armv7m.cmake
│   └── config-bare.armv7m.cmake
├── src/                               # Core libraries (all built by default)
│   ├── console/
│   ├── utils/
│   ├── executor/
│   ├── os/
│   ├── dcc/
│   └── openlcb/
├── applications/                      # Application targets (optional)
│   ├── async_blink/
│   ├── blink_raw/
│   ├── send_datagram/
│   ├── simple_client/
│   ├── io_board/
│   └── nucleo_io/
├── boards/                            # Board-specific configurations
│   ├── st-stm32f767zi-nucleo/
│   ├── st-stm32f303re-nucleo/
│   └── st-stm32f303re-nucleo-dev-board/
├── include/                           # Public header files
├── targets/                           # Test and build targets
├── build/                             # Build outputs (generated)
├── configure.sh                       # Linux/macOS build script
├── configure.bat                      # Windows build script
├── CMAKE_BUILD.md                     # Build documentation
├── CMAKE_MIGRATION.md                 # Migration guide
└── CMAKE_CONVERSION.md                # Conversion summary
```

## Build Outputs

After building, outputs are located in the build directory:

```
build/
├── bin/                               # Executables
│   ├── *.elf                          # ELF files
│   ├── *.hex                          # Intel HEX files (for programming)
│   └── *.map                          # Link maps (for debugging)
├── lib/                               # Static libraries
│   ├── libutils.a
│   ├── libexecutor.a
│   ├── libos.a
│   ├── libopenIcb.a
│   ├── libdcc.a
│   ├── libconsole.a
│   └── libfreertos.a
└── CMakeCache.txt                     # CMake configuration cache
```

## Frequently Asked Questions

### Q: How do I build for a different board?
```bash
cmake -DOPENMRN_BOARD=st-stm32f303re-nucleo ../
cmake --build .
```

### Q: How do I build only the libraries without applications?
```bash
cmake -DBUILD_APPLICATIONS=OFF ../
cmake --build .
```

### Q: How do I clean the build?
```bash
cmake --build . --target clean
# Or remove and recreate the build directory
rm -rf ../build && mkdir ../build
```

### Q: How do I build a specific application?
```bash
cmake --build . --target async_blink_st-stm32f767zi-nucleo
```

### Q: What if CMake can't find the ARM toolchain?
Set the `ARMGCCPATH` environment variable:
```bash
export ARMGCCPATH=/path/to/arm-none-eabi
```

### Q: What if CMake can't find FreeRTOS?
Set the `FREERTOSPATH` environment variable:
```bash
export FREERTOSPATH=/path/to/FreeRTOS
```

## Changes from Make to CMake

### What Was Removed
- All `Makefile` files (~80+ files)
- All `.mk` configuration files (~33 files)
- The recursive Make build system

### What Was Added
- CMake configuration files (16 `CMakeLists.txt` files)
- Toolchain definition for ARM GCC
- Platform-specific configuration files
- Build convenience scripts (Bash and Batch)
- Comprehensive documentation

### What Was Preserved
- All compiler flags and optimization settings
- FreeRTOS integration (supports both old and new structures)
- Multi-board support
- Multi-platform support
- All build outputs and artifacts

## Support

For more detailed information, see:
- **[CMAKE_BUILD.md](CMAKE_BUILD.md)** - Common build commands and examples
- **[CMAKE_MIGRATION.md](CMAKE_MIGRATION.md)** - Complete migration guide

## Additional Resources

- [CMake Documentation](https://cmake.org/documentation/)
- [GNU ARM Toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm)
- [FreeRTOS](https://www.freertos.org/)
- [OpenMRN Project](https://github.com/openmrn/openmrn)

---

**Last Updated:** January 5, 2026
**CMake Version:** 3.20+
**Status:** ✅ Ready for use
