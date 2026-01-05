# OpenMRN CMake Build System

This project has been converted from a Make-based build system to CMake.

## Build Requirements

- CMake 3.20 or later
- ARM GCC toolchain (arm-none-eabi-gcc)
- **FreeRTOS source code (REQUIRED)** - The source code has POSIX/FreeRTOS dependencies for OS abstraction

## Environment Setup

### Set ARM GCC Toolchain Path

Set the `ARMGCCPATH` environment variable to point to your ARM GCC installation:

```bash
# Windows
set ARMGCCPATH=C:\path\to\arm-none-eabi

# Linux/macOS
export ARMGCCPATH=/path/to/arm-none-eabi
```

### Set FreeRTOS Path

Set the `FREERTOSPATH` environment variable to point to your FreeRTOS installation:

```bash
# Windows
set FREERTOSPATH=C:\path\to\FreeRTOS

# Linux/macOS
export FREERTOSPATH=/path/to/FreeRTOS
```

## FreeRTOS Requirement

**This project requires FreeRTOS to compile.** The source code uses POSIX headers and abstractions provided by FreeRTOS:
- `endian.h` - Byte order conversion macros
- `semaphore.h` - POSIX semaphore definitions
- FreeRTOS OS kernel for OS abstraction layer

**To build successfully:**

1. Download FreeRTOS from https://www.freertos.org/
2. Set the `FREERTOSPATH` environment variable
3. Configure and build with CMake

Without FreeRTOS installed, the build will fail with "fatal error: endian.h: No such file or directory"

## Building

### Basic Build (with applications, default board: STM32F7)

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Build for Specific Board

```bash
mkdir build
cd build
cmake -DOPENMRN_BOARD=st-stm32f303re-nucleo ..
cmake --build .
```

### Build Without Applications

```bash
mkdir build
cd build
cmake -DBUILD_APPLICATIONS=OFF ..
cmake --build .
```

### Build with Tests

```bash
mkdir build
cd build
cmake -DBUILD_TESTS=ON ..
cmake --build .
```

### Build in Debug Mode

```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
```

## Available Options

- `OPENMRN_BOARD`: Target board (default: `st-stm32f767zi-nucleo`)
  - `st-stm32f767zi-nucleo`
  - `st-stm32f303re-nucleo`
  - `st-stm32f303re-nucleo-dev-board`

- `OPENMRN_TARGET`: Target platform (default: `freertos.armv7m`)
  - `freertos.armv7m` - FreeRTOS on ARM v7-M

- `BUILD_APPLICATIONS`: Build application targets (default: `ON`)

- `BUILD_TESTS`: Build test targets (default: `OFF`)

- `CMAKE_BUILD_TYPE`: Build type (default: `Release`)
  - `Release` - Optimized for size (-Os)
  - `Debug` - Debug symbols and minimal optimization (-O0 -g)

## Build Outputs

- Executables: `build/bin/`
- Libraries: `build/lib/`
- ELF files: `build/bin/*.elf`
- Hex files: `build/bin/*.hex`
- Map files: `build/bin/*.map`

## Toolchain Configuration

The ARM GCC toolchain is configured in `cmake/toolchain-arm-gcc.cmake`.

Platform-specific configurations are in:
- `cmake/config-freertos.armv7m.cmake`

## Project Structure

```
OpenMRN/
├── CMakeLists.txt           # Root CMake configuration
├── cmake/                   # CMake toolchain and configuration files
│   ├── toolchain-arm-gcc.cmake
│   └── config-freertos.armv7m.cmake
├── src/                     # Core libraries
│   ├── console/
│   ├── utils/
│   ├── executor/
│   ├── os/
│   ├── dcc/
│   ├── openlcb/
│   ├── withrottle/
│   ├── ble/
│   └── traction_modem/
├── applications/            # Application targets (optional)
│   ├── async_blink/
│   ├── blink_raw/
│   ├── send_datagram/
│   ├── simple_client/
│   ├── io_board/
│   └── nucleo_io/
├── boards/                  # Board-specific configurations
│   ├── st-stm32f767zi-nucleo/
│   ├── st-stm32f303re-nucleo/
│   └── st-stm32f303re-nucleo-dev-board/
├── include/                 # Public headers
└── targets/                 # Test and configuration targets
```

## Removing Old Build Files

The old Makefile-based build system can be completely removed. To clean up:

```bash
# Remove old build artifacts
rm -rf build_*
rm -rf *.elf
rm -rf *.hex
rm -rf *.map
rm -rf *.o
```

## Notes

- The CMake build system uses the same source files and configurations as the original Make-based system
- All compiler flags and optimization settings are preserved
- FreeRTOS library is automatically built as part of the CMake configuration
- Board-specific startup code and configuration is included during the build process
