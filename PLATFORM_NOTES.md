# Platform Notes for OpenMRN CMake Build

## Supported Platforms

OpenMRN's CMake build system has been tested and works on:

- **Linux** (Ubuntu, Debian, Fedora, etc.) - Primary platform ✅
- **macOS** - Fully supported ✅  
- **Windows** - Limited support ⚠️

## Platform-Specific Details

### Linux (Primary Platform)

Linux is the primary development platform for OpenMRN. All features work out of the box.

**Prerequisites:**
```bash
sudo apt-get install cmake build-essential libavahi-client-dev libavahi-common-dev
```

**Build:**
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### macOS

macOS is fully supported with minor differences from Linux.

**Prerequisites:**
```bash
brew install cmake
```

**Build:**
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### Windows

⚠️ **Important**: OpenMRN's codebase contains significant Unix/POSIX dependencies that make native Windows compilation challenging. The code includes:

- POSIX headers (`unistd.h`, `endian.h`, `sys/time.h`)
- Unix-specific system calls
- GCC/Clang specific attributes and inline assembly
- Socket CAN interfaces (Linux-specific)

**Recommended approaches for Windows users:**

1. **WSL2 (Windows Subsystem for Linux)** - Recommended ✅
   ```bash
   # In WSL2 Ubuntu
   sudo apt-get install cmake build-essential
   cd /mnt/c/path/to/openmrn
   mkdir build && cd build
   cmake ..
   cmake --build .
   ```

2. **MSYS2/MinGW** - Possible with modifications
   - Install MSYS2
   - Use MinGW toolchain
   - May require source code modifications

3. **Virtual Machine** - Traditional approach
   - Use VirtualBox or VMware
   - Install Linux distribution
   - Build natively in VM

4. **Docker** - Containerized build
   ```bash
   docker run -v ${PWD}:/openmrn -w /openmrn ubuntu:latest bash -c \
     "apt-get update && apt-get install -y cmake build-essential && \
      mkdir build && cd build && cmake .. && cmake --build ."
   ```

### Embedded Targets

For embedded/microcontroller builds (ARM, MIPS, etc.), the old Make-based system had extensive support via board-specific configurations in `boards/` and `etc/`. 

The CMake build system currently focuses on host platforms (Linux/macOS). Embedded target support can be added via CMake toolchain files in the future.

## Porting to Windows Native

If you need native Windows support, the following areas need work:

1. Replace POSIX headers with Windows equivalents or abstraction layer
2. Replace inline assembly with portable code or MSVC intrinsics
3. Abstract socket operations (Winsock vs BSD sockets)
4. Handle endianness portably
5. Replace GCC attributes with portable alternatives
6. Conditional compilation for platform-specific features

This is a significant undertaking and contributions are welcome!

## CI/CD

The included `.github/workflows/cmake.yml` runs automated builds on:
- Ubuntu (latest)
- macOS (latest)

Windows builds are not included in CI due to the platform compatibility issues mentioned above.
