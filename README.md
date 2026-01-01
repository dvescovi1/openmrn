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

OpenMRN is now built with CMake. The project targets a single embedded configuration:
`freertos.armv7m` on the `st-stm32f767zi-nucleo` STM32F767 Nucleo development board.

## Building with CMake

Requirements:
- CMake 3.15+
- ARM GCC toolchain (gcc-arm-none-eabi)
- Python 3 (for build scripts)

Build commands:
```bash
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=cmake/cortex-m-toolchain.cmake
cmake --build build
```

For more details on the CMake migration, see [CMAKE_MIGRATION.md](CMAKE_MIGRATION.md).

Most of the documentation for OpenMRN is in

  good machine. Download this as an ISO disk image
* Start the virtual machine. It will ask you to select a startup disk. Select
  the ISO file that you downloaded for Ubuntu desktop

Once you've done this, you'll need to do a few more things before you can get
the source code and build the documentation.

## Installing Required Software (Linux or Mac)

All software installation will be done through the command line, known as a
_terminal_ in Ubuntu Linux. Most of these commands will start with **sudo** in
order to run them with administrator privileges. Building the documentation
requires the gcc/g++ or LLVM compiler to be installed. Ubuntu LTS comes with the
compilers already installed.

Open a terminal window (for Ubuntu, right click anywhere on the desktop and
click **Open Terminal**) and enter the following commands (these all require
access to the internet):

```
sudo apt-get install git

cd ~
git clone https://github.com/bakerstu/openmrn/
```
## Building and Viewing the Documentation

At this point you can build, and then view the documentation. Here are the steps
to build the documentation:

```
cd ~/openmrn/doc
make html
```

That should create the HTML files. Now open the **Files** application, navigate
to Home/openmrn/doc and double-click **index.html**. This will open the OpenMRN
documentation in your browser (which is Firefox by default).
