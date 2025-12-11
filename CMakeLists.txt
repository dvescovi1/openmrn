cmake_minimum_required(VERSION 3.14)

project(OpenMRN VERSION 1.0.0 LANGUAGES C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Export compile commands for IDE integration
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set OPENMRNPATH for compatibility
set(OPENMRNPATH ${CMAKE_CURRENT_SOURCE_DIR})

# Add cmake modules directory
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Options
option(BUILD_APPLICATIONS "Build applications" ON)
option(BUILD_TESTS "Build tests" ON)
option(BUILD_DOC "Build documentation" ON)
option(ENABLE_COVERAGE "Enable code coverage" OFF)

# Platform detection
if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

# Find required packages for Linux builds
if(LINUX)
    find_package(Threads REQUIRED)
    find_package(PkgConfig)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(AVAHI avahi-client avahi-common)
    endif()
endif()

# Include subdirectories
add_subdirectory(src)

if(BUILD_APPLICATIONS)
    add_subdirectory(applications)
endif()

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

if(BUILD_DOC)
    add_subdirectory(doc)
endif()

# Installation rules
include(GNUInstallDirs)

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/openmrn
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hxx"
)

install(FILES LICENSE.md README.md
    DESTINATION ${CMAKE_INSTALL_DOCDIR}
)

# Export targets for use by other projects
install(EXPORT OpenMRNTargets
    FILE OpenMRNTargets.cmake
    NAMESPACE OpenMRN::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/OpenMRN
)

# Generate and install package config files
include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/OpenMRNConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/OpenMRNConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/OpenMRN
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/OpenMRNConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/OpenMRNConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/OpenMRNConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/OpenMRN
)

# Print configuration summary
message(STATUS "")
message(STATUS "OpenMRN Configuration:")
message(STATUS "  Version:              ${PROJECT_VERSION}")
message(STATUS "  Build type:           ${CMAKE_BUILD_TYPE}")
message(STATUS "  Build applications:   ${BUILD_APPLICATIONS}")
message(STATUS "  Build tests:          ${BUILD_TESTS}")
message(STATUS "  Build documentation:  ${BUILD_DOC}")
message(STATUS "  Enable coverage:      ${ENABLE_COVERAGE}")
message(STATUS "  Install prefix:       ${CMAKE_INSTALL_PREFIX}")
message(STATUS "")
