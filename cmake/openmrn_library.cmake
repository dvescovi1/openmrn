# Helper module for OpenMRN library creation

# Function to create an OpenMRN library from source files
# Usage: create_openmrn_library(library_name parent_directory)
function(create_openmrn_library lib_name parent_dir)
    set(lib_target "${parent_dir}_${lib_name}")
    set(src_dir "${OPENMRNPATH}/src/${parent_dir}/${lib_name}")
    
    # Find all source files
    file(GLOB_RECURSE sources
        "${src_dir}/*.c"
        "${src_dir}/*.cxx"
        "${src_dir}/*.cpp"
        "${src_dir}/*.S"
    )
    
    if(sources)
        add_library(${lib_target} STATIC ${sources})
        target_include_directories(${lib_target} PUBLIC
            ${OPENMRNPATH}/include
            ${OPENMRNPATH}/src
            ${CMAKE_CURRENT_SOURCE_DIR}/..
        )
        
        # Apply target-specific flags if they exist
        if(DEFINED CMAKE_C_FLAGS_TARGET)
            target_compile_options(${lib_target} PRIVATE ${CMAKE_C_FLAGS_TARGET})
        endif()
        if(DEFINED CMAKE_CXX_FLAGS_TARGET)
            target_compile_options(${lib_target} PRIVATE ${CMAKE_CXX_FLAGS_TARGET})
        endif()
        
        message(STATUS "Created library: ${lib_target} (${CMAKE_CURRENT_SOURCE_DIR})")
    else()
        message(STATUS "No sources found for ${lib_target} in ${src_dir}")
    endif()
endfunction()

# Function to create multiple libraries from a parent directory
function(create_openmrn_module_libs parent_dir)
    # Get the module name from the current directory
    get_filename_component(module_name "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
    
    # Find all subdirectories that should become libraries
    file(GLOB subdirs RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*")
    
    foreach(subdir ${subdirs})
        set(full_path "${CMAKE_CURRENT_SOURCE_DIR}/${subdir}")
        if(IS_DIRECTORY "${full_path}")
            create_openmrn_library("${subdir}" "${module_name}")
        endif()
    endforeach()
endfunction()
