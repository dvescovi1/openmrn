# OpenMRN Embedded ARM Library

# RTOS selection option
set(OPENMRN_RTOS "CMSIS_RTOS2" CACHE STRING "RTOS to use: CMSIS_RTOS2 or FREERTOS")
set_property(CACHE OPENMRN_RTOS PROPERTY STRINGS "CMSIS_RTOS2" "FREERTOS")

# Collect all source files
file(GLOB_RECURSE OPENMRN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/executor/*.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/openlcb/*.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/os/*.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/*.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/dcc/*.cxx
)

# Exclude test files and platform-specific implementations
# Don't exclude Mock files - they're needed for embedded use
list(FILTER OPENMRN_SOURCES EXCLUDE REGEX ".*[Tt]est[^M].*")
list(FILTER OPENMRN_SOURCES EXCLUDE REGEX ".*/linux/.*")
list(FILTER OPENMRN_SOURCES EXCLUDE REGEX ".*/mach/.*")
list(FILTER OPENMRN_SOURCES EXCLUDE REGEX ".*/win/.*")
list(FILTER OPENMRN_SOURCES EXCLUDE REGEX ".*/windows/.*")
list(FILTER OPENMRN_SOURCES EXCLUDE REGEX ".*gtest.*")
list(FILTER OPENMRN_SOURCES EXCLUDE REGEX ".*gmock.*")

# Collect all header files
file(GLOB_RECURSE OPENMRN_HEADERS
    console/*.h
    console/*.hxx
    dcc/*.h
    dcc/*.hxx
    executor/*.h
    executor/*.hxx
    openlcb/*.h
    openlcb/*.hxx
    os/*.h
    os/*.hxx
    traction_modem/*.h
    traction_modem/*.hxx
    utils/*.h
    utils/*.hxx
    freertos_drivers/**/*.h
    freertos_drivers/**/*.hxx
)

# Exclude test files
list(FILTER OPENMRN_HEADERS EXCLUDE REGEX ".*test.*")

# Create static library
add_library(openmrn STATIC ${OPENMRN_SOURCES} ${OPENMRN_HEADERS})
add_library(OpenMRN::openmrn ALIAS openmrn)

# Set include directories
target_include_directories(openmrn
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../inc>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../inc/freertos>
        $<INSTALL_INTERFACE:inc/openmrn>
)

# Embedded ARM specific definitions for STM32H563
target_compile_definitions(openmrn PUBLIC
    STM32H563xx
    USE_HAL_DRIVER
)

# Add RTOS-specific definition
if(OPENMRN_RTOS STREQUAL "FREERTOS")
    target_compile_definitions(openmrn PUBLIC __FreeRTOS__)
    message(STATUS "  RTOS: FreeRTOS")
elseif(OPENMRN_RTOS STREQUAL "CMSIS_RTOS2")
    target_compile_definitions(openmrn PUBLIC __CMSIS_RTOS2)
    message(STATUS "  RTOS: CMSIS RTOS v2")
else()
    message(FATAL_ERROR "Invalid OPENMRN_RTOS value: ${OPENMRN_RTOS}. Must be CMSIS_RTOS2 or FREERTOS")
endif()

# Compiler flags for ARM embedded targets
target_compile_options(openmrn PUBLIC
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wno-unused-parameter -fno-exceptions -fno-rtti>
    $<$<CXX_COMPILER_ID:ARMClang>:-Wall -Wextra -Wno-unused-parameter -fno-exceptions -fno-rtti>
)

# Summary
list(LENGTH OPENMRN_SOURCES SOURCE_COUNT)
message(STATUS "OpenMRN configured as STATIC library for STM32H563")
message(STATUS "  Found ${SOURCE_COUNT} source files to compile")
