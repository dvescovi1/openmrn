# Building OpenMRN with CMake

OpenMRN now uses CMake as its primary build system. The old Make-based build system has been removed.

## Prerequisites

- CMake 3.14 or later
- C++14 compatible compiler (GCC, Clang, or MSVC)
- C11 compatible C compiler

### Linux
```bash
sudo apt-get install cmake build-essential
```

### macOS
```bash
brew install cmake
```

### Windows
Download and install CMake from https://cmake.org/download/

## Quick Start

### Building the project

```bash
# Create build directory
mkdir build
cd build

# Configure
cmake ..

# Build
cmake --build .

# Run tests (optional)
ctest

# Install (optional)
sudo cmake --install .
```

### Build Options

- `BUILD_APPLICATIONS`: Build applications (default: ON)
- `BUILD_TESTS`: Build tests (default: ON)
- `BUILD_DOC`: Build documentation (default: ON)
- `ENABLE_COVERAGE`: Enable code coverage (default: OFF)

Example with options:
```bash
cmake -DBUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Release ..
```

### Build Types

- `Debug`: Debug build with symbols
- `Release`: Optimized release build
- `RelWithDebInfo`: Release with debug info
- `MinSizeRel`: Minimum size release

Example:
```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
```

## Building Specific Applications

After configuring with CMake, you can build specific applications:

```bash
cmake --build . --target hub
cmake --build . --target bootloader_client
```

## Running Tests

```bash
# Build and run all tests
ctest

# Run tests with verbose output
ctest -V

# Run specific test
ctest -R <test_name>
```

## Generating Documentation

If Doxygen is installed:
```bash
cmake --build . --target docs
```

The generated documentation will be in `build/doc/html/`.

## Installation

```bash
# Install to default location (/usr/local on Unix)
sudo cmake --install .

# Install to custom location
cmake --install . --prefix /opt/openmrn
```

## Integration with Other Projects

After installation, you can use OpenMRN in your CMake projects:

```cmake
find_package(OpenMRN REQUIRED)
target_link_libraries(your_target PRIVATE OpenMRN::openmrn)
```

## Troubleshooting

### Missing Dependencies

On Linux, you may need to install additional packages:
```bash
sudo apt-get install libavahi-client-dev libavahi-common-dev
```

### Build Failures

If you encounter build failures, try a clean build:
```bash
rm -rf build
mkdir build
cd build
cmake ..
cmake --build .
```

## Migration from Make

The CMake build system replaces the old Make system. Key differences:

- No need to set `OPENMRNPATH` environment variable
- Out-of-source builds are standard practice
- Cross-platform support is improved
- Easier integration with IDEs (VS Code, CLion, Visual Studio)

## IDE Support

### VS Code
Install the CMake Tools extension and it will automatically detect the project.

### CLion
Open the project directory - CLion will automatically detect the CMakeLists.txt.

### Visual Studio
Open the folder as a CMake project.
