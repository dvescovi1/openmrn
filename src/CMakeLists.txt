# OpenMRN Embedded ARM Library

# Collect all source files
file(GLOB_RECURSE OPENMRN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/executor/*.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/openlcb/*.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/os/*.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/*.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/dcc/*.cxx
)

# Exclude test files and platform-specific implementations
list(FILTER OPENMRN_SOURCES EXCLUDE REGEX ".*[Tt]est.*")
list(FILTER OPENMRN_SOURCES EXCLUDE REGEX ".*/linux/.*")
list(FILTER OPENMRN_SOURCES EXCLUDE REGEX ".*/mach/.*")
list(FILTER OPENMRN_SOURCES EXCLUDE REGEX ".*/win/.*")
list(FILTER OPENMRN_SOURCES EXCLUDE REGEX ".*/windows/.*")

# Debug: show what sources were found
list(LENGTH OPENMRN_SOURCES SOURCE_COUNT)
if(SOURCE_COUNT EQUAL 0)
    message(WARNING "No source files found in ${CMAKE_CURRENT_SOURCE_DIR}")
    message(WARNING "This will cause a build error. Check that .cxx files exist.")
endif()

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

# Create static library (ensure we have sources)
if(SOURCE_COUNT GREATER 0)
    add_library(openmrn STATIC ${OPENMRN_SOURCES} ${OPENMRN_HEADERS})
else()
    # Fallback: create INTERFACE library if no sources found
    message(WARNING "Creating INTERFACE library - add source files to user's build manually")
    add_library(openmrn INTERFACE)
    target_sources(openmrn INTERFACE ${OPENMRN_HEADERS})
endif()
add_library(OpenMRN::openmrn ALIAS openmrn)

# Set include directories (works for both STATIC and INTERFACE)
if(SOURCE_COUNT GREATER 0)
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
else()
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
endif()

# No installation targets for embedded library
if(SOURCE_COUNT GREATER 0)
    message(STATUS "OpenMRN configured as STATIC library for embedded ARM targets")
    message(STATUS "  Found ${SOURCE_COUNT} source files to compile")
else()
    message(STATUS "OpenMRN configured as INTERFACE (header-only) library")
    message(STATUS "  No source files found - headers only")
endif()
