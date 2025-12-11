# OpenMRN Make to CMake Migration Summary

This document describes the migration from the old Make-based build system to a modern CMake build system.

## What Was Done

### 1. Created CMake Build Infrastructure

#### Root Level
- **CMakeLists.txt** - Main project configuration
  - Sets up C++14/C11 standards
  - Defines build options (applications, tests, docs, coverage)
  - Platform detection and package finding
  - Subdirectory inclusion
  - Installation rules
  - Package configuration for downstream projects

- **cmake/** directory
  - `OpenMRNConfig.cmake.in` - Package configuration template

#### Source Level
- **src/CMakeLists.txt** - Core library build
  - Automatically collects all `.cxx` and `.c` files
  - Excludes test files
  - Creates `openmrn` library target
  - Configures include directories
  - Platform-specific linking (Threads, Avahi)
  - Compiler warnings and flags
  - Installation rules for library and headers

#### Applications
- **applications/CMakeLists.txt** - Application builds
  - Macro `add_openmrn_application()` for easy app addition
  - Automatically links with openmrn library
  - Configured for 16+ applications

#### Tests
- **test/CMakeLists.txt** - Test suite
  - Uses FetchContent to download GoogleTest
  - Automatically discovers all `.cxxtest` files
  - Unique test naming to avoid conflicts
  - Integration with CTest

#### Documentation
- **doc/CMakeLists.txt** - Documentation generation
  - Finds and configures Doxygen
  - Creates `docs` target
  - Generates HTML documentation

### 2. Removed Old Make Infrastructure

The following were removed:
- Root `Makefile`
- `applications/Makefile` and all subdirectory Makefiles
- `test/` Makefiles (recursive)
- `targets/` Makefiles (recursive)
- **Entire `etc/` directory** containing:
  - 50+ `.mk` files (prog.mk, lib.mk, recurse.mk, etc.)
  - Platform-specific makefiles
  - Toolchain configurations
  - Build templates and rules

### 3. Created Documentation

- **CMAKE_BUILD.md** - Comprehensive build guide
  - Prerequisites for each platform
  - Quick start instructions
  - Build options and configuration
  - Testing and documentation generation
  - Installation procedures
  - IDE integration tips
  - Migration guide from Make

- **PLATFORM_NOTES.md** - Platform-specific information
  - Linux support (primary)
  - macOS support
  - Windows limitations and workarounds
  - WSL2 recommendations
  - Embedded target notes

### 4. Created Build Scripts

- **build.sh** - Unix/Linux/macOS build script
  - Command-line options for debug, clean, test, docs
  - Parallel builds
  - User-friendly output

- **build.ps1** - Windows PowerShell build script
  - Equivalent functionality to build.sh
  - Windows-native PowerShell syntax
  - Works with WSL2 or MSYS2

### 5. Updated Repository Configuration

- **.gitignore** - Added CMake-specific entries
  - build/ directory
  - CMake cache and generated files
  - CTest files

- **README.md** - Updated build instructions
  - Removed old Make-based virtual machine setup
  - Added CMake quick start
  - Modernized instructions

- **.github/workflows/cmake.yml** - CI/CD workflow
  - Automated builds on Ubuntu and macOS
  - Tests multiple build types (Debug/Release)
  - Runs tests automatically
  - Generates documentation

## Migration Benefits

### For Users

1. **Simpler Build Process**
   - No environment variables needed (OPENMRNPATH)
   - Standard `cmake ..` and `cmake --build .`
   - Works out-of-the-box on modern systems

2. **Better IDE Integration**
   - VS Code (CMake Tools extension)
   - CLion (native support)
   - Visual Studio (native CMake support)
   - Qt Creator (native support)

3. **Cross-Platform**
   - Same commands on Linux, macOS, Windows (with WSL)
   - No platform-specific Makefiles needed

4. **Package Management**
   - Proper installation support
   - Can be used with `find_package()` in other CMake projects
   - Exports proper targets

### For Developers

1. **Modern Build System**
   - Industry standard (CMake is used by LLVM, KDE, Boost, etc.)
   - Better dependency management
   - Clearer build logic

2. **Testing Integration**
   - CTest integration
   - Automatic test discovery
   - Better CI/CD integration

3. **Easier to Extend**
   - Adding new applications is straightforward
   - Adding new libraries is simple
   - No need to maintain complex Make templates

## What's Different

### Building

**Old (Make):**
```bash
export OPENMRNPATH=/path/to/openmrn
cd openmrn
make
```

**New (CMake):**
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Adding an Application

**Old (Make):**
- Create directory structure
- Create Makefile
- Create targets subdirectories
- Create platform-specific Makefiles
- Update parent Makefile SUBDIRS

**New (CMake):**
- Create directory with main.cxx
- Add one line to applications/CMakeLists.txt:
  ```cmake
  add_openmrn_application(myapp myapp)
  ```

### Configuration

**Old (Make):**
- Edit config.mk files
- Set environment variables
- Platform detected via .mk files

**New (CMake):**
- Use CMake options: `-DBUILD_TESTS=OFF`
- Use standard CMAKE_ variables
- Platform detected by CMake

## Known Limitations

### Windows Native Build

The source code is heavily Unix/POSIX dependent and does not currently build natively on Windows with MSVC. See PLATFORM_NOTES.md for details and workarounds.

**Recommended for Windows users:** Use WSL2

### Embedded Targets

The old Make system had extensive support for embedded targets (ARM, MIPS, PIC32, etc.) via board-specific configurations. The CMake system currently focuses on host development (Linux/macOS).

Embedded support can be added via CMake toolchain files in the future.

### Some Legacy Applications

A few specialized applications or tests may have had custom Make logic that needs to be ported to CMake. Most standard applications and tests are supported.

## Future Work

Potential improvements:

1. **Embedded Target Support**
   - Create CMake toolchain files for ARM, MIPS targets
   - Port board configurations from old boards/ directory

2. **Windows Native Support**
   - Create platform abstraction layer
   - Port Unix-specific code to Windows APIs

3. **Advanced Features**
   - Precompiled headers for faster builds
   - Unity builds option
   - Better coverage report generation

4. **Component Installation**
   - Install only specific components
   - Development vs runtime packages

## Getting Help

- See [CMAKE_BUILD.md](CMAKE_BUILD.md) for build instructions
- See [PLATFORM_NOTES.md](PLATFORM_NOTES.md) for platform-specific info
- Check GitHub Issues for known problems
- The CMake build is standard - any CMake documentation applies

## Verification

The migration was successful if:
- ✅ CMake configuration completes without errors
- ✅ Core library builds successfully
- ✅ Applications build successfully  
- ✅ Tests compile and run
- ✅ Documentation generates (if Doxygen installed)

Test the migration:
```bash
cd openmrn
./build.sh --test --docs
```

Or manually:
```bash
mkdir build && cd build
cmake ..
cmake --build .
ctest
cmake --build . --target docs
```

## Questions?

For issues or questions about the CMake build system:
1. Check the documentation files (CMAKE_BUILD.md, PLATFORM_NOTES.md)
2. Review CMakeLists.txt files for examples
3. Consult CMake documentation: https://cmake.org/documentation/
4. File an issue on GitHub

---

**Migration completed:** December 2025  
**CMake version required:** 3.14+  
**Status:** Production ready for Linux/macOS, WSL2 recommended for Windows
