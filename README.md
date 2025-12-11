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
[doxygen](http://www.stack.nl/~dimitri/doxygen/) format. The best way to view
the documents is to build the HTML files (instructions below).

OpenMRN now uses **CMake** as its build system, which provides excellent
cross-platform support and IDE integration. See [CMAKE_BUILD.md](CMAKE_BUILD.md) 
for detailed build instructions.

## Quick Build Instructions

The easiest way to get started is:

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

For detailed instructions and build options, see [CMAKE_BUILD.md](CMAKE_BUILD.md).

## Installing Required Software (Linux or Mac)

Building OpenMRN requires CMake and a C++14 compatible compiler. Most modern
Linux distributions and macOS already include the necessary compilers.

Open a terminal window and enter the following commands:

```bash
# Ubuntu/Debian
sudo apt-get install git cmake build-essential doxygen

# macOS (requires Homebrew)
brew install git cmake doxygen

# Clone the repository
cd ~
git clone https://github.com/bakerstu/openmrn/
```

## Building and Viewing the Documentation

Build the documentation with CMake:

```bash
cd ~/openmrn
mkdir build
cd build
cmake ..
cmake --build . --target docs
```

The HTML documentation will be generated in `build/doc/html/`. Open
`build/doc/html/index.html` in your web browser to view it.
