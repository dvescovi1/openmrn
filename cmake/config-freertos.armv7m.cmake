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

# If we get here, FREERTOSPATH is set
if(FREERTOSPATH AND NOT EXISTS "${FREERTOSPATH}")
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
set(FREERTOS_INCLUDE_DIRS
    ${FREERTOS_KERNEL_PATH}/include
    ${FREERTOS_KERNEL_PATH}/portable/GCC/ARM_CM3
    ${OPENMRNPATH}/include/freertos
)

# FreeRTOS compile definitions
add_compile_definitions(
    __FreeRTOS__
    GCC_ARMCM3
)

# ARM Cortex-M3 specific flags
set(CMAKE_C_FLAGS "-mcpu=cortex-m3 -mthumb -mfloat-abi=soft ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-mcpu=cortex-m3 -mthumb -mfloat-abi=soft ${CMAKE_CXX_FLAGS}")
set(CMAKE_ASM_FLAGS "-mcpu=cortex-m3 -mthumb -mfloat-abi=soft ${CMAKE_ASM_FLAGS}")

# FreeRTOS-specific compile options
add_compile_options(
    -mfix-cortex-m3-ldrd
    -fno-builtin
)

# Include FreeRTOS headers globally
include_directories(${FREERTOS_INCLUDE_DIRS})
include_directories(${OPENMRNPATH}/include/freertos_select)
include_directories(${OPENMRNPATH}/src/freertos_drivers/common)

# Create FreeRTOS library
file(GLOB FREERTOS_SOURCES
    ${FREERTOS_KERNEL_PATH}/*.c
    ${FREERTOS_KERNEL_PATH}/portable/GCC/ARM_CM3/*.c
)

add_library(freertos STATIC ${FREERTOS_SOURCES})
target_include_directories(freertos PRIVATE ${FREERTOS_INCLUDE_DIRS})

message(STATUS "FreeRTOS Kernel Path: ${FREERTOS_KERNEL_PATH}")
message(STATUS "FreeRTOS Library: freertos")
