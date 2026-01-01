cmake_minimum_required(VERSION 3.15)

# Set OpenMRN path for subdirectories
set(OPENMRNPATH "${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Default configuration
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Default target architecture: cross-compile to ARM FreeRTOS
if(NOT OPENMRN_TARGET)
    set(OPENMRN_TARGET freertos.armv7m)
endif()

# Cross-compilation settings for freertos and bare targets
if(OPENMRN_TARGET MATCHES "freertos|bare")
    # Avoid running a host executable during compiler tests
    set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
    # Use the Cortex-M toolchain unless already specified by the caller
    if(NOT CMAKE_TOOLCHAIN_FILE)
        set(CMAKE_TOOLCHAIN_FILE "${OPENMRNPATH}/cmake/cortex-m-toolchain.cmake")
    endif()
endif()

project(OpenMRN
    VERSION 0.0.1
    DESCRIPTION "Open Model Railroad Network"
    LANGUAGES C CXX)

# Compiler settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 99)

# Add include directories
include_directories(${OPENMRNPATH}/include)

# Build output directories
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include CMake helper modules
include(${OPENMRNPATH}/cmake/openmrn_library.cmake)
include(${OPENMRNPATH}/cmake/default.cmake)

# Include core configuration based on target
set(TARGET_CONFIG_FILE "${OPENMRNPATH}/cmake/${OPENMRN_TARGET}.cmake")
if(EXISTS "${TARGET_CONFIG_FILE}")
    include("${TARGET_CONFIG_FILE}")
else()
    message(STATUS "Note: No specific configuration found for target ${OPENMRN_TARGET}")
endif()

# Main subdirectories
add_subdirectory(src)

# Print configuration summary
message(STATUS "")
message(STATUS "OpenMRN Configuration Summary:")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Target: ${OPENMRN_TARGET}")
message(STATUS "  Source Directory: ${OPENMRNPATH}")
message(STATUS "  Build Directory: ${CMAKE_BINARY_DIR}")
message(STATUS "")
