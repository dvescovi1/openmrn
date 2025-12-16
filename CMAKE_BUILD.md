# OpenMRN CMake Build System

This directory contains CMake build configuration for OpenMRN.

## Building with CMake

### Basic build (Linux/macOS):

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Install to system:

```bash
cmake --build . --target install
```

### Build with tests:

```bash
cmake .. -DBUILD_TESTS=ON
cmake --build .
ctest
```

### Cross-compile for ARM (requires toolchain):

```bash
cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/arm-cortex-m33.cmake \
         -DENABLE_FREERTOS=ON \
         -DFREERTOSPATH=/path/to/freertos
cmake --build .
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

- `BUILD_APPLICATIONS` - Build OpenMRN applications (default: ON)
- `BUILD_TESTS` - Build tests (default: OFF)
- `BUILD_SHARED_LIBS` - Build shared libraries instead of static (default: OFF)
- `ENABLE_FREERTOS` - Enable FreeRTOS support (default: OFF, requires FREERTOSPATH)

## Target Platforms

The build system supports:
- Linux x86/x64
- macOS
- ARM Cortex-M4 (ARMv7-M)
- ARM Cortex-M33 (ARMv8-M)
- TI TM4C129
- STM32F7/H5

## Board Configuration

When building for embedded targets, specify board:

```cmake
openmrn_add_application(my_app
    SOURCES main.cxx
    BOARD st-stm32h563zi-nucleo
    TARGET freertos.armv8m
)
```

## Toolchain Files

Toolchain files are provided in `cmake/toolchains/`:
- `arm-none-eabi.cmake` - Generic ARM bare-metal
- `arm-cortex-m4.cmake` - Cortex-M4 (ARMv7-M)
- `arm-cortex-m33.cmake` - Cortex-M33 (ARMv8-M)

## Compatibility

The CMake build system runs alongside the existing Makefile system.
Both can coexist during the transition period.

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

### Build for Linux:
```bash
mkdir build && cd build
cmake ..
make
```

### Build for STM32H563ZI Nucleo:
```bash
mkdir build && cd build
cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/arm-cortex-m33.cmake \
  -DSTM32CUBEH5PATH=/path/to/STM32CubeH5 \
  -DENABLE_FREERTOS=ON \
  -DFREERTOSPATH=/path/to/FreeRTOS
make
```

### Install OpenMRN:
```bash
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local
make
sudo make install
```

## Troubleshooting

### Missing pthread
If you get pthread errors on Linux, install:
```bash
sudo apt-get install libpthread-stubs0-dev
```

### Missing ARM toolchain
Install arm-none-eabi-gcc:
```bash
sudo apt-get install gcc-arm-none-eabi
```

### CMake version too old
OpenMRN requires CMake 3.14 or newer:
```bash
cmake --version
```
