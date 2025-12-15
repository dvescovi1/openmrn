# OpenMRN

OpenMRN (Open Model Railroad Network) is a set of software libraries that are
designed to make it easier to implement support for the NMRA's LCC (Layout
Command Control) bus.

The work to define LCC was done by the [OpenLCB group](http://openlcb.org/),
and that's a good place to go in order to learn more about LCC. In particular,
[Introduction to OpenLCB](http://openlcb.org/about-openlcb/introduction-to-openlcb/).
Whenever you see _OpenLCB_, just think of this as a public working group that is
creating standards that, once approved by the NMRA, become LCC standards. The 
[OpenLCB & LCC Standards](http://openlcb.org/openlcb-and-lcc-documents/layout-command-control-lcc/)
page will give you a good idea of status of the specifications.

## Why OpenMRN

OpenMRN is a set of C++ code that is designed to make it easier to implement
support for LCC. This might be in accessory decoders, in a command station, in a
throttle, or any other device. The code is designed to be able to run on
microcontrollers. There are currently a number of different 32-bit microcontrollers
supported. To find the full list, you'll need to check the boards folder.

**Note:** The software license terms are 2-clause BSD in order to be commercial
friendly. This allows commercial applications to be written that use the open
source OpenMRN libraries without having to be open source themselves.

# Getting Started

OpenMRN now uses **CMake** and is configured as a **header-only library** for embedded ARM Cortex-M targets.

## Quick Start for ARM Embedded Projects

```cmake
# In your firmware's CMakeLists.txt
include(FetchContent)

FetchContent_Declare(
    openmrn
    GIT_REPOSITORY https://github.com/bakerstu/openmrn.git
    GIT_TAG        master
)

FetchContent_MakeAvailable(openmrn)

# Link with your firmware
target_link_libraries(your_firmware.elf PRIVATE OpenMRN::openmrn)
```

That's it! No separate build step needed - the headers are included directly in your project.

For detailed instructions, see [EMBEDDED_BUILD.md](EMBEDDED_BUILD.md).

## What's Included

This is a **header-only library** containing:
- ✅ All OpenMRN headers (OpenLCB, DCC, executor, utils)
- ✅ FreeRTOS driver headers
- ✅ Core protocol implementations
- ✅ CMSIS RTOS v2 support for STM32H563

**Application Examples (14 total - STM32H563 + CMSIS RTOS v2):**
- ✅ async_blink - Asynchronous LED with OpenLCB events
- ✅ blink_raw - Simple LED blinking
- ✅ clinic_app - Full OpenLCB node with configuration
- ✅ cmsis_os2_example - CMSIS RTOS v2 API examples
- ✅ direct_hub - High-performance hub using DirectHub infrastructure
- ✅ empty_app - Minimal application template
- ✅ hub - CAN/OpenLCB hub
- ✅ hub_test - Hub performance and throughput testing
- ✅ io_board - General purpose I/O board
- ✅ load_test - Network load generator for performance testing
- ✅ ping - Network ping responder
- ✅ simple_client - Basic OpenLCB client
- ✅ time_client - Receives and displays time from server
- ✅ time_server - Broadcasts time/date to LCC network

**Target Platform:**
- 🎯 **STM32H563** (ARM Cortex-M33) with CMSIS RTOS v2
- All other platforms and build systems have been removed for simplicity

## Building Applications

See [applications/BUILD_GUIDE.md](applications/BUILD_GUIDE.md) for complete build instructions for STM32H563 applications.

## Using as a Library in Your Firmware Project

See [EMBEDDED_BUILD.md](EMBEDDED_BUILD.md) for examples of how to integrate
the OpenMRN library into your embedded firmware project via CMake FetchContent.
