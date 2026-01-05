cmake_minimum_required(VERSION 3.20)

# CRITICAL: Set toolchain BEFORE project() declaration
# This ensures cross-compiler detection happens correctly
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/toolchain-arm-gcc.cmake" CACHE STRING "CMake toolchain file" FORCE)

project(OpenMRN C CXX ASM)

# Set default target board and configuration
set(OPENMRN_BOARD "st-stm32f767zi-nucleo" CACHE STRING "Target board for the build")
set(OPENMRN_TARGET "freertos.armv7m" CACHE STRING "Target platform")
set(BUILD_APPLICATIONS ON CACHE BOOL "Build applications (ON/OFF)")
set(BUILD_TESTS OFF CACHE BOOL "Build tests (ON/OFF)")

# Set default to Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Set project root path before config include
set(OPENMRNPATH "${CMAKE_CURRENT_SOURCE_DIR}")

# Include the target configuration (after setting OPENMRN_TARGET)
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/config-${OPENMRN_TARGET}.cmake")

# Set project root path
set(OPENMRNPATH "${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Include directories for the entire project
include_directories("${OPENMRNPATH}/include")

# Set common compile options
add_compile_options(
    -fdata-sections
    -ffunction-sections
    -Wall
    -Werror
    -Wno-unknown-pragmas
)

# Language-specific flags
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-Wsuggest-override>)
    add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-Wno-psabi>)
endif()

# Linker flags
add_link_options(
    -fdata-sections
    -ffunction-sections
    -Wl,--gc-sections
    -Wl,--undefined=ignore_fn
)

# Add subdirectories
add_subdirectory(src)

if(BUILD_APPLICATIONS)
    add_subdirectory(applications)
endif()

if(BUILD_TESTS)
    add_subdirectory(targets/test)
endif()

# Summary
message(STATUS "")
message(STATUS "OpenMRN Build Configuration:")
message(STATUS "  Target Board:      ${OPENMRN_BOARD}")
message(STATUS "  Target Platform:   ${OPENMRN_TARGET}")
message(STATUS "  Build Type:        ${CMAKE_BUILD_TYPE}")
message(STATUS "  Build Applications: ${BUILD_APPLICATIONS}")
message(STATUS "  Build Tests:       ${BUILD_TESTS}")
message(STATUS "  Toolchain:         ${CMAKE_C_COMPILER}")
message(STATUS "")
