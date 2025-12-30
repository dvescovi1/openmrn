# Applications subdirectory CMakeLists.txt

# Auto-discover all applications with CMakeLists.txt files
file(GLOB app_dirs "${CMAKE_CURRENT_SOURCE_DIR}/*")

foreach(app_dir ${app_dirs})
    if(IS_DIRECTORY "${app_dir}")
        get_filename_component(app_name "${app_dir}" NAME)
        
        # Skip hidden directories and common non-app directories
        if(NOT app_name MATCHES "^\\." AND NOT app_name MATCHES "^CMake")
            if(EXISTS "${app_dir}/CMakeLists.txt")
                add_subdirectory(${app_name})
            else()
                # message(STATUS "Skipping application ${app_name} - CMakeLists.txt not found")
            endif()
        endif()
    endif()
endforeach()

# Convenience target
add_custom_target(all-applications
    COMMENT "Build all available applications"
)
