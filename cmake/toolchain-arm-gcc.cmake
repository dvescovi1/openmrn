# ARM GCC Toolchain file for OpenMRN

# Critical: Declare this as cross-compilation BEFORE finding compilers
# This prevents CMake from trying to apply host system (Windows) linking rules
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

# Skip executable tests for cross-compilation
# This prevents CMake from trying to link test executables with Windows libraries
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Find ARM GCC toolchain
if(DEFINED ENV{ARMGCCPATH})
    set(ARM_TOOLCHAIN_PATH "$ENV{ARMGCCPATH}")
else()
    # Try to find arm-none-eabi-gcc in PATH
    find_program(ARM_GCC_COMPILER arm-none-eabi-gcc)
    if(ARM_GCC_COMPILER)
        get_filename_component(ARM_TOOLCHAIN_PATH "${ARM_GCC_COMPILER}" DIRECTORY)
        get_filename_component(ARM_TOOLCHAIN_PATH "${ARM_TOOLCHAIN_PATH}" DIRECTORY)
        message(STATUS "Found ARM GCC at: ${ARM_TOOLCHAIN_PATH}")
    else()
        message(FATAL_ERROR "Could not find ARM GCC toolchain. Set ARMGCCPATH environment variable or ensure arm-none-eabi-gcc is in PATH")
    endif()
endif()

# Construct full paths to compiler executables
if(WIN32)
    set(ARM_GCC_PREFIX "${ARM_TOOLCHAIN_PATH}/bin/arm-none-eabi-")
    set(CMAKE_C_COMPILER "${ARM_GCC_PREFIX}gcc.exe")
    set(CMAKE_CXX_COMPILER "${ARM_GCC_PREFIX}g++.exe")
    set(CMAKE_ASM_COMPILER "${ARM_GCC_PREFIX}gcc.exe")
else()
    set(ARM_GCC_PREFIX "${ARM_TOOLCHAIN_PATH}/bin/arm-none-eabi-")
    set(CMAKE_C_COMPILER "${ARM_GCC_PREFIX}gcc")
    set(CMAKE_CXX_COMPILER "${ARM_GCC_PREFIX}g++")
    set(CMAKE_ASM_COMPILER "${ARM_GCC_PREFIX}gcc")
endif()

# Verify compilers exist
if(NOT EXISTS "${CMAKE_C_COMPILER}")
    message(FATAL_ERROR "C Compiler not found at: ${CMAKE_C_COMPILER}")
endif()
if(NOT EXISTS "${CMAKE_CXX_COMPILER}")
    message(FATAL_ERROR "C++ Compiler not found at: ${CMAKE_CXX_COMPILER}")
endif()
if(NOT EXISTS "${CMAKE_ASM_COMPILER}")
    message(FATAL_ERROR "ASM Compiler not found at: ${CMAKE_ASM_COMPILER}")
endif()

# Set other tools
set(CMAKE_AR "${ARM_GCC_PREFIX}ar.exe" CACHE FILEPATH "AR")
set(CMAKE_RANLIB "${ARM_GCC_PREFIX}ranlib.exe" CACHE FILEPATH "RANLIB")
set(CMAKE_OBJCOPY "${ARM_GCC_PREFIX}objcopy.exe" CACHE FILEPATH "OBJCOPY")
set(CMAKE_OBJDUMP "${ARM_GCC_PREFIX}objdump.exe" CACHE FILEPATH "OBJDUMP")
set(CMAKE_SIZE "${ARM_GCC_PREFIX}size.exe" CACHE FILEPATH "SIZE")
set(CMAKE_GDB "${ARM_GCC_PREFIX}gdb.exe" CACHE FILEPATH "GDB")

# Disable compiler checks
set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)
set(CMAKE_ASM_COMPILER_FORCED TRUE)

# Skip platform-specific detection
set(CMAKE_SKIP_RPATH TRUE)

# Generic ARM Cortex-M settings
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Common ARM flags (can be overridden per target)
set(ARM_COMMON_FLAGS
    "-mcpu=cortex-m7"
    "-mthumb"
    "-mfloat-abi=hard"
    "-mfpu=fpv5-d16"
    "-fno-builtin"
    "-fno-stack-protector"
    "--specs=nano.specs"
)

# Convert list to string for CMAKE_C_FLAGS and CMAKE_CXX_FLAGS
string(REPLACE ";" " " ARM_COMMON_FLAGS_STR "${ARM_COMMON_FLAGS}")

set(CMAKE_C_FLAGS "${ARM_COMMON_FLAGS_STR} -std=c99" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS "${ARM_COMMON_FLAGS_STR} -std=c++14 -fno-exceptions -fno-rtti" CACHE STRING "" FORCE)
set(CMAKE_ASM_FLAGS "${ARM_COMMON_FLAGS_STR}" CACHE STRING "" FORCE)

# Optimization flags
set(CMAKE_C_FLAGS_RELEASE "-Os" CACHE STRING "")
set(CMAKE_CXX_FLAGS_RELEASE "-Os" CACHE STRING "")
set(CMAKE_C_FLAGS_DEBUG "-O0 -g" CACHE STRING "")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g" CACHE STRING "")

# Ensure -g is always added
add_compile_options(-g -MD -MP)

message(STATUS "ARM Toolchain: ${ARM_TOOLCHAIN_PATH}")
