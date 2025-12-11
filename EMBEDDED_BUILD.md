# Building OpenMRN for Embedded ARM Targets

OpenMRN is now configured as a static library for embedded ARM targets only.

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

### 2. Configure and Build

```bash
# Create build directory
mkdir build-arm && cd build-arm

# Configure with ARM toolchain
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/arm-none-eabi.toolchain.cmake.example ..

# Build
cmake --build .
```

The result will be `libopenmrn.a` in `build-arm/src/`.

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

The library is built with:
- **No exceptions** (`-fno-exceptions`)
- **No RTTI** (`-fno-rtti`)
- **Static linking**
- **FreeRTOS driver support included**
- **Optimized for size** (Release build)

## Build Options

### Debug Build
```bash
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../cmake/arm.toolchain.cmake ..
```

### Release Build (Size Optimized)
```bash
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=../cmake/arm.toolchain.cmake ..
```

### MinSizeRel (Minimum Size)
```bash
cmake -DCMAKE_BUILD_TYPE=MinSizeRel -DCMAKE_TOOLCHAIN_FILE=../cmake/arm.toolchain.cmake ..
```

## Excluded Components

The following are **NOT** built:
- ❌ Applications (hub, bootloader_client, etc.)
- ❌ Tests
- ❌ Documentation
- ❌ Linux/POSIX specific code (sockets, avahi, etc.)

Only the core embedded library is built.

## Verifying the Build

```bash
# Check library was created
ls -lh src/libopenmrn.a

# Check symbols
arm-none-eabi-nm src/libopenmrn.a | grep -i "openlcb"

# Check architecture
arm-none-eabi-readelf -h src/libopenmrn.a
```

## Memory Footprint

Approximate flash usage (varies by configuration):
- **Core library**: ~200-300 KB
- **With FreeRTOS drivers**: ~250-350 KB
- **Full OpenLCB stack**: ~300-400 KB

RAM usage depends on:
- Buffer pool sizes
- Number of nodes
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
**Library Type:** Static (.a)
