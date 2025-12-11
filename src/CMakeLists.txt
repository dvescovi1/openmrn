# OpenMRN Embedded ARM Library

# Collect all source files
file(GLOB_RECURSE OPENMRN_SOURCES
    ble/*.cxx
    console/*.cxx
    dcc/*.cxx
    executor/*.cxx
    openlcb/*.cxx
    os/*.cxx
    os/*.c
    traction_modem/*.cxx
    utils/*.cxx
    utils/*.c
    freertos_drivers/common/*.cxx
)

# Exclude test files and platform-specific files not needed for ARM
list(FILTER OPENMRN_SOURCES EXCLUDE REGEX ".*\\.cxxtest$")
list(FILTER OPENMRN_SOURCES EXCLUDE REGEX ".*test.*\\.cxx$")
list(FILTER OPENMRN_SOURCES EXCLUDE REGEX ".*OSSelectWakeup.*")
list(FILTER OPENMRN_SOURCES EXCLUDE REGEX ".*MDNS.*")
list(FILTER OPENMRN_SOURCES EXCLUDE REGEX ".*socket_listener.*")
list(FILTER OPENMRN_SOURCES EXCLUDE REGEX ".*SocketClient.*")
list(FILTER OPENMRN_SOURCES EXCLUDE REGEX ".*SocketCan.*")

# Create static library for embedded targets
add_library(openmrn STATIC ${OPENMRN_SOURCES})
add_library(OpenMRN::openmrn ALIAS openmrn)

# Set include directories
target_include_directories(openmrn
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../inc>
        $<INSTALL_INTERFACE:inc/openmrn>
)

# Embedded ARM specific settings
target_compile_definitions(openmrn PUBLIC
    TARGET_IS_CC3200
    PART_TM4C1294NCPDT
)

# Compiler flags for ARM embedded targets
target_compile_options(openmrn PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wno-unused-parameter -fno-exceptions -fno-rtti>
    $<$<CXX_COMPILER_ID:ARMClang>:-Wall -Wextra -Wno-unused-parameter -fno-exceptions -fno-rtti>
)

# Set properties for embedded use
set_target_properties(openmrn PROPERTIES
    VERSION ${PROJECT_VERSION}
    POSITION_INDEPENDENT_CODE OFF
    C_STANDARD 11
    CXX_STANDARD 14
)

# No installation targets for embedded library
message(STATUS "OpenMRN library configured for embedded ARM targets")
