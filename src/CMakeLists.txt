# OpenMRN Embedded Library - Header-Only Configuration

cmake_minimum_required(VERSION 3.14)

# RTOS selection option
set(OPENMRN_RTOS "CMSIS_RTOS2" CACHE STRING "RTOS to use: CMSIS_RTOS2 or FREERTOS")
set_property(CACHE OPENMRN_RTOS PROPERTY STRINGS "CMSIS_RTOS2" "FREERTOS")

message(STATUS "Configuring OpenMRN as header-only INTERFACE library")
message(STATUS "  RTOS: ${OPENMRN_RTOS}")

# Collect all header files (for IDE support)
file(GLOB_RECURSE OPENMRN_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/console/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/console/*.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/dcc/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/dcc/*.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/executor/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/executor/*.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/openlcb/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/openlcb/*.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/os/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/os/*.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/traction_modem/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/traction_modem/*.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/*.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/freertos_drivers/**/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/freertos_drivers/**/*.hxx
)

# Exclude test files
list(FILTER OPENMRN_HEADERS EXCLUDE REGEX ".*[Tt]est.*")

list(LENGTH OPENMRN_HEADERS HEADER_COUNT)
message(STATUS "  Found ${HEADER_COUNT} header files")

# Create INTERFACE library (header-only)
add_library(openmrn INTERFACE)
add_library(OpenMRN::openmrn ALIAS openmrn)

# Set include directories
target_include_directories(openmrn
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../inc>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../inc/freertos>
        $<INSTALL_INTERFACE:inc/openmrn>
)

# Add RTOS-specific definition
if(OPENMRN_RTOS STREQUAL "FREERTOS")
    target_compile_definitions(openmrn INTERFACE __FreeRTOS__)
    message(STATUS "  Defines: __FreeRTOS__")
elseif(OPENMRN_RTOS STREQUAL "CMSIS_RTOS2")
    target_compile_definitions(openmrn INTERFACE __CMSIS_RTOS2)
    message(STATUS "  Defines: __CMSIS_RTOS2")
else()
    message(FATAL_ERROR "Invalid OPENMRN_RTOS value: ${OPENMRN_RTOS}. Must be CMSIS_RTOS2 or FREERTOS")
endif()

# Embedded ARM specific definitions for STM32H563
target_compile_definitions(openmrn INTERFACE
    STM32H563xx
    USE_HAL_DRIVER
    __STDC_FORMAT_MACROS
)

# Compiler flags for ARM embedded targets
target_compile_options(openmrn INTERFACE
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wno-unused-parameter -fno-exceptions -fno-rtti>
    $<$<CXX_COMPILER_ID:ARMClang>:-Wall -Wextra -Wno-unused-parameter -fno-exceptions -fno-rtti>
)

message(STATUS "OpenMRN configured successfully as INTERFACE library")
