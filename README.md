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
- ✅ Ready for ARM Cortex-M targets

**Not included:**
- ❌ Applications (hub, bootloader_client, etc.)
- ❌ Tests
- ❌ Documentation generation
- ❌ Linux/POSIX host tools

## Using in Your Firmware Project

See [EMBEDDED_BUILD.md](EMBEDDED_BUILD.md) for complete examples of how to integrate
the OpenMRN library into your embedded firmware project.
