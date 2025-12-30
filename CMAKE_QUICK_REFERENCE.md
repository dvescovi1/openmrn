# OpenMRN CMake Quick Reference

## Configuration & Build

```bash
# Basic configuration
cmake -B build

# Configure for specific target
cmake -B build -DOPENMRN_TARGET=freertos.armv7m

# Build
cmake --build build

# Build with parallelism (4 jobs)
cmake --build build -j 4

# Clean
cmake --build build --target clean
```

## CMake Options

| Option | Default | Description |
|--------|---------|-------------|
| `OPENMRN_TARGET` | `linux.x86` | Target architecture |
| `CMAKE_BUILD_TYPE` | `Release` | Build type (Debug, Release, RelWithDebInfo) |
| `BUILD_TESTING` | `ON` | Enable/disable testing |
| `BUILD_SHARED_LIBS` | `OFF` | Build shared (ON) or static (OFF) libraries |
| `ENABLE_LTO` | `OFF` | Enable Link Time Optimization |
| `CMAKE_CXX_STANDARD` | `17` | C++ standard (14, 17, 20) |

## Common Commands

```bash
# Build in Debug mode with verbose output
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -- VERBOSE=1

# Build with Ninja instead of Make
cmake -B build -G Ninja
ninja -C build

# Generate for Visual Studio
cmake -B build -G "Visual Studio 17 2022"

# Generate for Xcode (macOS)
cmake -B build -G Xcode

# Cross-compile for ARM
cmake -B build -DOPENMRN_TARGET=freertos.armv7m \
                -DCMAKE_TOOLCHAIN_FILE=cmake/cortex-m-toolchain.cmake
```

## Project Structure

```
targets/<target>/
├── CMakeLists.txt          Main target configuration
├── os/
│   └── CMakeLists.txt      OS module
├── executor/
│   └── CMakeLists.txt      Executor module
├── dcc/
│   └── CMakeLists.txt      DCC module
├── openlcb/
│   └── CMakeLists.txt      OpenLCB module
├── utils/
│   └── CMakeLists.txt      Utilities module
└── ... (other modules)
```

## Library Dependencies

```
openlcb
  ├── executor
  ├── utils
  ├── os_main
  └── dcc

dcc
  ├── executor
  └── os_main

utils
  ├── executor
  └── os_main

withrottle
  ├── executor
  ├── os_main
  ├── openlcb
  └── if

if
  ├── executor
  ├── os_main
  ├── openlcb
  └── dcc
```

## Available Targets

- **linux.x86** (fully migrated ✓)
- **linux.llvm** (needs CMakeLists.txt)
- **linux.armv7a** (needs CMakeLists.txt)
- **mach.x86_64** (needs CMakeLists.txt)
- **freertos.armv7m** (needs CMakeLists.txt)
- **freertos.armv6m** (needs CMakeLists.txt)
- **bare.armv7m** (needs CMakeLists.txt)
- **js.emscripten** (needs CMakeLists.txt)
- **mingw.x86** (needs CMakeLists.txt)
- **nonos.xtensa-lx106.esp8266** (needs CMakeLists.txt)

## Troubleshooting

### Compiler not found
```bash
# Specify compiler explicitly
cmake -B build -DCMAKE_C_COMPILER=/path/to/gcc \
               -DCMAKE_CXX_COMPILER=/path/to/g++
```

### Cache issues
```bash
# Remove CMake cache and rebuild
rm -rf build
cmake -B build
```

### Verbose output
```bash
# See detailed compilation commands
cmake --build build -- VERBOSE=1

# Or with CMake verbose
cmake -B build -DCMAKE_VERBOSE_MAKEFILE=ON
```

## File Locations

- **Source files**: `src/`
- **Header files**: `include/`
- **Build artifacts**: `build/lib/` and `build/bin/`
- **CMake modules**: `cmake/`
- **Target configurations**: `targets/`
- **Applications**: `applications/`

## Documentation

- [CMAKE_MIGRATION.md](CMAKE_MIGRATION.md) - Full migration guide
- [CONVERSION_SUMMARY.md](CONVERSION_SUMMARY.md) - Conversion details
- [CMake Documentation](https://cmake.org/cmake/help/latest/)
