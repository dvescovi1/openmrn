cmake_minimum_required(VERSION 3.14)

# For bare-metal cross-compilation, skip executable linking during compiler checks
if(CMAKE_CROSSCOMPILING OR CMAKE_C_COMPILER MATCHES "arm-none-eabi")
    set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
endif()

project(OpenMRN VERSION 1.0.0 LANGUAGES C CXX ASM)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Export compile commands for IDE integration
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set OPENMRNPATH for compatibility
set(OPENMRNPATH ${CMAKE_CURRENT_SOURCE_DIR})

# Add cmake modules directory
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Embedded ARM library build - no applications, tests, or docs
# For cross-compilation, use: cmake -DCMAKE_TOOLCHAIN_FILE=<toolchain.cmake> ..

# Include subdirectories
add_subdirectory(src)

# Print configuration summary
message(STATUS "")
message(STATUS "OpenMRN Embedded ARM Library Configuration:")
message(STATUS "  Version:              ${PROJECT_VERSION}")
message(STATUS "  Build type:           ${CMAKE_BUILD_TYPE}")
message(STATUS "  C Compiler:           ${CMAKE_C_COMPILER}")
message(STATUS "  CXX Compiler:         ${CMAKE_CXX_COMPILER}")
message(STATUS "  System Processor:     ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "  Target:               Embedded ARM library only")
message(STATUS "")
