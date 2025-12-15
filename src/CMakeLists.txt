# OpenMRN Embedded ARM Library

# Collect all source files
file(GLOB_RECURSE OPENMRN_SOURCES
    executor/*.cxx
    openlcb/*.cxx
    os/*.cxx
    utils/*.cxx
    dcc/*.cxx
)

# Exclude test files and platform-specific implementations
list(FILTER OPENMRN_SOURCES EXCLUDE REGEX ".*test.*")
list(FILTER OPENMRN_SOURCES EXCLUDE REGEX ".*linux.*")
list(FILTER OPENMRN_SOURCES EXCLUDE REGEX ".*mach.*")
list(FILTER OPENMRN_SOURCES EXCLUDE REGEX ".*win.*")

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

# Create static library
add_library(openmrn STATIC ${OPENMRN_SOURCES} ${OPENMRN_HEADERS})
add_library(OpenMRN::openmrn ALIAS openmrn)

# Set include directories
target_include_directories(openmrn
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../inc>
        $<INSTALL_INTERFACE:inc/openmrn>
)

# Embedded ARM specific definitions for STM32H563
target_compile_definitions(openmrn PUBLIC
    STM32H563xx
    USE_HAL_DRIVER
    __CMSIS_RTOS2
)

# Compiler flags for ARM embedded targets
target_compile_options(openmrn PUBLIC
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wno-unused-parameter -fno-exceptions -fno-rtti>
    $<$<CXX_COMPILER_ID:ARMClang>:-Wall -Wextra -Wno-unused-parameter -fno-exceptions -fno-rtti>
)

# No installation targets for embedded library
message(STATUS "OpenMRN configured as STATIC library for embedded ARM targets (${CMAKE_CURRENT_SOURCE_DIR})")
message(STATUS "Found ${list(LENGTH OPENMRN_SOURCES)} source files")
