cmake_minimum_required(VERSION 3.14)

# Skip compiler test for bare-metal toolchains (must be before project())
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

project(OpenMRN VERSION 1.0.0 LANGUAGES C CXX ASM)

# Export compile commands for IDEs
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

# Define OPENMRNPATH
set(OPENMRNPATH ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "OpenMRN root path")

# Options for different build configurations
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(BUILD_APPLICATIONS "Build OpenMRN applications" OFF)
option(BUILD_TESTS "Build tests" OFF)
option(ENABLE_FREERTOS "Enable FreeRTOS support" OFF)

# Include helper functions
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/openmrn_helpers.cmake)

# Include directories
set(OPENMRN_INCLUDE_DIRS
    ${OPENMRNPATH}/src
    ${OPENMRNPATH}/inc
    ${CMAKE_CURRENT_BINARY_DIR}
)

# Core library sources - exclude test files
file(GLOB_RECURSE OPENMRN_BLE_SOURCES 
    ${OPENMRNPATH}/src/ble/*.cxx
    ${OPENMRNPATH}/src/ble/*.c
)
list(FILTER OPENMRN_BLE_SOURCES EXCLUDE REGEX ".*\\.cxxtest$")

file(GLOB_RECURSE OPENMRN_CONSOLE_SOURCES 
    ${OPENMRNPATH}/src/console/*.cxx
    ${OPENMRNPATH}/src/console/*.c
)
list(FILTER OPENMRN_CONSOLE_SOURCES EXCLUDE REGEX ".*\\.cxxtest$")

file(GLOB_RECURSE OPENMRN_DCC_SOURCES 
    ${OPENMRNPATH}/src/dcc/*.cxx
    ${OPENMRNPATH}/src/dcc/*.c
)
list(FILTER OPENMRN_DCC_SOURCES EXCLUDE REGEX ".*\\.cxxtest$")

file(GLOB_RECURSE OPENMRN_EXECUTOR_SOURCES 
    ${OPENMRNPATH}/src/executor/*.cxx
    ${OPENMRNPATH}/src/executor/*.c
)
list(FILTER OPENMRN_EXECUTOR_SOURCES EXCLUDE REGEX ".*\\.cxxtest$")

file(GLOB_RECURSE OPENMRN_OPENLCB_SOURCES 
    ${OPENMRNPATH}/src/openlcb/*.cxx
    ${OPENMRNPATH}/src/openlcb/*.c
)
list(FILTER OPENMRN_OPENLCB_SOURCES EXCLUDE REGEX ".*\\.cxxtest$")

file(GLOB_RECURSE OPENMRN_OS_SOURCES 
    ${OPENMRNPATH}/src/os/*.cxx
    ${OPENMRNPATH}/src/os/*.c
)
list(FILTER OPENMRN_OS_SOURCES EXCLUDE REGEX ".*\\.cxxtest$")

file(GLOB_RECURSE OPENMRN_UTILS_SOURCES 
    ${OPENMRNPATH}/src/utils/*.cxx
    ${OPENMRNPATH}/src/utils/*.c
)
list(FILTER OPENMRN_UTILS_SOURCES EXCLUDE REGEX ".*\\.cxxtest$")

file(GLOB_RECURSE OPENMRN_TRACTION_MODEM_SOURCES 
    ${OPENMRNPATH}/src/traction_modem/*.cxx
    ${OPENMRNPATH}/src/traction_modem/*.c
)
list(FILTER OPENMRN_TRACTION_MODEM_SOURCES EXCLUDE REGEX ".*\\.cxxtest$")

file(GLOB_RECURSE OPENMRN_WITHROTTLE_SOURCES 
    ${OPENMRNPATH}/src/withrottle/*.cxx
    ${OPENMRNPATH}/src/withrottle/*.c
)
list(FILTER OPENMRN_WITHROTTLE_SOURCES EXCLUDE REGEX ".*\\.cxxtest$")

# Combine all core sources
set(OPENMRN_CORE_SOURCES
    ${OPENMRN_BLE_SOURCES}
    ${OPENMRN_CONSOLE_SOURCES}
    ${OPENMRN_DCC_SOURCES}
    ${OPENMRN_EXECUTOR_SOURCES}
    ${OPENMRN_OPENLCB_SOURCES}
    ${OPENMRN_OS_SOURCES}
    ${OPENMRN_UTILS_SOURCES}
    ${OPENMRN_TRACTION_MODEM_SOURCES}
    ${OPENMRN_WITHROTTLE_SOURCES}
)

# Create core library
add_library(openmrn-core ${OPENMRN_CORE_SOURCES})
target_include_directories(openmrn-core PUBLIC
    $<BUILD_INTERFACE:${OPENMRNPATH}/src>
    $<BUILD_INTERFACE:${OPENMRNPATH}/inc>
    $<INSTALL_INTERFACE:include>
)

# Add compile definitions
target_compile_definitions(openmrn-core PUBLIC
    OPENMRNPATH="${OPENMRNPATH}"
)

# Platform-specific settings
if(UNIX AND NOT APPLE)
    target_compile_definitions(openmrn-core PUBLIC __LINUX__)
    target_link_libraries(openmrn-core PUBLIC pthread rt)
elseif(APPLE)
    target_compile_definitions(openmrn-core PUBLIC __MACH__)
    target_link_libraries(openmrn-core PUBLIC pthread)
elseif(WIN32)
    target_compile_definitions(openmrn-core PUBLIC __WIN32__)
endif()

# FreeRTOS support
if(ENABLE_FREERTOS)
    if(DEFINED FREERTOSPATH)
        target_compile_definitions(openmrn-core PUBLIC __FreeRTOS__)
        target_include_directories(openmrn-core PUBLIC ${FREERTOSPATH}/Source/include)
        
        # Add FreeRTOS drivers
        file(GLOB_RECURSE FREERTOS_DRIVER_SOURCES 
            ${OPENMRNPATH}/src/freertos_drivers/common/*.cxx
            ${OPENMRNPATH}/src/freertos_drivers/common/*.c
        )
        
        add_library(openmrn-freertos ${FREERTOS_DRIVER_SOURCES})
        target_link_libraries(openmrn-freertos PUBLIC openmrn-core)
        target_include_directories(openmrn-freertos PUBLIC
            $<BUILD_INTERFACE:${FREERTOSPATH}/Source/include>
            $<INSTALL_INTERFACE:include/freertos>
        )
    endif()
endif()

# Add alias for use in subdirectories
add_library(OpenMRN::core ALIAS openmrn-core)

# Build applications if requested
if(BUILD_APPLICATIONS)
    add_subdirectory(applications)
endif()

# Build tests if requested
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

# Installation rules
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS openmrn-core
    EXPORT OpenMRNTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install headers
install(DIRECTORY ${OPENMRNPATH}/src/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/openmrn
    FILES_MATCHING 
    PATTERN "*.h"
    PATTERN "*.hxx"
    PATTERN "*.cxxtest" EXCLUDE
)

install(DIRECTORY ${OPENMRNPATH}/inc/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/openmrn
    FILES_MATCHING 
    PATTERN "*.h"
    PATTERN "*.hxx"
)

# Export targets
install(EXPORT OpenMRNTargets
    FILE OpenMRNTargets.cmake
    NAMESPACE OpenMRN::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/OpenMRN
)

# Create config file
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/OpenMRNConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/OpenMRNConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/OpenMRN
)

# Create version file
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/OpenMRNConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# Install config files
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/OpenMRNConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/OpenMRNConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/OpenMRN
)

# Export for use from build tree
export(EXPORT OpenMRNTargets
    FILE ${CMAKE_CURRENT_BINARY_DIR}/OpenMRNTargets.cmake
    NAMESPACE OpenMRN::
)

# Register package
export(PACKAGE OpenMRN)

# Print configuration summary
message(STATUS "")
message(STATUS "OpenMRN Configuration:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  FreeRTOS: ${ENABLE_FREERTOS}")
message(STATUS "  Build applications: ${BUILD_APPLICATIONS}")
message(STATUS "  Build tests: ${BUILD_TESTS}")
message(STATUS "  Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "")
