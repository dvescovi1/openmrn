# OpenMRN CMake Build System

This directory contains CMake build configuration for OpenMRN.

## Building with CMake

### Using CMake Presets (Recommended):

```bash
cmake --preset arm-cortex-m33
cmake --build build/arm-cortex-m33
```

### Manual configuration:

```bash
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/arm-cortex-m33.cmake
cmake --build .
```

**Note**: OpenMRN requires FreeRTOS and automatically defines `__FreeRTOS__`. Ensure your FreeRTOS kernel is available in your include paths.

### Build applications:

```bash
cmake --preset arm-cortex-m33 -DBUILD_APPLICATIONS=ON
cmake --build build/arm-cortex-m33
```

## Using OpenMRN in Your CMake Project

### Option 1: find_package (after installation)

```cmake
find_package(OpenMRN REQUIRED)
target_link_libraries(your_target PRIVATE OpenMRN::core)
```

### Option 2: add_subdirectory (as submodule)

```cmake
add_subdirectory(path/to/openmrn)
target_link_libraries(your_target PRIVATE OpenMRN::core)
```

### Option 3: FetchContent

```cmake
include(FetchContent)
FetchContent_Declare(
    OpenMRN
    GIT_REPOSITORY https://github.com/openmrn/openmrn.git
    GIT_TAG main
)
FetchContent_MakeAvailable(OpenMRN)
target_link_libraries(your_target PRIVATE OpenMRN::core)
```

## CMake Options

- `BUILD_APPLICATIONS` - Build OpenMRN applications (default: OFF)
- `BUILD_TESTS` - Build tests (default: OFF)
- `BUILD_SHARED_LIBS` - Build shared libraries instead of static (default: OFF)

## Supported Target

The build system supports:
- **ARM Cortex-M33 (ARMv8-M)** - STM32H5 series
- **Board**: ST STM32H563ZI Nucleo-144
- **Target**: freertos.armv8m (FreeRTOS only)

## Application Example

```cmake
openmrn_add_application(app_name
    SOURCES main.cxx
    BOARD st-stm32h563zi-nucleo
    TARGET freertos.armv8m
)
```

## Toolchain Files

Toolchain files are provided in `cmake/toolchains/`:
- `arm-none-eabi.cmake` - Generic ARM bare-metal base
- `arm-cortex-m33.cmake` - Cortex-M33 (ARMv8-M) for STM32H5

## CMake Presets

Use CMake presets for streamlined configuration:
- `arm-cortex-m33` - STM32H563ZI Nucleo with FreeRTOS

## Helper Functions
### openmrn_add_application

Creates an application executable:

```cmake
openmrn_add_application(app_name
    SOURCES main.cxx other.cxx
    BOARD board-name
    TARGET target-name
    LIBS additional-libs
)
```

### openmrn_configure_arm_cortex

Configures ARM Cortex-M settings:

```cmake
openmrn_configure_arm_cortex(target_name
    CPU cortex-m33
    FPU fpv5-sp-d16
    FLOAT_ABI hard
)
```

### openmrn_configure_stm32

Configures STM32 board:

```cmake
openmrn_configure_stm32(target_name
    CHIP H5
    BOARD BOARD_STM32H563ZI_NUCLEO
    SOURCES hwInit.cxx
    INCLUDES /path/to/includes
)
```

### openmrn_add_board

Adds board-specific files:

```cmake
openmrn_add_board(target_name board-path
    SOURCES additional-sources.c
)
```

## Examples

### Build OpenMRN Library:
```bash
cmake --preset arm-cortex-m33
cmake --build build/arm-cortex-m33
```

### Build with Applications:
```bash
cmake --preset arm-cortex-m33 -DBUILD_APPLICATIONS=ON
cmake --build build/arm-cortex-m33
```

### Using in VS Code:
1. Open Command Palette (Ctrl+Shift+P)
2. Run: **CMake: Configure**
3. Select preset: **arm-cortex-m33**
4. Run: **CMake: Build**

## Requirements

- **CMake**: 3.14 or newer
- **ARM Toolchain**: arm-none-eabi-gcc (e.g., from STM32CubeCLT)
- **FreeRTOS**: Required for all builds
- **STM32CubeH5**: Required for STM32H5 HAL drivers

## Troubleshooting

### Compiler test failures
If you see "The C compiler is not able to compile a simple test program":
- Delete the `build/` directory: `rm -rf build`
- Reconfigure: `cmake --preset arm-cortex-m33`

### Missing ARM toolchain
Ensure arm-none-eabi-gcc is in PATH or use STM32CubeCLT.

### VS Code CMake extension issues
- Run: **Developer: Reload Window**
- Delete cache: **CMake: Delete Cache and Reconfigure**