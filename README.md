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

Most of the documentation for OpenMRN is in
[doxygen](http://www.stack.nl/~dimitri/doxygen/) format.

OpenMRN now uses **CMake** as its build system and is configured to build
**only the embedded ARM library** (static library for ARM Cortex-M targets).

## Quick Build for ARM Embedded Targets

```bash
# Install ARM toolchain
sudo apt-get install gcc-arm-none-eabi  # Ubuntu/Debian
brew install --cask gcc-arm-embedded    # macOS

# Build the library
mkdir build-arm && cd build-arm
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/arm-none-eabi.toolchain.cmake.example ..
cmake --build .
```

Result: `libopenmrn.a` static library for ARM embedded targets.

For detailed instructions, see [EMBEDDED_BUILD.md](EMBEDDED_BUILD.md).

## What's Included

This build produces **only**:
- ✅ Static library (`libopenmrn.a`) for ARM Cortex-M targets
- ✅ Core OpenLCB protocol implementation
- ✅ FreeRTOS driver support
- ✅ DCC, executor, and utility modules

**Not included:**
- ❌ Applications (hub, bootloader_client, etc.)
- ❌ Tests
- ❌ Documentation generation
- ❌ Linux/POSIX host tools

## Installing ARM Toolchain

### Linux (Ubuntu/Debian)
```bash
sudo apt-get install gcc-arm-none-eabi
```

### macOS
```bash
brew install --cask gcc-arm-embedded
```

### Windows
Download from [ARM Developer](https://developer.arm.com/downloads/-/gnu-rm)

## Using in Your Firmware Project

See [EMBEDDED_BUILD.md](EMBEDDED_BUILD.md) for complete examples of how to integrate
the OpenMRN library into your embedded firmware project.
