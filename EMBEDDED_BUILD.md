# Building OpenMRN for Embedded ARM Targets

OpenMRN is now configured as a **static library** for embedded ARM targets with **FreeRTOS** or **CMSIS RTOS v2** support.

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

**OpenMRN compiles .cxx source files into a static library** that you link with your firmware. The library adapts to **FreeRTOS** or **CMSIS RTOS v2** based on your defines.

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
   - **CPU flags** (Cortex-M0/M3/M4/M7/M33, etc.)
   - **FPU settings**
   - Custom compiler/linker flags
   - Target-specific definitions

3. **Important:** The toolchain file must include these critical settings:
```cmake
set(CMAKE_SYSTEM_NAME Generic)              # Bare-metal target
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)  # Skip executable tests
```

4. Use your toolchain when configuring:
```bash
cmake -DCMAKE_TOOLCHAIN_FILE=cmake/my-board.toolchain.cmake ..
cmake --build .
```

### Common Issues

**CMake tries to link with Windows/Linux libraries:** Make sure your toolchain file sets `CMAKE_SYSTEM_NAME` to `Generic` and `CMAKE_TRY_COMPILE_TARGET_TYPE` to `STATIC_LIBRARY`.

**Compiler check fails:** This is normal for bare-metal targets. The `CMAKE_TRY_COMPILE_TARGET_TYPE` setting bypasses executable linking tests.

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

The easiest way to integrate OpenMRN is using CMake's `FetchContent` module. This automatically downloads and makes the library available without manual cloning.

#### Complete Example

Create a `CMakeLists.txt` for your firmware project:

```cmake
cmake_minimum_required(VERSION 3.14)

# IMPORTANT: Set toolchain BEFORE project() command
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/arm-m33.toolchain.cmake)

project(MyFirmware C CXX ASM)

# Enable FetchContent
include(FetchContent)

# Declare OpenMRN dependency
FetchContent_Declare(
    openmrn
    GIT_REPOSITORY https://github.com/yourusername/openmrn.git
    GIT_TAG        main           # Use 'main', a tag like 'v1.0', or specific commit SHA
    GIT_SHALLOW    TRUE           # Optional: faster clone
    SOURCE_DIR     ${CMAKE_BINARY_DIR}/_deps/openmrn-src  # Optional: control location
)

# Make OpenMRN available (downloads if needed)
FetchContent_MakeAvailable(openmrn)

# Your firmware executable
add_executable(firmware.elf
    src/main.cpp
    src/hardware_init.c
    src/application.cpp
    # ... your other sources
)

# Link with OpenMRN - this gives you all headers and compile settings
target_link_libraries(firmware.elf 
    PRIVATE 
        OpenMRN::openmrn
)

# Optional: Add your own compile definitions
target_compile_definitions(firmware.elf PRIVATE
    MY_BOARD_VERSION=3
    USE_FREERTOS=1
)

# Link script for your MCU
set_target_properties(firmware.elf PROPERTIES
    LINK_FLAGS "-T${CMAKE_SOURCE_DIR}/linker/STM32F767ZI.ld -Wl,-Map=firmware.map"
)

# Generate hex and bin files for flashing
add_custom_command(TARGET firmware.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:firmware.elf> firmware.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:firmware.elf> firmware.bin
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:firmware.elf>
    COMMENT "Generating flash images and showing size"
    VERBATIM
)
```

#### Project Structure with FetchContent

```
your-firmware/
├── CMakeLists.txt          # Your main build file (shown above)
├── cmake/
│   └── arm-m33.toolchain.cmake  # Your toolchain file
├── src/
│   ├── main.cpp
│   ├── hardware_init.c
│   └── application.cpp
├── linker/
│   └── STM32F767ZI.ld
└── build/                  # Created when you run cmake
    └── _deps/
        └── openmrn-src/    # OpenMRN downloaded here automatically
```

#### Building Your Project

```bash
# Create build directory
mkdir build && cd build

# Configure (downloads OpenMRN automatically)
cmake ..

# Build
cmake --build .

# Flash files are ready
ls firmware.hex firmware.bin
```

#### Benefits of FetchContent

- ✅ **No manual cloning** - CMake downloads OpenMRN automatically
- ✅ **Version pinning** - Use specific tags or commits via `GIT_TAG`
- ✅ **Offline builds** - Downloaded once, cached in `_deps/`
- ✅ **Clean separation** - Your project stays independent
- ✅ **Easy updates** - Change `GIT_TAG` and rebuild

### Option 2: As Git Submodule

If you prefer manual control:

```bash
# Add OpenMRN as submodule
cd your-firmware
git submodule add https://github.com/yourusername/openmrn.git libs/openmrn
git submodule update --init --recursive
```

Then in your `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.14)

# Set toolchain before project()
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/arm-m33.toolchain.cmake)

project(MyFirmware C CXX ASM)

# Add OpenMRN as subdirectory
add_subdirectory(libs/openmrn)

# Your firmware executable
add_executable(firmware.elf
    src/main.cpp
    src/hardware_init.c
)

# Link with OpenMRN
target_link_libraries(firmware.elf PRIVATE OpenMRN::openmrn)

# Link script
set_target_properties(firmware.elf PROPERTIES
    LINK_FLAGS "-T${CMAKE_SOURCE_DIR}/linker/device.ld"
)

# Generate flash files
add_custom_command(TARGET firmware.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:firmware.elf> firmware.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:firmware.elf> firmware.bin
)
```

### Option 3: Local Directory

If OpenMRN is already checked out separately:

### Option 3: Local Directory

If OpenMRN is already checked out separately:

```cmake
cmake_minimum_required(VERSION 3.14)
project(MyFirmware C CXX ASM)

# Add OpenMRN from a local path
add_subdirectory(/path/to/openmrn ${CMAKE_BINARY_DIR}/openmrn-build)

# Link with OpenMRN
target_link_libraries(firmware.elf PRIVATE OpenMRN::openmrn)
```

---

## Quick Start Examples

### Minimal FetchContent Example

The absolute minimum to get started:

```cmake
cmake_minimum_required(VERSION 3.14)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/arm.toolchain.cmake)
project(MinimalFirmware C CXX ASM)

include(FetchContent)
FetchContent_Declare(openmrn GIT_REPOSITORY https://github.com/yourusername/openmrn.git GIT_TAG main)
FetchContent_MakeAvailable(openmrn)

add_executable(app.elf src/main.cpp)
target_link_libraries(app.elf PRIVATE OpenMRN::openmrn)
```

### Using a Specific Version

```cmake
FetchContent_Declare(
    openmrn
    GIT_REPOSITORY https://github.com/yourusername/openmrn.git
    GIT_TAG        v2.1.0        # Use a specific release tag
    GIT_SHALLOW    TRUE          # Only download that version
)
```

### Offline Development

After the first build, CMake caches OpenMRN in `build/_deps/`. To force re-download:

```bash
rm -rf build/_deps/openmrn-*
cmake ..
```

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
