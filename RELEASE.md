# Release process

This document details how releases need to be built and published for OpenMRN.

## Purpose and current scope

The project now targets a single embedded configuration
(`freertos.armv7m` with board `st-stm32f767zi-nucleo`). We no longer publish
host or JavaScript binaries. Releases are currently source-only tags; users
build firmware locally with the ARM GCC toolchain.

Releases for OpenMRNLite (Arduino compatible library) are documented in the
[arduino/RELEASE.md](arduino/RELEASE.md) file.

## Requirements for building releases

- ARM GCC toolchain (for example, the `gcc-arm-none-eabi` toolchain used by the
  default CMake configuration).
- CMake 3.16+ and Python 3 for the build system scripts.
- A developer workstation with access to the project source (no host
  cross-builds for desktop or JS are supported).
  
## Release numbering

Release numbers are marked as major.minor.patch, such as `v2.10.1`.

- Major release numbers are incremented once per year (2020 is 0, 2021 is 1,
  2022 is 2, ...).

- Minor release numbers are 10, 20, 30, 40 for given quarters, then incremented
  by one if there are multiple releases built within a quarter.

- Patch release numbers start at 1, and are only incremented if the same
  release needs to be re-built with a patch.

  
## How to build

All of the make commands here need to be run in the openmrn directory
(toplevel).

1. Start with a clean checkout. Run `cmake -S . -B build` (the default target is
  `freertos.armv7m` and uses the Cortex-M toolchain file).
2. Build the project: `cmake --build build`.
3. Flash or package the firmware as appropriate for the
  `st-stm32f767zi-nucleo` board (application-specific steps may vary).
4. Tag the release on GitHub (for example `release-v2.10.1`) after verifying the
  build.
