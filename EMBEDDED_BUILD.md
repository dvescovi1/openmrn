# Building OpenMRN for Embedded ARM Targets

OpenMRN is now configured as a **header-only library** for embedded ARM targets.

## Quick Start

### 1. Install ARM Toolchain

**Ubuntu/Debian:**
```bash
sudo apt-get install gcc-arm-none-eabi
```

**macOS:**
```bash
brew install --cask gcc-arm-embedded
```

**Windows:**
Download from [ARM Developer](https://developer.arm.com/downloads/-/gnu-rm)

### 2. Use in Your Project

**No separate build needed!** Simply include OpenMRN in your firmware project and the headers will be available.

```bash
# Option 1: Clone as subdirectory
cd your-firmware-project
git submodule add https://github.com/bakerstu/openmrn.git libs/openmrn

# Option 2: Use FetchContent (see below)
```

## Customizing for Your Target

1. Copy the example toolchain file:
```bash
cp cmake/arm-none-eabi.toolchain.cmake.example cmake/my-board.toolchain.cmake
```

2. Edit `cmake/my-board.toolchain.cmake` to match your target:
   - CPU flags (Cortex-M0/M3/M4/M7/M33, etc.)
   - FPU settings
   - Memory layout
   - Custom definitions

3. Build with your toolchain:
```bash
cmake -DCMAKE_TOOLCHAIN_FILE=cmake/my-board.toolchain.cmake ..
cmake --build .
```

## Target-Specific Customization

The default toolchain is configured for **Cortex-M33**. You can customize for other targets:

### Cortex-M0/M0+
```cmake
set(CPU_FLAGS "-mcpu=cortex-m0 -mthumb")
```

### Cortex-M3
```cmake
set(CPU_FLAGS "-mcpu=cortex-m3 -mthumb")
```

### Cortex-M4 (no FPU)
```cmake
set(CPU_FLAGS "-mcpu=cortex-m4 -mthumb")
```

### Cortex-M4 (with FPU)
```cmake
set(CPU_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16")
```

### Cortex-M7 (with FPU)
```cmake
set(CPU_FLAGS "-mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16")
```

### Cortex-M33 (with FPU) - Default
```cmake
set(CPU_FLAGS "-mcpu=cortex-m33 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16")
```

## Using the Library in Your Firmware

### Option 1: Using FetchContent (Recommended)

Automatically download and build OpenMRN as part of your firmware project:

```cmake
cmake_minimum_required(VERSION 3.14)

# Set toolchain before project()
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/arm-m33.toolchain.cmake)

project(MyFirmware C CXX ASM)

# Fetch OpenMRN from Git repository
include(FetchContent)

FetchContent_Declare(
    openmrn
    GIT_REPOSITORY https://github.com/bakerstu/openmrn.git
    GIT_TAG        master  # or specific commit/tag
)

FetchContent_MakeAvailable(openmrn)

# Your firmware executable
add_executable(firmware.elf
    src/main.cpp
    src/hardware_init.c
    # ... your sources
)

# Link with OpenMRN
target_link_libraries(firmware.elf PRIVATE OpenMRN::openmrn)

# Link script
set_target_properties(firmware.elf PROPERTIES
    LINK_FLAGS "-T${CMAKE_SOURCE_DIR}/linker/device.ld"
)

# Generate hex and bin files
add_custom_command(TARGET firmware.elf POST_BUILD
    COMMAND arm-none-eabi-objcopy -O ihex firmware.elf firmware.hex
    COMMAND arm-none-eabi-objcopy -O binary firmware.elf firmware.bin
    COMMENT "Generating hex and bin files"
)
```

### Option 2: As Subdirectory

If you have OpenMRN checked out locally:

```cmake
cmake_minimum_required(VERSION 3.14)
project(MyFirmware C CXX ASM)

# Add OpenMRN as subdirectory
add_subdirectory(libs/openmrn)

# Your firmware executable
add_executable(firmware.elf
    src/main.cpp
    src/hardware_init.c
    # ... your sources
)

# Link with OpenMRN
target_link_libraries(firmware.elf PRIVATE OpenMRN::openmrn)

# Link script
set_target_properties(firmware.elf PROPERTIES
    LINK_FLAGS "-T${CMAKE_SOURCE_DIR}/linker/device.ld"
)

# Generate hex and bin files
add_custom_command(TARGET firmware.elf POST_BUILD
    COMMAND arm-none-eabi-objcopy -O ihex firmware.elf firmware.hex
    COMMAND arm-none-eabi-objcopy -O binary firmware.elf firmware.bin
    COMMENT "Generating hex and bin files"
)
```

## Library Configuration

The library is configured as **header-only** with:
- **No compilation required** - headers are included directly in your project
- **No exceptions** (`-fno-exceptions` when used)
- **No RTTI** (`-fno-rtti` when used)
- **All code inline** or in headers
- **FreeRTOS driver support included**
- **Zero build time for the library itself**

## Build Options

Since this is a header-only library, there are no build options. The library is configured via:
- Compiler definitions (set via `target_compile_definitions`)
- Toolchain selection
- Your project's build configuration

### Debug vs Release

The library headers adapt to your project's build type automatically through compiler flags.

## Excluded Components

The following are **NOT** built:
- ❌ Applications (hub, bootloader_client, etc.)
- ❌ Tests
- ❌ Documentation
- ❌ Linux/POSIX specific code (sockets, avahi, etc.)

Only the core embedded library is built.

## Verifying the Setup

```bash
# Check headers are available
ls libs/openmrn/src/openlcb/*.hxx
ls libs/openmrn/inc/*.h

# Build your project - OpenMRN headers will be included automatically
cmake --build .
```

## Memory Footprint

As a header-only library, memory footprint depends entirely on which headers and features you use:
- **Unused code is eliminated** by the linker automatically
- **Only what you include is compiled** into your firmware
- **Typical usage**: 200-400 KB flash (varies greatly by features used)

RAM usage depends on:
- Buffer pool sizes you configure
- Number of nodes you instantiate
- Stack configuration

## Troubleshooting

### Toolchain not found
```bash
# Verify toolchain is in PATH
which arm-none-eabi-gcc

# Or specify full path in toolchain file
set(CMAKE_C_COMPILER /usr/bin/arm-none-eabi-gcc)
```

### Wrong architecture
Check your CPU_FLAGS match your target MCU exactly.

### Linker errors
Ensure you provide a linker script appropriate for your target.

## Example Projects

See the `applications/` directory in the original repository for example firmware projects that use OpenMRN on embedded targets.

## Support

For embedded-specific questions:
- Review FreeRTOS driver implementations in `src/freertos_drivers/`
- Check application examples in `applications/` directory
- Consult OpenLCB documentation

---
**Default Target:** ARM Cortex-M33  
**Supported:** ARM Cortex-M series (M0/M0+/M3/M4/M7/M33)  
**Toolchain:** arm-none-eabi-gcc  
**RTOS:** FreeRTOS (optional)  
**Library Type:** Header-only (INTERFACE)
