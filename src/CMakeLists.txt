# OpenMRN Embedded ARM Library - Header Only

# Collect all header files
file(GLOB_RECURSE OPENMRN_HEADERS
    ble/*.h
    ble/*.hxx
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

# Create INTERFACE library (header-only)
add_library(openmrn INTERFACE)
add_library(OpenMRN::openmrn ALIAS openmrn)

# Set include directories
target_include_directories(openmrn
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../inc>
        $<INSTALL_INTERFACE:inc/openmrn>
)

# Embedded ARM specific definitions for STM32H563
target_compile_definitions(openmrn INTERFACE
    STM32H563xx
    USE_HAL_DRIVER
    __CMSIS_RTOS2
)

# Compiler flags for ARM embedded targets
target_compile_options(openmrn INTERFACE
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wno-unused-parameter -fno-exceptions -fno-rtti>
    $<$<CXX_COMPILER_ID:ARMClang>:-Wall -Wextra -Wno-unused-parameter -fno-exceptions -fno-rtti>
)

# Add headers as sources for IDE visibility
target_sources(openmrn INTERFACE ${OPENMRN_HEADERS})

# No installation targets for embedded library
message(STATUS "OpenMRN configured as INTERFACE (header-only) library for embedded ARM targets")
