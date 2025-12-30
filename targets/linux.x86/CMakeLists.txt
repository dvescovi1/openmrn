# CMakeLists.txt for linux.x86 target

project(linux_x86_target C CXX)

set(TARGET "linux.x86")

# Set 32-bit compilation flags if not cross-compiling
if(NOT CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
    message(STATUS "Added -m32 flag for 32-bit compilation on ${CMAKE_SYSTEM_PROCESSOR}")
endif()

# Include helper modules
include(${OPENMRNPATH}/cmake/openmrn_library.cmake)

# Define the core module libraries that need to be built for this target
set(MODULES
    os
    executor
    dcc
    openlcb
    utils
    withrottle
    if
    console
    ble
    cue
    traction_modem
)

# Add module subdirectories
foreach(module ${MODULES})
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${module}")
        add_subdirectory(${module})
        message(STATUS "Added module: ${module}")
    endif()
endforeach()

# Convenience target to build all libraries for this target
add_custom_target(linux.x86-all
    COMMENT "Build all libraries for linux.x86 target"
)

# Add optional test subdirectory if it exists
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test")
    add_subdirectory(test)
endif()
