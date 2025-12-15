# Building OpenMRN Applications for STM32H563 with CMSIS RTOS v2

This guide explains how to build OpenMRN application examples targeting the STM32H563 microcontroller with CMSIS RTOS v2.

## Prerequisites

1. **ARM GNU Toolchain**: arm-none-eabi-gcc (version 10.3 or later)
2. **CMake**: Version 3.14 or later
3. **STM32CubeH5**: STM32 HAL and CMSIS libraries for H5 series
4. **Ninja** (optional but recommended): Fast build system

### Installation (Windows with STM32CubeIDE)

If you have STM32CubeIDE installed, the toolchain is typically at:
```
C:\ST\STM32CubeCLT_1.18.0\GNU-tools-for-STM32\bin\
```

STM32CubeH5 SDK is usually at (any V1.x version works):
```
C:\Users\<username>\STM32Cube\Repository\STM32Cube_FW_H5_V1.x.x\
```

## Quick Start

### 1. Set Environment Variables

```powershell
# PowerShell
$env:STM32CUBEH5_PATH = "C:\Users\<your-username>\STM32Cube\Repository\STM32Cube_FW_H5_V1.0.0"
$env:PATH = "C:\ST\STM32CubeCLT_1.18.0\GNU-tools-for-STM32\bin;$env:PATH"
```

```bash
# Linux/macOS
export STM32CUBEH5_PATH="/opt/STM32Cube/Repository/STM32Cube_FW_H5_V1.0.0"
export PATH="/usr/local/gcc-arm-none-eabi/bin:$PATH"
```

### 2. Configure and Build

```powershell
# From the openmrn root directory
mkdir build
cd build

# Configure with CMake (using the H563 toolchain file)
cmake -G Ninja `
  -DCMAKE_BUILD_TYPE=Debug `
  -DCMAKE_TOOLCHAIN_FILE=..\cmake\arm-none-eabi-h563.cmake `
  -DBUILD_APPLICATIONS=ON `
  ..

# Build all applications
ninja

# Or build a specific application
ninja blink_raw
ninja async_blink
ninja clinic_app
```

### 3. Flash to Target

The build produces `.elf`, `.hex`, and `.bin` files in `applications/<app_name>/`:

```powershell
# Using ST-LINK utility or OpenOCD
openocd -f board/st_nucleo_h563zi.cfg -c "program applications/blink_raw/blink_raw.elf verify reset exit"
```

## Available Applications

All applications have been converted to use CMSIS RTOS v2 APIs and target STM32H563:

- **blink_raw**: Simple LED blinking without OpenLCB stack
- **async_blink**: Asynchronous LED control with OpenLCB events
- **clinic_app**: Full OpenLCB node with configuration
- **direct_hub**: High-performance hub using DirectHub infrastructure
- **empty_app**: Minimal application template
- **hub**: CAN/OpenLCB hub application
- **hub_test**: Hub performance testing and throughput measurement
- **io_board**: General purpose I/O board with OpenLCB
- **load_test**: Network load generator for performance testing
- **ping**: Network ping responder
- **simple_client**: Basic OpenLCB client
- **time_server**: Broadcasts time/date information to all nodes on LCC network
- **time_client**: Receives and displays time from time server nodes

## Project Structure

```
openmrn/
├── applications/
│   ├── CMakeLists.txt          # Master applications build file
│   ├── blink_raw/
│   │   ├── CMakeLists.txt      # App-specific build
│   │   └── main.cxx            # CMSIS RTOS v2 compatible
│   ├── async_blink/
│   └── ...
├── boards/
│   └── st-stm32h563-nucleo/
│       ├── hardware.hxx        # Pin definitions
│       ├── HwInit.cxx          # Hardware initialization
│       ├── startup.c           # Reset handler & vectors
│       └── STM32H563ZITx_FLASH.ld  # Linker script
├── cmake/
│   └── arm-none-eabi-h563.cmake    # Toolchain file
└── src/
    └── CMakeLists.txt          # Core library build
```

## CMSIS RTOS v2 Integration

Applications use CMSIS RTOS v2 API instead of native FreeRTOS:

### Delay Example
```cpp
#include "cmsis_os2.h"

// Instead of: usleep(500000);
osDelay(500);  // 500ms delay
```

### Thread Creation
```cpp
osThreadId_t tid = osThreadNew(thread_func, NULL, &thread_attr);
```

### Semaphores
```cpp
osSemaphoreId_t sem = osSemaphoreNew(1, 0, NULL);
osSemaphoreAcquire(sem, osWaitForever);
osSemaphoreRelease(sem);
```

## Customization

### Different STM32H5 Variant

Edit [`cmake/arm-none-eabi-h563.cmake`](cmake/arm-none-eabi-h563.cmake) and [`boards/st-stm32h563-nucleo/STM32H563ZITx_FLASH.ld`](boards/st-stm32h563-nucleo/STM32H563ZITx_FLASH.ld) for different flash/RAM sizes.

### Add New Application

1. Create directory under `applications/<new_app>/`
2. Add `CMakeLists.txt`:
   ```cmake
   add_executable(new_app
       main.cxx
       ${CMAKE_CURRENT_SOURCE_DIR}/../../boards/st-stm32h563-nucleo/startup.c
   )
   
   target_link_libraries(new_app PRIVATE openmrn)
   
   target_link_options(new_app PRIVATE
       -T${LINKER_SCRIPT}
       -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/new_app.map
   )
   
   add_custom_command(TARGET new_app POST_BUILD
       COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:new_app> new_app.hex
       COMMAND ${CMAKE_SIZE} $<TARGET_FILE:new_app>
   )
   ```
3. Add `add_subdirectory(new_app)` to [`applications/CMakeLists.txt`](applications/CMakeLists.txt)

## Troubleshooting

### Compiler Not Found
Ensure `arm-none-eabi-gcc` is in your PATH:
```powershell
arm-none-eabi-gcc --version
```

### STM32CubeH5 Not Found
Set `STM32CUBEH5_PATH` or pass it to cmake:
```powershell
cmake -DSTM32CUBEH5_PATH="C:\path\to\STM32Cube_FW_H5" ..
```

### Linker Errors
Check that the linker script memory regions match your hardware:
- STM32H563ZI: 2MB Flash, 640KB RAM (default)
- Other variants: Edit [`STM32H563ZITx_FLASH.ld`](boards/st-stm32h563-nucleo/STM32H563ZITx_FLASH.ld)

## Build Options

```cmake
# Don't build applications (library only)
cmake -DBUILD_APPLICATIONS=OFF ..

# Release build with optimizations
cmake -DCMAKE_BUILD_TYPE=Release ..

# Verbose build output
cmake --build . --verbose
```

## Next Steps

- Add FDCAN driver integration for CAN bus communication
- Implement ThreadX-based FreeRTOS compatibility layer
- Add STM32H5 HAL peripheral drivers (UART, SPI, I2C)
- Create application-specific hardware configurations

For more information, see the main [README.md](../README.md).
