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

# Auto-discover module subdirectories
file(GLOB module_dirs "${CMAKE_CURRENT_SOURCE_DIR}/*")

foreach(module_dir ${module_dirs})
    if(IS_DIRECTORY "${module_dir}")
        get_filename_component(module_name "${module_dir}" NAME)
        
        # Skip special directories
        if(NOT module_name MATCHES "^\\." AND NOT module_name MATCHES "^CMake" AND NOT module_name STREQUAL "test")
            if(EXISTS "${module_dir}/CMakeLists.txt")
                add_subdirectory(${module_name})
                message(STATUS "Added module: ${module_name}")
            endif()
        endif()
    endif()
endforeach()

# Convenience target to build all libraries for this target
add_custom_target(linux.x86-all
    COMMENT "Build all libraries for linux.x86 target"
)

# Add optional test subdirectory if it exists
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test/CMakeLists.txt")
    add_subdirectory(test)
endif()
