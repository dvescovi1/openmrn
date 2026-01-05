# Configuration for FreeRTOS ARM v7-M target

# Find FreeRTOS
if(NOT DEFINED FREERTOSPATH)
    if(DEFINED ENV{FREERTOSPATH})
        set(FREERTOSPATH $ENV{FREERTOSPATH})
    else()
        # FreeRTOS not found - provide helpful message
        message(STATUS "")
        message(STATUS "⚠️  FreeRTOS not found - attempting to build without it")
        message(STATUS "")
        message(STATUS "To enable FreeRTOS support, set FREERTOSPATH:")
        message(STATUS "  Windows:   set FREERTOSPATH=C:\\path\\to\\FreeRTOS")
        message(STATUS "  Linux/Mac: export FREERTOSPATH=/path/to/FreeRTOS")
        message(STATUS "")
        message(STATUS "Or pass it to CMake:")
        message(STATUS "  cmake -DFREERTOSPATH=/path/to/FreeRTOS ..")
        message(STATUS "")
        
        # For now, set a dummy path to continue configuration
        # Actual FreeRTOS targets will fail if this isn't set correctly
        set(FREERTOSPATH "" CACHE PATH "Path to FreeRTOS installation")
        return()
    endif()
endif()

# If FREERTOSPATH is empty or not set, skip FreeRTOS configuration
if(NOT FREERTOSPATH)
    return()
endif()

# Normalize path to use forward slashes (CMake standard)
file(TO_CMAKE_PATH "${FREERTOSPATH}" FREERTOSPATH)

# If we get here, FREERTOSPATH is set and non-empty
if(NOT EXISTS "${FREERTOSPATH}")
    message(FATAL_ERROR "FREERTOSPATH points to non-existent directory: ${FREERTOSPATH}")
endif()

# Detect FreeRTOS directory structure
if(EXISTS "${FREERTOSPATH}/FreeRTOS-Kernel/include/FreeRTOS.h")
    set(FREERTOS_KERNEL_PATH "${FREERTOSPATH}/FreeRTOS-Kernel")
elseif(EXISTS "${FREERTOSPATH}/Source/include/FreeRTOS.h")
    set(FREERTOS_KERNEL_PATH "${FREERTOSPATH}/Source")
else()
    message(FATAL_ERROR "Could not find FreeRTOS kernel at ${FREERTOSPATH}")
endif()

# FreeRTOS includes
set(FREERTOS_PORT_DIR "${FREERTOS_KERNEL_PATH}/portable/GCC/ARM_CM7/r0p1")
if(NOT EXISTS "${FREERTOS_PORT_DIR}")
    message(FATAL_ERROR "FreeRTOS ARM_CM7 port not found at ${FREERTOS_PORT_DIR}. Confirm FREERTOSPATH points to a FreeRTOS distribution with GCC/ARM_CM7/r0p1.")
endif()

set(FREERTOS_INCLUDE_DIRS
    ${FREERTOS_KERNEL_PATH}/include
    ${FREERTOS_PORT_DIR}
    ${OPENMRNPATH}/include/freertos
)

# FreeRTOS compile definitions - use add_definitions for global scope
add_definitions(-D__FreeRTOS__ -DGCC_ARMCM7)

# ARM Cortex-M7 with hardware FPU
set(CMAKE_C_FLAGS "-mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 ${CMAKE_CXX_FLAGS}")
set(CMAKE_ASM_FLAGS "-mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 ${CMAKE_ASM_FLAGS}")

# FreeRTOS-specific compile options
add_compile_options(
    -fno-builtin
)

# Include FreeRTOS headers globally
include_directories(${FREERTOS_INCLUDE_DIRS})
include_directories(${OPENMRNPATH}/include/freertos_select)
include_directories(${OPENMRNPATH}/src/freertos_drivers/common)
include_directories(${OPENMRNPATH}/src/freertos_drivers/st)

# Create FreeRTOS library
file(GLOB FREERTOS_SOURCES
    ${FREERTOS_KERNEL_PATH}/*.c
    ${FREERTOS_PORT_DIR}/*.c
    ${FREERTOS_KERNEL_PATH}/portable/MemMang/heap_4.c
)

add_library(freertos STATIC ${FREERTOS_SOURCES})
target_include_directories(freertos PRIVATE 
    ${FREERTOS_INCLUDE_DIRS}
    ${OPENMRNPATH}/src
)

message(STATUS "FreeRTOS Kernel Path: ${FREERTOS_KERNEL_PATH}")
message(STATUS "FreeRTOS Library: freertos (ARM_CM7 hard-float)")
