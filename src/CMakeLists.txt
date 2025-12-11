# OpenMRN Core Library

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
)

# Exclude test files
list(FILTER OPENMRN_SOURCES EXCLUDE REGEX ".*\\.cxxtest$")
list(FILTER OPENMRN_SOURCES EXCLUDE REGEX ".*test.*\\.cxx$")

# Platform-specific sources
if(FREERTOS)
    file(GLOB FREERTOS_SOURCES
        freertos_drivers/common/*.cxx
    )
    list(APPEND OPENMRN_SOURCES ${FREERTOS_SOURCES})
endif()

# Create the main library
add_library(openmrn ${OPENMRN_SOURCES})
add_library(OpenMRN::openmrn ALIAS openmrn)

# Set include directories
target_include_directories(openmrn
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
        $<INSTALL_INTERFACE:include/openmrn>
)

# Platform-specific settings
if(UNIX AND NOT APPLE)
    target_link_libraries(openmrn PUBLIC Threads::Threads)
    target_compile_definitions(openmrn PUBLIC _POSIX_C_SOURCE=200112L)
    
    # Add Avahi support if available
    if(AVAHI_FOUND)
        target_link_libraries(openmrn PUBLIC ${AVAHI_LIBRARIES})
        target_include_directories(openmrn PUBLIC ${AVAHI_INCLUDE_DIRS})
        target_compile_definitions(openmrn PUBLIC HAVE_AVAHI_CLIENT)
    endif()
endif()

if(APPLE)
    target_link_libraries(openmrn PUBLIC Threads::Threads)
endif()

# Compiler flags
target_compile_options(openmrn PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wno-unused-parameter -Werror>
    $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wno-unused-parameter -Werror>
    $<$<CXX_COMPILER_ID:AppleClang>:-Wall -Wextra -Wno-unused-parameter -Werror>
)

# Coverage support
if(ENABLE_COVERAGE)
    target_compile_options(openmrn PRIVATE --coverage)
    target_link_options(openmrn PRIVATE --coverage)
endif()

# Set properties
set_target_properties(openmrn PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    POSITION_INDEPENDENT_CODE ON
)

# Installation
install(TARGETS openmrn
    EXPORT OpenMRNTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install headers
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/openmrn/src
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN "*.hxx"
    PATTERN "*test*" EXCLUDE
)
