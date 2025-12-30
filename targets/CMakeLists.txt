# Targets subdirectory CMakeLists.txt
# This builds the target-specific libraries and executables

# Auto-discover all targets with CMakeLists.txt files
file(GLOB target_dirs "${CMAKE_CURRENT_SOURCE_DIR}/*")

foreach(target_dir ${target_dirs})
    if(IS_DIRECTORY "${target_dir}")
        get_filename_component(target_name "${target_dir}" NAME)
        
        # Skip special directories
        if(NOT target_name MATCHES "^\\." AND NOT target_name MATCHES "^CMake" AND NOT target_name STREQUAL "test")
            if(EXISTS "${target_dir}/CMakeLists.txt")
                add_subdirectory(${target_name})
                message(STATUS "Added target: ${target_name}")
            else()
                message(STATUS "Skipping target ${target_name} - CMakeLists.txt not found")
            endif()
        endif()
    endif()
endforeach()

# Convenience target
add_custom_target(all-targets
    COMMENT "Build all available targets"
)

# Test target
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test/CMakeLists.txt")
