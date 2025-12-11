# OpenMRN CMake Quick Reference

## One-Time Setup
```bash
# Clone repository
git clone https://github.com/bakerstu/openmrn.git
cd openmrn

# Install dependencies (Ubuntu/Debian)
sudo apt-get install cmake build-essential libavahi-client-dev doxygen

# Install dependencies (macOS)
brew install cmake doxygen
```

## Basic Build Commands

```bash
# Configure
mkdir build && cd build
cmake ..

# Build everything
cmake --build .

# Build specific target
cmake --build . --target openmrn      # Just the library
cmake --build . --target hub          # Just hub application
cmake --build . --target docs         # Just documentation

# Build with multiple cores
cmake --build . -j 8

# Build in Release mode
cmake --build . --config Release
```

## Configuration Options

```bash
# Disable tests
cmake -DBUILD_TESTS=OFF ..

# Disable applications
cmake -DBUILD_APPLICATIONS=OFF ..

# Enable coverage
cmake -DENABLE_COVERAGE=ON ..

# Set install prefix
cmake -DCMAKE_INSTALL_PREFIX=/opt/openmrn ..

# Debug build
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Release build
cmake -DCMAKE_BUILD_TYPE=Release ..
```

## Testing

```bash
# Run all tests
ctest

# Run tests with output
ctest -V

# Run specific test
ctest -R <test_name>

# Run tests in parallel
ctest -j 8
```

## Installation

```bash
# Install to /usr/local (default)
sudo cmake --install .

# Install to custom location
cmake --install . --prefix /opt/openmrn

# Install only specific component
cmake --install . --component libraries
```

## Documentation

```bash
# Generate documentation
cmake --build . --target docs

# View documentation
# Open build/doc/html/index.html in browser
```

## Helper Scripts

```bash
# Unix/Linux/macOS - Full build with tests and docs
./build.sh --test --docs

# Debug build
./build.sh --debug

# Clean build
./build.sh --clean

# Windows PowerShell
.\build.ps1 -Test -Docs
.\build.ps1 -Debug
.\build.ps1 -Clean
```

## Common Tasks

### Clean Build
```bash
rm -rf build
mkdir build && cd build
cmake ..
cmake --build .
```

### Parallel Build (faster)
```bash
cmake --build . -j $(nproc)          # Linux
cmake --build . -j $(sysctl -n hw.ncpu)  # macOS
```

### Verbose Build
```bash
cmake --build . --verbose
# or
cmake -DCMAKE_VERBOSE_MAKEFILE=ON ..
```

### Out of source build (recommended)
```bash
# Build in build/
mkdir build && cd build
cmake ..

# Or build in separate directory
mkdir ../openmrn-build
cd ../openmrn-build
cmake ../openmrn
```

## IDE Integration

### VS Code
```json
// Install CMake Tools extension
// .vscode/settings.json
{
  "cmake.buildDirectory": "${workspaceFolder}/build",
  "cmake.configureOnOpen": true
}
```

### CLion
- Open project folder
- CLion auto-detects CMakeLists.txt
- Use Run/Debug configurations

### Visual Studio
- File → Open → Folder
- Select openmrn directory
- VS auto-detects CMakeLists.txt

## Platform-Specific

### Linux (Primary Platform)
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### macOS
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### Windows (WSL2 Recommended)
```bash
# In WSL2
cd /mnt/c/path/to/openmrn
./build.sh
```

### Windows (PowerShell)
```powershell
# Configuration works, but build requires WSL2
mkdir build
cd build
cmake ..
```

## Using OpenMRN in Your Project

```cmake
# In your CMakeLists.txt
find_package(OpenMRN REQUIRED)

add_executable(myapp main.cpp)
target_link_libraries(myapp PRIVATE OpenMRN::openmrn)
```

## Troubleshooting

### CMake cache issues
```bash
rm -rf build/CMakeCache.txt
cmake ..
```

### Complete clean
```bash
rm -rf build
mkdir build && cd build
cmake ..
```

### Check CMake version
```bash
cmake --version  # Need 3.14+
```

### Verbose configuration
```bash
cmake .. --trace
```

## Environment

### No environment variables needed!
- Old Make system required `OPENMRNPATH`
- CMake automatically finds everything
- Just run `cmake ..` from any build directory

## Quick Comparison

| Task | Old (Make) | New (CMake) |
|------|-----------|-------------|
| Setup | `export OPENMRNPATH=...` | None needed |
| Configure | Automatic | `cmake ..` |
| Build | `make` | `cmake --build .` |
| Test | `make tests` | `ctest` |
| Clean | `make clean` | `rm -rf build` |
| Install | `make install` | `cmake --install .` |

## Getting Help

- Build guide: [CMAKE_BUILD.md](CMAKE_BUILD.md)
- Platform notes: [PLATFORM_NOTES.md](PLATFORM_NOTES.md)
- Migration info: [CMAKE_MIGRATION.md](CMAKE_MIGRATION.md)
- CMake docs: https://cmake.org/documentation/

---
**CMake Version:** 3.14+  
**Status:** Production Ready  
**Platforms:** Linux ✅ | macOS ✅ | WSL2 ✅
